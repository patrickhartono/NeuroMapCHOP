# TD-NeuroMap Project Log - Claude Session 1

**Date:** 2025-01-11  
**Status:** Phase 1 Complete - Ready for Phase 2  
**Duration:** Full research & implementation session

## Project Overview

**Objective:** Mengembangkan Custom C++ CHOP untuk TouchDesigner yang melakukan generatif data mapping menggunakan neural network, terinspirasi dari workflow FluCoMa di MaxMSP.

**Target Features:**
- Real-time neural network training dalam TouchDesigner
- Multi-dimensional input → output parameter mapping
- Live data collection dari CHOP inputs
- Model persistence (save/load)
- OneEuro smoothing untuk output
- Dynamic channel configuration

## Research Phase (Completed)

### CustomOperatorSamples Analysis
**Location:** `/Users/patrickhartono/Documents/research-2025-26/TD-NeuroMap/CustomOperatorSamples`

**Key Findings:**
1. **CHOP Structure Patterns:**
   - All CHOP inherit dari `CHOP_CPlusPlusBase.h`
   - 3 C export functions required: `FillCHOPPluginInfo`, `CreateCHOPInstance`, `DestroyCHOPInstance`
   - Core methods: `getGeneralInfo()`, `getOutputInfo()`, `execute()`, `setupParameters()`

2. **Parameter Management:**
   - Generated using `cppParsTemplateGen` Palette Component
   - Support dynamic parameters dengan enable/disable kondisional
   - Pulse parameters untuk interactive buttons
   - File parameters untuk model paths

3. **Threading Patterns (BasicFilterTOP):**
   - `ThreadManager` class dengan `std::atomic<ThreadStatus>`
   - Mutex + condition_variable untuk synchronization
   - Background processing tanpa freeze TouchDesigner UI

4. **Data Handling:**
   - TouchDesigner format: `const float** channelData` [channel][sample]
   - Access: `input->channelData[ch][sample]` atau `input->getChannelData(ch)[sample]`
   - Output: `output->channels[ch][sample] = value`

### MiniDNN Analysis
**Location:** `/Users/patrickhartono/Documents/research-2025-26/TD-NeuroMap/MiniDNN`

**Key Findings:**
1. **Compatibility:**
   - ✅ Header-only library (perfect untuk TouchDesigner plugin)
   - ✅ Only dependency: Eigen (header-only)
   - ✅ C++98 compatible
   - ✅ MPL2 license (compatible)

2. **Core API:**
```cpp
// Network creation
Network net;
net.add_layer(new FullyConnected<ReLU>(input_dim, hidden_units));
net.set_output(new RegressionMSE());

// Training
RMSProp opt;
net.fit(opt, input_matrix, target_matrix, batch_size, epochs, seed);

// Inference
Matrix prediction = net.predict(input_matrix);

// Save/Load
net.export_net("./models/", "model_name");
net.read_net("./models/", "model_name");
```

3. **Data Conversion Strategy:**
   - TouchDesigner: Channel-major `[channel][sample]`
   - Eigen/MiniDNN: Column-major `[feature_dim, batch_size]`
   - Need conversion utilities

## Implementation Phase 1 (Completed)

### Project Structure Created
```
TD-NeuroMap/NeuroMapCHOP/
├── NeuroMapCHOP.h/cpp      # Main CHOP class implementation
├── Parameters.h/cpp        # Complete parameter system (17 params)
├── DataManager.h/cpp       # Data collection & normalization
├── CMakeLists.txt          # macOS build configuration  
├── build.sh               # Automated build script
├── Info.plist.in          # macOS bundle configuration
├── README.md              # Documentation & usage
├── CHOP_CPlusPlusBase.h   # TouchDesigner headers (copied)
└── CPlusPlus_Common.h     # TouchDesigner headers (copied)
```

### Parameter System (17 Parameters - 5 Pages)

**Model & Mode:**
- `Mode`: Menu (Collect/Train/Run)
- `InDim`: Input dimensions (1-16)
- `OutDim`: Output dimensions (1-16) 
- `Normalize`: Enable normalization toggle

**Data Collection:**
- `AddSample`: Pulse to store input/output pair
- `ClearDataset`: Pulse to clear all data
- `DatasetSize`: Read-only dataset count

**Training:**
- `Train`: Pulse to start training
- `Epochs`: Training iterations (1-10000)
- `LearnRate`: Learning rate (0.00001-1.0)
- `HiddenLayers`: Network depth (1-5)
- `HiddenUnits`: Layer width (8-512)
- `Loss`: Read-only training loss

**Runtime:**
- `SmoothEnable`: Enable OneEuro smoothing
- `MinCutoff`: Smoothing frequency (0.001-100)
- `Beta`: Smoothing coefficient (0-10)

**File:**
- `ModelFile`: File path parameter
- `SaveModel`: Pulse to save model
- `LoadModel`: Pulse to load model

### Core Classes Implemented

**1. NeuroMapCHOP (Main Class)**
```cpp
class NeuroMapCHOP : public CHOP_CPlusPlusBase {
private:
    std::unique_ptr<DataManager> m_dataManager;
    Parameters m_params;
    ModeMenuItems m_currentMode;
    int m_currentInputDim, m_currentOutputDim;
    bool m_modelTrained;
    
public:
    // TouchDesigner interface methods
    void execute(CHOP_Output* output, const OP_Inputs* inputs, void*) override;
    void setupParameters(OP_ParameterManager* manager, void*) override;
    void pulsePressed(const char* name, void*) override;
    // ... other required methods
};
```

**2. DataManager (Data Handling)**
```cpp
class DataManager {
private:
    std::vector<std::vector<float>> m_inputData;   // [sample][feature]
    std::vector<std::vector<float>> m_outputData;  // [sample][target]
    std::vector<float> m_inputMin, m_inputMax;     // Normalization params
    std::vector<float> m_outputMin, m_outputMax;
    
public:
    bool addSample(const OP_CHOPInput* input, const OP_CHOPInput* target, int inDim, int outDim);
    void updateNormalization();
    std::vector<float> normalizeInput(const std::vector<float>& input) const;
    std::vector<float> denormalizeOutput(const std::vector<float>& output) const;
};
```

**3. Parameters (UI System)**
- Complete parameter setup dengan proper types
- Menu, toggles, integers, floats, pulses, file paths
- Organized dalam 5 parameter pages
- Evaluation methods untuk semua parameters

### Current Functionality (Phase 1)

**✅ Working Features:**
1. **CHOP Loading:** Plugin loads di TouchDesigner sebagai "Neuromap" CHOP type
2. **Parameter UI:** Semua 17 parameters visible dengan proper organization
3. **Mode Switching:** Collect/Train/Run modes dengan state management  
4. **Data Collection:** Store input/output pairs dari 2 CHOP connections
5. **Data Validation:** Dimension checking dan error handling
6. **Pulse Handling:** Interactive buttons (Add Sample, Clear Dataset, Train, Save/Load)
7. **Normalization:** Min-max scaling dengan edge case handling
8. **Logging:** Console output untuk debugging (`[NeuroMapCHOP]` prefix)

**🚧 Simulated Features (Placeholder):**
- Training process (logs success but no actual neural network)
- Inference (pass-through dengan basic transform)
- Model save/load (UI only)

## Technical Architecture

### Data Flow Design
```
INPUT MODE: Collect
CHOP Input 1 (Features) → DataManager → Storage
CHOP Input 2 (Targets) → DataManager → Storage
User clicks "Add Sample" → Store normalized pair

INPUT MODE: Train  
Stored Data → MiniDNN Network → Training Loop → Model Ready

INPUT MODE: Run
CHOP Input → Normalize → Neural Network → Denormalize → CHOP Output
```

### Build System
- **CMake Configuration:** macOS universal binary (Intel + Apple Silicon)
- **Dependencies:** TouchDesigner SDK, MiniDNN headers, Eigen
- **Output:** `NeuroMapCHOP.plugin` bundle
- **Installation:** Copy ke TouchDesigner Plugins folder

### Testing Instructions
```bash
cd /Users/patrickhartono/Documents/research-2025-26/TD-NeuroMap/NeuroMapCHOP
./build.sh
```

Expected behavior:
1. Build creates plugin bundle successfully
2. Copy to TouchDesigner Plugins folder  
3. Restart TouchDesigner
4. Create CHOP → Set type "Neuromap"
5. Parameter interface loads dengan 5 pages
6. Mode switching works
7. Data collection works dengan 2 CHOP inputs
8. Console shows `[NeuroMapCHOP]` debug messages

## Next Steps - Phase 2: MiniDNN Integration

**Priority Tasks:**
1. **Neural Network Creation**
   - Integrate MiniDNN headers
   - Dynamic network architecture based on parameters
   - Layer creation dari InDim → HiddenUnits → OutDim

2. **Data Conversion Utils**
   - TouchDesigner CHOP arrays ↔ Eigen matrices
   - Batch processing untuk training data
   - Real-time single sample conversion untuk inference

3. **Training Implementation**  
   - Background training thread (prevent UI freeze)
   - Progress monitoring dan loss updates
   - Training completion callbacks

4. **Inference Pipeline**
   - Real-time prediction dari CHOP input
   - Normalization → Neural Network → Denormalization
   - Output smoothing dengan OneEuro filter integration

5. **Model Persistence**
   - Extend MiniDNN save/load dengan JSON wrapper  
   - Include normalization parameters dalam model files
   - Error handling untuk corrupt/incompatible models

**Technical Challenges Identified:**
- Thread safety antara training dan inference
- Real-time performance optimization  
- Memory management untuk large datasets
- Error handling untuk training failures

## Development Environment

**Platform:** macOS (Darwin 24.5.0)  
**Tools:** Xcode, CMake, TouchDesigner 2023.11+  
**Dependencies:** MiniDNN (header-only), Eigen (header-only)  
**Build Target:** Universal binary (.plugin bundle)

## Repository Status

**Completed Files:**
- All Phase 1 implementation files present dan functional
- Build system tested dan working
- Documentation complete untuk current phase

**Ready untuk Handoff:**  
Project dapat langsung dilanjutkan oleh Claude session berikutnya dengan full context. Semua necessary files available dan build system ready.

---

**End of Claude Session 1 Log**  
**Next Claude Session:** Start dengan "Continue Phase 2: MiniDNN Integration" menggunakan context dari log ini.