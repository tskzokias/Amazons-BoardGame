#include "main.h"

int main(int argc, char** argv)
{
	std::cout << "******** -- Amazons Project MAT-564 -- *********" << std::endl;

	// 10 X 10 Matrix for amazon's board
	GameBoard** gameboard;
	gameboard = new GameBoard * [10]; // dynamic array (size 10) of pointers to GameBoard*

	for (int i = 0; i < 10; ++i)
	{
		gameboard[i] = new GameBoard[10];
		// each i-th pointer is now pointing to dynamic array (size 10)
		// of actual GameBoard values

	} // because we want gameboard[10][10]

	std::cout << "\n Enter File-Name with .txt as extension. Refer solution Directory for filenames: \n";

	std::string filename; // filenames located in solution directory. Attach .txt to string for complete filename
	std::cin >> filename;

	// Reading File and all its contents to store in the GameBoard
	//std::string filename = "testboard.txt";
	std::ifstream in(filename, std::ios::in);
	if (!in.is_open())
	{
		std::cerr << "Error: Unable to open File" << filename << "to read" << std::endl;
		return false;
	}

	// Intializing the Game Board
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			// Read each character until the end
			in >> gameboard[i][j].mark_value;
		}
	}

	in.close();

	PrintGameBoard(gameboard);

	/* Game Notations:
		1. " _ " - no move has been made on that particular square
		2. " 1 " - 1st player move onto the square it's placed
		3. " 2 " - 2nd player move onto the square it's placed
		4. " X " - Either of the players have marked their X on the square (means square destroyed)
	*/

	/*
		Two players for a combinatorial game
		1. Left Player, 1st Player
		2. Right Player, 2nd Player
	*/
	//Player player1, player2;
	int px, py; // for inserting as player's position
	int mx, my; // for player's moving to position

	// 'X' is placed inside the funciton call(placeX), so it is taken care of

	// boolean value to control game win
	bool run_until = true; // change to true to start below while loop

	// Odd count for 1st player and Even count for 2nd Player
	int count = 0;

	piecepos player1, player2;
	piecepos_moveloc player2move;

	while (run_until)
	{
		count++;

		player1 = FindFirstPlayer(gameboard);
		player2 = FindSecondPlayer(gameboard);

		// TODO: Player win condition, run_until = false
		if (ScopeForPlayer(gameboard, player1.first, player1.second) < 1 ||
			ScopeForPlayer(gameboard, player2.first, player2.second) < 1)
		{
			run_until = false;
			std::cout << "First Player Scope: " << ScopeForPlayer(gameboard, player1.first, player1.second);
			std::cout << "Second Player Scope:" << ScopeForPlayer(gameboard, player2.first, player2.second);

			if (ScopeForPlayer(gameboard, player1.first, player1.second) > ScopeForPlayer(gameboard, player2.first, player2.second))
				std::cout << "\n FIRST PLAYER WIN, HUMAN WIN, CONGRATULATIONS";
			else
				std::cout << "\n SECOND PLAYER WIN, AI WON, YOU LOST";
		}

		if (count % 2 != 0)
		{
			PrintGameBoard(gameboard);
			std::cout << "\n 1st Player's move";
			std::cout << "\n Give positions for placing the player";
			std::cout << "\n Remember to give positions for where your see the player";
			std::cin >> px >> py; // entering values for which piece of the player
			std::cout << "\n Move Player to: first x-pos(row) and second y-pos(column)";
			std::cin >> mx >> my; // where to place the player and for 'X' will be asked later
			placePlayer(gameboard, px, py, mx, my);
		}
		else
		{
			std::cout << "\n 2nd Player's move";
			NextMove(gameboard, player2.first, player2.second);
			// TODO: Put AI instead of normally inputting values
			PrintGameBoard(gameboard);
		}

	}

	std::cout << "\n *********************  GAME OVER  ******************************";
	std::cout << "\n Press anything to Continue";
	std::getchar();

	// NOTE: Use std::shared_ptr<Gamebaord**> next time to initialize and delete dynamic arrays. Easier to manage

	// Deleting dynamically initialized 2D array
	for (int i = 0; i < 10; ++i)
	{
		delete[] gameboard[i]; //deletes an inner array of GameBoards;
	}

	delete[] gameboard; //delete pointer holding array of pointers;

	return 0;
}