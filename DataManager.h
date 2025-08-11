/* TD-NeuroMap Data Management
 * Handles data collection, normalization and storage
 */

#pragma once

#include <vector>
#include <memory>

// Forward declarations
namespace TD {
    class OP_CHOPInput;
}

class DataManager
{
public:
    DataManager();
    ~DataManager();

    // Data Collection
    bool addSample(const TD::OP_CHOPInput* inputCHOP, const TD::OP_CHOPInput* targetCHOP, 
                   int inputDim, int outputDim);
    void clearDataset();
    int getDatasetSize() const { return static_cast<int>(m_inputData.size()); }

    // Data Access
    const std::vector<std::vector<float>>& getInputData() const { return m_inputData; }
    const std::vector<std::vector<float>>& getOutputData() const { return m_outputData; }

    // Normalization
    void updateNormalization();
    bool isNormalizationReady() const { return m_normalizationReady; }
    
    std::vector<float> normalizeInput(const std::vector<float>& input) const;
    std::vector<float> denormalizeOutput(const std::vector<float>& output) const;
    std::vector<float> normalizeOutput(const std::vector<float>& output) const; // For training

    // Data validation
    bool validateDimensions(const TD::OP_CHOPInput* inputCHOP, const TD::OP_CHOPInput* targetCHOP,
                           int expectedInputDim, int expectedOutputDim) const;

private:
    // Data storage
    std::vector<std::vector<float>> m_inputData;    // [sample][feature]
    std::vector<std::vector<float>> m_outputData;   // [sample][target]

    // Normalization parameters
    std::vector<float> m_inputMin, m_inputMax;
    std::vector<float> m_outputMin, m_outputMax;
    bool m_normalizationReady;

    // Helper methods
    std::vector<float> extractChannelData(const TD::OP_CHOPInput* chop, int maxChannels) const;
    void calculateNormalizationParams();
    float normalizeValue(float value, float minVal, float maxVal) const;
    float denormalizeValue(float normalizedValue, float minVal, float maxVal) const;
};