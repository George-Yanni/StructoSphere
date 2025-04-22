#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>

using namespace std;

// Function to read a PPM image from a file
vector<vector<vector<int>>> readPPM(const string& filename, int& width, int& height, int& maxVal, string& format) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Error: Could not open file " << filename << endl;
        exit(1);
    }

    file >> format;
    if (format != "P3" && format != "P6") {
        cerr << "Error: Unsupported file format" << endl;
        exit(1);
    }

    file >> width >> height >> maxVal;
    file.ignore(256, '\n'); // Ignore the newline after maxVal

    vector<vector<vector<int>>> image(height, vector<vector<int>>(width, vector<int>(3)));

    if (format == "P3") {
        // Read ASCII PPM
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                file >> image[i][j][0] >> image[i][j][1] >> image[i][j][2];
            }
        }
    } else if (format == "P6") {
        // Read binary PPM
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                unsigned char r, g, b;
                file.read(reinterpret_cast<char*>(&r), 1);
                file.read(reinterpret_cast<char*>(&g), 1);
                file.read(reinterpret_cast<char*>(&b), 1);
                image[i][j][0] = static_cast<int>(r);
                image[i][j][1] = static_cast<int>(g);
                image[i][j][2] = static_cast<int>(b);
            }
        }
    }

    file.close();
    return image;
}

// Function to write a PPM image to a file
void writePPM(const string& filename, const vector<vector<vector<int>>>& image, int width, int height, int maxVal, const string& format) {
    ofstream file(filename, ios::binary);
    if (!file) {
        cerr << "Error: Could not open file " << filename << endl;
        exit(1);
    }

    file << format << endl;
    file << width << " " << height << endl;
    file << maxVal << endl;

    if (format == "P3") {
        // Write ASCII PPM
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                file << image[i][j][0] << " " << image[i][j][1] << " " << image[i][j][2] << " ";
            }
            file << endl;
        }
    } else if (format == "P6") {
        // Write binary PPM
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                unsigned char r = static_cast<unsigned char>(image[i][j][0]);
                unsigned char g = static_cast<unsigned char>(image[i][j][1]);
                unsigned char b = static_cast<unsigned char>(image[i][j][2]);
                file.write(reinterpret_cast<const char*>(&r), 1);
                file.write(reinterpret_cast<const char*>(&g), 1);
                file.write(reinterpret_cast<const char*>(&b), 1);
            }
        }
    }

    file.close();
}

// Function to apply convolution to an image using a kernel
vector<vector<vector<int>>> applyConvolution(const vector<vector<vector<int>>>& image, const vector<vector<float>>& kernel) {
    int height = image.size();
    int width = image[0].size();
    int kernelSize = kernel.size();
    int pad = kernelSize / 2;

    vector<vector<vector<int>>> output(height, vector<vector<int>>(width, vector<int>(3, 0)));

    for (int i = pad; i < height - pad; ++i) {
        for (int j = pad; j < width - pad; ++j) {
            for (int c = 0; c < 3; ++c) {
                float sum = 0.0;
                for (int ki = 0; ki < kernelSize; ++ki) {
                    for (int kj = 0; kj < kernelSize; ++kj) {
                        sum += image[i - pad + ki][j - pad + kj][c] * kernel[ki][kj];
                    }
                }
                output[i][j][c] = static_cast<int>(sum);
            }
        }
    }

    return output;
}

int main() {
    int width, height, maxVal;
    string format;

    // Read the input PPM image
    vector<vector<vector<int>>> image = readPPM("colorful.ppm", width, height, maxVal, format);

    // Define a 5x5 Gaussian blur kernel
    vector<vector<float>> kernel = {
        {1/273.0, 4/273.0, 7/273.0, 4/273.0, 1/273.0},
        {4/273.0, 16/273.0, 26/273.0, 16/273.0, 4/273.0},
        {7/273.0, 26/273.0, 41/273.0, 26/273.0, 7/273.0},
        {4/273.0, 16/273.0, 26/273.0, 16/273.0, 4/273.0},
        {1/273.0, 4/273.0, 7/273.0, 4/273.0, 1/273.0}
    };

    // Apply convolution to the image 3 times for a more aggressive blur
    vector<vector<vector<int>>> temp = image;
    for (int i = 0; i < 10; ++i) {  // Apply 10 times
        temp = applyConvolution(temp, kernel);
        printf("Iteration (%d) of Bluring completed.\n", i + 1);
    }
    vector<vector<vector<int>>> blurredImage = temp;

    // Write the blurred image to a file
    writePPM("blurred_image.ppm", blurredImage, width, height, maxVal, format);

    cout << "Image convolution completed successfully." << endl;
    return 0;
}
