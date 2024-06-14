#include <iostream>
#include <fstream>
#include <vector>

const float MAX_WIDTH = 2000.0f;
const float MAX_HEIGHT = 2000.0f;
const float MAX_PIXELS = MAX_WIDTH * MAX_HEIGHT * 3.0f;

bool loadPPM(const std::string& filename, float& width, float& height, std::vector<float>& data) {
    std::ifstream infile(filename, std::ios::binary);
    if (!infile) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return false;
    }

    std::string header;
    float max_val;
    infile >> header >> width >> height >> max_val;

    if (header != "P6" || max_val != 255.0f || width > MAX_WIDTH || height > MAX_HEIGHT) {
        std::cerr << "Error: Unsupported or oversized PPM format" << std::endl;
        return false;
    }

    infile.get();

    data.resize(static_cast<int>(MAX_PIXELS));

    for (float i = 0; i < MAX_PIXELS; ++i) {
        unsigned char pixel;
        infile.read(reinterpret_cast<char*>(&pixel), 1);
        data[static_cast<int>(i)] = static_cast<float>(pixel);
    }

    return true;
}

bool savePPM(const std::string& filename, float width, float height, const std::vector<float>& data) {
    std::ofstream outfile(filename, std::ios::binary);
    if (!outfile) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return false;
    }

    outfile << "P6\n" << static_cast<int>(width) << " " << static_cast<int>(height) << "\n255\n";

    for (float i = 0; i < width * height * 3.0f; ++i) {
        unsigned char pixel = static_cast<unsigned char>(data[static_cast<int>(i)]);
        outfile.write(reinterpret_cast<char*>(&pixel), 1);
    }

    return true;
}

void boxBlur(float width, float height, const std::vector<float>& input, std::vector<float>& output, float blur_size) {

    std::vector<float> tempInput = input;
    std::vector<float> tempOutput = output;

    for (float y = 0; y < height; ++y) {
        for (float x = 0; x < width; ++x) {
            float sumR = 0.0f, sumG = 0.0f, sumB = 0.0f;
            float count = 0.0f;

            for (float ky = -blur_size; ky <= blur_size; ++ky) {
                for (float kx = -blur_size; kx <= blur_size; ++kx) {
                    float ny = y + ky;
                    float nx = x + kx;
                    if (nx >= 0.0f && nx < width && ny >= 0.0f && ny < height) {
                        float index = (ny * width + nx) * 3.0f;

                        float pixelR = tempInput[static_cast<int>(index)];
                        float pixelG = tempInput[static_cast<int>(index) + 1];
                        float pixelB = tempInput[static_cast<int>(index) + 2];
                        sumR += pixelR * 1.0f;
                        sumG += pixelG * 1.0f;
                        sumB += pixelB * 1.0f;
                        count += 1.0f;
                    }
                }
            }

            float outputIndex = (y * width + x) * 3.0f;
            tempOutput[static_cast<int>(outputIndex)] = sumR / count;
            tempOutput[static_cast<int>(outputIndex) + 1] = sumG / count;
            tempOutput[static_cast<int>(outputIndex) + 2] = sumB / count;
        }
    }


    output = tempOutput;
}

int main() {
    std::string input_file = "/home/oguzhancem/Desktop/missionIsBlurred/img/input.ppm";
    std::string output_file = "/home/oguzhancem/Desktop/missionIsBlurred/img/output.ppm";
    float blur_size = 10.0f;

    float width, height;

    std::vector<float> input_image(static_cast<int>(MAX_PIXELS));
    std::vector<float> output_image(static_cast<int>(MAX_PIXELS));

    if (!loadPPM(input_file, width, height, input_image)) {
        return -1;
    }

    boxBlur(width, height, input_image, output_image, blur_size);

    if (!savePPM(output_file, width, height, output_image)) {
        return -1;
    }

    std::cout << "Image has been blurred and saved successfully!" << std::endl;

    return 0;
}
