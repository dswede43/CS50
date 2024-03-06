// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Choose number of buckets in hash table
const unsigned int N = 10000;

// Hash table
node *table[N];

// Define variables
unsigned int word_count; // Word counter
unsigned int hash_code; // Hash code

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Hash text word to return hash code
    hash_code = hash(word);

    // Point traverser to first node of indexed linked list
    node *trav = table[hash_code];

    // Traverse the linked list containing dictionary words
    while (trav != NULL) // While traverser is not NULL
    {
        // If text word matches dictionary word
        if (strcasecmp(word, trav->word) == 0)
        {
            return true;
        }
        trav = trav->next;
    }
    return false;
}

// Hashes word to a number
// I created a modifed version of the djb2 string hashing algorithm by Dan Berstein (http://www.cse.yorku.ca/~oz/hash.html)
unsigned int hash(const char *word)
{
    // Define variables
    unsigned long hash = 5381; //Set seed value
    int c = *word; // Pointer to first char of word (ie. assign c as string)
    c = tolower(c); // Make first letter in word lowercase

    // While letter of word is NOT the null character
    while (*word != '\0')
    {
        hash = ((hash<<5) + hash) + c; /*hash * 2^5 + hash = hash * 33*/
        c = *word++; // Repeat for every letter in word
        c = tolower(c); // Make every letter lowercase
    }
    return hash % N; // Return hash code (remainder when divided by N)
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open dictionary file for reading
    FILE *file = fopen(dictionary, "r");
    if (file == NULL) // If dictionary file cannot be opened
    {
        printf("Unable to open dictionary file %s\n", dictionary); // Print error message
        return false;
    }

    // Define word variable
    char word[LENGTH + 1];

    // While read strings are not the end of file (EOF)
    while (fscanf(file, "%s", word) != EOF)
    {
        // Dynammically allocate memory for new node
        node *n = malloc(sizeof(node));

        //If malloc fails
        if (n == NULL)
        {
            return false;
        }

        // Copy read word into new node
        strcpy(n->word, word);

        // Hash word to return hash code
        hash_code = hash(word);

        // Set pointer of new node to proper linked list of hash table
        n->next = table[hash_code];

        // Set new node as head of linked list
        table[hash_code] = n;

        // Update word counter
        word_count++;
    }
    // Close dictionary file
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // If word counter is greater than zero
    if (word_count > 0)
    {
        return word_count; // Return size of dictionary
    }
    return 0;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // Iterate through hash table
    for (int i = 0; i < N; i++) // For length of hash table
    {
        // Point traverser to first node of linked list
        node *trav = table[i];

        // Traverse the linked list containing dictionary words
        while (trav != NULL) // While trav is not NULL
        {
            // Define temporary pointer variable pointing to same node as traverser
            node *tmp = trav;

            // Move traverser to next node
            trav = trav->next;

            // Free current node
            free(tmp);
        }

        // If traverser is NULL
        if (i == N - 1 && trav == NULL)
        {
            return true;
        }
    }
    return false;
}
