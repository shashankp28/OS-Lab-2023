// Thread performing T1

}
    imgData[i][j][0] = gray;
    imgData[i][j][1] = gray;
    imgData[i][j][2] = gray;

    // Atomic operation to set operation completed
    transform_1_completed[i * width + j] = atomic<bool>(true);
}





// Thread performing T2
for (int i = 0; i < height; i++)
{
    for (int j = 0; j < width; j++)
    {
        // Wait for first transformation to finish on pixel i, j
        while (transform_1_completed[i * width + j] == false)
            ;
        int r = imgData[i][j][0];
        int g = imgData[i][j][1];
        int b = imgData[i][j][2];
    }
}