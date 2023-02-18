// First Thread Transformation
{
    // Update pixel values
    imgData[i][j][0] = r;
    imgData[i][j][1] = g;
    imgData[i][j][2] = b;

    // Increment sem post variable in first transformation
    sem_post(&binarySemaphore);
}

// Second Thread Transformation
{
    // Wait for first process to call sem_post
    sem_wait(&binarySemaphore);
    r = imgData[i][j][0];
    g = imgData[i][j][1];
    b = imgData[i][j][2];
}