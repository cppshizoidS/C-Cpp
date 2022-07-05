//
// Created by Cortuzz on 5/26/2022.
//
#pragma once
#include <vector>
#include <iostream>
#include "lodepng.h"

std::vector<unsigned char> decodeOneStep(const char* filename) {
    std::vector<unsigned char> image;
    unsigned width, height;

    unsigned error = lodepng::decode(image, width, height, filename);
    if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
    return image;
}

void encodeOneStep(const char* filename, const std::vector<unsigned char>& image, unsigned width, unsigned height) {
    unsigned error = lodepng::encode(filename, image, width, height);

    if(error) std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;
}