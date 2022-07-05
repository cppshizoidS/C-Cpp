//
// Created by Cortuzz on 5/26/2022.
//

#pragma once
#include <utility>
#include <vector>
#include <chrono>
#include <intrin.h>
#include <immintrin.h>
#include <omp.h>

struct PicturesMergingAlgorithm {
protected:
    std::vector<unsigned char> firstPicture;
    std::vector<unsigned char> secondPicture;
    std::vector<unsigned char> resultPicture;
    int size {};

public:
    void setPictures(std::vector<unsigned char> pix1, std::vector<unsigned char> pix2, int _size) {
        firstPicture = std::move(pix1);
        secondPicture = std::move(pix2);
        size = _size;
        resultPicture = std::vector<unsigned char>(size, 0);
    }

    virtual void plusPictures() {
        throw std::exception();
    }

    virtual void minusPictures() {
        throw std::exception();
    }

    virtual void gainExtractPictures() {
        throw std::exception();
    }

    virtual void gainMergePictures() {
        throw std::exception();
    }

    std::vector<unsigned char> getResult() {
        return resultPicture;
    }
};

struct VectorizedPicturesMergingAlgorithm:PicturesMergingAlgorithm {
private:
    const int CHUNK_SIZE = 32;
    unsigned long long _fullData[8]{ -1, -1, -1, -1, -1, -1, -1, -1 };
	unsigned long long _01Data[8]{ 0x0202020202020202,0x0202020202020202,0x0202020202020202,0x0202020202020202,0x0202020202020202,0x0202020202020202,0x0202020202020202,0x0202020202020202 };
	const __m256i reg256i255 = _mm256_loadu_epi64(&_fullData);
	const __m256i reg256i2 = _mm256_loadu_epi64(_01Data);
	const __m256i reg256i127 = _mm256_div_epu8(reg256i255, reg256i2);
    
public:
    void plusPictures() override {
        omp_set_num_threads(16);
        
        int i;
        #pragma omp parallel for private(i) 
        for (i = 0; i < size; i += CHUNK_SIZE) {
            __m256i v1 = _mm256_load_si256((__m256i*) & firstPicture[i]);
            __m256i v2 = _mm256_load_si256((__m256i*) & secondPicture[i]);
            __m256i reg = _mm256_adds_epu8(v1, v2);
            _mm256_store_si256((__m256i*) & resultPicture[i], reg);
        }
    }

    void minusPictures() override {
        for (int i = 0; i < size; i += CHUNK_SIZE) {
            __m256i v1 = _mm256_load_si256((__m256i*)&firstPicture[i]);
            __m256i v2 = _mm256_load_si256((__m256i*)&secondPicture[i]);
            __m256i reg = _mm256_subs_epu8(v1, v2);
            _mm256_store_si256((__m256i*)&resultPicture[i], reg);
        }
    }
    
    void gainExtractPictures() override {
		for (i = 0; i < resultLength; i += 32) {
			__m256i v1 = _mm256_load_si256((__m256i*)&image1[i]);
			__m256i v2 = _mm256_load_si256((__m256i*)&image2[i]);
			__m256i reg = _mm256_subs_epu8(_mm256_adds_epu8(v1, v2), reg256i127);
			_mm256_store_si256((__m256i*) & result[i], reg);
		}
    }
    
    void gainMergePictures() override {
		for (i = 0; i < resultLength; i += 32) {
			__m256i v1 = _mm256_load_si256((__m256i*)&image1[i]);
			__m256i v2 = _mm256_load_si256((__m256i*)&image2[i]);
			__m256i reg = _mm256_adds_epu8(_mm256_subs_epu8(v1, v2), reg256i127);
			_mm256_store_si256((__m256i*) & result[i], reg);
		}
    }
};
        
struct DefaultPicturesMergingAlgorithm:PicturesMergingAlgorithm {
    void plusPictures() override {
        omp_set_num_threads(16);

        int i;
        #pragma omp parallel for private(i) 
        for (i = 0; i < size; i++) {
            resultPicture[i] = ((firstPicture[i] + secondPicture[i] > 255) ? 255 : firstPicture[i] + secondPicture[i]);
        }
      
    }

    void minusPictures() override {
        for (int i = 0; i < size; i++) {
            (i % 4 != 3) ? resultPicture[i] = ((firstPicture[i] - secondPicture[i] < 0) ? 0 : firstPicture[i] - secondPicture[i]) :
                    resultPicture[i] = 255;
        }
    }

    void gainExtractPictures() override {
        for (int i = 0; i < size; i++) {
            (i % 4 != 3) ? resultPicture[i] = ((firstPicture[i] - secondPicture[i] < -128) ? 0 : firstPicture[i] - secondPicture[i] + 128) :
                    resultPicture[i] = 255;
        }
    }

    void gainMergePictures() override {
        for (int i = 0; i < size; i++) {
            resultPicture[i] = ((firstPicture[i] + secondPicture[i] - 128 > 255) ? 255 : firstPicture[i] + secondPicture[i] - 128);
        }
    }
};
