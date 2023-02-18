#include <cmath>
#include <chrono>
#include <thread>
#include <vector>
#include <fstream>
#include <sstream>
#include <semaphore.h>
#include <fcntl.h>
#include <bits/stdc++.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <fstream>

using namespace std;
using namespace std::chrono;

// Transformation To increase brightness of image

void IncreaseBrightness(vector<vector<vector<int>>> &imgData, int height, int width, int write_fd)
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

            imgData[i][j][0] = r;
            imgData[i][j][1] = g;
            imgData[i][j][2] = b;
        }
    }

    // Write data to pipe
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            write(write_fd, imgData[i][j].data(), imgData[i][j].size() * sizeof(int));
        }
    }
}

// Transformation To convert image to grayscale
void RBGToGrayScale(int height, int width, ofstream &out_file, int read_fd)
{
    out_file << "P3\n"
             << width << " " << height << "\n"
             << "255\n";
    int r, g, b, gray;


    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Read data from pipe
            vector<int> buffer(3);
            read(read_fd, buffer.data(), buffer.size() * sizeof(int));

            r = buffer[0];
            g = buffer[1];
            b = buffer[2];

            int gray = r * (0.299) + g * (0.587) + b * (0.114);

            out_file << gray << " " << gray << " " << gray << " ";
        }
        out_file << "\n";
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
    auto start = high_resolution_clock::now();
    char ppmVersion[20];
    int imgWidth, imgHeight, imgColorMax, r, g, b;
    FILE *input = fopen(argv[1], "r");
    fscanf(input, "%s%d%d%d", ppmVersion, &imgWidth, &imgHeight, &imgColorMax);
    vector<vector<vector<int>>> imgData;
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

    // Pipes for communication

    int fd[2];

    if (pipe(fd) == -1)
    {
        cerr << "Failed to create pipe." << endl;
        return 1;
    }

    // Fork made to create 2 processes

    pid_t pid = fork();

    if (pid == -1)
    {
        cerr << "Failed to fork process." << endl;
        return 1;
    }

    if (pid == 0)
    {
        ofstream outfile;
        outfile.open(argv[2], ios::trunc); // open file in append mode


        // Child process
        close(fd[1]);
        vector<vector<vector<int>>> input_data(imgHeight, vector<vector<int>>(imgWidth, vector<int>(3)));

        // Transform image to grayscale
        RBGToGrayScale(imgHeight, imgWidth, outfile, fd[0]);

        outfile.close(); // close file descriptor
        close(fd[0]);
    }
    else
    {
        // Parent process
        // Increase brightness of image
        close(fd[0]);
        IncreaseBrightness(imgData, imgHeight, imgWidth, fd[1]);
        close(fd[1]);
        wait(NULL);
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    if (pid > 0)
        cout << "Time of Execution: " << duration.count() << " us" << endl;

    return 0;
}