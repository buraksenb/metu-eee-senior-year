#include <iostream>
#include "Sokoban.h"
#include  "StackQueue.h"
#include  "StackQueue.cpp"
#include "StackQueueLL.h"
#include "StackQueueLL.cpp"
// TIME SPENT FOR THIS ASSIGNMENT IS APPROXIMATELY 10 HOURS
// THIS VALUE IS 1 FOR Q3 AND IT IS 0 FOR Q4
#define QUESTION 1

int main()
{

	Sokoban puzzleboard("map1.txt");
	// QUESTION == 1 , StackQueue is used which is for Question 3 implementation
#if QUESTION
	StackQueue<Sokoban> allmoves;
	// ELSE StackQueueLL is used which is the linked list version of StackQueue
	// And it is for Question 4 implementation.
#else
	StackQueueLL<Sokoban> allmoves;
#endif


	puzzleboard.print_puzzle();
	allmoves.push_front(puzzleboard);
	char input;
	 while(true)
	{
	 	// Check if puzzle is solved at every loop.
	 	if(puzzleboard.is_solved())
	 	{
			std::cout << "Puzzle is solved. Congratulations!" << std::endl;
	 	}
		std::cout << "Next move: ";
		std::cin >> input;
		if(input == 'a' || input == 'A')
		{
			if(puzzleboard.move_left())
			{
				allmoves.push_front(puzzleboard);
			}
			puzzleboard.print_puzzle();

		}
		else if(input == 's' || input == 'S')
		{
			if (puzzleboard.move_down())
			{
				allmoves.push_front(puzzleboard);
			}
			puzzleboard.print_puzzle();
		}
		else if (input == 'd' || input == 'D')
		{
			if (puzzleboard.move_right())
			{
				allmoves.push_front(puzzleboard);
			}
			puzzleboard.print_puzzle();
		}
		else if (input == 'w' || input == 'W')
		{
			if (puzzleboard.move_up())
			{
				allmoves.push_front(puzzleboard);
			}
			puzzleboard.print_puzzle();
		}
		else if (input == 'z' || input == 'Z')
		{
			// Remove last move from stackqueue
			allmoves.pop_front();
			// let puzzleboard equal to the new last move
			puzzleboard = allmoves.pop_front();
			// Push the new last move to stackqueue and print it
			allmoves.push_front(puzzleboard);
			puzzleboard.print_puzzle();
		}
		else if (input == 'r' || input == 'R')
		{
			while(!allmoves.StackQueueEmpty())
			{

				allmoves.pop_rear().print_puzzle();
			}
			std::cout << "All of the moves were played again!!\n";
			break;
		}
	}


	return 0;
}
