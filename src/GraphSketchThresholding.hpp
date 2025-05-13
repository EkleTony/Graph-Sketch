// -----------------------------------------------------------------------------
// GraphSketchThresholding: EWMA-Based Dynamic Thresholding
//
// Author: Ocheme Anthony Ekle
// Description: Applies Exponentially Weighted Moving Average (EWMA)
// to adaptively adjust the anomaly score threshold over time.
// -----------------------------------------------------------------------------

#pragma once
#include <deque>
#include <cmath>
#include <algorithm>

namespace GraphSketch
{
    class GraphSketchThresholding
    {
    private:
        float ewmaThreshold; // Current dynamic threshold
        float alpha;         // EWMA decay factor (e.g., 0.3 to 0.7)
        bool initialized;

    public:
        /**
         * @brief Constructor
         * @param alpha EWMA decay factor (0 < alpha <= 1)
         * @param initThreshold Initial threshold value
         */
        GraphSketchThresholding(float alpha = 0.65, float initThreshold = 0.45)
            : ewmaThreshold(initThreshold), alpha(alpha), initialized(false) {}

        /**
         * @brief Update EWMA threshold based on new anomaly score
         * @param score The latest anomaly score
         * @return The updated dynamic threshold
         */
        float update(float score)
        {
            if (!initialized)
            {
                ewmaThreshold = score; // Initialize with first score
                initialized = true;
            }
            else
            {
                ewmaThreshold = alpha * score + (1 - alpha) * ewmaThreshold;
            }
            return ewmaThreshold;
        }

        /**
         * @brief Get the current threshold
         */
        float getThreshold() const
        {
            return ewmaThreshold;
        }
    };
} // namespace GraphSketch
