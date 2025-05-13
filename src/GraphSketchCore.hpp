// -----------------------------------------------------------------------------
// GraphSketchCore: Edge-Based Anomaly Detection with CMSCU (With Temporal Decay + Bayesian Scoring)
//
// Author: Ocheme Anthony Ekle
// Description: Implements real-time anomaly detection using Count-Min Sketch
// with Conservative Update (CMSCU) to track edge frequency patterns in
// dynamic graph streams, while incorporating temporal decay, Bayesian scoring,
// and normalized + fused scoring.
// -----------------------------------------------------------------------------

#pragma once

#include <cmath>
#include <limits>
#include "GraphSketchCMS.hpp"
#include "GraphSketchBayesian.hpp"

namespace GraphSketch
{
    class GraphSketchCore
    {
    private:
        int timestamp = 1; // Tracks the current time window
        int *indices;
        float decayFactor;

        float minScoreSeen = std::numeric_limits<float>::max();
        float maxScoreSeen = std::numeric_limits<float>::lowest();

        CountMinSketchCMSCU sketchCurrent;
        CountMinSketchCMSCU sketchTotal;
        CountMinSketchCMSCU sketchPrevTotal; // For MIDAS-R style smoothing

    public:
        GraphSketchCore(int numRows, int numCols, float decayFactor = 0.15f)
            : indices(new int[numRows]),
              sketchCurrent(numRows, numCols),
              sketchTotal(numRows, numCols),
              sketchPrevTotal(numRows, numCols),
              decayFactor(decayFactor) {}

        ~GraphSketchCore() { delete[] indices; }

        void applyDecay()
        {
            for (int i = 0; i < sketchCurrent.lenData; ++i)
            {
                sketchCurrent.data[i] *= decayFactor;
            }
        }

        static float computeScore(int a, int s, int t)
        {
            return (s == 0 || t - 1 == 0) ? 0.0f : pow((a - s / t) * t, 2) / (s * (t - 1));
        }

        /**
         * @brief Processes edge and returns normalized + fused score.
         * @param src Source node
         * @param dst Destination node
         * @param time Current timestamp
         * @param useBayesian Whether to use Bayesian scoring
         * @return Pair of scores: {normalized_stat_score, final_fused_score}
         */
        std::pair<float, float> processEdge(int src, int dst, int time, bool useBayesian = true)
        {
            if (timestamp < time)
            {
                applyDecay();
                sketchPrevTotal.copyFrom(sketchTotal);
                sketchCurrent.clearAll();
                timestamp = time;
            }

            sketchCurrent.addEdge(src, dst);
            sketchTotal.addEdge(src, dst);

            int freqCurrent = sketchCurrent.estimate(src, dst);
            int freqTotal = sketchTotal.estimate(src, dst);
            int prevFreq = sketchPrevTotal.estimate(src, dst);

            // Raw statistical score
            float raw_score = computeScore(freqCurrent, freqTotal, time);

            // Update min/max for normalization
            minScoreSeen = std::min(minScoreSeen, raw_score);
            maxScoreSeen = std::max(maxScoreSeen, raw_score);

            // Normalized score
            float normalized_score = 0.0f;
            if (maxScoreSeen > minScoreSeen)
                normalized_score = (raw_score - minScoreSeen) / (maxScoreSeen - minScoreSeen);

            // Bayesian score
            float bayes_score = useBayesian
                                    ? GraphSketchBayesian::bayesianFrequencyScore(freqCurrent, freqTotal, 5.0f, 10.0f)
                                    : 0.0f;

            // Final fused score (adjust weights as needed)
            float score = 0.7f * bayes_score + 0.3f * normalized_score;

            return {normalized_score, score};
        }
    };
} // namespace GraphSketch
