// src/cuda_cmscu.cu
__global__ void addEdgeKernel(float* data, int* indices, int r, float updateStep) {
    int i = threadIdx.x;
    float minVal = data[indices[0]];
    for (int j = 1; j < r; j++) {
        minVal = fminf(minVal, data[indices[j]]);
    }
    if (data[indices[i]] == minVal) {
        data[indices[i]] += updateStep;
    }
}
