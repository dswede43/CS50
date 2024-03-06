//Runoff
//---
//For a democratic election, considering the ranked preferences of voters allows for a chosen winner that
//may better reflect the preferences of voters. One such ranked choice voting system is the instant runoff
//system. The process is the following: Voters rank the candidates in order of preference. Ballots are
//initially counted for each voter's top choice. If a candidate has more than half of the first-choice votes,
//that candidate wins. If not, then the candidate with the fewest votes is eliminated, and the voters who
//selected the defeated candidate as a first choice then have their votes added to the totals of their next
//choice. This process continues until a candidate has more than half of the votes.

//This script holds an instant runoff election.

#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    // For candidates (candidates matrix cols)
    for (int i = 0; i < candidate_count; i++)
    {
        // If input name matches a candidates name
        if (strcmp(name, candidates[i].name) == 0)
        {
            // Update voter preference
            preferences[voter][rank] = i;
            return true;
        }
    }
    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    // For voters (preferences matrix rows)
    for (int i = 0; i < voter_count; i++)
    {
        // For candidates (preferences matrix cols)
        for (int j = 0; j < candidate_count; j++)
        {
            // If candidate is not eliminated
            if (candidates[preferences[i][j]].eliminated == false)
            {
                // Add vote to candidate
                candidates[preferences[i][j]].votes++;
                break;
            }
        }
    }
    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    // For candidates (candidates matrix cols)
    for (int i = 0; i < candidate_count; i++)
    {
        // If a candidates vote is greater than the majority
        if (candidates[i].votes > voter_count / 2)
        {
            // Print the name of the candidate with majority votes
            printf("%s\n", candidates[i].name);
            return true;
        }
    }
    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    // Variable defining the min number of votes
    int min = voter_count;

    // For candidates (candidates matrix cols)
    for (int i = 0; i < candidate_count; i++)
    {
        // If a candidate is not eliminated and has less votes than the min
        if (candidates[i].eliminated == false && candidates[i].votes < min)
        {
            // Update this candidate as having the min number of votes
            min = candidates[i].votes;
        }
    }
    return min;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    // Define variables
    int remaining = 0; // Number of candidates remaining
    int min_count = 0; // Number of candidates with min votes

    // For candidates (candidates matrix cols)
    for (int i = 0; i < candidate_count; i++)
    {
        // If a candidate is eliminated
        if (!candidates[i].eliminated)
        {
            // Update the number of remaining candidates
            remaining++;
        }
        // If a candidate has the min number of votes
        if (candidates[i].votes == min)
        {
            // Update the number of candidates with the min votes
            min_count++;
        }
    }

    // If the number of remaining candidates is equal to the number of candidates with min votes
    if (remaining == min_count)
    {
        // There is a tie
        return true;
    }
    return false;
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    // For candidates (candidates matrix cols)
    for (int i = 0; i < candidate_count; i++)
    {
        // If a candidate has the min number of votes
        if (candidates[i].votes == min)
        {
            // Eliminate candidate
            candidates[i].eliminated = true;
        }
    }
    return;
}