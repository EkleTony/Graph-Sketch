// -----------------------------------------------------------------------------
<<<<<<< HEAD
// GraphSketchFiltering: Enhanced Anomaly Filtering using Tensor CMSCU with Decay
//
// Author: Ocheme Anthony Ekle
// Description: This module applies Tensor Count-Min Sketch with Conservative Update (CMSCU)
=======
<<<<<<< HEAD
// GraphSketchFiltering: Enhanced Anomaly Filtering using CMSCU with Temporal Decay
//
// Author: Ocheme Anthony Ekle
// Description: This module applies Count-Min Sketch with Conservative Update (CMSCU)
=======
// GraphSketchFiltering: Enhanced Anomaly Filtering using Tensor CMSCU with Decay
//
// Author: Ocheme Anthony Ekle
// Description: This module applies Tensor Count-Min Sketch with Conservative Update (CMSCU)
>>>>>>> e0f993a (local changes before pulling)
>>>>>>> 724517a (adding darpa data)
// and EWMA-based dynamic thresholding to filter and score anomalous edges in real-time.
// -----------------------------------------------------------------------------

#pragma once
<<<<<<< HEAD
#include "GraphSketchTensor.hpp"             // ✅ Tensor sketch manager
#include "GraphSketchThresholding.hpp"       // EWMA Threshold Module
=======
<<<<<<< HEAD
#include "GraphSketchCMS.hpp"
#include "GraphSketchBayesian.hpp"
#include "GraphSketchThresholding.hpp" // EWMA Threshold Module
=======
#include "GraphSketchTensor.hpp"             // ✅ Tensor sketch manager
#include "GraphSketchThresholding.hpp"       // EWMA Threshold Module
>>>>>>> e0f993a (local changes before pulling)
>>>>>>> 724517a (adding darpa data)
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
<<<<<<< HEAD
        int lastTimestamp = -1;
        SketchTensorManager sketchTensor;     // ✅ Multi-layer sketch manager
        GraphSketchThresholding ewma;         // Adaptive threshold using EWMA
=======
<<<<<<< HEAD
        int timestamp = 1;

        int *indexEdge;
        int *indexSource;
        int *indexDestination;

        CountMinSketchCMSCU numCurrentEdge, numTotalEdge;
        CountMinSketchCMSCU numCurrentSource, numTotalSource;
        CountMinSketchCMSCU numCurrentDestination, numTotalDestination;

        GraphSketchThresholding ewma; // Adaptive threshold using EWMA
>>>>>>> 724517a (adding darpa data)

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

<<<<<<< HEAD
        // ✅ Compute anomaly score using deviation formula
=======
        /**
         * @brief Computes the anomaly score for an edge or node using Bayesian-inspired
         *        statistical deviation with temporal normalization.
         *
         * @param a Current count estimate (e.g., from Count-Min Sketch at this time)
         * @param s Total historical count (cumulative over time)
         * @param t Current timestamp (time step)
         *
         * @return Anomaly score: higher values indicate higher deviation from expected behavior
         */
        
=======
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
>>>>>>> e0f993a (local changes before pulling)
>>>>>>> 724517a (adding darpa data)
        static float computeScore(float a, float s, float t)
        {
            if (s <= 1e-6f || t <= 1.0f)
                return 0.0f;

            float score = std::pow((a - s / t) * t, 2) / (s * (t - 1));
            return std::isfinite(score) ? score : 0.0f;
        }

        /**
<<<<<<< HEAD
         * @brief Detects anomaly for given edge with timestamp & protocol, applies EWMA threshold.
=======
<<<<<<< HEAD
         * @brief Detects anomaly and applies EWMA dynamic threshold.
>>>>>>> 724517a (adding darpa data)
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

<<<<<<< HEAD
            return {score, isAnomaly};
=======
            float edgeScore = computeScore(numCurrentEdge.estimate(source, destination),
                                           numTotalEdge.estimate(source, destination), time);

            float sourceScore = computeScore(numCurrentSource.estimate(source, 0),
                                             numTotalSource.estimate(source, 0), time);

            float destinationScore = computeScore(numCurrentDestination.estimate(destination, 0),
                                                  numTotalDestination.estimate(destination, 0), time);

            float maxScore = std::max({edgeScore, sourceScore, destinationScore});

            // Update EWMA threshold dynamically
            float currentThreshold = ewma.update(maxScore);
            bool isAnomaly = maxScore > currentThreshold;

            return {maxScore, isAnomaly};
=======
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
>>>>>>> e0f993a (local changes before pulling)
>>>>>>> 724517a (adding darpa data)
        }
    };
} // namespace GraphSketch
