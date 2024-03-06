//Filter
//---
//Functions to apply various filters to .bmp images. These filters include
//grayscale, sepia, horizontal reflection and blurring.

#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // For height of image (row)
    for (int i = 0; i < height; i++)
    {
        // For width of image (col)
        for (int j = 0; j < width; j++)
        {
            // Convert pixels to floats
            float Red = image[i][j].rgbtRed;
            float Green = image[i][j].rgbtGreen;
            float Blue = image[i][j].rgbtBlue;

            // Find average of RGB values of pixels
            int avg = round((Red + Green + Blue) / 3);

            // Update pixel RGB values of pixels to the average
            image[i][j].rgbtRed = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtBlue = avg;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // For height of image (row)
    for (int i = 0; i < height; i++)
    {
        // For width of image (col)
        for (int j = 0; j < width; j++)
        {
            // Convert pixels to floats
            float originalRed = image[i][j].rgbtRed;
            float originalGreen = image[i][j].rgbtGreen;
            float originalBlue = image[i][j].rgbtBlue;

            // Apply sepia algorithm to pixels
            int sepiaRed = round(.393 * originalRed + .769 * originalGreen + .189 * originalBlue);
            int sepiaGreen = round(.349 * originalRed + .686 * originalGreen + .168 * originalBlue);
            int sepiaBlue = round(.272 * originalRed + .534 * originalGreen + .131 * originalBlue);

            // Cap RGB pixel values to 255
            if (sepiaRed > 255) // Sepia red
            {
                sepiaRed = 255;
            }

            if (sepiaGreen > 255) // Sepia green
            {
                sepiaGreen = 255;
            }

            if (sepiaBlue > 255) // Sepia blue
            {
                sepiaBlue = 255;
            }

            // Update RGB values of pixels to sepia
            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // For height of image (row)
    for (int i = 0; i < height; i++)
    {
        // For width of image (col)
        for (int j = 0; j < width / 2; j++)
        {
            // Swap pixel values along width of image
            RGBTRIPLE tmp = image[i][j]; // Move original pixel values to temporary variable
            image[i][j] = image[i][width - (j + 1)]; // Move new pixel values to original pixel value location
            image[i][width - (j + 1)] = tmp; // Move temporary pixel values to location of new pixel values
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Create temporary copt of image
    RGBTRIPLE tmp[height][width];

    // For height of image (row)
    for (int i = 0; i < height; i++)
    {
        // For width of image (col)
        for (int j = 0; j < width; j++)
        {
            tmp[i][j] = image[i][j]; // Assign original pixel values to temporary array
        }
    }

    // For height of image (row)
    for (int i = 0; i < height; i++)
    {
        // For width of image (col)
        for (int j = 0; j < width; j++)
        {
            // Define variables to count values for valid neighboring pixels
            int totalRed = 0; // Red values
            int totalGreen = 0; // Green values
            int totalBlue = 0; // Blue values

            //Define float counter for number of valid neighboring pixels
            float counter = 0.00;

            // Determine valid neighboring pixels
            for (int x = -1; x < 2; x++) // For neighboring pixel heights (row)
            {
                for (int y = -1; y < 2; y++) // For neighboring pixel widths (col)
                {
                    // Define valid neighboring pixels
                    int currentX = i + x;
                    int currentY = j + y;

                    // Define invalid neighboring pixels
                    if (currentX < 0 || currentY < 0 || currentX > (height - 1) || currentY > (width - 1))
                    {
                        continue;
                    }

                    // Add up values for valid neighboring pixels
                    totalRed += image[currentX][currentY].rgbtRed;
                    totalGreen += image[currentX][currentY].rgbtGreen;
                    totalBlue += image[currentX][currentY].rgbtBlue;

                    // Add up total number of valid neighboring pixels
                    counter++;
                }

                // Calculate average value of valid neighboring pixels
                tmp[i][j].rgbtRed = round(totalRed / counter);
                tmp[i][j].rgbtGreen = round(totalGreen / counter);
                tmp[i][j].rgbtBlue = round(totalBlue / counter);
            }
        }
    }

    // Copy new pixel values into original image
    // For height of image (row)
    for (int i = 0; i < height; i++)
    {
        // For width of image (col)
        for (int j = 0; j < width; j++)
        {
            // Assign temporary pixel values to original image pixels
            image[i][j].rgbtRed = tmp[i][j].rgbtRed; // For red
            image[i][j].rgbtGreen = tmp[i][j].rgbtGreen; // For green
            image[i][j].rgbtBlue = tmp[i][j].rgbtBlue; // For blue
        }
    }
    return;
}