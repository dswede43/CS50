//Recover
//---
//Script to recover deleted .jpg image files from the memory card of a camera using the FAT
//file system.

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    // If the command-line argument count is not 2
    if (argc != 2)
    {
        printf("Usage: ./recover <.jpg file>\n"); // Print usage warning message
        return 1;
    }

    // Open memory card input file
    FILE *infile = fopen(argv[1], "r");

    // If input file cannot open (NULL)
    if (infile == NULL)
    {
        printf("Could not open input file\n");
        return 2;
    }

    // Define variables
    unsigned char buffer[512]; // Group bytes into array of 512 bytes
    int image_cts = 0; // Counter for number of images recovered
    FILE *outfile = NULL; // File pointer for recovered JPEGs
    char *filename = malloc(8 * sizeof(char)); // Dynamically allocate memory block for filename

    // While reading memory card in blocks of 512 bytes
    while (fread(buffer, sizeof(char), 512, infile))
    {
        // If block begins with JPEG hexidemical values
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // Create filename for recovered JPEG
            sprintf(filename, "%03i.jpg", image_cts);

            // Open output file to write JPEG data
            outfile = fopen(filename, "w");

            // Update image counter
            image_cts++;
        }
        // If output file is not NULL
        if (outfile != NULL)
        {
            // Write JPEG data to output file
            fwrite(buffer, sizeof(char), 512, outfile);
        }
    }
    // Free dynamically allocated memory for filenames
    free(filename);

    // Close opened files
    fclose(infile); // Input file
    fclose(outfile); // Output file

    return 0;
}