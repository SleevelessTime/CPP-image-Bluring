#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>

const int MAX_WIDTH = 2000;
const int MAX_HEIGHT = 2000;
const int NUM_THREADS = 16; // 16 threads for AMD Ryzen 7 4800H

std::mutex count_mutex;
std::mutex queue_mutex;
std::condition_variable cv;
std::queue<std::string> tasks;
bool done = false;

bool loadPPM(const std::string& filename, int& width, int& height, std::vector<unsigned char>& data) {
    std::ifstream infile(filename, std::ios::binary);
    if (!infile) {
        return false;
    }

    std::string header;
    int max_val;
    infile >> header >> width >> height >> max_val;

    if (header != "P6" || max_val != 255 || width > MAX_WIDTH || height > MAX_HEIGHT) {
        return false;
    }

    infile.get(); // Skip newline character

    data.resize(width * height * 3);
    infile.read(reinterpret_cast<char*>(data.data()), data.size());

    return infile.good();
}

bool savePPM(const std::string& filename, int width, int height, const std::vector<unsigned char>& data) {
    std::ofstream outfile(filename, std::ios::binary);
    if (!outfile) {
        return false;
    }

    outfile << "P6\n" << width << " " << height << "\n255\n";
    outfile.write(reinterpret_cast<const char*>(data.data()), data.size());

    return outfile.good();
}

void boxBlurSection(int width, int height, const std::vector<unsigned char>& input, std::vector<unsigned char>& output, int blur_size, int startY, int endY) {
    for (int y = startY; y < endY; ++y) {
        for (int x = 0; x < width; ++x) {
            int sumR = 0, sumG = 0, sumB = 0;
            int count = 0;

            for (int ny = std::max(0, y - blur_size); ny <= std::min(height - 1, y + blur_size); ++ny) {
                for (int nx = std::max(0, x - blur_size); nx <= std::min(width - 1, x + blur_size); ++nx) {
                    int index = (ny * width + nx) * 3;
                    sumR += input[index];
                    sumG += input[index + 1];
                    sumB += input[index + 2];
                    ++count;
                }
            }

            int outputIndex = (y * width + x) * 3;
            output[outputIndex] = sumR / count;
            output[outputIndex + 1] = sumG / count;
            output[outputIndex + 2] = sumB / count;
        }
    }
}

void boxBlur(int width, int height, const std::vector<unsigned char>& input, std::vector<unsigned char>& output, int blur_size) {
    int chunk_size = height / NUM_THREADS;
    std::vector<std::thread> threads;

    for (int i = 0; i < NUM_THREADS; ++i) {
        int startY = i * chunk_size;
        int endY = (i == NUM_THREADS - 1) ? height : startY + chunk_size;
        threads.emplace_back(boxBlurSection, width, height, std::cref(input), std::ref(output), blur_size, startY, endY);
    }

    for (auto& thread : threads) {
        thread.join();
    }
}

void processImage(const std::string& input_file, int blur_size, int& success_count, int& failure_count) {
    int width, height;
    std::vector<unsigned char> input_image, output_image;

    if (loadPPM(input_file, width, height, input_image)) {
        output_image.resize(width * height * 3);
        boxBlur(width, height, input_image, output_image, blur_size);

        if (savePPM(input_file, width, height, output_image)) {
            std::lock_guard<std::mutex> lock(count_mutex);
            ++success_count;
        } else {
            std::lock_guard<std::mutex> lock(count_mutex);
            ++failure_count;
        }
    } else {
        std::lock_guard<std::mutex> lock(count_mutex);
        ++failure_count;
    }
}

void workerThread(int blur_size, int& success_count, int& failure_count) {
    while (true) {
        std::string task;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            cv.wait(lock, [] { return !tasks.empty() || done; });

            if (tasks.empty() && done) {
                return;
            }

            task = std::move(tasks.front());
            tasks.pop();
        }
        processImage(task, blur_size, success_count, failure_count);
    }
}

int main() {
    std::string directory = "/home/oguzhancem/Desktop/missionIsBlurred/img";
    int blur_size = 10;

    int success_count = 0;
    int failure_count = 0;

    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.path().extension() == ".ppm") {
            std::unique_lock<std::mutex> lock(queue_mutex);
            tasks.push(entry.path().string());
        }
    }

    std::vector<std::thread> threads;
    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back(workerThread, blur_size, std::ref(success_count), std::ref(failure_count));
    }

    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        done = true;
    }
    cv.notify_all();

    for (auto& thread : threads) {
        thread.join();
    }

    std::cout << "Total images processed successfully: " << success_count << std::endl;
    std::cout << "Total images failed to process: " << failure_count << std::endl;

    return 0;
}
