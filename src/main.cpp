// -----------------------------------------------------------------------------
// GraphSketch: Streaming Anomaly Detection in Dynamic Graphs
//
// Author: Ocheme Anthony Ekle
// Description: This program reads a dataset of dynamic graph edges (source,
// destination, timestamp) and applies the Count-Min Sketch with Conservative
// Update (CMSCU) in a tensor format (time × protocol) for real-time anomaly
// detection. Results are logged to `score.txt` and evaluated using
// Python evaluation metrics (AUC-ROC, Precision@K, Recall@K, Average Precision).
//
// License: Apache License 2.0
// -----------------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <cstdlib>
#include "GraphSketchTensor.hpp"          // Multi-layer sketch manager

using namespace GraphSketch;              // use GraphSketch namespace
using namespace std::chrono;

int main()
{
    // Start total timing
    auto startTotalTime = high_resolution_clock::now();
    auto startProcessingTime = high_resolution_clock::now();

    // ✅ Initialize tensor-based sketch manager
    SketchTensorManager sketchTensor(3, 800, 0.98f, 15.0f);
    // c = number of hash rows (r)  // ISCX-ids (3, 800, 0.98, 15.0) and alpah = 0.85 and auc = 0.843
    // c = umber of columns (c)  
    // 0.95f =decay factor
    // step  = how much we increment anomaly score per sketch update


    // ✅ Open dataset (you can switch by commenting/uncommenting below)
    // std::ifstream file("data/DARPA/darpa_edges.txt");           // ✅ DARPA
    std::ifstream file("data/ISCX_IDS2012/data_ids2012.csv"); // ISCX-IDS 2012
    // std::ifstream file("data/CIC_IDS2017/friday_edges.txt");  // CIC-IDS 2017 (Friday)
    // std::ifstream file("data/CIC_IDS2017/tuesday_edges.txt"); // CIC-IDS 2017 (Tuesday)
    // std::ifstream file("data/CIC_IDS2017/combined_edges.txt");// CIC-IDS 2017 (All)
    // std::ifstream file("data/dataset/ctu_edges.csv");         // CTU-13

    if (!file.is_open())
    {
        std::cerr << "Error: Could not open dataset file." << std::endl;
        return 1;
    }
    std::cout << "Dataset file opened successfully." << std::endl;

    // ✅ Prepare output file for anomaly scores
    std::ofstream scoreFile("score.txt");
    if (!scoreFile.is_open())
    {
        std::cerr << "Error: Could not open score output file." << std::endl;
        return 1;
    }

    int src, dst, timestamp;
    std::string line;
    int edgeCount = 0;

    // ✅ EWMA parameters (temporal smoothing)
    float alpha = 0.85f;         // EWMA smoothing factor // 0.85 for darpa
    float ema_score = 0.1f;     // Current EWMA score

    // ✅ Read and process each edge (src, dst, timestamp)
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        char comma;

        // Parse CSV line
        if (!(iss >> src >> comma >> dst >> comma >> timestamp)) {
            std::cerr << "Skipping malformed line: " << line << std::endl;
            continue;
        }

        int protocolID = 0;  // default protocol ID (could be updated later if needed)

        // ✅ Update sketch
        sketchTensor.addEdge(src, dst, timestamp, protocolID);

        // ✅ Get raw anomaly score
        float raw_score = sketchTensor.estimateScore(src, dst, timestamp, protocolID);

        // ✅ Apply EWMA smoothing to raw score
        ema_score = (edgeCount == 0) ? raw_score : (alpha * raw_score + (1 - alpha) * ema_score);

        // ✅ Write smoothed score to file
        scoreFile << src << "," << dst << "," << timestamp << "," << ema_score << "\n";

        edgeCount++;

        // ✅ Apply decay and prune old sketches every 10K edges
        if (edgeCount % 10000 == 0) {
            sketchTensor.applyDecay();
            sketchTensor.pruneOldWindows(timestamp / 300, 10);  // Keep last 5 windows
        }
    }

    auto endProcessingTime = high_resolution_clock::now();
    std::chrono::duration<double> processingDuration = endProcessingTime - startProcessingTime;

    file.close();
    scoreFile.close();

    std::cout << "\nFinished processing " << edgeCount << " edges." << std::endl;
    std::cout << "Graph Processing time: " << processingDuration.count() << " seconds." << std::endl;
    std::cout << "Anomaly scores saved to 'score.txt'." << std::endl;

    // ✅ Start evaluation phase
    auto startEvalTime = high_resolution_clock::now();
    std::cout << "\nGraph Processing completed-- Now Running Python evaluation..." << std::endl;

    // ✅ Run Python evaluation (edit label file path if switching dataset)

    // 1. DARPA
    // int eval_status = system("python3 src/GraphSketch_eval.py score.txt data/DARPA/darpa_ground_truth.csv");

    // 2. ISCX-IDS 2012
    int eval_status = system("python3 src/GraphSketch_eval.py score.txt data/ISCX_IDS2012/label_ids2012.csv");

    // 3. cic-IDS 2017
    // int eval_status = system("python3 src/GraphSketch_eval.py score.txt data/CIC_IDS2017/friday_labels.csv");

    // CTU-13
    //int eval_status = system("python3 src/GraphSketch_eval.py score.txt data/dataset/ctu_labels.csv");

    if (eval_status != 0)
    {
        std::cerr << "Python evaluation script failed. Please check your paths or script." << std::endl;
    }
    else
    {
        std::cout << "Python evaluation completed successfully." << std::endl;
    }

    auto endEvalTime = high_resolution_clock::now();
    auto endTotalTime = high_resolution_clock::now();

    std::chrono::duration<double> evalDuration = endEvalTime - startEvalTime;
    std::chrono::duration<double> totalDuration = endTotalTime - startTotalTime;

    std::cout << "\n========= Timing Summary =========" << std::endl;
    std::cout << "Graph Processing Time: " << processingDuration.count() << " seconds" << std::endl;
    std::cout << "Evaluation Time: " << evalDuration.count() << " seconds" << std::endl;
    std::cout << "TOTAL Time (Processing + Evaluation): " << totalDuration.count() << " seconds" << std::endl;
    std::cout << "===================================" << std::endl;

    return 0;
}
