// -----------------------------------------------------------------------------
// GraphSketchFiltering: Enhanced Anomaly Filtering using Tensor CMSCU with Decay
//
// Author: Ocheme Anthony Ekle
// Description: This module applies Tensor Count-Min Sketch with Conservative Update (CMSCU)
// and EWMA-based dynamic thresholding to filter and score anomalous edges in real-time.
// -----------------------------------------------------------------------------

#pragma once
#include "GraphSketchTensor.hpp"             // ✅ Tensor sketch manager
#include "GraphSketchThresholding.hpp"       // EWMA Threshold Module
#include <algorithm>
#include <cmath>
#include <utility>
#include <limits>

namespace GraphSketch
{
    class GraphSketchFiltering
    {
    private:
        float decayFactor;
        int lastTimestamp = -1;
        SketchTensorManager sketchTensor;     // ✅ Multi-layer sketch manager
        GraphSketchThresholding ewma;         // Adaptive threshold using EWMA

    public:
        GraphSketchFiltering(int numRow, int numColumn, float initThreshold = 0.5f, float decay = 0.95f)
            : decayFactor(decay),
              sketchTensor(numRow, numColumn, decayFactor, 1.0f),
              ewma(0.9f, initThreshold) // EWMA smoother + initial threshold
        {
        }

        // ✅ Apply decay manually (called externally if needed)
        void applyDecay()
        {
            sketchTensor.applyDecay();
        }

        // ✅ Compute anomaly score using deviation formula
        static float computeScore(float a, float s, float t)
        {
            if (s <= 1e-6f || t <= 1.0f)
                return 0.0f;

            float score = std::pow((a - s / t) * t, 2) / (s * (t - 1));
            return std::isfinite(score) ? score : 0.0f;
        }

        /**
         * @brief Detects anomaly for given edge with timestamp & protocol, applies EWMA threshold.
         *
         * @return pair<max_score, is_anomaly>
         */
        std::pair<float, bool> detect(int source, int destination, int timestamp, int protocolID = 0)
        {
            // Decay and prune when timestamp advances
            if (timestamp > lastTimestamp)
            {
                sketchTensor.applyDecay();
                sketchTensor.pruneOldWindows(timestamp / 300, 5);
                lastTimestamp = timestamp;
            }

            // ✅ Add edge and update sketch
            sketchTensor.addEdge(source, destination, timestamp, protocolID);

            // ✅ Estimate raw frequency score
            float currentFreq = sketchTensor.estimateScore(source, destination, timestamp, protocolID);
            float totalFreq = sketchTensor.estimateTotal(source, destination, protocolID);
            float score = computeScore(currentFreq, totalFreq, timestamp);

            // ✅ Update EWMA and determine anomaly
            float threshold = ewma.update(score);
            bool isAnomaly = score > threshold;

            return {score, isAnomaly};
        }
    };
} // namespace GraphSketch
