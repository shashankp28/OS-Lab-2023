#include <cmath>
#include <chrono>
#include <thread>
#include <vector>
#include <atomic>
#include <fstream>
#include <sstream>
#include <pthread.h>
#include <bits/stdc++.h>
#include <mutex>

// Wrapper for atomic operations
template <typename T>
struct atomwrapper
{
    std::atomic<T> _a;

    atomwrapper()
        : _a()
    {
    }

    atomwrapper(const std::atomic<T> &a)
        : _a(a.load())
    {
    }

    atomwrapper(const atomwrapper &other)
        : _a(other._a.load())
    {
    }

    atomwrapper &operator=(const atomwrapper &other)
    {
        _a.store(other._a.load());
        return *this;
    }
    bool operator==(const bool &other)
    {
        return _a.load() == other;
    }
    bool operator<(const atomwrapper &other)
    {
        return _a.load() < other._a.load();
    }
};

using namespace std;
using namespace std::chrono;

// Mutex Lock for sanity check
mutex mtx;

// Address Space Variables for Inter-Thread communication
vector<vector<vector<int>>> imgData;
vector<atomwrapper<bool>> transform_1_completed;
vector<int> trans1({0, 0}), trans2({0, 0});
int counts = 0;

// Transformation Functions
// Transformation To increase brightness of image
void IncreaseBrightness(int height, int width)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Wait for first transformation to finish on pixel i, j
            int r = imgData[i][j][0];
            int g = imgData[i][j][1];
            int b = imgData[i][j][2];

            // Increase brightness of each channel by the specified amount
            r = min(int(r * 3), 255);
            g = min(int(g * 3), 255);
            b = min(int(b * 3), 255);

            // Update pixel values
            imgData[i][j][0] = r;
            imgData[i][j][1] = g;
            imgData[i][j][2] = b;

            mtx.lock();
            trans1[0] = i;
            trans1[1] = j;
            mtx.unlock();

            // Atomic operation to set operation completed
            transform_1_completed[i * width + j] = atomic<bool>(true);
        }
    }
}

// Transformation To convert image to grayscale
void RBGToGrayScale(int height, int width)
{
    int r, g, b, gray;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            while (transform_1_completed[i * width + j] == false)
                ;
            r = imgData[i][j][0];
            g = imgData[i][j][1];
            b = imgData[i][j][2];

            // Convert to grayscale by calculating the weighted sum of current r, g, b values

            int gray = r * (0.299) + g * (0.587) + b * (0.114);
            imgData[i][j][0] = gray;
            imgData[i][j][1] = gray;
            imgData[i][j][2] = gray;


            mtx.lock();
            trans2[0] = i;
            trans2[1] = j;
            if (trans1[0] < trans2[0] || (trans1[0] == trans2[0] && trans1[1] < trans2[1])){
                cout << "T2 overtook T1 " << trans2[0]<<" "<<trans2[1] << "\n";
            }

            mtx.unlock();
        }
    }
}

int main(int argc, char **argv)
{
    auto start = high_resolution_clock::now();

    // Check number of arguments
    if (argc != 3)
    {
        cout << "Usage: ./a.out <path-to-original-image> <path-to-transformed-image>\n";
        exit(0);
    }

    // Read PPM file
    char ppmVersion[20];
    int imgWidth, imgHeight, imgColorMax, r, g, b;
    FILE *input = fopen(argv[1], "r");
    fscanf(input, "%s%d%d%d", ppmVersion, &imgWidth, &imgHeight, &imgColorMax);

    // Store pixel information in a matrix
    for (int i = 0; i < imgHeight; i++)
    {
        vector<vector<int>> row;
        for (int j = 0; j < imgWidth; j++)
        {
            fscanf(input, "%d%d%d", &r, &g, &b);
            vector<int> tempVec = {r, g, b};
            row.push_back(tempVec);
            transform_1_completed.push_back(atomic<bool>(false));
        }
        imgData.push_back(row);
        row.clear();
    }

    // Close input file
    fclose(input);

    // Transform Images
    // Threads made for each image transformation
    thread T1(IncreaseBrightness, imgHeight, imgWidth);
    thread T2(RBGToGrayScale, imgHeight, imgWidth);

    // Waiting for T1 & T2 to complete execution
    T1.join();
    T2.join();

    // Write transformed image to output file
    FILE *output = fopen(argv[2], "w");
    fprintf(output, "%s\n%d %d\n%d\n", ppmVersion, imgWidth, imgHeight, imgColorMax);

    for (int i = 0; i < imgHeight; i++)
    {
        for (int j = 0; j < imgWidth; j++)
        {
            fprintf(output, "%d ", imgData[i][j][0]);
            fprintf(output, "%d ", imgData[i][j][1]);
            fprintf(output, "%d ", imgData[i][j][2]);
        }
        fprintf(output, "\n");
    }

    // Close output file
    fclose(output);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    cout << "Time of Execution: " << duration.count() << " us" << endl;

    return 0;
}