/* TD-NeuroMap CHOP Parameters
 * Generated for TouchDesigner Custom CHOP
 * Neural Network Data Mapping for Real-time Parameter Control
 */

#pragma once

#include <string>

#pragma region ParNames and ParLabels

namespace TD 
{
    class OP_Inputs;
    class OP_ParameterManager;
}

// Model & Mode Parameters
constexpr static char ModeName[] = "Mode";
constexpr static char ModeLabel[] = "Mode";

constexpr static char InDimName[] = "Indim";
constexpr static char InDimLabel[] = "Input Dimensions";

constexpr static char OutDimName[] = "Outdim";
constexpr static char OutDimLabel[] = "Output Dimensions";

constexpr static char NormalizeName[] = "Normalize";
constexpr static char NormalizeLabel[] = "Normalize Data";

// Data Collection Parameters
constexpr static char AddSampleName[] = "Addsample";
constexpr static char AddSampleLabel[] = "Add Sample";

constexpr static char ClearDatasetName[] = "Cleardataset";
constexpr static char ClearDatasetLabel[] = "Clear Dataset";

constexpr static char DatasetSizeName[] = "Datasetsize";
constexpr static char DatasetSizeLabel[] = "Dataset Size";

// Training Parameters
constexpr static char TrainName[] = "Train";
constexpr static char TrainLabel[] = "Train Model";

constexpr static char EpochsName[] = "Epochs";
constexpr static char EpochsLabel[] = "Training Epochs";

constexpr static char LearnRateName[] = "Learnrate";
constexpr static char LearnRateLabel[] = "Learning Rate";

constexpr static char HiddenLayersName[] = "Hiddenlayers";
constexpr static char HiddenLayersLabel[] = "Hidden Layers";

constexpr static char HiddenUnitsName[] = "Hiddenunits";
constexpr static char HiddenUnitsLabel[] = "Hidden Units";

constexpr static char LossName[] = "Loss";
constexpr static char LossLabel[] = "Training Loss";

// Runtime/Smoothing Parameters
constexpr static char SmoothEnableName[] = "Smoothenable";
constexpr static char SmoothEnableLabel[] = "Enable Smoothing";

constexpr static char MinCutoffName[] = "Mincutoff";
constexpr static char MinCutoffLabel[] = "Min Cutoff Frequency";

constexpr static char BetaName[] = "Beta";
constexpr static char BetaLabel[] = "Speed Coefficient";

// Model File Parameters
constexpr static char ModelFileName[] = "Modelfile";
constexpr static char ModelFileLabel[] = "Model File Path";

constexpr static char SaveModelName[] = "Savemodel";
constexpr static char SaveModelLabel[] = "Save Model";

constexpr static char LoadModelName[] = "Loadmodel";
constexpr static char LoadModelLabel[] = "Load Model";

#pragma endregion

#pragma region Menus

enum class ModeMenuItems
{
    Collect = 0,
    Train = 1,  
    Run = 2
};

#pragma endregion

#pragma region Parameters

class Parameters
{
public:
    static void setup(TD::OP_ParameterManager* manager);

    // Model & Mode
    static ModeMenuItems evalMode(const TD::OP_Inputs* inputs);
    static int evalInDim(const TD::OP_Inputs* inputs);
    static int evalOutDim(const TD::OP_Inputs* inputs);
    static bool evalNormalize(const TD::OP_Inputs* inputs);

    // Data Collection  
    static int evalAddSample(const TD::OP_Inputs* inputs);
    static int evalClearDataset(const TD::OP_Inputs* inputs);
    static int evalDatasetSize(const TD::OP_Inputs* inputs);

    // Training
    static int evalTrain(const TD::OP_Inputs* inputs);
    static int evalEpochs(const TD::OP_Inputs* inputs);
    static double evalLearnRate(const TD::OP_Inputs* inputs);
    static int evalHiddenLayers(const TD::OP_Inputs* inputs);
    static int evalHiddenUnits(const TD::OP_Inputs* inputs);
    static double evalLoss(const TD::OP_Inputs* inputs);

    // Runtime/Smoothing
    static bool evalSmoothEnable(const TD::OP_Inputs* inputs);
    static double evalMinCutoff(const TD::OP_Inputs* inputs);
    static double evalBeta(const TD::OP_Inputs* inputs);

    // Model File
    static std::string evalModelFile(const TD::OP_Inputs* inputs);
    static int evalSaveModel(const TD::OP_Inputs* inputs);
    static int evalLoadModel(const TD::OP_Inputs* inputs);
    
    // Dynamic channel name getters (will be implemented later)
    static std::string evalInChannelName(const TD::OP_Inputs* inputs, int index);
    static std::string evalOutChannelName(const TD::OP_Inputs* inputs, int index);
};

#pragma endregion