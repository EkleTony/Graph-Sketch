# GraphSketch: Real-Time Anomaly Detection in Dynamic Graphs

GraphSketch is a memory-efficient anomaly detection system for **dynamic graph streams**. It leverages **Count-Min Sketch with Conservative Update (CMSCU)** to enable scalable, real-time anomaly detection under strict memory constraints.

This work was presented and published at **IEEE ICKG 2025**.

📄 [Read the Paper (arXiv)](https://arxiv.org/pdf/2509.11633)

## GraphSketch Framework Overview

![GraphSketch Framework](Graphsketch_framework.jpg)

*Figure 1: End-to-end pipeline for streaming edge ingestion, multi-layer hashing, 3D tensor sketch construction (CMSCU), temporal decay with sliding windows, and Bayesian threshold-based anomaly detection.*

## Features

- **Streaming anomaly detection** for large graphs
- **Low-memory footprint** using Count-Min Sketch
- **Scalable & efficient** processing
- **Flexible dataset support**, including DARPA and custom data.

## Project Structure

```
GraphSketch/
│── src/                   # Source files
│   ├── GraphSketchCMS.hpp   # Count-Min Sketch implementation
│   ├── GraphSketchCore.hpp  # Core anomaly detection
│   ├── GraphSketchFiltering.hpp  # Filtering logic
│   ├── main.cpp             # Program entry
│── data/                   # Dataset folder
│   ├── DARPA/              # DARPA datasets
│── README.md               # Documentation
│── graphsketch             # Compiled executable
```

## Build & Run

## Installation & Build

### Prerequisites

- C++11 or later
- CMake
- A C++ compiler (GCC, Clang, or MSVC)

### Steps

````sh
cd GraphSketchProject
g++ -o graphsketch src/*.cpp -std=c++11
./graphsketch 

### Running the Program
```sh
./graphsketch data/DARPA/darpa_edges.csv
````

## Contributing

Pull requests are welcome!

## License

Apache License 2.0

## References & Acknowledgments

GraphSketch is inspired by **MIDAS (Microcluster-Based Detector of Anomalies in Edge Streams)**, an anomaly detection framework for streaming graphs.
 📄 [Read the Paper](https://arxiv.org/pdf/2009.08452)       

### 📂 GitHub Repository

We acknowledge the contributions of **MIDAS**, as GraphSketch extends and builds upon their ideas with **Count-Min Sketch with Conservative Update (CMSCU)**, Bayesian inference, and dynamic thresholding.
