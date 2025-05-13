#pragma once
#include "GraphSketchCMS.hpp"
#include <unordered_map>

namespace GraphSketch {

    class SketchTensorManager {
    private:
        int r, c;
        float decay;
        float step;

        // ✅ Fix: Declare the tensorSketch map
        // Format: time_window → (protocol_id → sketch)
        std::unordered_map<int, std::unordered_map<int, CountMinSketchCMSCU>> tensorSketch;

    public:
        SketchTensorManager(int rows, int cols, float decayFactor = 0.95f, float stepSize = 1.0f);

        void addEdge(int src, int dst, int timestamp, int protoID);
        float estimateScore(int src, int dst, int timestamp, int protoID);
        float estimateTotal(int src, int dst, int protoID); // ✅ Added
        void applyDecay();
        void pruneOldWindows(int currentWindow, int keepLastN = 5);
    };

} // namespace GraphSketch
