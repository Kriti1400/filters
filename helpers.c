#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // find average of red, green, and blue
    int average = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            average = round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / (float)3.0);

            // apply the averages to the image
            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue = average;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // compute new rgb values
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            double sepiaRed = round((.393 * image[i][j].rgbtRed) + (.769 * image[i][j].rgbtGreen) + (.189 * image[i][j].rgbtBlue));
            double sepiaGreen = round((.349 * image[i][j].rgbtRed) + (.686 * image[i][j].rgbtGreen) + (.168 * image[i][j].rgbtBlue));
            double sepiaBlue = round((.272 * image[i][j].rgbtRed) + (.534 * image[i][j].rgbtGreen) + (.131 * image[i][j].rgbtBlue));

            // cap at 225 value
            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }
            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }
            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }

            // apply the new values to the image
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
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < (width / 2); j++)
        {
            //swaps pixels on opposite sides
            RGBTRIPLE tmp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = tmp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //initialize a tmp array
    RGBTRIPLE tmp[height][width];

    // set tmp array to value of original image so that the blured image is not used for future calculations
    for (int k = 0; k < height; k++)
    {
        for (int l = 0; l < width; l++)
        {
            tmp[k][l].rgbtRed = image[k][l].rgbtRed;
            tmp[k][l].rgbtGreen = image[k][l].rgbtGreen;
            tmp[k][l].rgbtBlue = image[k][l].rgbtBlue;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {

            //introduces variables for colour storage
            int sumRed = 0;
            int sumGreen = 0;
            int sumBlue = 0;

            int avgRed = 0;
            int avgGreen = 0;
            int avgBlue = 0;

            int count = 0;

            // adds the values for the pixel
            // adds image[i] first to ensure that it is not repeated
            sumRed += tmp[i][j].rgbtRed;
            sumGreen += tmp[i][j].rgbtGreen;
            sumBlue += tmp[i][j].rgbtBlue;
            count++;

            // case for pixels at the top
            if (i == 0)
            {
                sumRed += tmp[i + 1][j].rgbtRed;
                sumGreen += tmp[i + 1][j].rgbtGreen;
                sumBlue += tmp[i + 1][j].rgbtBlue;
                count++;

                //case for top left corner pixel
                if (j == 0)
                {
                    sumRed = sumRed + tmp[i][j + 1].rgbtRed + tmp[i + 1][j + 1].rgbtRed;
                    sumGreen = sumGreen + tmp[i][j + 1].rgbtGreen + tmp[i + 1][j + 1].rgbtGreen;
                    sumBlue = sumBlue + tmp[i][j + 1].rgbtBlue + tmp[i + 1][j + 1].rgbtBlue;
                    count += 2;
                }

                //case for top right corner pixel
                else if (j == width - 1)
                {
                    sumRed = sumRed + tmp[i][j - 1].rgbtRed + tmp[i + 1][j - 1].rgbtRed;
                    sumGreen = sumGreen + tmp[i][j - 1].rgbtGreen + tmp[i + 1][j - 1].rgbtGreen;
                    sumBlue = sumBlue + tmp[i][j - 1].rgbtBlue + tmp[i + 1][j - 1].rgbtBlue;
                    count += 2;
                }

                //case for all other pixels along the top row
                else
                {
                    sumRed = sumRed + tmp[i][j - 1].rgbtRed + tmp[i][j + 1].rgbtRed + tmp[i + 1][j + 1].rgbtRed + tmp[i + 1][j - 1].rgbtRed;
                    sumGreen = sumGreen + tmp[i][j - 1].rgbtGreen + tmp[i][j + 1].rgbtGreen + tmp[i + 1][j + 1].rgbtGreen + tmp[i + 1][j - 1].rgbtGreen;
                    sumBlue = sumBlue + tmp[i][j - 1].rgbtBlue + tmp[i][j + 1].rgbtBlue + tmp[i + 1][j + 1].rgbtBlue + tmp[i + 1][j - 1].rgbtBlue;
                    count += 4;
                }
            }

            // case for pixels at the bottom
            else if (i == (height - 1))
            {
                sumRed += tmp[i - 1][j].rgbtRed;
                sumGreen += tmp[i - 1][j].rgbtGreen;
                sumBlue += tmp[i - 1][j].rgbtBlue;
                count++;

                // case for bottom left corner pixel
                if (j == 0)
                {
                    sumRed = sumRed + tmp[i][j + 1].rgbtRed + tmp[i - 1][j + 1].rgbtRed;
                    sumGreen = sumGreen + tmp[i][j + 1].rgbtGreen + tmp[i - 1][j + 1].rgbtGreen;
                    sumBlue = sumBlue + tmp[i][j + 1].rgbtBlue + tmp[i - 1][j + 1].rgbtBlue;
                    count += 2;
                }

                //case for bottom right corner pixel
                else if (j == (width - 1))
                {
                    sumRed = sumRed + tmp[i][j - 1].rgbtRed + tmp[i - 1][j - 1].rgbtRed;
                    sumGreen = sumGreen + tmp[i][j - 1].rgbtGreen + tmp[i - 1][j - 1].rgbtGreen;
                    sumBlue = sumBlue + tmp[i][j - 1].rgbtBlue + tmp[i - 1][j - 1].rgbtBlue;
                    count += 2;
                }

                //case for all other pixels along the bottom row
                else
                {
                    sumRed = sumRed + tmp[i][j - 1].rgbtRed + tmp[i][j + 1].rgbtRed + tmp[i - 1][j - 1].rgbtRed + tmp[i - 1][j + 1].rgbtRed;
                    sumGreen = sumGreen + tmp[i][j - 1].rgbtGreen + tmp[i][j + 1].rgbtGreen + tmp[i - 1][j - 1].rgbtGreen + tmp[i - 1][j + 1].rgbtGreen;
                    sumBlue = sumBlue + tmp[i][j - 1].rgbtBlue + tmp[i][j + 1].rgbtBlue + tmp[i - 1][j - 1].rgbtBlue + tmp[i - 1][j + 1].rgbtBlue;
                    count += 4;
                }
            }

            // case for all other pixels
            else
            {
                // case for pixels on the left side
                if (j == 0)
                {
                    sumRed = sumRed + tmp[i][j + 1].rgbtRed + tmp[i - 1][j].rgbtRed + tmp[i + 1][j].rgbtRed + tmp[i - 1][j + 1].rgbtRed +
                             tmp[i + 1][j + 1].rgbtRed;
                    sumGreen = sumGreen + tmp[i][j + 1].rgbtGreen + tmp[i - 1][j].rgbtGreen + tmp[i + 1][j].rgbtGreen + tmp[i - 1][j + 1].rgbtGreen +
                               tmp[i + 1][j + 1].rgbtGreen;
                    sumBlue = sumBlue + tmp[i][j + 1].rgbtBlue + tmp[i - 1][j].rgbtBlue + tmp[i + 1][j].rgbtBlue + tmp[i - 1][j + 1].rgbtBlue +
                              tmp[i + 1][j + 1].rgbtBlue;
                    count += 5;
                }

                //case for pixels on the right side
                else if (j == (width - 1))
                {
                    sumRed = sumRed + tmp[i][j - 1].rgbtRed + tmp[i - 1][j].rgbtRed + tmp[i + 1][j].rgbtRed + tmp[i - 1][j - 1].rgbtRed +
                             tmp[i + 1][j - 1].rgbtRed;
                    sumGreen = sumGreen + tmp[i][j - 1].rgbtGreen + tmp[i - 1][j].rgbtGreen + tmp[i + 1][j].rgbtGreen + tmp[i - 1][j - 1].rgbtGreen +
                               tmp[i + 1][j - 1].rgbtGreen;
                    sumBlue = sumBlue + tmp[i][j - 1].rgbtBlue + tmp[i - 1][j].rgbtBlue + tmp[i + 1][j].rgbtBlue + tmp[i - 1][j - 1].rgbtBlue +
                              tmp[i + 1][j - 1].rgbtBlue;
                    count += 5;
                }

                // final case for pixels in the middle
                else if ((i != 0) && (i != (height - 1)) && (j != 0) && (j != (width - 1)))
                {
                    sumRed = sumRed + tmp[i][j - 1].rgbtRed + tmp[i][j + 1].rgbtRed + tmp[i - 1][j].rgbtRed + tmp[i + 1][j].rgbtRed +
                             tmp[i - 1][j + 1].rgbtRed + tmp[i - 1][j - 1].rgbtRed + tmp[i + 1][j - 1].rgbtRed + tmp[i + 1][j + 1].rgbtRed;
                    sumGreen = sumGreen + tmp[i][j - 1].rgbtGreen + tmp[i][j + 1].rgbtGreen + tmp[i - 1][j].rgbtGreen + tmp[i + 1][j].rgbtGreen +
                               tmp[i - 1][j + 1].rgbtGreen + tmp[i - 1][j - 1].rgbtGreen + tmp[i + 1][j - 1].rgbtGreen + tmp[i + 1][j + 1].rgbtGreen;
                    sumBlue = sumBlue + tmp[i][j - 1].rgbtBlue + tmp[i][j + 1].rgbtBlue + tmp[i - 1][j].rgbtBlue + tmp[i + 1][j].rgbtBlue +
                              tmp[i - 1][j + 1].rgbtBlue + tmp[i - 1][j - 1].rgbtBlue + tmp[i + 1][j - 1].rgbtBlue + tmp[i + 1][j + 1].rgbtBlue;
                    count += 8;
                }
            }

            // calculate the average
            avgRed = round(sumRed / (float)count);
            avgGreen = round(sumGreen / (float)count);
            avgBlue = round(sumBlue / (float)count);

            // apply the average to the pixel
            image[i][j].rgbtRed = avgRed;
            image[i][j].rgbtGreen = avgGreen;
            image[i][j].rgbtBlue = avgBlue;
        }
    }
    return;
}