#include <vector>
#include <chrono>
#include "lib/coding.h"
#include "lib/picturesMerger.h"
#include <string>

float checkTime(DefaultPicturesMergingAlgorithm &alg) {
    std::chrono::high_resolution_clock::time_point startDefault = std::chrono::high_resolution_clock::now();
    alg.plusPictures();
    std::chrono::high_resolution_clock::time_point endDefault = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(endDefault - startDefault).count() / 1000000.0;
}

float checkTime(VectorizedPicturesMergingAlgorithm &alg) {
    std::chrono::high_resolution_clock::time_point startDefault = std::chrono::high_resolution_clock::now();
    alg.plusPictures();
    std::chrono::high_resolution_clock::time_point endDefault = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(endDefault - startDefault).count() / 1000000.0;
}

int main() {
    const int n = 5;
    const float count = 10.0;

    int sizes[n] = {400, 500, 600, 950, 2400};
    float normalValues[n] = {0, 0, 0, 0, 0};
    float vectorizedValues[n] = {0, 0, 0, 0, 0};

    for (int i = 0; i < count; i++) {
        for (int j = 0; j < n; j++) {
            int size = sizes[j];

            std::string mainPath = "C:/Users/Cortuzz/CLionProjects/PicsMerging/PL-PIXS/" +
                                   std::to_string(size) + "x" + std::to_string(size);

            std::vector<unsigned char> pix1 = decodeOneStep((mainPath + " 1.png").c_str());
            std::vector<unsigned char> pix2 = decodeOneStep((mainPath + " 2.png").c_str());

            DefaultPicturesMergingAlgorithm defaultAlg;
            VectorizedPicturesMergingAlgorithm vectorizedAlg;

            defaultAlg.setPictures(pix1, pix2, size * size * 4);
            vectorizedAlg.setPictures(pix1, pix2, size * size * 4);

            normalValues[j] += checkTime(defaultAlg);
            vectorizedValues[j] += checkTime(vectorizedAlg);
        }
    }

    for (int i = 0; i < n; i++) {
        std::cout << std::endl << "SIZE: " << sizes[i] << std::endl;

        std::cout << "NORMAL: " << normalValues[i] / count << std::endl;
        std::cout << "VECTORIZED: " << vectorizedValues[i] / count << std::endl;
    }

    return 0;
}
