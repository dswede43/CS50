//Mario
//---
//Toward the beginning of World 1-1 in Nintendo’s Super Mario Brothers, Mario must hop over adjacent
//pyramids of blocks. This script creates those pyramids with ASCII graphics.

#include <cs50.h>
#include <stdio.h>

int main(void)
{
	//define integer variable
	int n;
	
	do
	{
		//prompt user for integer
		n = get_int("Height: ");
	}
	while (n < 1 || n > 8);

	//print pyramid
	//for each row
	for (int i = 0; i < n; i++)
	{
		//for each col
		for (int j = 0; j < n; j++)
		{
			//print left side pyramid
			if (i + j >= n - 1)
			printf("#");

			else
			printf(" ");
		}

		//add two spaces
		printf(" ");

		for (int j = 0; j < n; j++)
		{
			//print right side pyramid
			if (i + j >= n - 1)
			printf("#");
		}
		
		//move to next row
		printf("\n");
	}
}
