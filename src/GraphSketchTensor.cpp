// -----------------------------------------------------------------------------
// GraphSketchTensor.cpp
//
// Implements multi-layer Count-Min Sketch (CMSCU) for edge stream anomaly detection.
// -----------------------------------------------------------------------------

#include "GraphSketchTensor.hpp"
#include <algorithm>
#include <iostream>

namespace GraphSketch {

    // Constructor
    SketchTensorManager::SketchTensorManager(int rows, int cols, float decayFactor, float stepSize)
        : r(rows), c(cols), decay(decayFactor), step(stepSize) {}

    // Helper: bin timestamp into a 5-minute window
    int getTimeWindow(int timestamp) {
        return timestamp / 300; // 250 best for cic-ids // darpa =40 // ids-18 = 
    }

    // ✅ Add edge to sketch (safe against default construction)
    void SketchTensorManager::addEdge(int src, int dst, int timestamp, int protoID) {
        int timeWindow = getTimeWindow(timestamp);

        auto twIt = tensorSketch.find(timeWindow);
        if (twIt == tensorSketch.end()) {
            // First time window ever seen — construct proto map with the sketch
            std::unordered_map<int, CountMinSketchCMSCU> protoMap;
            protoMap.emplace(protoID, CountMinSketchCMSCU(r, c, decay, step));
            tensorSketch.emplace(timeWindow, std::move(protoMap));
        } else {
            // Time window exists
            auto& protoMap = twIt->second;
            auto protoIt = protoMap.find(protoID);
            if (protoIt == protoMap.end()) {
                protoMap.emplace(protoID, CountMinSketchCMSCU(r, c, decay, step));
            }
            protoMap.at(protoID).addEdge(src, dst);
        }
    }

    // Estimate anomaly score safely
    float SketchTensorManager::estimateScore(int src, int dst, int timestamp, int protoID) {
        int timeWindow = getTimeWindow(timestamp);

        auto twIt = tensorSketch.find(timeWindow);
        if (twIt == tensorSketch.end()) return 0.0f;

        auto& protoMap = twIt->second;
        auto protoIt = protoMap.find(protoID);
        if (protoIt == protoMap.end()) return 0.0f;

        return protoIt->second.estimate(src, dst);
    }

    // Apply decay to all sketches
    void SketchTensorManager::applyDecay() {
        for (auto& [t, protoMap] : tensorSketch) {
            for (auto& [p, sketch] : protoMap) {
                sketch.applyDecay();
            }
        }
    }

    // Remove sketches older than N time windows
    void SketchTensorManager::pruneOldWindows(int currentWindow, int keepLastN) {
        std::vector<int> toRemove;
        for (const auto& [window, _] : tensorSketch) {
            if (window < currentWindow - keepLastN) {
                toRemove.push_back(window);
            }
        }

        for (int win : toRemove) {
            tensorSketch.erase(win);
        }
    }

} // namespace GraphSketch
