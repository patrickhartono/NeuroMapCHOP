/* TD-NeuroMap CHOP
 * Neural Network Data Mapping for TouchDesigner
 * Inspired by FluCoMa workflow for real-time parameter mapping
 */

#pragma once

#include "CHOP_CPlusPlusBase.h"
#include "Parameters.h"
#include "DataManager.h"
#include <memory>

using namespace TD;

class NeuroMapCHOP : public CHOP_CPlusPlusBase
{
public:
    NeuroMapCHOP(const OP_NodeInfo* info);
    virtual ~NeuroMapCHOP();

    // TouchDesigner CHOP Interface
    virtual void getGeneralInfo(CHOP_GeneralInfo* ginfo, const OP_Inputs* inputs, void*) override;
    virtual bool getOutputInfo(CHOP_OutputInfo* info, const OP_Inputs* inputs, void*) override;
    virtual void getChannelName(int32_t index, OP_String* name, const OP_Inputs* inputs, void*) override;
    virtual void execute(CHOP_Output* output, const OP_Inputs* inputs, void*) override;
    virtual void setupParameters(OP_ParameterManager* manager, void*) override;
    virtual void pulsePressed(const char* name, void*) override;

private:
    // Core components
    std::unique_ptr<DataManager> m_dataManager;
    Parameters m_params;

    // State management
    ModeMenuItems m_currentMode;
    int m_currentInputDim;
    int m_currentOutputDim;
    bool m_modelTrained;
    
    // Internal methods
    void handleModeChange(ModeMenuItems newMode, const OP_Inputs* inputs);
    void handleDataCollection(const OP_Inputs* inputs);
    void handleTraining(const OP_Inputs* inputs);
    void handleInference(const OP_Inputs* inputs, CHOP_Output* output);
    
    // Parameter helpers
    void updateReadOnlyParams(const OP_Inputs* inputs);
    bool validateInputs(const OP_Inputs* inputs) const;
    
    // Utility methods
    std::string generateChannelName(bool isInput, int index) const;
    void logMessage(const std::string& message) const;
};