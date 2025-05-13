// -----------------------------------------------------------------------------
// GraphSketch: Count-Min Sketch with Conservative Update (CMSCU) - Enhanced
//
// Author: O.A.Ekle
// Description: Implements Count-Min Sketch with Conservative Update (CMSCU)
// for streaming graph data, tracking edge frequencies efficiently with
// temporal decay and spatial relations handling.
//
// License: Apache License 2.0
// -----------------------------------------------------------------------------

#pragma once
#include <vector>
#include <algorithm>
#include <limits>
#include <iostream>

namespace GraphSketch
{
    struct CountMinSketchCMSCU
    {
        const int r, c, m = 104729;                                               // Number of rows, columns, and a large prime number for hashing
        const int lenData;                                                        // Total number of counters = r * c
        int *param1;                                                              // Hash function coefficients (a)
        int *param2;                                                              // Hash function coefficients (b)
        float *data;                                                              // Main sketch data array
        constexpr static float infinity = std::numeric_limits<float>::infinity(); // Infinity used for min comparison
        float decayFactor;                                                        // Temporal decay factor (used in applyDecay)
        float updateStep;                                                         // ⏎ NEW: Step size for Smoothed Conservative Update (default: 1.0f)

        /**
         * @brief Constructor to initialize the Count-Min Sketch
         */
        CountMinSketchCMSCU(int numRow, int numColumn, float decayFactor = 0.95f, float step = 55.0f)
            : r(numRow),
              c(numColumn),
              lenData(r * c),
              param1(new int[r]),
              param2(new int[r]),
              data(new float[lenData]),
              decayFactor(decayFactor),
              updateStep(step)
        {
            for (int i = 0; i < r; i++)
            {
                param1[i] = rand() + 1;
                param2[i] = rand();
            }
            std::fill(data, data + lenData, 0);
        }

        /**
         * @brief Destructor
         */
        ~CountMinSketchCMSCU()
        {
            delete[] param1;
            delete[] param2;
            delete[] data;
        }

        // ❌ Disable copy constructor and assignment
        CountMinSketchCMSCU(const CountMinSketchCMSCU &) = delete;
        CountMinSketchCMSCU &operator=(const CountMinSketchCMSCU &) = delete;

        // ✅ Move constructor
        CountMinSketchCMSCU(CountMinSketchCMSCU &&other) noexcept
            : r(other.r), c(other.c), m(other.m), lenData(other.lenData),
              param1(other.param1), param2(other.param2), data(other.data),
              decayFactor(other.decayFactor), updateStep(other.updateStep)
        {
            other.param1 = nullptr;
            other.param2 = nullptr;
            other.data = nullptr;
        }

        // ✅ Move assignment
        CountMinSketchCMSCU &operator=(CountMinSketchCMSCU &&other) noexcept
        {
            if (this != &other)
            {
                delete[] param1;
                delete[] param2;
                delete[] data;

                const_cast<int &>(r) = other.r;
                const_cast<int &>(c) = other.c;
                const_cast<int &>(m) = other.m;
                const_cast<int &>(lenData) = other.lenData;

                param1 = other.param1;
                param2 = other.param2;
                data = other.data;

                decayFactor = other.decayFactor;
                updateStep = other.updateStep;

                other.param1 = nullptr;
                other.param2 = nullptr;
                other.data = nullptr;
            }
            return *this;
        }

        /**
         * @brief Clears the entire sketch
         */
        void clearAll()
        {
            std::fill(data, data + lenData, 0);
        }

        /**
         * @brief Apply temporal decay
         */
        void applyDecay()
        {
            for (int i = 0; i < lenData; i++)
                data[i] *= decayFactor;

            // std::cout << "Decay applied with factor: " << decayFactor << std::endl;
        }

        /**
         * @brief Hash edge into r indices
         */
        void hash(int *indexOut, int a, int b = 0) const
        {
            for (int i = 0; i < r; i++)
            {
                indexOut[i] = ((a + m * b) * param1[i] + param2[i]) % c;
                indexOut[i] += i * c + (indexOut[i] < 0 ? c : 0); // Ensure positive
            }
        }

        /**
         * @brief Add edge using conservative update
         */
        void addEdge(int a, int b)
        {
            int indices[r];
            hash(indices, a, b);

            float minValue = infinity;
            for (int i = 0; i < r; i++)
                minValue = std::min(minValue, data[indices[i]]);

            for (int i = 0; i < r; i++)
                if (data[indices[i]] == minValue)
                    data[indices[i]] += updateStep;
        }

        /**
         * @brief Estimate edge frequency
         */
        int estimate(int a, int b) const
        {
            int indices[r];
            hash(indices, a, b);

            float minValue = infinity;
            for (int i = 0; i < r; i++)
                minValue = std::min(minValue, data[indices[i]]);

            return static_cast<int>(minValue);
        }

        /**
         * @brief Copy from another sketch (must match dimensions)
         */
        void copyFrom(const CountMinSketchCMSCU &other)
        {
            if (lenData != other.lenData || r != other.r || c != other.c)
            {
                std::cerr << "[copyFrom] Sketch dimension mismatch! Copy aborted." << std::endl;
                return;
            }

            for (int i = 0; i < lenData; ++i)
                data[i] = other.data[i];
        }
    };
}
