#include <iostream>
#include <vector>
#include <bitset>
#include <string>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace std;

// Function to generate a heart-shaped image
vector<vector<int>> generateImage(int width, int height) {
    vector<vector<int>> image(height, vector<int>(width));
    
    // Define heart shape (1 = white, 0 = black)
    int heartPattern[8][8] = {
        {0, 1, 1, 0, 0, 1, 1, 0},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {0, 1, 1, 1, 1, 1, 1, 0},
        {0, 0, 1, 1, 1, 1, 0, 0},
        {0, 0, 0, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0}
    };

    // Copy pattern to image
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            image[i][j] = heartPattern[i][j];
        }
    }
    return image;
}

// Function to print the image (console)
void printImage(const vector<vector<int>>& image) {
    for (const auto& row : image) {
        for (int pixel : row) {
            cout << (pixel == 0 ? "B" : "W") << " ";
        }
        cout << endl;
    }
}

// Function to save the image as PNG
void saveImageAsPNG(const vector<vector<int>>& image, int width, int height, const string& filename) {
    vector<unsigned char> data(width * height * 3); // RGB image
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int idx = (i * width + j) * 3;
            unsigned char color = image[i][j] ? 255 : 0; // White (255) or Black (0)
            data[idx] = data[idx + 1] = data[idx + 2] = color; // Set RGB to same value
        }
    }
    stbi_write_png(filename.c_str(), width, height, 3, data.data(), width * 3);
}

// Function to compress the image into a bit array
bitset<64> compressImageToBitArray(const vector<vector<int>>& image, int width, int height) {
    bitset<64> compressedData;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int index = i * width + j;
            if (image[i][j] == 1) {
                compressedData.set(index);
            }
        }
    }
    return compressedData;
}

// Function to decompress the bit array into the image
vector<vector<int>> decompressBitArrayToImage(const bitset<64>& compressedData, int width, int height) {
    vector<vector<int>> decompressedImage(height, vector<int>(width));
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int index = i * width + j;
            decompressedImage[i][j] = compressedData.test(index) ? 1 : 0;
        }
    }
    return decompressedImage;
}

int main() {
    int width = 8;
    int height = 8;

    // Generate and display original heart image
    cout << "Original Heart Image:" << endl;
    vector<vector<int>> originalImage = generateImage(width, height);
    printImage(originalImage);
    saveImageAsPNG(originalImage, width, height, "original_heart.png"); // Save as PNG

    // Compress the image
    bitset<64> compressedData = compressImageToBitArray(originalImage, width, height);
    cout << "\nCompressed Data (Bit Array): " << compressedData << endl;

    // Decompress and display
    vector<vector<int>> decompressedImage = decompressBitArrayToImage(compressedData, width, height);
    cout << "\nDecompressed Heart Image:" << endl;
    printImage(decompressedImage);
    saveImageAsPNG(decompressedImage, width, height, "decompressed_heart.png"); // Save as PNG

    return 0;
}
// Goal:
// The code compresses a binary image (black/white pixels) of size 8x8 using a bit array instead of storing each pixel as a full int (typically 4 bytes).

// Explanation:
// A binary image is represented as a 2D array of integers (vector<vector<int>>), where each pixel is either 0 (black) or 1 (white).

// Instead of storing 64 ints (one for each pixel), we use a bitset<64> which internally stores just 64 bits (1 bit per pixel).

// Each bit in the bitset represents one pixel:

// 1 for white

// 0 for black

// This drastically reduces memory usage.

// Memory Usage Comparison

// Storage Method	Unit Size	Total Pixels	Total Size
// vector<vector<int>>	4 bytes	64	64 × 4 = 256 bytes
// bitset<64>	1 bit	64	64 / 8 = 8 bytes
// Space Saved:
// Original Image (int): 256 bytes

// Compressed Bit Array: 8 bytes
// Space saved = 256 - 8 = 248 bytes ✅ Reduction ≈ 96.9%