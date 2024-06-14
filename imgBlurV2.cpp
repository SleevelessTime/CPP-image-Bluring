#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>

const int MAX_WIDTH = 2000;
const int MAX_HEIGHT = 2000;
const int MAX_PIXELS = MAX_WIDTH * MAX_HEIGHT * 3;

bool loadPPM(const std::string& filename, int& width, int& height, std::vector<int>& data) {
    std::ifstream infile(filename, std::ios::binary);
    if (!infile) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return false;
    }

    std::string header;
    int max_val;
    infile >> header >> width >> height >> max_val;

    if (header != "P6" || max_val != 255 || width > MAX_WIDTH || height > MAX_HEIGHT) {
        std::cerr << "Error: Unsupported or oversized PPM format" << std::endl;
        return false;
    }

    infile.get(); // Skip newline character

    data.resize(width * height * 3);

    for (int i = 0; i < width * height * 3; ++i) {
        unsigned char pixel;
        infile.read(reinterpret_cast<char*>(&pixel), 1);
        data[i] = static_cast<int>(pixel);
    }

    return true;
}

bool savePPM(const std::string& filename, int width, int height, const std::vector<int>& data) {
    std::ofstream outfile(filename, std::ios::binary);
    if (!outfile) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return false;
    }

    outfile << "P6\n" << width << " " << height << "\n255\n";

    for (int i = 0; i < width * height * 3; ++i) {
        unsigned char pixel = static_cast<unsigned char>(data[i]);
        outfile.write(reinterpret_cast<char*>(&pixel), 1);
    }

    return true;
}

void boxBlur(int width, int height, const std::vector<int>& input, std::vector<int>& output, int blur_size) {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int sumR = 0, sumG = 0, sumB = 0;
            int count = 0;

            for (int ky = -blur_size; ky <= blur_size; ++ky) {
                for (int kx = -blur_size; kx <= blur_size; ++kx) {
                    int ny = y + ky;
                    int nx = x + kx;
                    if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                        int index = (ny * width + nx) * 3;
                        sumR += input[index];
                        sumG += input[index + 1];
                        sumB += input[index + 2];
                        ++count;
                    }
                }
            }

            int outputIndex = (y * width + x) * 3;
            output[outputIndex] = sumR / count;
            output[outputIndex + 1] = sumG / count;
            output[outputIndex + 2] = sumB / count;
        }
    }
}

int main() {
    std::string directory = "/home/oguzhancem/Desktop/missionIsBlurred/img";
    int blur_size = 10;

    int success_count = 0;
    int failure_count = 0;

    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.path().extension() == ".ppm") {
            std::string input_file = entry.path().string();
            int width, height;

            std::vector<int> input_image;
            std::vector<int> output_image;

            if (!loadPPM(input_file, width, height, input_image)) {
                std::cerr << "Failed to load image: " << input_file << std::endl;
                ++failure_count;
                continue;
            }

            output_image.resize(width * height * 3);
            boxBlur(width, height, input_image, output_image, blur_size);

            if (!savePPM(input_file, width, height, output_image)) {
                std::cerr << "Failed to save image: " << input_file << std::endl;
                ++failure_count;
                continue;
            }

            ++success_count;
        }
    }

    std::cout << "Total images processed successfully: " << success_count << std::endl;
    std::cout << "Total images failed to process: " << failure_count << std::endl;

    return 0;
}
