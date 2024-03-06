//Caesar
//---
//Encryption converts plaintext into ciphertext using a key (ie. plaintext + key = ciphertext).
//Julius Caesar used to encrypt messages by shifting each letter therein by some number of places.
//This script applies Caesars encryption key to any plaintext message.

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int main(int argc, string argv[])
{
    // Check for 2 command-line arguments
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    // Check for character of 2nd command-line argument
    for (int i = 0; i < strlen(argv[1]); i++)
    {
        if (!isdigit(argv[1][i]))
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }

    // Convert argv[1] from string to int
    int key = atoi(argv[1]);

    // Ask user for plaintext
    string plaintext = get_string("Plaintext: ");

    // Print ciphertext
    printf("Ciphertext: ");

    // Apply key to plaintext
    for (int j = 0; j < strlen(plaintext); j++)
    {
        // For lowercase letters
        if (islower(plaintext[j]))
        {
            printf("%c", (plaintext[j] - 97 + key) % 26 + 97);
        }

        // For uppercase letters
        else if (isupper(plaintext[j]))
        {
            printf("%c", (plaintext[j] - 65 + key) % 26 + 65);
        }

        // For non-letter characters
        else
        {
            printf("%c", plaintext[j]);
        }
    }
    printf("\n");
}