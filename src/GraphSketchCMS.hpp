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
<<<<<<< HEAD
        int *param1;                                                              // Hash function coefficients (a)
        int *param2;                                                              // Hash function coefficients (b)
        float *data;                                                              // Main sketch data array
=======
<<<<<<< HEAD
        int *const param1;                                                        // Hash function coefficients (a)
        int *const param2;                                                        // Hash function coefficients (b)
        float *const data;                                                        // Main sketch data array
=======
        int *param1;                                                              // Hash function coefficients (a)
        int *param2;                                                              // Hash function coefficients (b)
        float *data;                                                              // Main sketch data array
>>>>>>> e0f993a (local changes before pulling)
>>>>>>> 724517a (adding darpa data)
        constexpr static float infinity = std::numeric_limits<float>::infinity(); // Infinity used for min comparison
        float decayFactor;                                                        // Temporal decay factor (used in applyDecay)
        float updateStep;                                                         // ⏎ NEW: Step size for Smoothed Conservative Update (default: 1.0f)

        /**
         * @brief Constructor to initialize the Count-Min Sketch
<<<<<<< HEAD
         */
        CountMinSketchCMSCU(int numRow, int numColumn, float decayFactor = 0.95f, float step = 55.0f)
=======
<<<<<<< HEAD
         * @param numRow Number of hash functions (rows)
         * @param numColumn Number of bins per hash (columns)
         * @param decayFactor Optional: Factor for decay smoothing (default = 0.95)
         * @param step Optional: Increment step size (default = 1.0)
         */
        CountMinSketchCMSCU(int numRow, int numColumn, float decayFactor = 0.95f, float step = 2.0f)
=======
         */
        CountMinSketchCMSCU(int numRow, int numColumn, float decayFactor = 0.95f, float step = 55.0f)
>>>>>>> e0f993a (local changes before pulling)
>>>>>>> 724517a (adding darpa data)
            : r(numRow),
              c(numColumn),
              lenData(r * c),
              param1(new int[r]),
              param2(new int[r]),
              data(new float[lenData]),
              decayFactor(decayFactor),
              updateStep(step)
        {
<<<<<<< HEAD
=======
<<<<<<< HEAD
            // Initialize hash parameters randomly
=======
>>>>>>> e0f993a (local changes before pulling)
>>>>>>> 724517a (adding darpa data)
            for (int i = 0; i < r; i++)
            {
                param1[i] = rand() + 1;
                param2[i] = rand();
            }
<<<<<<< HEAD
=======
<<<<<<< HEAD

            // Set all sketch values to 0
=======
>>>>>>> e0f993a (local changes before pulling)
>>>>>>> 724517a (adding darpa data)
            std::fill(data, data + lenData, 0);
        }

        /**
<<<<<<< HEAD
         * @brief Destructor
=======
<<<<<<< HEAD
         * @brief Destructor to release memory
=======
         * @brief Destructor
>>>>>>> e0f993a (local changes before pulling)
>>>>>>> 724517a (adding darpa data)
         */
        ~CountMinSketchCMSCU()
        {
            delete[] param1;
            delete[] param2;
            delete[] data;
        }

<<<<<<< HEAD
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
=======
<<<<<<< HEAD
        /**
         * @brief Clears the entire sketch — all counters set to 0
=======
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
>>>>>>> e0f993a (local changes before pulling)
>>>>>>> 724517a (adding darpa data)
         */
        void clearAll()
        {
            std::fill(data, data + lenData, 0);
        }

        /**
<<<<<<< HEAD
         * @brief Apply temporal decay
=======
<<<<<<< HEAD
         * @brief Applies temporal decay to all counters
         * This retains memory from previous timestamps but with less influence
=======
         * @brief Apply temporal decay
>>>>>>> e0f993a (local changes before pulling)
>>>>>>> 724517a (adding darpa data)
         */
        void applyDecay()
        {
            for (int i = 0; i < lenData; i++)
                data[i] *= decayFactor;

<<<<<<< HEAD
            // std::cout << "Decay applied with factor: " << decayFactor << std::endl;
        }

        /**
         * @brief Hash edge into r indices
=======
<<<<<<< HEAD
            std::cout << "Decay applied with factor: " << decayFactor << std::endl;
        }

        /**
         * @brief Hashes an edge (a, b) into r hash functions
         * Resulting indices are written into the indexOut array
=======
            // std::cout << "Decay applied with factor: " << decayFactor << std::endl;
        }

        /**
         * @brief Hash edge into r indices
>>>>>>> e0f993a (local changes before pulling)
>>>>>>> 724517a (adding darpa data)
         */
        void hash(int *indexOut, int a, int b = 0) const
        {
            for (int i = 0; i < r; i++)
            {
                indexOut[i] = ((a + m * b) * param1[i] + param2[i]) % c;
<<<<<<< HEAD
                indexOut[i] += i * c + (indexOut[i] < 0 ? c : 0); // Ensure positive
=======
<<<<<<< HEAD
                indexOut[i] += i * c + (indexOut[i] < 0 ? c : 0); // Ensure positive index
=======
                indexOut[i] += i * c + (indexOut[i] < 0 ? c : 0); // Ensure positive
>>>>>>> e0f993a (local changes before pulling)
>>>>>>> 724517a (adding darpa data)
            }
        }

        /**
<<<<<<< HEAD
         * @brief Add edge using conservative update
=======
<<<<<<< HEAD
         * @brief Adds an edge to the sketch using Smoothed Conservative Update
         * Increments only the minimum count among all r hash locations using a small step
=======
         * @brief Add edge using conservative update
>>>>>>> e0f993a (local changes before pulling)
>>>>>>> 724517a (adding darpa data)
         */
        void addEdge(int a, int b)
        {
            int indices[r];
            hash(indices, a, b);

<<<<<<< HEAD
=======
<<<<<<< HEAD
            // Find minimum value among hash buckets
=======
>>>>>>> e0f993a (local changes before pulling)
>>>>>>> 724517a (adding darpa data)
            float minValue = infinity;
            for (int i = 0; i < r; i++)
                minValue = std::min(minValue, data[indices[i]]);

<<<<<<< HEAD
=======
<<<<<<< HEAD
            // Increment only those with minimum value using updateStep
>>>>>>> 724517a (adding darpa data)
            for (int i = 0; i < r; i++)
                if (data[indices[i]] == minValue)
                    data[indices[i]] += updateStep;
        }

        /**
<<<<<<< HEAD
         * @brief Estimate edge frequency
=======
         * @brief Estimates frequency of edge (a, b) using CMSCU
         * Returns the minimum counter among all r hash locations
=======
            for (int i = 0; i < r; i++)
                if (data[indices[i]] == minValue)
                    data[indices[i]] += updateStep;
        }

        /**
         * @brief Estimate edge frequency
>>>>>>> e0f993a (local changes before pulling)
>>>>>>> 724517a (adding darpa data)
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
<<<<<<< HEAD
         * @brief Copy from another sketch (must match dimensions)
=======
<<<<<<< HEAD
         * @brief Copies all values from another sketch of same dimension
         * @param other Sketch to copy from (must have same dimensions)
=======
         * @brief Copy from another sketch (must match dimensions)
>>>>>>> e0f993a (local changes before pulling)
>>>>>>> 724517a (adding darpa data)
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
