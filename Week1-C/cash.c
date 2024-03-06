//Cash
//---
//Script to minimize the number of coins owed to a customer after a cash transaction (using Greedy algorithm).

#include <cs50.h>
#include <stdio.h>

//list functions
int get_cents(void);
int calculate_quarters(int cents);
int calculate_dimes(int cents);
int calculate_nickels(int cents);
int calculate_pennies(int cents);

int main(void)
{
    //ask how many cents the customer is owed
    int cents = get_cents();

    //calculate the number of quarters to give the customer
    int quarters = calculate_quarters(cents);
    cents = cents - quarters * 25;

    //calculate the number of dimes to give the customer
    int dimes = calculate_dimes(cents);
    cents = cents - dimes * 10;

    //calculate the number of nickels to give the customer
    int nickels = calculate_nickels(cents);
    cents = cents - nickels * 5;

    //calculate the number of pennies to give the customer
    int pennies = calculate_pennies(cents);
    cents = cents - pennies * 1;

    //sum coins
    int coins = quarters + dimes + nickels + pennies;

    //print total number of coins to give the customer
    printf("%i\n", coins);
}

int get_cents(void)
{
    //define cents as integer
    int cents;

    //ask user for change owed
    do
    {
        cents = get_int("Change owed: ");
    }
    while(cents < 0);
    return cents;
}

//create function to calculate the number of quarters given the change owed
int calculate_quarters(int cents)
{
    int quarters = 0;
    while (cents >=25)
    {
        cents = cents - 25;
        quarters++;
    }
    return quarters;
}

//create function to calculate the number of dimes given the change owed
int calculate_dimes(int cents)
{
    int dimes = 0;
    while (cents >= 10)
    {
        cents = cents - 10;
        dimes++;
    }
    return dimes;
}

//create function to calculate the number of nickels given the change owed
int calculate_nickels(int cents)
{
    int nickels = 0;
    while (cents >= 5)
    {
        cents = cents - 5;
        nickels++;
    }
    return nickels;
}

//create function to calculate the number of pennies given the change owed
int calculate_pennies(int cents)
{
    int pennies = 0;
    while (cents >= 1)
    {
        cents = cents - 1;
        pennies++;
    }
    return pennies;
}