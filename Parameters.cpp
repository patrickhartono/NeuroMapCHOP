/* TD-NeuroMap CHOP Parameters Implementation
 * Generated for TouchDesigner Custom CHOP
 */

#include <string>
#include <array>
#include "CPlusPlus_Common.h"
#include "Parameters.h"

#pragma region Parameter Evaluation Methods

// Model & Mode
ModeMenuItems Parameters::evalMode(const TD::OP_Inputs* inputs)
{
    return static_cast<ModeMenuItems>(inputs->getParInt(ModeName));
}

int Parameters::evalInDim(const TD::OP_Inputs* inputs)
{
    return inputs->getParInt(InDimName);
}

int Parameters::evalOutDim(const TD::OP_Inputs* inputs)
{
    return inputs->getParInt(OutDimName);
}

bool Parameters::evalNormalize(const TD::OP_Inputs* inputs)
{
    return inputs->getParInt(NormalizeName) ? true : false;
}

// Data Collection
int Parameters::evalAddSample(const TD::OP_Inputs* inputs)
{
    return inputs->getParInt(AddSampleName);
}

int Parameters::evalClearDataset(const TD::OP_Inputs* inputs)
{
    return inputs->getParInt(ClearDatasetName);
}

int Parameters::evalDatasetSize(const TD::OP_Inputs* inputs)
{
    return inputs->getParInt(DatasetSizeName);
}

// Training
int Parameters::evalTrain(const TD::OP_Inputs* inputs)
{
    return inputs->getParInt(TrainName);
}

int Parameters::evalEpochs(const TD::OP_Inputs* inputs)
{
    return inputs->getParInt(EpochsName);
}

double Parameters::evalLearnRate(const TD::OP_Inputs* inputs)
{
    return inputs->getParDouble(LearnRateName);
}

int Parameters::evalHiddenLayers(const TD::OP_Inputs* inputs)
{
    return inputs->getParInt(HiddenLayersName);
}

int Parameters::evalHiddenUnits(const TD::OP_Inputs* inputs)
{
    return inputs->getParInt(HiddenUnitsName);
}

double Parameters::evalLoss(const TD::OP_Inputs* inputs)
{
    return inputs->getParDouble(LossName);
}

// Runtime/Smoothing
bool Parameters::evalSmoothEnable(const TD::OP_Inputs* inputs)
{
    return inputs->getParInt(SmoothEnableName) ? true : false;
}

double Parameters::evalMinCutoff(const TD::OP_Inputs* inputs)
{
    return inputs->getParDouble(MinCutoffName);
}

double Parameters::evalBeta(const TD::OP_Inputs* inputs)
{
    return inputs->getParDouble(BetaName);
}

// Model File
std::string Parameters::evalModelFile(const TD::OP_Inputs* inputs)
{
    return inputs->getParString(ModelFileName);
}

int Parameters::evalSaveModel(const TD::OP_Inputs* inputs)
{
    return inputs->getParInt(SaveModelName);
}

int Parameters::evalLoadModel(const TD::OP_Inputs* inputs)
{
    return inputs->getParInt(LoadModelName);
}

// Dynamic channel names (placeholder for now)
std::string Parameters::evalInChannelName(const TD::OP_Inputs* inputs, int index)
{
    // TODO: Implement dynamic parameter reading
    return "in" + std::to_string(index + 1);
}

std::string Parameters::evalOutChannelName(const TD::OP_Inputs* inputs, int index)
{
    // TODO: Implement dynamic parameter reading  
    return "out" + std::to_string(index + 1);
}

#pragma endregion

#pragma region Parameter Setup

void Parameters::setup(TD::OP_ParameterManager* manager)
{
    // Model & Mode Page
    {
        TD::OP_StringParameter p;
        p.name = ModeName;
        p.label = ModeLabel;
        p.page = "Model";
        p.defaultValue = "Collect";
        std::array<const char*, 3> Names = {"Collect", "Train", "Run"};
        std::array<const char*, 3> Labels = {"Collect", "Train", "Run"};
        TD::OP_ParAppendResult res = manager->appendMenu(p, Names.size(), Names.data(), Labels.data());
        assert(res == TD::OP_ParAppendResult::Success);
    }

    {
        TD::OP_NumericParameter p;
        p.name = InDimName;
        p.label = InDimLabel;
        p.page = "Model";
        p.defaultValues[0] = 2;
        p.minValues[0] = 1;
        p.maxValues[0] = 16;
        p.clampMins[0] = true;
        p.clampMaxes[0] = true;
        TD::OP_ParAppendResult res = manager->appendInt(p);
        assert(res == TD::OP_ParAppendResult::Success);
    }

    {
        TD::OP_NumericParameter p;
        p.name = OutDimName;
        p.label = OutDimLabel;
        p.page = "Model";
        p.defaultValues[0] = 2;
        p.minValues[0] = 1;
        p.maxValues[0] = 16;
        p.clampMins[0] = true;
        p.clampMaxes[0] = true;
        TD::OP_ParAppendResult res = manager->appendInt(p);
        assert(res == TD::OP_ParAppendResult::Success);
    }

    {
        TD::OP_NumericParameter p;
        p.name = NormalizeName;
        p.label = NormalizeLabel;
        p.page = "Model";
        p.defaultValues[0] = true;
        TD::OP_ParAppendResult res = manager->appendToggle(p);
        assert(res == TD::OP_ParAppendResult::Success);
    }

    // Data Collection Page
    {
        TD::OP_NumericParameter p;
        p.name = AddSampleName;
        p.label = AddSampleLabel;
        p.page = "Data";
        TD::OP_ParAppendResult res = manager->appendPulse(p);
        assert(res == TD::OP_ParAppendResult::Success);
    }

    {
        TD::OP_NumericParameter p;
        p.name = ClearDatasetName;
        p.label = ClearDatasetLabel;
        p.page = "Data";
        TD::OP_ParAppendResult res = manager->appendPulse(p);
        assert(res == TD::OP_ParAppendResult::Success);
    }

    {
        TD::OP_NumericParameter p;
        p.name = DatasetSizeName;
        p.label = DatasetSizeLabel;
        p.page = "Data";
        p.defaultValues[0] = 0;
        p.readOnly = true;
        TD::OP_ParAppendResult res = manager->appendInt(p);
        assert(res == TD::OP_ParAppendResult::Success);
    }

    // Training Page
    {
        TD::OP_NumericParameter p;
        p.name = TrainName;
        p.label = TrainLabel;
        p.page = "Training";
        TD::OP_ParAppendResult res = manager->appendPulse(p);
        assert(res == TD::OP_ParAppendResult::Success);
    }

    {
        TD::OP_NumericParameter p;
        p.name = EpochsName;
        p.label = EpochsLabel;
        p.page = "Training";
        p.defaultValues[0] = 100;
        p.minValues[0] = 1;
        p.maxValues[0] = 10000;
        p.clampMins[0] = true;
        p.clampMaxes[0] = false;
        TD::OP_ParAppendResult res = manager->appendInt(p);
        assert(res == TD::OP_ParAppendResult::Success);
    }

    {
        TD::OP_NumericParameter p;
        p.name = LearnRateName;
        p.label = LearnRateLabel;
        p.page = "Training";
        p.defaultValues[0] = 0.001;
        p.minValues[0] = 0.00001;
        p.maxValues[0] = 1.0;
        p.clampMins[0] = true;
        p.clampMaxes[0] = false;
        TD::OP_ParAppendResult res = manager->appendFloat(p);
        assert(res == TD::OP_ParAppendResult::Success);
    }

    {
        TD::OP_NumericParameter p;
        p.name = HiddenLayersName;
        p.label = HiddenLayersLabel;
        p.page = "Training";
        p.defaultValues[0] = 2;
        p.minValues[0] = 1;
        p.maxValues[0] = 5;
        p.clampMins[0] = true;
        p.clampMaxes[0] = true;
        TD::OP_ParAppendResult res = manager->appendInt(p);
        assert(res == TD::OP_ParAppendResult::Success);
    }

    {
        TD::OP_NumericParameter p;
        p.name = HiddenUnitsName;
        p.label = HiddenUnitsLabel;
        p.page = "Training";
        p.defaultValues[0] = 64;
        p.minValues[0] = 8;
        p.maxValues[0] = 512;
        p.clampMins[0] = true;
        p.clampMaxes[0] = false;
        TD::OP_ParAppendResult res = manager->appendInt(p);
        assert(res == TD::OP_ParAppendResult::Success);
    }

    {
        TD::OP_NumericParameter p;
        p.name = LossName;
        p.label = LossLabel;
        p.page = "Training";
        p.defaultValues[0] = 0.0;
        p.readOnly = true;
        TD::OP_ParAppendResult res = manager->appendFloat(p);
        assert(res == TD::OP_ParAppendResult::Success);
    }

    // Runtime Page
    {
        TD::OP_NumericParameter p;
        p.name = SmoothEnableName;
        p.label = SmoothEnableLabel;
        p.page = "Runtime";
        p.defaultValues[0] = false;
        TD::OP_ParAppendResult res = manager->appendToggle(p);
        assert(res == TD::OP_ParAppendResult::Success);
    }

    {
        TD::OP_NumericParameter p;
        p.name = MinCutoffName;
        p.label = MinCutoffLabel;
        p.page = "Runtime";
        p.defaultValues[0] = 1.0;
        p.minValues[0] = 0.001;
        p.maxValues[0] = 100.0;
        p.clampMins[0] = true;
        p.clampMaxes[0] = false;
        TD::OP_ParAppendResult res = manager->appendFloat(p);
        assert(res == TD::OP_ParAppendResult::Success);
    }

    {
        TD::OP_NumericParameter p;
        p.name = BetaName;
        p.label = BetaLabel;
        p.page = "Runtime";
        p.defaultValues[0] = 0.1;
        p.minValues[0] = 0.0;
        p.maxValues[0] = 10.0;
        p.clampMins[0] = true;
        p.clampMaxes[0] = false;
        TD::OP_ParAppendResult res = manager->appendFloat(p);
        assert(res == TD::OP_ParAppendResult::Success);
    }

    // Model File Page
    {
        TD::OP_StringParameter p;
        p.name = ModelFileName;
        p.label = ModelFileLabel;
        p.page = "File";
        p.defaultValue = "";
        TD::OP_ParAppendResult res = manager->appendFile(p);
        assert(res == TD::OP_ParAppendResult::Success);
    }

    {
        TD::OP_NumericParameter p;
        p.name = SaveModelName;
        p.label = SaveModelLabel;
        p.page = "File";
        TD::OP_ParAppendResult res = manager->appendPulse(p);
        assert(res == TD::OP_ParAppendResult::Success);
    }

    {
        TD::OP_NumericParameter p;
        p.name = LoadModelName;
        p.label = LoadModelLabel;
        p.page = "File";
        TD::OP_ParAppendResult res = manager->appendPulse(p);
        assert(res == TD::OP_ParAppendResult::Success);
    }
}

#pragma endregion