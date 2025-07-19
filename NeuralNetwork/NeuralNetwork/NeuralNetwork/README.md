
# NeuralNetworkMPI

A high-performance C++ implementation of a feedforward neural network supporting mini-batch training, modular layers, multiple activation/loss functions, and optional MPI-based parallelism.

---

## 🚀 Features

- **Modular architecture** with support for:
  - Multiple layers (`LayerBatchEfficient`, `LastLayerBatchEfficient`)
  - Customizable activation and loss functions
  - Optimizer abstraction (`SGD`, `Momentum`)
- **Efficient batch computation** using Eigen
- **Robust file-based input loading**
- **MPI support** for distributed training using `MPI_Allreduce`
- Modern C++ (C++17) with `std::unique_ptr`, RAII, and clear resource management

---

## 📁 Project Structure

```
.
├── main.cpp                      # Entry point
├── NeuralNetwork.*              # Core training logic
├── NeuralNetworkMPI.*           # MPI-enabled training logic
├── LayerBatchEfficient.*        # General layer class
├── LastLayerBatchEfficient.*    # Special last layer (e.g., softmax + loss)
├── Activations.*                # Activation functions
├── Loss.*                       # Loss functions
├── Optimizers.*                 # Optimizers (SGD, Momentum)
├── Input.*                      # File-based input handling
├── InputMPI.*                   # MPI input partitioning
```

---

### 🧱 Data
train.csv and test.csv files were taken from the Kaggle Tabular Playground Series - Nov 2021 

https://www.kaggle.com/competitions/tabular-playground-series-nov-2021/ 

under the Attribution 4.0 International (CC BY 4.0) license 

https://creativecommons.org/licenses/by/4.0/




## ⚙️ Build Instructions

### 🧱 Requirements

- C++17 compatible compiler (e.g., `g++ >= 9`, `clang++`, MSVC)
- [Eigen](https://eigen.tuxfamily.org/) (header-only)
- MPI library (e.g., OpenMPI, MPICH)

### 🧪 Build (Serial)

```bash
g++ -std=c++17 -I /path/to/eigen \
    main.cpp NeuralNetwork.cpp LayerBatchEfficient.cpp LastLayerBatchEfficient.cpp \
    Activations.cpp Loss.cpp Optimizers.cpp Input.cpp \
    -o neural_net
```

### 🚀 Build with MPI

```bash
mpic++ -std=c++17 -I /path/to/eigen \
    main.cpp NeuralNetworkMPI.cpp LayerBatchEfficient.cpp LastLayerBatchEfficient.cpp \
    Activations.cpp Loss.cpp Optimizers.cpp InputMPI.cpp \
    -o neural_net_mpi
```

---

## 📦 Example Usage

Make sure your input files (e.g., `input.csv`, `labels.csv`) are properly formatted.

```bash
./neural_net
```

Or run in parallel:

```bash
mpirun -np 4 ./neural_net_mpi
```

---

## 🧠 Design Highlights

- **Extensible optimizers** via `std::unique_ptr<Optimizer>`
- **Enum-based activations and losses** for simplicity
- **Clear memory ownership** with smart pointers
- **MPI backend** uses `MPI_Allreduce` to synchronize gradients across ranks

---

## 📌 TODOs and Suggestions

- [ ] Add support for Adam, RMSProp
- [ ] JSON-based config loading
- [ ] Layer normalization or dropout
- [ ] Multi-threaded parallelism (OpenMP) for matrix ops
- [ ] Logging and training visualization

---

## 📄 License

This project is currently proprietary/unlicensed. Contact the author for usage rights.

---

## 👤 Author

Mahdi Tavakol  
University of Oxford  
Email: mahdi.tavakol@eng.ox.ac.uk

---

## 🧪 Disclaimer

This codebase is meant for research, prototyping, and educational purposes. It is not optimized for production use.
