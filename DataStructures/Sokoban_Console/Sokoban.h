#pragma once
#include <string>

class Sokoban
{
private:
	// Simple struct to help finding location of player
	struct Location
	{
		int X, Y;
		Location() { X = 0; Y = 0; }
	};

	// For knowing location of the player for all the time "p" is used
	Location p;
	char board[6][8];
	// Since ASCII characters for different cell states were not readable, I defined char variable for each of them
	// The reason they are static is that I did not want to have instances of this variable since they can be shared by
	// instances of the Sokoban class. Moreover, they are defined as constant as expected.
	static const char PLAYER = '@';
	static const char PLAYER_AT_TARGET = '+';
	static const char WALL = '#';
	static const char EMPTYCELL= ' ';
	static const char TARGETLOCATION = '.';
	static const char MOVABLEBOX = '$';
	static const char MOVABLEBOX_AT_TARGET = '*';

public:
	// Declarations for all methods
	Sokoban();
	Sokoban(std::string path);
	Sokoban(char _board[6][8]);
	Sokoban(const Sokoban& other);
	Sokoban& operator = (const Sokoban& other);
	bool isMovable(const int xdirection, const int ydirection);
	bool move_up();
	bool move_down();
	bool move_left();
	bool move_right();
	bool is_solved() const;
	void print_puzzle() const; 
	
};

