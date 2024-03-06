//Hello
//---
//Script to print hello <user>.

#include <stdio.h>
#include <cs50.h>

int main(void)
{
	//ask for users name
	string name = get_string("What is your name? ");
	
	//print users name
	printf("hello, %s\n", name);
}
