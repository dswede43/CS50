//Plurality
//---
//The simplest method for an election is the “plurality vote” (aka “winner take all”). Every voter gets to
//vote for one candidate and at the end of the election, whichever candidate has the greatest number of
//votes is declared the winner. This script holds a plurality vote election.

#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    // Impliment linear search
    for (int j = 0; j < candidate_count; j++)
    {
        // Look for candidate name
        if(strcmp(name, candidates[j].name) == 0)
        {
            // Update voter count for candidate
            candidates[j].votes++;
            return true;
        }

    }
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    // Create max votes variable
    int max_votes = 0;

    // Linear search for highest number of votes
    for(int k = 0; k < candidate_count; k++)
    {
        if (candidates[k].votes > max_votes)
        {
            // Assign candidate with most votes
            max_votes = candidates[k].votes;
        }
    }

    // Linear search for candidate with most votes
    for (int l = 0; l < candidate_count; l++)
    {
        if (candidates[l].votes == max_votes)
        {
            // Print name of candidate
            printf("%s\n", candidates[l].name);
        }
    }
}