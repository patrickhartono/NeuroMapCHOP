/* TD-NeuroMap CHOP Implementation */

#include "NeuroMapCHOP.h"
#include <cassert>
#include <string>
#include <iostream>

// TouchDesigner Plugin Entry Points
extern "C"
{

DLLEXPORT
void FillCHOPPluginInfo(CHOP_PluginInfo* info)
{
    info->apiVersion = CHOPCPlusPlusAPIVersion;

    OP_CustomOPInfo& customInfo = info->customOPInfo;
    customInfo.opType->setString("Neuromap");
    customInfo.opLabel->setString("NeuroMap");
    customInfo.authorName->setString("TD-NeuroMap");
    customInfo.authorEmail->setString("support@neuralmap.ai");

    // This CHOP takes 1-2 inputs (input features + target outputs)
    customInfo.minInputs = 1;
    customInfo.maxInputs = 2;
}

DLLEXPORT
CHOP_CPlusPlusBase* CreateCHOPInstance(const OP_NodeInfo* info)
{
    return new NeuroMapCHOP(info);
}

DLLEXPORT
void DestroyCHOPInstance(CHOP_CPlusPlusBase* instance)
{
    delete (NeuroMapCHOP*)instance;
}

}; // extern "C"

NeuroMapCHOP::NeuroMapCHOP(const OP_NodeInfo* info)
    : m_dataManager(std::make_unique<DataManager>())
    , m_currentMode(ModeMenuItems::Collect)
    , m_currentInputDim(2)
    , m_currentOutputDim(2)
    , m_modelTrained(false)
{
    logMessage("NeuroMapCHOP initialized");
}

NeuroMapCHOP::~NeuroMapCHOP()
{
    logMessage("NeuroMapCHOP destroyed");
}

void NeuroMapCHOP::getGeneralInfo(CHOP_GeneralInfo* ginfo, const OP_Inputs* inputs, void*)
{
    ginfo->cookEveryFrameIfAsked = true;
    ginfo->timeslice = false;
    ginfo->inputMatchIndex = 0; // Match first input by default
}

bool NeuroMapCHOP::getOutputInfo(CHOP_OutputInfo* info, const OP_Inputs* inputs, void*)
{
    // Get current parameters
    m_currentOutputDim = m_params.evalOutDim(inputs);
    
    // Output dimensions depend on current mode
    ModeMenuItems mode = m_params.evalMode(inputs);
    
    if (mode == ModeMenuItems::Run && m_modelTrained)
    {
        // In Run mode, output the predicted values
        info->numChannels = m_currentOutputDim;
        info->numSamples = 1; // Single sample output
        info->sampleRate = 60.0f; // Default sample rate
        return true;
    }
    else
    {
        // In Collect/Train mode, pass through first input
        return false; // Use inputMatchIndex
    }
}

void NeuroMapCHOP::getChannelName(int32_t index, OP_String* name, const OP_Inputs* inputs, void*)
{
    std::string channelName = generateChannelName(false, index); // Output channel
    name->setString(channelName.c_str());
}

void NeuroMapCHOP::execute(CHOP_Output* output, const OP_Inputs* inputs, void*)
{
    if (!validateInputs(inputs))
    {
        return;
    }

    // Get current parameters
    ModeMenuItems mode = m_params.evalMode(inputs);
    m_currentInputDim = m_params.evalInDim(inputs);
    m_currentOutputDim = m_params.evalOutDim(inputs);

    // Handle mode changes
    if (mode != m_currentMode)
    {
        handleModeChange(mode, inputs);
        m_currentMode = mode;
    }

    // Update read-only parameters
    updateReadOnlyParams(inputs);

    // Mode-specific execution
    switch (m_currentMode)
    {
        case ModeMenuItems::Collect:
            handleDataCollection(inputs);
            break;
            
        case ModeMenuItems::Train:
            handleTraining(inputs);
            break;
            
        case ModeMenuItems::Run:
            handleInference(inputs, output);
            break;
    }

    // If we're not in Run mode or model not trained, pass through input
    if (m_currentMode != ModeMenuItems::Run || !m_modelTrained)
    {
        const OP_CHOPInput* input = inputs->getInputCHOP(0);
        if (input)
        {
            // Simple pass-through for now
            for (int i = 0; i < output->numChannels && i < input->numChannels; ++i)
            {
                for (int j = 0; j < output->numSamples && j < input->numSamples; ++j)
                {
                    output->channels[i][j] = input->channelData[i][j];
                }
            }
        }
    }
}

void NeuroMapCHOP::setupParameters(OP_ParameterManager* manager, void*)
{
    m_params.setup(manager);
}

void NeuroMapCHOP::pulsePressed(const char* name, void*)
{
    std::string paramName(name);
    
    if (paramName == AddSampleName)
    {
        logMessage("Add Sample pulse pressed");
        // Will be handled in execute() method
    }
    else if (paramName == ClearDatasetName)
    {
        logMessage("Clear Dataset pulse pressed");
        m_dataManager->clearDataset();
    }
    else if (paramName == TrainName)
    {
        logMessage("Train Model pulse pressed");
        // Will be handled in execute() method
    }
    else if (paramName == SaveModelName)
    {
        logMessage("Save Model pulse pressed");
        // TODO: Implement model saving
    }
    else if (paramName == LoadModelName)
    {
        logMessage("Load Model pulse pressed");
        // TODO: Implement model loading
    }
}

void NeuroMapCHOP::handleModeChange(ModeMenuItems newMode, const OP_Inputs* inputs)
{
    logMessage("Mode changed to: " + std::to_string(static_cast<int>(newMode)));
    
    switch (newMode)
    {
        case ModeMenuItems::Collect:
            logMessage("Entering data collection mode");
            break;
            
        case ModeMenuItems::Train:
            logMessage("Entering training mode");
            // Update normalization before training
            if (m_params.evalNormalize(inputs))
            {
                m_dataManager->updateNormalization();
            }
            break;
            
        case ModeMenuItems::Run:
            logMessage("Entering inference mode");
            break;
    }
}

void NeuroMapCHOP::handleDataCollection(const OP_Inputs* inputs)
{
    // Check if Add Sample was pressed
    if (m_params.evalAddSample(inputs) > 0)
    {
        const OP_CHOPInput* inputCHOP = inputs->getInputCHOP(0);
        const OP_CHOPInput* targetCHOP = inputs->getInputCHOP(1);
        
        if (inputCHOP && targetCHOP)
        {
            bool success = m_dataManager->addSample(inputCHOP, targetCHOP, 
                                                   m_currentInputDim, m_currentOutputDim);
            if (success)
            {
                logMessage("Sample added successfully. Dataset size: " + 
                          std::to_string(m_dataManager->getDatasetSize()));
            }
            else
            {
                logMessage("Failed to add sample - dimension mismatch or invalid data");
            }
        }
        else
        {
            logMessage("Cannot add sample - need both input and target CHOP connections");
        }
    }
}

void NeuroMapCHOP::handleTraining(const OP_Inputs* inputs)
{
    // Check if Train was pressed
    if (m_params.evalTrain(inputs) > 0)
    {
        logMessage("Training requested");
        
        int datasetSize = m_dataManager->getDatasetSize();
        if (datasetSize < 2)
        {
            logMessage("Cannot train - need at least 2 samples in dataset");
            return;
        }
        
        // TODO: Implement actual training with MiniDNN
        // For now, just simulate training
        logMessage("Training with " + std::to_string(datasetSize) + " samples");
        
        // Simulate training completion
        m_modelTrained = true;
        logMessage("Training completed (simulated)");
    }
}

void NeuroMapCHOP::handleInference(const OP_Inputs* inputs, CHOP_Output* output)
{
    if (!m_modelTrained)
    {
        // No model trained yet, output zeros
        for (int i = 0; i < output->numChannels; ++i)
        {
            for (int j = 0; j < output->numSamples; ++j)
            {
                output->channels[i][j] = 0.0f;
            }
        }
        return;
    }

    const OP_CHOPInput* inputCHOP = inputs->getInputCHOP(0);
    if (!inputCHOP)
    {
        return;
    }

    // TODO: Implement actual neural network inference
    // For now, just pass through input data (limited to output dimensions)
    int outputChannels = std::min(output->numChannels, inputCHOP->numChannels);
    
    for (int i = 0; i < outputChannels; ++i)
    {
        for (int j = 0; j < output->numSamples && j < inputCHOP->numSamples; ++j)
        {
            // Simple transformation for demonstration
            output->channels[i][j] = inputCHOP->channelData[i][j] * 0.8f + 0.1f;
        }
    }
}

void NeuroMapCHOP::updateReadOnlyParams(const OP_Inputs* inputs)
{
    // Update dataset size parameter
    // Note: In a real implementation, you would use a method to set parameter values
    // This is a placeholder for the concept
}

bool NeuroMapCHOP::validateInputs(const OP_Inputs* inputs) const
{
    if (!inputs)
        return false;

    int numInputs = inputs->getNumInputs();
    
    // In Collect mode, we need 2 inputs (features + targets)
    if (m_currentMode == ModeMenuItems::Collect && numInputs < 2)
    {
        return true; // Don't block execution, just log warning
    }
    
    // In Run mode, we need at least 1 input (features)
    if (m_currentMode == ModeMenuItems::Run && numInputs < 1)
    {
        return false;
    }

    return true;
}

std::string NeuroMapCHOP::generateChannelName(bool isInput, int index) const
{
    std::string prefix = isInput ? "in" : "out";
    return prefix + std::to_string(index + 1);
}

void NeuroMapCHOP::logMessage(const std::string& message) const
{
    // Simple logging to console for now
    // In production, this could use TouchDesigner's logging system
    std::cout << "[NeuroMapCHOP] " << message << std::endl;
}