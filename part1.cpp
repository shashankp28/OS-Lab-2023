#include <bits/stdc++.h>
#include <chrono>
#include <cmath>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

// Transformation Functions

void IncreaseBrightness(vector<vector<vector<int>>> &data, int height, int width)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int r = data[i][j][0];
            int g = data[i][j][1];
            int b = data[i][j][2];

            // Increase brightness of each channel by the specified amount
            r = min(int(r * 3), 255);
            g = min(int(g * 3), 255);
            b = min(int(b * 3), 255);

            // Update pixel values
            data[i][j][0] = r;
            data[i][j][1] = g;
            data[i][j][2] = b;
        }
    }
}

void RBGToGrayScale(vector<vector<vector<int>>> &data, int height, int width)
{
    int r, g, b, gray;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            r = data[i][j][0];
            g = data[i][j][1];
            b = data[i][j][2];

            // Convert to grayscale by calculating the weighted sum of current r, g, b values

            int gray = r * (0.299) + g * (0.587) + b * (0.114);
            data[i][j][0] = gray;
            data[i][j][1] = gray;
            data[i][j][2] = gray;
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
    vector<vector<vector<int>>> imgData;

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

    // Transform Images
    IncreaseBrightness(imgData, imgHeight, imgWidth);
    RBGToGrayScale(imgData, imgHeight, imgWidth);

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