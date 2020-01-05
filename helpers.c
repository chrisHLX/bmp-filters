#include "helpers.h"
#include <string.h>
#include <math.h>

// Need to treat RGBTRIPLE as double not int to get a more precise output
int average(double x, double y, double k);
int sepiaR(double b, double r, double g);
int sepiaG(double b, double r, double g);
int sepiaB(double b, double r, double g);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int same = average(image[i][j].rgbtBlue, image[i][j].rgbtRed, image[i][j].rgbtGreen);
            image[i][j].rgbtBlue = same;
            image[i][j].rgbtRed = same;
            image[i][j].rgbtGreen = same;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE sepia[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            sepia[i][j].rgbtBlue = sepiaB(image[i][j].rgbtBlue, image[i][j].rgbtRed, image[i][j].rgbtGreen);
            sepia[i][j].rgbtRed = sepiaR(image[i][j].rgbtBlue, image[i][j].rgbtRed, image[i][j].rgbtGreen);
            sepia[i][j].rgbtGreen = sepiaG(image[i][j].rgbtBlue, image[i][j].rgbtRed, image[i][j].rgbtGreen);
        }
    }

    // Need to store sepia colors in external RBTRIPLE to avoid changing the original colors when converting to sepia
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = sepia[i][j].rgbtBlue;
            image[i][j].rgbtRed = sepia[i][j].rgbtRed;
            image[i][j].rgbtGreen = sepia[i][j].rgbtGreen;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE flip[height][width];
    // Start writing the images from the right to left in the new RGBTRIPLE
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            flip[i][width - (j + 1)].rgbtBlue = image[i][j].rgbtBlue;
            flip[i][width - (j + 1)].rgbtRed = image[i][j].rgbtRed;
            flip[i][width - (j + 1)].rgbtGreen = image[i][j].rgbtGreen;
        }
    }

    // Recompose the original image using the flipped RGBTRIPLE
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = flip[i][j].rgbtBlue;
            image[i][j].rgbtRed = flip[i][j].rgbtRed;
            image[i][j].rgbtGreen = flip[i][j].rgbtGreen;
        }
    }


    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    double blurRed = 0.0;
    double blurGreen = 0.0;
    double blurBlue = 0.0;
    int count;

    RGBTRIPLE blr[height][width];

    // Get the blurred vals and store them blr to avoid blurring an already blurred rgbt
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (i + 1 >= height && j + 1 >= width)
            {
                count = 0;
                for (int x = -1; x < 1; x++)
                {
                    for (int y = -1; y < 1; y++)
                    {
                        blurRed += round(image[i + x][j + y].rgbtRed);
                        blurGreen += round(image[i + x][j + y].rgbtGreen);
                        blurBlue += round(image[i + x][j + y].rgbtBlue);
                        count++;
                    }
                }
            }
            else if (i - 1 < 0 && j + 1 >= width)
            {
                count = 0;
                for (int x = 0; x < 2; x++)
                {
                    for (int y = -1; y < 1; y++)
                    {
                        blurRed += round(image[i + x][j + y].rgbtRed);
                        blurGreen += round(image[i + x][j + y].rgbtGreen);
                        blurBlue += round(image[i + x][j + y].rgbtBlue);
                        count++;
                    }
                }
            }
            else if (i - 1 < 0 && j - 1 < 0)
            {
                count = 0;
                for (int x = 0; x < 2; x++)
                {
                    for (int y = 0; y < 2; y++)
                    {
                        blurRed += round(image[i + x][j + y].rgbtRed);
                        blurGreen += round(image[i + x][j + y].rgbtGreen);
                        blurBlue += round(image[i + x][j + y].rgbtBlue);
                        count++;
                    }
                }
            }
            else if (i + 1 >= height && j - 1 < 0)
            {
                count = 0;
                for (int x = -1; x < 1; x++)
                {
                    for (int y = 0; y < 2; y++)
                    {
                        blurRed += round(image[i + x][j + y].rgbtRed);
                        blurGreen += round(image[i + x][j + y].rgbtGreen);
                        blurBlue += round(image[i + x][j + y].rgbtBlue);
                        count++;
                    }
                }
            }
            else if (i + 1 >= height)
            {
                count = 0;
                for (int x = -1; x < 1; x++)
                {
                    for (int y = -1; y < 2; y++)
                    {
                        blurRed += round(image[i + x][j + y].rgbtRed);
                        blurGreen += round(image[i + x][j + y].rgbtGreen);
                        blurBlue += round(image[i + x][j + y].rgbtBlue);
                        count++;
                    }
                }
            }
            else if (j + 1 >= width)
            {
                count = 0;
                for (int x = -1; x < 2; x++)
                {
                    for (int y = -1; y < 1; y++)
                    {
                        blurRed += round(image[i + x][j + y].rgbtRed);
                        blurGreen += round(image[i + x][j + y].rgbtGreen);
                        blurBlue += round(image[i + x][j + y].rgbtBlue);
                        count++;
                    }
                }
            }
            else if (i - 1 < 0)
            {
                count = 0;
                for (int x = 0; x < 2; x++)
                {
                    for (int y = -1; y < 2; y++)
                    {
                        blurRed += round(image[i + x][j + y].rgbtRed);
                        blurGreen += round(image[i + x][j + y].rgbtGreen);
                        blurBlue += round(image[i + x][j + y].rgbtBlue);
                        count++;
                    }
                }
            }
            else if (j - 1 < 0)
            {
                count = 0;
                for (int x = -1; x < 2; x++)
                {
                    for (int y = 0; y < 2; y++)
                    {
                        blurRed += round(image[i + x][j + y].rgbtRed);
                        blurGreen += round(image[i + x][j + y].rgbtGreen);
                        blurBlue += round(image[i + x][j + y].rgbtBlue);
                        count++;
                    }
                }
            }
            else
            {
                count = 0;
                for (int x = -1; x < 2; x++)
                {
                    for (int y = -1; y < 2; y++)
                    {
                        blurRed += round(image[i + x][j + y].rgbtRed);
                        blurGreen += round(image[i + x][j + y].rgbtGreen);
                        blurBlue += round(image[i + x][j + y].rgbtBlue);
                        count++;
                    }
                }
            }

            // Actual image updates
            blr[i][j].rgbtRed = round(round(blurRed) / count);
            blr[i][j].rgbtGreen = round(round(blurGreen) / count);
            blr[i][j].rgbtBlue = round(round(blurBlue) / count);

            blurRed = 0.0;
            blurGreen = 0.0;
            blurBlue = 0.0;
        }
    }

    // Write to the actual image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0 ; j < width; j++)
        {
            image[i][j].rgbtRed = blr[i][j].rgbtRed;
            image[i][j].rgbtGreen = blr[i][j].rgbtGreen;
            image[i][j].rgbtBlue = blr[i][j].rgbtBlue;
        }
    }
    return;
}

// Function to work out the average needed for the greyscale filter
int average(double x, double y, double k)
{
    int av = round((x + y + k) / 3);
    return av;
}

// Sepia algorithm sets highest value to be 255
int sepiaR(double b, double r, double g)
{
    double originalBlue = b;
    double originalRed = r;
    double originalGreen = g;

    int sepiaRed = round((.393 * originalRed) + (.769 * originalGreen) + (.189 * originalBlue));

    if (sepiaRed >= 255)
    {
        return 255;
    }
    else
    {
        return sepiaRed;
    }
}

int sepiaG(double b, double r, double g)
{
    double originalBlue = b;
    double originalRed = r;
    double originalGreen = g;

    int sepiaGreen = round((.349 * originalRed) + (.686 * originalGreen) + (.168 * originalBlue));

    if (sepiaGreen >= 255)
    {
        return 255;
    }
    else
    {
        return sepiaGreen;
    }
}

int sepiaB(double b, double r, double g)
{
    double originalBlue = b;
    double originalRed = r;
    double originalGreen = g;

    int sepiaBlue = round((.272 * originalRed) + (.534 * originalGreen) + (.131 * originalBlue));

    if (sepiaBlue >= 255)
    {
        return 255;
    }
    else
    {
        return sepiaBlue;
    }
}
