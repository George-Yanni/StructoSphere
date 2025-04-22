#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <sstream>
#include <chrono>
#include <thread>


/*
 * =====================================================================
 *  Program: Image Quantizer using Lookup Table and Grayscale Palette
 *  Language: C++
 *
 *  Description:
 *  ------------
 *  This program reads a PPM (P6) image file and performs color quantization
 *  using a fixed 16-color grayscale palette. The quantization is done
 *  efficiently using a precomputed lookup table that maps all 16.7 million
 *  possible 24-bit RGB colors to their closest palette match.
 *
 *  Steps:
 *    1. Define a grayscale palette of 16 evenly spaced shades from black
 *       to white: (0,0,0), (17,17,17), ..., (255,255,255)
 *
 *    2. Generate a lookup table that maps every possible RGB color (16,777,216)
 *       to the closest color in the palette using Euclidean distance in RGB space.
 *
 *    3. Read the original PPM image (binary P6 format).
 *
 *    4. Replace each pixel in the image using the lookup table to assign its
 *       closest grayscale value.
 *
 *    5. Save the quantized image to a new PPM file.
 *
 * ---------------------------------------------------------------------
 *
 *  Quantization Note:
 *  ------------------
 *  After quantization, every pixel in the image is replaced by the closest color
 *  from a reduced grayscale palette of 16 shades, ranging from (0, 0, 0) to (255, 255, 255)
 *  in uniform steps of 17. This means:
 *
 *    - All quantized pixels will be pure grayscale (R = G = B)
 *    - Only the following 16 values are possible per channel: 
 *        {0, 17, 34, 51, 68, 85, 102, 119, 136, 153, 170, 187, 204, 221, 238, 255}
 *    - A quantized pixel like (119, 119, 255) is **not possible** because the R, G, and B
 *      components must be equal. All pixels are mapped to one of the predefined grayscale shades.
 *
 *  This method provides fast image quantization and ensures uniform color mapping,
 *  especially useful for compression, low-color displays, or stylized rendering.
 *
 * =====================================================================
 */


// Structure to represent a color
struct Color {
    uint8_t r, g, b;
};

// Function to compute the Euclidean distance between two colors
double computeColorDistance(const Color& color1, const Color& color2) {
    return std::sqrt(std::pow(color1.r - color2.r, 2) +
                     std::pow(color1.g - color2.g, 2) +
                     std::pow(color1.b - color2.b, 2));
}

// Function to find the closest color in the palette
Color findClosestColor(const Color& color, const std::vector<Color>& palette) {
    Color closestColor = palette[0];
    double minDistance = computeColorDistance(color, palette[0]);

    for (const auto& paletteColor : palette) {
        double distance = computeColorDistance(color, paletteColor);
        if (distance < minDistance) {
            minDistance = distance;
            closestColor = paletteColor;
        }
    }
    return closestColor;
}

// Function to create a lookup table for color quantization
std::unordered_map<int, Color> createLookupTable(const std::vector<Color>& palette) {
    std::unordered_map<int, Color> lookupTable;
    const int totalColors = 256 * 256 * 256;
    int progress = 0;
    int lastProgressPrint = 0;

    for (int b = 0; b < 256; ++b) {
        for (int g = 0; g < 256; ++g) {
            for (int r = 0; r < 256; ++r) {
                Color color = {static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b)};
                Color closestColor = findClosestColor(color, palette);
                int key = (r << 16) | (g << 8) | b;
                lookupTable[key] = closestColor;

                // Update progress
                ++progress;
                int currentProgress = (progress * 100) / totalColors;
                if (currentProgress > lastProgressPrint) {
                    std::cout << "\rCreating lookup table: " << currentProgress << "% complete";
                    std::cout.flush();
                    lastProgressPrint = currentProgress;
                }
            }
        }
    }
    std::cout << std::endl; // Ensure the progress line ends properly
    return lookupTable;
}

// Function to read a PPM image
bool readPPM(const std::string& filename, std::vector<Color>& pixels, int& width, int& height) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return false;
    }

    std::string format;
    file >> format;
    if (format != "P6") {
        std::cerr << "Error: Unsupported format " << format << std::endl;
        return false;
    }

    file >> width >> height;
    int maxVal;
    file >> maxVal;
    file.get(); // Read the newline character after the header

    pixels.resize(width * height);
    file.read(reinterpret_cast<char*>(pixels.data()), pixels.size() * 3);
    file.close();
    return true;
}

// Function to write a PPM image
bool writePPM(const std::string& filename, const std::vector<Color>& pixels, int width, int height) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return false;
    }

    file << "P6\n" << width << " " << height << "\n255\n";
    file.write(reinterpret_cast<const char*>(pixels.data()), pixels.size() * 3);
    file.close();
    return true;
}

// Function to quantize an image using the lookup table
void quantizeImage(std::vector<Color>& pixels, const std::unordered_map<int, Color>& lookupTable) {
    for (auto& pixel : pixels) {
        int key = (pixel.r << 16) | (pixel.g << 8) | pixel.b;
        pixel = lookupTable.at(key);
    }
}

int main() {
    // Define a reduced color palette (for simplicity, using a smaller grayscale palette)
    std::vector<Color> palette;
    for (int i = 0; i < 16; ++i) { // Reduced palette size for faster computation
        uint8_t value = static_cast<uint8_t>(i * 17); // Distribute values evenly
        palette.push_back({value, value, value});
    }

    // Create the lookup table
    std::cout << "Creating lookup table..." << std::endl;
    std::unordered_map<int, Color> lookupTable = createLookupTable(palette);
    std::cout << "Lookup table created." << std::endl;

    // Read the input image
    std::vector<Color> pixels;
    int width, height;
    if (!readPPM("colorful.ppm", pixels, width, height)) {
        return -1;
    }
    std::cout << "Image read successfully." << std::endl;

    // Quantize the image using the lookup table
    std::cout << "Quantizing image..." << std::endl;
    quantizeImage(pixels, lookupTable);
    std::cout << "Image quantized." << std::endl;

    // Write the quantized image
    if (!writePPM("quantized_image.ppm", pixels, width, height)) {
        return -1;
    }
    std::cout << "Quantized image saved as quantized_image.ppm" << std::endl;

    return 0;
}
