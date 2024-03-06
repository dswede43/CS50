//Readability
//---
//A number of “readability tests” have been developed for computing the reading level of a text. One such
//is the Coleman-Liau index, which is designed to output the grade level (US) that is needed to understand
//some text.

//The formula is: Index = 0.0588L – 0.296S – 15.8,
//L = average number of letters per 100 words
//S = average number of sentences per 100 words

//This script applies the Coleman-Liau index to a given text to measure its reading level.

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int CL_index(string text);

int main(void)
{
    // Ask user for text
    string text = get_string("Text: ");

    //Calculate reading level
    int level = CL_index(text);

    // Print grade reading level
    if (level < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (level > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", level);
    }
}

int CL_index(string text)
{
    // Initialize variables
    int letters = 0;
    int words = 1;
    int sentences = 0;

    // Count number of letters, words and sentences
    for (int i = 0; i < strlen(text); i++)
    {
        // Count number of letters
        if (isupper(text[i]) || islower(text[i]))
        {
            letters++;
        }

        // Count number of words
        else if (text[i] == ' ')
        {
            words++;
        }

        // Count number of sentences
        else if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentences++;
        }
    }

    // Define L and S variables
    float L = (float) letters / (float) words * 100;
    float S = (float) sentences / (float) words * 100;

    // Calculate reading level using Coleman-Liau index
    int index = round(0.0588 * L - 0.296 * S - 15.8);
    return index;
}