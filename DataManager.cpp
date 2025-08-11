/* TD-NeuroMap Data Management Implementation */

#include "DataManager.h"
#include "CPlusPlus_Common.h"
#include <algorithm>
#include <limits>
#include <cmath>

DataManager::DataManager()
    : m_normalizationReady(false)
{
}

DataManager::~DataManager()
{
}

bool DataManager::addSample(const TD::OP_CHOPInput* inputCHOP, const TD::OP_CHOPInput* targetCHOP, 
                           int inputDim, int outputDim)
{
    if (!validateDimensions(inputCHOP, targetCHOP, inputDim, outputDim))
    {
        return false;
    }

    // Extract input data (use first sample of each channel)
    std::vector<float> inputSample = extractChannelData(inputCHOP, inputDim);
    std::vector<float> outputSample = extractChannelData(targetCHOP, outputDim);

    if (inputSample.size() != static_cast<size_t>(inputDim) || 
        outputSample.size() != static_cast<size_t>(outputDim))
    {
        return false;
    }

    // Store the data
    m_inputData.push_back(inputSample);
    m_outputData.push_back(outputSample);

    // Invalidate normalization (will be recalculated when needed)
    m_normalizationReady = false;

    return true;
}

void DataManager::clearDataset()
{
    m_inputData.clear();
    m_outputData.clear();
    m_inputMin.clear();
    m_inputMax.clear();
    m_outputMin.clear();
    m_outputMax.clear();
    m_normalizationReady = false;
}

void DataManager::updateNormalization()
{
    if (m_inputData.empty())
    {
        m_normalizationReady = false;
        return;
    }

    calculateNormalizationParams();
    m_normalizationReady = true;
}

std::vector<float> DataManager::normalizeInput(const std::vector<float>& input) const
{
    if (!m_normalizationReady || input.size() != m_inputMin.size())
    {
        return input; // Return original if normalization not ready
    }

    std::vector<float> normalized;
    normalized.reserve(input.size());

    for (size_t i = 0; i < input.size(); ++i)
    {
        normalized.push_back(normalizeValue(input[i], m_inputMin[i], m_inputMax[i]));
    }

    return normalized;
}

std::vector<float> DataManager::denormalizeOutput(const std::vector<float>& output) const
{
    if (!m_normalizationReady || output.size() != m_outputMin.size())
    {
        return output; // Return original if normalization not ready
    }

    std::vector<float> denormalized;
    denormalized.reserve(output.size());

    for (size_t i = 0; i < output.size(); ++i)
    {
        denormalized.push_back(denormalizeValue(output[i], m_outputMin[i], m_outputMax[i]));
    }

    return denormalized;
}

std::vector<float> DataManager::normalizeOutput(const std::vector<float>& output) const
{
    if (!m_normalizationReady || output.size() != m_outputMin.size())
    {
        return output; // Return original if normalization not ready
    }

    std::vector<float> normalized;
    normalized.reserve(output.size());

    for (size_t i = 0; i < output.size(); ++i)
    {
        normalized.push_back(normalizeValue(output[i], m_outputMin[i], m_outputMax[i]));
    }

    return normalized;
}

bool DataManager::validateDimensions(const TD::OP_CHOPInput* inputCHOP, const TD::OP_CHOPInput* targetCHOP,
                                    int expectedInputDim, int expectedOutputDim) const
{
    if (!inputCHOP || !targetCHOP)
        return false;

    if (inputCHOP->numChannels < expectedInputDim || targetCHOP->numChannels < expectedOutputDim)
        return false;

    if (inputCHOP->numSamples == 0 || targetCHOP->numSamples == 0)
        return false;

    return true;
}

std::vector<float> DataManager::extractChannelData(const TD::OP_CHOPInput* chop, int maxChannels) const
{
    std::vector<float> data;
    data.reserve(maxChannels);

    int channelsToExtract = std::min(maxChannels, chop->numChannels);
    
    for (int ch = 0; ch < channelsToExtract; ++ch)
    {
        // Use the first sample from each channel
        // This assumes we're capturing current state, not time-series data
        if (chop->numSamples > 0)
        {
            data.push_back(chop->channelData[ch][0]);
        }
        else
        {
            data.push_back(0.0f);
        }
    }

    // Pad with zeros if we don't have enough channels
    while (data.size() < static_cast<size_t>(maxChannels))
    {
        data.push_back(0.0f);
    }

    return data;
}

void DataManager::calculateNormalizationParams()
{
    if (m_inputData.empty())
        return;

    size_t inputDim = m_inputData[0].size();
    size_t outputDim = m_outputData[0].size();

    // Initialize min/max vectors
    m_inputMin.assign(inputDim, std::numeric_limits<float>::max());
    m_inputMax.assign(inputDim, std::numeric_limits<float>::lowest());
    m_outputMin.assign(outputDim, std::numeric_limits<float>::max());
    m_outputMax.assign(outputDim, std::numeric_limits<float>::lowest());

    // Find min/max for input data
    for (const auto& sample : m_inputData)
    {
        for (size_t i = 0; i < sample.size() && i < inputDim; ++i)
        {
            m_inputMin[i] = std::min(m_inputMin[i], sample[i]);
            m_inputMax[i] = std::max(m_inputMax[i], sample[i]);
        }
    }

    // Find min/max for output data
    for (const auto& sample : m_outputData)
    {
        for (size_t i = 0; i < sample.size() && i < outputDim; ++i)
        {
            m_outputMin[i] = std::min(m_outputMin[i], sample[i]);
            m_outputMax[i] = std::max(m_outputMax[i], sample[i]);
        }
    }

    // Handle cases where min == max (constant values)
    for (size_t i = 0; i < inputDim; ++i)
    {
        if (std::abs(m_inputMax[i] - m_inputMin[i]) < 1e-6f)
        {
            m_inputMin[i] -= 0.5f;
            m_inputMax[i] += 0.5f;
        }
    }

    for (size_t i = 0; i < outputDim; ++i)
    {
        if (std::abs(m_outputMax[i] - m_outputMin[i]) < 1e-6f)
        {
            m_outputMin[i] -= 0.5f;
            m_outputMax[i] += 0.5f;
        }
    }
}

float DataManager::normalizeValue(float value, float minVal, float maxVal) const
{
    if (std::abs(maxVal - minVal) < 1e-6f)
        return 0.5f; // If range is too small, return middle value

    return (value - minVal) / (maxVal - minVal);
}

float DataManager::denormalizeValue(float normalizedValue, float minVal, float maxVal) const
{
    return normalizedValue * (maxVal - minVal) + minVal;
}