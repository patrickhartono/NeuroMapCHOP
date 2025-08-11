# TD-NeuroMap CHOP - Proof of Concept

Neural Network Data Mapping CHOP for TouchDesigner, inspired by FluCoMa workflow.

## Current Status: Phase 1 Complete

This is a **Proof of Concept** implementation with basic functionality:

### ✅ Implemented Features (Phase 1)

1. **Basic CHOP Structure**
   - TouchDesigner plugin framework
   - Parameter system with organized pages
   - Mode switching (Collect/Train/Run)

2. **Parameter Interface**
   - **Model Page**: Mode, Input/Output Dimensions, Normalization
   - **Data Page**: Add Sample, Clear Dataset, Dataset Size
   - **Training Page**: Train, Epochs, Learning Rate, Architecture params
   - **Runtime Page**: Smoothing controls  
   - **File Page**: Model save/load (UI only)

3. **Data Collection System**
   - Store input/output pairs from CHOP inputs
   - Data validation and dimension checking
   - Dataset management with normalization support

4. **Basic Mode Logic**
   - Mode switching with state management
   - Parameter validation
   - Logging and debugging output

### 🚧 Next Phases

**Phase 2: MiniDNN Integration**
- Neural network creation and training
- Eigen matrix conversions
- Real inference pipeline

**Phase 3: Advanced Features** 
- Threading for background training
- OneEuro smoothing filters
- JSON model persistence
- Dynamic channel naming

## Build Instructions

### Prerequisites
- macOS 10.13+
- Xcode Command Line Tools
- CMake 3.10+
- TouchDesigner 2023.11+

### Build Steps

```bash
cd NeuroMapCHOP
./build.sh
```

### Installation
1. Copy `build/NeuroMapCHOP.plugin` to TouchDesigner Plugins folder
2. Restart TouchDesigner  
3. Create CHOP node → Set Type to "Neuromap"

## Usage (Current)

### Data Collection Mode
1. Set Mode to "Collect"
2. Connect input features to Input 1  
3. Connect target outputs to Input 2
4. Set Input/Output Dimensions
5. Click "Add Sample" to store data pairs

### Training Mode (Simulated)
1. Set Mode to "Train" 
2. Configure training parameters
3. Click "Train" (currently simulated)

### Run Mode (Pass-through)
1. Set Mode to "Run"
2. Input data flows through with basic transform

## Project Structure

```
NeuroMapCHOP/
├── NeuroMapCHOP.h/cpp      # Main CHOP class
├── Parameters.h/cpp        # Parameter system  
├── DataManager.h/cpp       # Data collection & normalization
├── CMakeLists.txt          # Build configuration
├── build.sh               # Build script
└── README.md              # This file
```

## Development Notes

- **Thread Safety**: Current implementation is single-threaded
- **Performance**: Not optimized for real-time yet
- **Error Handling**: Basic validation only
- **Testing**: Manual testing required

## Next Steps for Phase 2

1. Integrate MiniDNN for actual neural network training
2. Implement Eigen ↔ TouchDesigner data conversion  
3. Add background training thread
4. Implement real inference pipeline
5. Add proper error handling and validation

## Debugging

Enable console output to see logging:
```bash
# Run TouchDesigner from terminal to see logs
/Applications/TouchDesigner.app/Contents/MacOS/TouchDesigner
```

Look for `[NeuroMapCHOP]` prefix in console output.