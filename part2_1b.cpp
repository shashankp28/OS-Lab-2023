#include <cmath>
#include <mutex>
#include <chrono>
#include <thread>
#include <vector>
#include <fstream>
#include <sstream>
#include <semaphore.h>
#include <bits/stdc++.h>

using namespace std;
// Address Space Variables for threads

vector<vector<vector<int>>> imgData;
sem_t binarySemaphore;

// Transformation Functions

// Transformation To increase brightness of image
void IncreaseBrightness(int height, int width)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
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
            sem_post(&binarySemaphore);
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
            sem_wait(&binarySemaphore);
            r = imgData[i][j][0];
            g = imgData[i][j][1];
            b = imgData[i][j][2];

            // Convert to grayscale by calculating the weighted sum of current r, g, b values

            int gray = r * (0.299) + g * (0.587) + b * (0.114);
            imgData[i][j][0] = gray;
            imgData[i][j][1] = gray;
            imgData[i][j][2] = gray;
        }
    }
}

int main(int argc, char **argv)
{
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
        }

        imgData.push_back(row);
        row.clear();
    }

    // Close input file
    fclose(input);

    // Threads made for each image transformation
    sem_init(&binarySemaphore, 0, 0);

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

    return 0;
}