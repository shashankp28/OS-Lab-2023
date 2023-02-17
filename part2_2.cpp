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

// Transformation Functions
// Transformation To increase brightness of image
int shmid;
key_t key = 110;
int *data;
const char *semName = "binarySemaphore";
sem_t *sem_id = sem_open(semName, O_CREAT, 0644, 1);

void IncreaseBrightness(vector<vector<vector<int>>> &imgData, int height, int width)
{
    // cout << "Parent process started" << endl;
    if (sem_id == SEM_FAILED)
    {
        perror("Parent  : [sem_open] Failed\n");
        return;
    }
    if ((data = (int *)shmat(shmid, NULL, 0)) == (int *)-1)
    {
        perror("shmat");
        exit(1);
    }
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

            // Add data to shared memory
            data[i * width * 3 + j * 3 + 0] = r;
            data[i * width * 3 + j * 3 + 1] = g;
            data[i * width * 3 + j * 3 + 2] = b;
            sem_post(sem_id);
        }
    }
}

// Transformation To convert image to grayscale
void RBGToGrayScale(int height, int width, ofstream &out_file)
{

    if (sem_id == SEM_FAILED)
    {
        perror("Child  : [sem_open] Failed\n");
        return;
    }

    out_file << "P3\n"
           << width << " " << height << "\n"
           << "255\n";
    int r, g, b, gray;
    if ((data = (int *)shmat(shmid, NULL, 0)) == (int *)-1)
    {
        perror("shmat");
        exit(1);
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            sem_wait(sem_id);

            r = data[i * width * 3 + j * 3 + 0];
            g = data[i * width * 3 + j * 3 + 1];
            b = data[i * width * 3 + j * 3 + 2];

            // Convert to grayscale by calculating the weighted sum of current r, g, b values

            int gray = r * (0.299) + g * (0.587) + b * (0.114);
            
            out_file << gray << " " << gray << " " << gray << " ";
        }
        out_file << "\n";
    }

    if (shmdt(data) == -1)
    {
        perror("shmdt");
        exit(1);
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

    // Fork made to create 2 processes
    // sem_init(&binarySemaphore, 0, 0);
    if ((shmid = shmget(key, imgWidth * imgHeight * 3 * sizeof(int), IPC_CREAT | 0666)) < 0)
    {
        cout << "shmget error" << endl;
        perror("shmget");
        return 1;
    }

    int pid = fork();

    if (pid == 0)
    {
        ofstream outfile;
        cout<<argv[2]<<endl;
        // string out = argv[2];
        outfile.open(argv[2], ios::trunc); // open file in append mode
        cout << outfile.is_open() << endl;
        // Child process
        // Transform image to grayscale
        RBGToGrayScale(imgHeight, imgWidth, outfile);
        outfile.close(); // close file descriptor
    }
    else
    {
        // Parent process
        // Increase brightness of image
        IncreaseBrightness(imgData, imgHeight, imgWidth);
        wait(NULL);
    }

    // delete the shared memory segment
    if (shmctl(shmid, IPC_RMID, NULL) == -1)
    {
        perror("shmctl");
        exit(1);
    }
    // Waiting for T1 & T2 to complete execution

    // Write transformed image to output file
    // out_file.close();
    return 0;
}