import csv
import sys


def main():

    # Check for command-line usage
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py <STR csv> <DNA sequence>")

    # Read STR csv into a dictionary
    STR_csv = []
    with open(sys.argv[1], "r") as file:
        reader = csv.DictReader(file)
        for row in reader:
            STR_csv.append(row)

    # Read DNA sequence file into a list
    with open(sys.argv[2], "r") as file:
        DNA = file.read()

    # Find longest match of each STR in DNA sequence
    STRs = list(STR_csv[0].keys())[1:] # Obtain STR sequences (keys) from STR csv

    STR_count = {} # Create dictionary for STR counts
    for STR in STRs:
        STR_count[STR] = longest_match(DNA, STR) # Call longest_match function on DNA and STR sequences

    # Check database for matching profiles
    for person in STR_csv:
        match = 0 # Initialize match to zero
        for STR in STRs:
            if int(person[STR]) == STR_count[STR]: # If STR key value for person matches STR key value in results
                match += 1 # Update the number of matches

        if match == len(STRs): # If the number of matches equals the number of STRs (ie. if all STRs match)
            print(person["name"]) # Print the persons name
            return

    print("No match") # Print result if no perfect matches found


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
