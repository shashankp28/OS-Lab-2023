// Create shared memoory segment
if ((shmid = shmget(key, imgWidth *imgHeight * 3 * sizeof(int), IPC_CREAT | 0666)) < 0)
{
    cout << "shmget error" << endl;
    perror("shmget");
    return 1;
}

// Fork the process
int pid = fork();

// Child Process second transformation
if (pid == 0)
{
    ofstream outfile;
    // string out = argv[2];
    outfile.open(argv[2], ios::trunc); // open file in append mode
    // Child process
    // Transform image to grayscale
    RBGToGrayScale(imgHeight, imgWidth, outfile);
    outfile.close(); // close file descriptor
}

// Parent process first transformation
else
{
    // Parent process
    // Increase brightness of image
    IncreaseBrightness(imgData, imgHeight, imgWidth);
    wait(NULL);
}