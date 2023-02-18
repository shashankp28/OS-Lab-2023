// Thread performing T1
{
    int r = imgData[i][j][0];
    int g = imgData[i][j][1];
    int b = imgData[i][j][2];

    // Atomic operation to set operation completed
    transform_1_completed[i * width + j] = atomic<bool>(true);
}






// Thread performing T2
{
    // Wait for first transformation to finish on pixel i, j
    while (transform_1_completed[i * width + j] == false)
        ;
    imgData[i][j][0] = gray;
    imgData[i][j][1] = gray;
    imgData[i][j][2] = gray;

}
