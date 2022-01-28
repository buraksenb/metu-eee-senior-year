#include "Sokoban.h"
#include <fstream> 
#include <iostream>

// Construct Sokoban puzzle from a char array
Sokoban::Sokoban(char _board[6][8])
{
	for(auto i=0 ; i<6 ; i++)
	{
		for(auto j=0 ; j<8 ; j++)
		{
			board[i][j] = _board[i][j];
			if(board[i][j] == PLAYER || board[i][j] == PLAYER_AT_TARGET)
			{
				p.X = i; 
				p.Y = j; 
			}
		}
	}
}
// Construct Sokoban puzzle from a text file
Sokoban::Sokoban(std::string path)
{
	
	/* newline character will be */
	/* read in dummy */
	char dummy;
	/* input file stream */
	std::ifstream file;
	file.open(path);
	for (int i = 0; i < 6; ++i) {
		for (int j = 0; j < 8; ++j) {
			/* read a character like cin */
			/* noskipws is required for */
			/* reading whitespaces */
			file >> std::noskipws >> board[i][j];
			if (board[i][j] == PLAYER || board[i][j] == PLAYER_AT_TARGET)
			{
				p.X = i;
				p.Y = j;
			}
		}

		/* read the newline character */
		/* at the end of each line */
		file >> std::noskipws >> dummy;
	}
	file.close();
}

// Default constructor which only initializes every coordinate to '0'
Sokoban::Sokoban()
{
	for (auto i = 0; i < 6; i++)
	{
		for (auto j = 0; j < 8; j++)
		{
			board[i][j] = '0';
		}
	}
}

// Copy Constructor
Sokoban::Sokoban(const Sokoban& other)
{
	for (auto i = 0; i < 6; i++)
	{
		for (auto j = 0; j < 8; j++)
		{
			board[i][j] = other.board[i][j];
			// Take the location of Player
			if (board[i][j] == PLAYER || board[i][j] == PLAYER_AT_TARGET)
			{
				p.X = i;
				p.Y = j;
			}
		}
	}
}
// Assignment operator
Sokoban& Sokoban::operator=(const Sokoban& other)
{
	for (auto i = 0; i < 6; i++)
	{
		for (auto j = 0; j < 8; j++)
		{
			this->board[i][j] = other.board[i][j];
			// Take the location of Player
			if (this->board[i][j] == PLAYER || this->board[i][j] == PLAYER_AT_TARGET)
			{
				this->p.X = i;
				this->p.Y = j;
			}
		}
	}


	return *this;
}

// Since movability is same for all directions. I did not want to copy-paste same code with little to no difference.
// This isMovable function takes the x and y direction and moves the player according to the direction.
bool Sokoban::isMovable(const int xdirection,const int ydirection)
{
	int ynextone = 0;
	int xnextone = 0;

	// Since directions are variable, I have to determine the next coordinate in the same direction
	// to check if the move is valid.
	if(xdirection == 0)
	{
		ynextone = ydirection > 0 ? 2 : -2;
		xnextone = 0; 
	}
	if(ydirection == 0)
	{
		xnextone = xdirection > 0 ? 2 : -2;
		ynextone = 0; 
	}
	// Since WALLs are not movable, movement is denied.
	if (board[p.X + xdirection][p.Y + ydirection] == WALL)
	{
		return false;
	}
	// If direction coordinate is EMPTYCELL we can move to that direction. However, we must check several things.
	if (board[p.X + xdirection][p.Y + ydirection] == EMPTYCELL)
	{
		board[p.X+ xdirection][p.Y + ydirection] = PLAYER;
		// If player was at target, we have to change that coordinate to TARGETLOCATION to not lose a target in puzzle.
		
		if (board[p.X][p.Y] == PLAYER_AT_TARGET)
		{
			board[p.X][p.Y] = TARGETLOCATION;
		}
		// Otherwise, player's old location is empty
		else
		{
			board[p.X][p.Y] = EMPTYCELL;

		}
		// Update the location of player in p variable.
		p.Y += ydirection;
		p.X += xdirection;
		return true;
	}
	if (board[p.X + xdirection][p.Y + ydirection] == MOVABLEBOX)
	{
		// If next coordinate is wall,box or box at target. Movement is not possible
		if (board[p.X + xnextone][p.Y + ynextone] == WALL 
			|| board[p.X + xnextone][p.Y + ynextone] == MOVABLEBOX 
			|| board[p.X + xnextone][p.Y + ynextone] == MOVABLEBOX_AT_TARGET)
		{
			return false;
		}
		// If empty than simple
		if (board[p.X + xnextone][p.Y + ynextone] == EMPTYCELL)
		{
			board[p.X + xnextone][p.Y + ynextone] = MOVABLEBOX;
			board[p.X + xdirection][p.Y + ydirection] = PLAYER;
			// Check if players old location was target.
			if (board[p.X][p.Y] == PLAYER_AT_TARGET)
			{
				board[p.X][p.Y] = TARGETLOCATION;
			}
			else
			{
				board[p.X][p.Y] = EMPTYCELL;
			}
			// Update player location.
			p.Y += ydirection;
			p.X += xdirection;
			return true;
		}
		// If next coordinate is target, then our box moves to the target location.
		if (board[p.X + xnextone][p.Y + ynextone] == TARGETLOCATION)
		{
			board[p.X + xnextone][p.Y + ynextone] = MOVABLEBOX_AT_TARGET;
			board[p.X + xdirection][p.Y + ydirection] = PLAYER;
			// Check players old location if it was target.
			if (board[p.X][p.Y] == PLAYER_AT_TARGET)
			{
				board[p.X][p.Y] = TARGETLOCATION;
			}
			else
			{
				board[p.X][p.Y] = EMPTYCELL;
			}
			// Update location
			p.Y += ydirection;
			p.X += xdirection;
			return true;
		}
	}
	
	if (board[p.X + xdirection][p.Y + ydirection] == MOVABLEBOX_AT_TARGET)
	{
		// If next coord is wall or box, movement is not possible.
		if (board[p.X + xnextone][p.Y + ynextone] == WALL 
			|| board[p.X + xnextone][p.Y + ynextone] == MOVABLEBOX 
			|| board[p.X + xnextone][p.Y + ynextone] == MOVABLEBOX_AT_TARGET)
		{
			return false;
		}
		// If it is empty cell it is easy
		if (board[p.X + xnextone][p.Y + ynextone] == EMPTYCELL)
		{
			board[p.X + xnextone][p.Y + ynextone] = MOVABLEBOX;
			// Since box was at target, player is now at target
			board[p.X + xdirection][p.Y + ydirection] = PLAYER_AT_TARGET;
			if (board[p.X][p.Y] == PLAYER_AT_TARGET)
			{
				board[p.X][p.Y] = TARGETLOCATION;
			}
			else
			{
				board[p.X][p.Y] = EMPTYCELL;
			}
			p.Y += ydirection;
			p.X += xdirection;
			return true;
		}
		if (board[p.X + xnextone][p.Y + ynextone] == TARGETLOCATION)
		{
			// Since next coordinate was a target, box is now at a target location.
			board[p.X + xnextone][p.Y + ynextone] = MOVABLEBOX_AT_TARGET;
			// Since box was at target, player is now at a target location.
			board[p.X + xdirection][p.Y + ydirection] = PLAYER_AT_TARGET;
			if (board[p.X][p.Y] == PLAYER_AT_TARGET)
			{
				board[p.X][p.Y] = TARGETLOCATION;
			}
			else
			{
				board[p.X][p.Y] = EMPTYCELL;
			}
			p.Y += ydirection;
			p.X += xdirection;
			return true;
		}
	}
	if (board[p.X + xdirection][p.Y + ydirection] == TARGETLOCATION)
	{
		// Since direction coordinate is target, player is at target.
		board[p.X + xdirection][p.Y + ydirection] = PLAYER_AT_TARGET;
		// Check if player was at target.
		if (board[p.X][p.Y] == PLAYER_AT_TARGET)
		{
			board[p.X][p.Y] = TARGETLOCATION;
		}
		else
		{
			board[p.X][p.Y] = EMPTYCELL;
		}
		p.Y += ydirection;
		p.X += xdirection;
		return true;
	}

	return false;
}

bool Sokoban::move_up()
{
		// Call isMovable function with up direction. Since board is chararray
		// upwards (x,y) direction is (-1,0)
		if(isMovable(-1,0))
		{
			return true; 
		}
		return false;
}

bool Sokoban::move_down()
{
	// Call isMovable function with down direction. Since board is an array
	// downwards (x,y) direction is (1,0)
	if (isMovable(1,0))
	{
		return true;
	}
	return false;
}

bool Sokoban::move_left()
{
	// Call isMovable function with down direction. Since board is an array
	// leftwards (x,y) direction is (0,-1)
	if (isMovable(0, -1))
	{
		return true;
	}
	return false;
}

bool Sokoban::move_right()
{
	// Call isMovable function with down direction. Since board is an array
	// rightwards (x,y) direction is (0,1)
	if (isMovable(0, 1))
	{
		return true;
	}
	return false;
}

bool Sokoban::is_solved() const
{
	// In a solved puzzle, there is no player at target, movable box and target location.
	// If there are return puzzle is not solved and return false.
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if(board[i][j] == PLAYER_AT_TARGET 
				|| board[i][j] == TARGETLOCATION 
				|| board[i][j]== MOVABLEBOX)
			{
				return false;
			}
		}
		
	}
	// Otherwise puzzle is solved.
	return true; 
}

void Sokoban::print_puzzle() const
{
	for (auto i = 0; i < 6; i++)
	{
		for (auto j = 0; j < 8; j++)
		{
			std::cout << board[i][j]; 
		}
		// New line at the end of each row.
		std::cout << std::endl;
	}
}
