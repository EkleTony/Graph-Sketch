// -----------------------------------------------------------------------------
// GraphSketchBayesian: Bayesian Posterior Scoring for Edge Anomalies
//
// Author: Ocheme Anthony Ekle
// Description: Provides Gaussian-based posterior scoring methods for
// dynamic graph anomaly detection, including smoothed memory-aware
// anomaly scoring, with adaptive prior boosting AUC. Also supports
// discrete Bayesian smoothing using Beta-Bernoulli prior.
// -----------------------------------------------------------------------------

#pragma once
#include <cmath>
#include <algorithm>

namespace GraphSketch
{
    class GraphSketchBayesian
    {
    public:
        /**
         * @brief Gaussian Probability Density Function (PDF).
         */
        static float gaussian(float x, float mean, float variance)
        {
            if (variance <= 0.0001f)
                return 0.0f;
            float denom = std::sqrt(2 * M_PI * variance);
            float exponent = -std::pow(x - mean, 2) / (2 * variance);
            return (1.0f / denom) * std::exp(exponent);
        }

        /**
         * @brief Computes Bayesian Posterior Probability of anomaly using Gaussian.
         *
         * @param observed Current frequency (e.g., CMSCU count).
         * @param total Total historical frequency.
         * @param t Current timestamp (time window).
         * @param prior Prior probability of anomaly (default: adaptive).
         * @param delta Shift in anomaly mean from normal mean (default: +1).
         */
        static float posteriorAnomaly(float observed, float total, int t, float prior = -1.0f, float delta = 1.0f)
        {
            if (total == 0 || t <= 1)
                return 0;

            float mu = total / static_cast<float>(t);            // Expected frequency
            float var = std::max(1e-3f, total / (float)(t * t)); // Smoothing variance
            float muA = mu + delta;
            float varA = 2 * var;

            // 📌 Dynamic prior: If prior not manually set (>0), adapt based on activity
            float adaptivePrior = (prior > 0.0f) ? prior : std::min(0.5f, 1.0f / (1.0f + mu));

            float p_normal = gaussian(observed, mu, var);
            float p_anomaly = gaussian(observed, muA, varA);

            float numerator = p_anomaly * adaptivePrior;
            float denominator = numerator + p_normal * (1.0f - adaptivePrior);

            return (denominator == 0) ? 0.0f : numerator / denominator;
        }

        /**
         * @brief MIDAS-R Inspired: Smoothed Posterior using memory of prior total frequencies.
         *
         * @param currentFreq Count from current sketch
         * @param prevTotalFreq Total frequency from previous time tick
         * @param recentTotalFreq Total frequency up to t-1
         * @param t Current timestamp
         * @param prior Prior anomaly probability (default: adaptive)
         * @param delta Mean shift for anomalies (default: 1.0)
         * @param memoryWeight Balance between previous and recent (default: 0.5)
         */
        static float smoothedPosterior(int currentFreq, float prevTotalFreq, float recentTotalFreq, int t,
                                       float prior = -1.0f, float delta = 1.0f, float memoryWeight = 0.5f)
        {
            float combinedTotal = memoryWeight * prevTotalFreq + (1.0f - memoryWeight) * recentTotalFreq;
            return posteriorAnomaly(currentFreq, combinedTotal, t, prior, delta);
        }

        /**
         * @brief Bayesian smoothed frequency-based anomaly score using Beta-Bernoulli model.
         *
         *        This models the expected probability of edge activity using a prior (α, β).
         *        The anomaly score is defined as 1 - smoothed_prob(e), making rare edges stand out.
         *
         * @param edgeCount Observed frequency of the edge.
         * @param totalCount Total observed edge frequency so far.
         * @param alpha Prior strength for the edge (default: 5.0).
         * @param beta Prior strength for other edges (default: 10.0).
         * @return float Smoothed anomaly score (1 - smoothed frequency).
         */
        static float bayesianFrequencyScore(int edgeCount, int totalCount, float alpha = 5.0f, float beta = 10.0f)
        {
            if (totalCount < 0 || edgeCount < 0)
                return 0.0f;

            float numerator = alpha + edgeCount;
            float denominator = alpha + beta + totalCount;

            float smoothed_prob = numerator / denominator;
            return 1.0f - smoothed_prob; // Higher = more anomalous
        }
    };
}
