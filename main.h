#include <iostream>
#include <cmath>
#include <list>
#include <vector>
#include <utility>
#include <fstream>
#include <string>

class WholeBoard;

// piece's x and y position
typedef std::pair<int, int> piecepos;

// first is piece position stored then its move location
typedef std::pair<piecepos, piecepos> piecepos_moveloc; 

// first is piece position and then the player scope due to that move
typedef std::pair<piecepos, int> piecepos_scope;

typedef std::pair<piecepos, std::pair<piecepos, piecepos>> piecepos_moveloc_placeX;

typedef std::pair<WholeBoard, int> board_scope;

class GameBoard // This is one Cell of the GameBoard, Don't be confused by the name
{
public:
	GameBoard(): 
		//mark_as(EachSquare::EMPTY), 
		mark_value('_')
	{}

	GameBoard& operator=(const char x)
	{
		switch (x)
		{
		case '1': 
			mark_value = '1'; 
			//mark_as = EachSquare::PLAYER1; 
			break;

		case '2':
			mark_value = '2';
			//mark_as = EachSquare::PLAYER2;
			break;

		case 'X':
			mark_value = 'X';
			//mark_as = EachSquare::X;
			break;

		case '_':
			mark_value = '_';
			//mark_as = EachSquare::EMPTY;
			break;

		default:
			std::cout << "Wrong Value entered, please try again";
			break;
		}
		return *this;
	}

	const bool operator==(const char x) const 
	{
		if (mark_value == '1' && x == '1')
			return true;
		else if (mark_value == '2' && x == '2')
			return true;
		else if (mark_value == 'X' && x == 'X')
			return true;
		else if (mark_value == '_' && x == '_')
			return true;
		else
			return false;
	}

	const bool operator!=(const char x) const
	{
		return !operator==(x);
	}

	const bool operator==(GameBoard** board) const
	{
		if (mark_value == (**board).mark_value)
			return true;
		else
			return false;
	}

	char operator[](const int x) const
	{
		if (x < 0 || x > 9)
			std::cout << "Trying to access out of bounds";

		return mark_value;
	}

	//EachSquare mark_as; // For security
	char mark_value;
};

class WholeBoard // Specifically created to use in NextMove function, since STL can only hold objects which are copyable and assignable
{
public:
	WholeBoard() = default;

	WholeBoard(GameBoard** board) {} // Works as a conversion function/contructor

	// Data Members
	char wholeboard[10][10]; // contains value of GameBoard(cell value)
};

// Forward declaration
void placeX(GameBoard** board, int playerx, int playery, int moveto_x, int moveto_y);
void PrintGameBoard(GameBoard** board);
int MeasureScope(GameBoard** board, int playerx, int playery);
void NextMove(GameBoard** board, int playerx, int playery);
int ScopeForPlayer(WholeBoard board, int playerx, int playery);
piecepos FindSecondPlayer(GameBoard** board);
piecepos FindFirstPlayer(GameBoard** board);

void placePlayer(GameBoard** board, int playerx, int playery, int moveto_x, int moveto_y)// Can not move an X and not move to a marked location 
{
	if (playerx > 9 || playerx < 0 || playery > 9 || playery < 0 || board[playerx][playery] == 'X')
	{
		std::cout << "\n player location wrong, out of bounds, please try again, NO PLAYER available OR X(destroyed) location selected";
		PrintGameBoard(board);
		std::cout << "\n Remember to give positions for where you see the player";
		std::cout << "\n Enter first x-pos(row) and second y-pos(column)";
		std::cin >> playerx >> playery;
		placePlayer(board, playerx, playery, moveto_x, moveto_y);
	}

	if (moveto_x > 9 || moveto_x < 0 || moveto_y > 9 || moveto_y < 0)
	{
		std::cout << " Marked as NON-PLAYABLE POSITION, try a different position";
		PrintGameBoard(board);
		std::cout << "\n Try moving to a position where you see a '_' means EMPTY";
		std::cout << "\n Remember to give positions for where you WANT the player to go";
		std::cout << "\n Enter first x-pos(row) and second y-pos(column)";
		std::cin >> moveto_x >> moveto_y;
		placePlayer(board, playerx, playery, moveto_x, moveto_y);
	}

	if (board[playerx][playery] == '_')
	{
		std::cout << "\n The position you've given is an Empty position";
		std::cout << "\n Remember to give positions for where you see the player";
		std::cout << "\n Enter first x-pos(row) and second y-pos(column)";
		std::cin >> playerx >> playery;
		placePlayer(board, playerx, playery, moveto_x, moveto_y);
	}

	if (board[moveto_x][moveto_y] == 'X' || board[moveto_x][moveto_y] == '1' || board[moveto_x][moveto_y] == '2')
	{
		std::cout << " Marked as NON-PLAYABLE POSITION, try a different position";
		PrintGameBoard(board);
		std::cout << "\n Try moving to a position where you see a '_' means EMPTY";
		std::cout << "\n Remember to give positions for where you WANT the player to go";
		std::cout << "\n Enter first x-pos(row) and second y-pos(column)";
		std::cin >> moveto_x >> moveto_y;
		placePlayer(board, playerx, playery, moveto_x, moveto_y);
	}
	
	int dx = moveto_x - playerx; int dy = moveto_y - playery;
	int signofx ,signofy;
	int loop_break = dx; // lets us cut an extra for loop

	int new_px, new_py; // for new player position to later call second function in line

	char player = board[playerx][playery].mark_value; // For either 1st or 2nd player

	// Handling 0/0 conditions for both x and y
	if (dx == 0)
	{
		signofx = 0;
		loop_break = dy;
	}
	else
		signofx = dx / std::abs(dx);

	if (dy == 0)
	{
		signofy = 0;
		loop_break = dx;
	}
	else
		signofy = dy / std::abs(dy);

	//char moveto = board[moveto_x][moveto_y].mark_value;

	for (int i = 0; i <= std::abs(loop_break); ++i) // If this is not working then, add loop for(int j...)
	{
		// if loop makes us reach the same value we started off from
		if (playerx + (signofx * i) == playerx && playery + (signofy * i) == playery) 
			continue;

		if ((board[playerx + (signofx * i)][playery + (signofy * i)] == '1' ||
			board[playerx + (signofx * i)][playery + (signofy * i)] == '2' ||
			board[playerx + (signofx * i)][playery + (signofy * i)] == 'X'))
		{
			std::cout << "Position you're trying to reach already has an Element IN BETWEEN; NOT ALLOWED";
			std::cout << "\n Jumping over elements to mark a value is not allowed ";
			PrintGameBoard(board);
			std::cout << "\n Try inserting a new location for the player to move to";
			std::cout << "\n Enter first x-pos(row) and second y-pos(column)";
			std::cin >> playerx >> playery;
			placePlayer(board, playerx, playery, moveto_x, moveto_y);
		}
		else
		if (playerx + (signofx * i) == moveto_x && playery + (signofy * i) == moveto_y)
		{
			if (board[playerx + (signofx * i)][playery + (signofy * i)] == '_') // Maybe redundant check
			{
				//if (board[playerx][playery] == player) // 1st player move
				{
					std::swap(board[playerx + (signofx * i)][playery + (signofy * i)].mark_value, board[playerx][playery].mark_value);

					// ^ OR board[moveto_x][moveto_y] = player;

					PrintGameBoard(board);

					// Storing the new poisitons so that now an X can be marked from this pos
					new_px = playerx + (signofx * i); //  <- OR new_px = moveto_x;
					new_py = playery + (signofy * i); //  <- OR new_py = moveto_y;

					std::cout << "\n Player has moved, Place an X to destroy the position on board";
					std::cout << "\n Give positions to place an X, first x-pos(row) and second y-pos(column)";
					std::cin >> moveto_x >> moveto_y;

					placeX(board, new_px, new_py, moveto_x, moveto_y);
				}
			}
		}
		else
		if (board[playerx + (signofx * i)][playery + (signofy * i)] == '_')
		{
			continue;
		}
		else
		{
			std::cout << board[playerx + (signofx * i)][playery + (signofy * i)].mark_value;
			std::cout << "\n Positon you've entered is wrong, CANT MOVE THERE";
			PrintGameBoard(board);
			
			std::cout << "\n choose a new location to place player, current location inserted is invalid";
			std::cout << "\n Enter first x-pos(row) and second y-pos(column)";
			std::cin >> moveto_x >> moveto_y;
			placePlayer(board, playerx, playery, moveto_x, moveto_y);

			break;
			//continue;
		}
	}
}

void placeX(GameBoard** board, int playerx, int playery, int moveto_x, int moveto_y)
{
	if (playerx > 9 || playerx < 0 || playery > 9 || playery < 0 || board[playerx][playery] == 'X' ) // Can not move an X and not move to a marked location
	{
		std::cout << "\n move location - out of bounds, please try again OR X(destroyed) location selected";
		PrintGameBoard(board);
		std::cout << "\n Enter first x-pos(row) and second y-pos(column)";
		std::cin >> playerx >> playery;
		placeX(board, playerx, playery, moveto_x, moveto_y);
	}

	if (moveto_x > 9 || moveto_x < 0 || moveto_y > 9 || moveto_y < 0)
	{
		std::cout << " Marked as NON-PLAYABLE POSITION, try a different position";
		PrintGameBoard(board);
		std::cout << "\n Try moving to a position where you see a '_' means EMPTY";
		std::cout << "\n Remember to give positions for where you WANT the player to go";
		std::cout << "\n Enter first x-pos(row) and second y-pos(column)";
		std::cin >> moveto_x >> moveto_y;
		placePlayer(board, playerx, playery, moveto_x, moveto_y);
	}

	if (board[moveto_x][moveto_y] == 'X' || board[moveto_x][moveto_y] == '1' || board[moveto_x][moveto_y] == '2')
	{
		std::cout << "\n Marked as non-playable position, try a different position OR what you're trying to move, may not be a player, CHECK again";
		std::cout << "\n Try moving to a position where you see a '_' means EMPTY";
		PrintGameBoard(board);
		std::cout << "\n Remember to give positions for where you WANT the player to go";
		std::cout << "\n Enter first x-pos(row) and second y-pos(column)";
		std::cin >> moveto_x >> moveto_y;
		placePlayer(board, playerx, playery, moveto_x, moveto_y); // <- OR placeX(board, playerx, playery, moveto_x, moveto_y);
		return;
	}

	int dx = moveto_x - playerx;
	int dy = moveto_y - playery;
	int signofx, signofy;
	int loop_break = dx; // lets us cut an extra for loop

	int new_px, new_py; // for new player position to later call second function in line

	char player = board[playerx][playery].mark_value; // For either 1st or 2nd player

	// Handling 0/0 conditions for both x and y
	if (dx == 0)
	{
		signofx = 0;
		loop_break = dy;
	}
	else
		signofx = dx / std::abs(dx);

	if (dy == 0)
	{
		signofy = 0;
		loop_break = dx;
	}
	else
		signofy = dy / std::abs(dy);

	//char moveto = board[moveto_x][moveto_y].mark_value;

	for (int i = 0; i <= std::abs(loop_break); ++i)
	{
		// if loop makes us reach the same value we started off from
		if (playerx + (signofx * i) == playerx && playery + (signofy * i) == playery)
			continue;

		// If this is not working then, add loop for(int j...)
		if (board[playerx + (signofx * i)][playery + (signofy * i)] == '1' ||
			board[playerx + (signofx * i)][playery + (signofy * i)] == '2' ||
			board[playerx + (signofx * i)][playery + (signofy * i)] == 'X')
		{
			std::cout << "\n Position you're trying to reach already has an Element; is NOT EMPTY";
			std::cout << "\n Jumping over elements to mark a value is not allowed";
			PrintGameBoard(board);
			std::cout << "\n Try inserting a new location for the player to move to";
			std::cout << "\n Enter first x-pos(row) and second y-pos(column)";
			std::cin >> playerx >> playery;
			placePlayer(board, playerx, playery, moveto_x, moveto_y); // <- OR placeX(board, playerx, playery, moveto_x, moveto_y);
		}
		else
		if (playerx + (signofx * i) == moveto_x && playery + (signofy * i) == moveto_y)
		{
			board[playerx + (signofx * i)][playery + (signofy * i)] = 'X';
			PrintGameBoard(board);
		}
		else
		if (board[playerx + (signofx * i)][playery + (signofy * i)] == '_')
		{
			continue;
		}
		else
		{
			std::cout << "\n Positon you've entered is wrong, CANT MOVE THERE";
			PrintGameBoard(board);

			std::cout << "\n choose a new location to place 'X', current location is invalid";
			std::cout << "\n Enter first x-pos(row) and second y-pos(column)";
			std::cin >> moveto_x >> moveto_y;
			placePlayer(board, playerx, playery, moveto_x, moveto_y); // <- OR placeX(board, playerx, playery, moveto_x, moveto_y);

			break;
		}
	}
}

void PrintGameBoard(GameBoard** board)
{
	std::cout << std::endl;
	// Printing out the gameboard
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			std::cout << board[i][j].mark_value << "  ";
			// 2 spaces gap between each element on the board
		}
		std::cout << std::endl;
	}
}

// Function required to pick one(move possiblity) out for AI functionality
std::list<piecepos_moveloc> ListMoves(GameBoard** board, int playerx, int playery)
{
	// can be any player, we just need to know the '1' or '2'
	char player = board[playerx][playery].mark_value;

	//std::cout << "\n Inside ListMoves Function";

	if (player == 'X' || player == '_')
	{
		std::cout << "\n the position of ths square holds 'X' || '_'";
		std::cout << "\n Give a position of a player";
		std::cout << "\n Enter first x-pos(row) and second y-pos(column)";
		std::cin >> playerx >> playery;

		ListMoves(board, playerx, playery);
	}

	std::list<piecepos> pieces; // list to contain all pieces of the player

	for (int i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			if (board[i][j] == player)
				pieces.push_back(std::make_pair(i, j));
		}
	}
	// pieces list now has info of each player's piece x and y

	std::list<piecepos_moveloc> movelist;

	piecepos temp;

	// NOTE: 
	/* Optimization of the below loop should be to run a single loop(preferably while loop), 
	with an array which does (+i+j, +i j, i +j, -i-j, -i j, i -j, +i-j, -i+j), OR maybe this can't be done because break statment will cause a problem
	covering all directions and not have 8 for-loops like below */

	while (!pieces.empty()) // Covers for all pieces
	{
		temp = *pieces.begin();
		pieces.remove(*pieces.begin()); // iterate over the list with removal

		// for 1 piece of the player only, while loop covers for the remaining players
		// SE Direction
		for (int i = 1; i < 10; ++i)
		{
			if (temp.first + i > 9 || temp.second + i > 9)
				break;

			if (board[temp.first + i][temp.second + i] != '_') 
				break; // Something is in between either 1, 2, X. Debug for more info
			else
				movelist.push_back(std::make_pair(std::make_pair(temp.first, temp.second),std::make_pair(temp.first + i, temp.second + i)));
				// ^ above is the pair of piece position and the move it can play
		}

		// NW Direction
		for (int i = 1; i < 10; ++i)
		{
			if (temp.first - i < 0 || temp.second - i < 0)
				break;

			if (board[temp.first - i][temp.second - i] != '_')
				break;
			else
				movelist.push_back(std::make_pair(std::make_pair(temp.first, temp.second), std::make_pair(temp.first - i, temp.second - i)));
		}

		// SW Direction
		for (int i = 1; i < 10; ++i)
		{
			if (temp.first + i > 9 || temp.second - i < 0)
				break;

			if (board[temp.first + i][temp.second - i] != '_')
				break;
			else
				movelist.push_back(std::make_pair(std::make_pair(temp.first, temp.second), std::make_pair(temp.first + i, temp.second - i)));
		}

		// NE Direction
		for (int i = 1; i < 10; ++i)
		{
			if (temp.first - i < 0 || temp.second + i > 9)
				break;

			if (board[temp.first - i][temp.second + i] != '_')
				break;
			else
				movelist.push_back(std::make_pair(std::make_pair(temp.first, temp.second), std::make_pair(temp.first - i, temp.second + i)));
		}

		// N Direction
		for (int i = 1; i < 10; ++i)
		{
			if (temp.first - i < 0)
				break;

			if (board[temp.first - i][temp.second] != '_')
				break;
			else
				movelist.push_back(std::make_pair(std::make_pair(temp.first, temp.second), std::make_pair(temp.first - i, temp.second)));
		}

		// E Direction
		for (int i = 1; i < 10; ++i)
		{
			if (temp.second + i > 9)
				break;

			if (board[temp.first][temp.second + i] != '_')
				break;
			else
				movelist.push_back(std::make_pair(std::make_pair(temp.first, temp.second), std::make_pair(temp.first, temp.second + i)));
		}

		// W Direction
		for (int i = 1; i < 10; ++i)
		{
			if (temp.second - i < 0)
				break;

			if (board[temp.first][temp.second - i] != '_')
				break;
			else
				movelist.push_back(std::make_pair(std::make_pair(temp.first, temp.second), std::make_pair(temp.first, temp.second - i)));
		}

		// S Direction
		for (int i = 1; i < 10; ++i)
		{
			if (temp.first + i > 9)
				break;

			if (board[temp.first + i][temp.second] != '_')
				break;
			else
				movelist.push_back(std::make_pair(std::make_pair(temp.first, temp.second), std::make_pair(temp.first + i, temp.second)));
		}
	}


	return movelist;
}

// put in any player pos and recieve the scope of the player
int ScopeForPlayer(GameBoard** board, int playerx, int playery)
{
	char player = board[playerx][playery].mark_value;

	//std::cout << "\n Inside ScopeForPlayer Function";

	if (player == 'X' || player == '_')
	{
		std::cout << "\n the position of ths square holds 'X' || '_'";
		std::cout << "\n Give a position of a player";
		std::cout << "\n Enter first x-pos(row) and second y-pos(column)";
		std::cin >> playerx >> playery;

		ScopeForPlayer(board, playerx, playery);
	}

	std::list<piecepos> pieces; // list to contain all pieces of the player

	for (int i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			if (board[i][j] == player)
				pieces.push_back(std::make_pair(i, j));
		}
	}
	/*
		Optimization alert: If Player surrounded with X, do not count it.
		Can be improved with just counting the scope of the player to be > 0
	*/

	// pieces list now has info of each player's piece x and y

	int count = 0;
	piecepos temp;

	while (!pieces.empty())
	{
		temp = *pieces.begin(); // player's pos is stored in temp
		pieces.remove(*pieces.begin()); // remove from list to iterate over list

		count += MeasureScope(board, temp.first, temp.second);
	}

	return count;
}


// Conversion function for WholeBoard to GameBoard**
int ScopeForPlayer(WholeBoard board, int playerx, int playery)
{
	GameBoard** temp;

	temp = new GameBoard * [10]; // dynamic array (size 10) of pointers to GameBoard*

	for (int i = 0; i < 10; ++i)
	{
		temp[i] = new GameBoard[10];
		// each i-th pointer is now pointing to dynamic array (size 10)
		// of actual GameBoard values

	} // because we want gameboard[10][10]

	for (int i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			temp[i][j].mark_value = board.wholeboard[i][j];
		}
	}

	int scope =  ScopeForPlayer(temp, playerx, playery);

	// Deleting dynamically initialized 2D array
	for (int i = 0; i < 10; ++i)
	{
		delete[] temp[i]; //deletes an inner array of GameBoards;
	}

	delete[] temp; //delete pointer holding array of pointers;

	return scope;
}


// put in any player pos to determine which piece has what scope
int MeasureScope(GameBoard** board, int playerx, int playery)
{
	char player = board[playerx][playery].mark_value;

	//std::cout << "\n Inside MeasureScope Function";

	if (player == 'X' || player == '_')
	{
		std::cout << "\n the position of ths square holds 'X' || '_'";
		std::cout << "\n Give a position of a player";
		std::cout << "\n Enter first x-pos(row) and second y-pos(column)";
		std::cin >> playerx >> playery;

		MeasureScope(board, playerx, playery);
	}

	// this increases as each piece's playable squares on the board increases
	int countNW = 0, countN = 0 , countNE = 0, countE = 0, countSE = 0, countS = 0, countSW = 0, countW = 0;

	// for 1 piece of the player only 
	// SE Direction
	for (int i = 1; i < 10; ++i)
	{
		if (playerx + i > 9 || playery + i > 9)
			break;

		if (board[playerx + i][playery + i] != '_')
			break;
		else
			countSE++;
	}
				
	// NW Direction
	for (int i = 1; i < 10; ++i)
	{
		if (playerx - i < 0 || playery - i < 0)
			break;

		if (board[playerx - i][playery - i] != '_')
			break;
		else
			countNW++;
	}
	
	// SW Direction
	for (int i = 1; i < 10; ++i)
	{
		if (playerx + i > 9 || playery - i < 0)
			break;

		if (board[playerx + i][playery - i] != '_')
			break;
		else
			countSW++;
	}
		
	// NE Direction
	for (int i = 1; i < 10; ++i)
	{
		if (playerx - i < 0 || playery + i > 9)
			break;

		if (board[playerx - i][playery + i] != '_')
			break;
		else
			countNE++;
	}

	// N Direction
	for (int i = 1; i < 10; ++i)
	{
		if (playerx - i < 0)
			break;

		if (board[playerx - i][playery] != '_')
			break;
		else
			countN++;
	}

	// E Direction
	for (int i = 1; i < 10; ++i)
	{
		if (playery + i > 9)
			break;

		if (board[playerx][playery + i] != '_')
			break;
		else
			countE++;
	}

	// W Direction
	for (int i = 1; i < 10; ++i)
	{
		if (playery - i < 0)
			break;

		if (board[playerx][playery - i] != '_')
			break;
		else
			countW++;
	}

	// S Direction
	for (int i = 1; i < 10; ++i)
	{
		if (playerx + i > 9)
			break;

		if (board[playerx + i][playery] != '_')
			break;
		else
			countS++;
	}
	
	return countNW + countN + countNE + countE + countSE + countS + countSW + countW;
}

// AI function

void NextMove(GameBoard** board, int playerx, int playery)
{
	// Determine which player
	char player = board[playerx][playery].mark_value;

	//std::cout << "\n Inside NextMove function";

	if (player == '_' || player == 'X')
	{
		std::cout << "\n Wrong player selected, please select a player";
		std::cout << "\n Give a position of a player";
		std::cout << "\n Enter first x-pos(row) and second y-pos(column)";
		std::cin >> playerx >> playery;
		NextMove(board, playerx, playery);
	}

	// Stores the list of moves possible by one player
	std::list<piecepos_moveloc> moveslist = ListMoves(board, playerx, playery);

	//value to be returned
	piecepos_moveloc nextmove;

	// Making a copy so that the original stays the same
	WholeBoard board_copy;

	// Copying current board state into board_copy[][]
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			board_copy.wholeboard[i][j] = board[i][j].mark_value;
		}
	}

	int orig_scope = ScopeForPlayer(board, playerx, playery); // original scope without a move being played
	int aftermove_scope; // Scope of the current player after the move has been played

	// Keep a list of modified game states for each piece's move
	std::vector<WholeBoard> gamestates;

	/*
	Optimzation writing code wise to go in all directions
	{
		for(int i = -1; i <= +1; ++i)
		{
			for(int j = -1; j <= +1; ++j)
			{
				if(i == 0 && j == 0)
					continue;

				for(int k = 1; k <= 10; ++k)
				{
					board[playerx + (k * i)][playery + (k * j)] = "something";

					// break conditions as necesaary
				}
			}
		}
	}
	*/

	for (auto i = std::begin(moveslist); i != std::end(moveslist); ++i)
	{
		// TODO: Place an X for each move in all all directions and all in the possible places
		
		// Make necessary changes
		{
			// mark the move location
			board_copy.wholeboard[i->second.first][i->second.second] = player;

			// empty the piece from where it moved
			board_copy.wholeboard[i->first.first][i->first.second] = '_';
		}

		// Remember to debug after each change before inserting into list to 

		//gamestates.push_back(board_copy);
		
		// Below Code could have been avoided, by writing Beter ListMoves function
		// SE Direction
		for (int in = 1; in < 10; ++in)
		{
			if (i->second.first + in > 9 || i->second.second + in > 9)
				break;

			if (board_copy.wholeboard[i->second.first + in][i->second.second + in] != '_')
				break;
			else
			{
				// Place an X to the location
				board_copy.wholeboard[i->second.first + in][i->second.second + in] = 'X';

				gamestates.push_back(board_copy);

				// Remove the X after changing in the direction
				board_copy.wholeboard[i->second.first + in][i->second.second + in] = '_';
			}
		}

		// NW Direction
		for (int in = 1; in < 10; ++in)
		{
			if (i->second.first - in < 0 || i->second.second - in < 0)
				break;

			if (board_copy.wholeboard[i->second.first - in][i->second.second - in] != '_')
				break;
			else
			{
				// Place an X to the location
				board_copy.wholeboard[i->second.first - in][i->second.second - in] = 'X';

				gamestates.push_back(board_copy);

				// Remove the X after changing in the direction
				board_copy.wholeboard[i->second.first - in][i->second.second - in] = '_';
			}
		}

		// SW Direction
		for (int in = 1; in < 10; ++in)
		{
			if (i->second.first + in > 9 || i->second.second - in < 0)
				break;

			if (board_copy.wholeboard[i->second.first + in][i->second.second - in] != '_')
				break;
			else
			{
				// Place an X to the location
				board_copy.wholeboard[i->second.first + in][i->second.second - in] = 'X';

				gamestates.push_back(board_copy);

				// Remove the X after changing in the direction
				board_copy.wholeboard[i->second.first + in][i->second.second - in] = '_';
			}
		}

		// NE Direction
		for (int in = 1; in < 10; ++in)
		{
			if (i->second.first - in < 0 || i->second.second + in > 9)
				break;

			if (board_copy.wholeboard[i->second.first - in][i->second.second + in] != '_')
				break;
			else
			{
				// Place an X to the location
				board_copy.wholeboard[i->second.first - in][i->second.second + in] = 'X';

				gamestates.push_back(board_copy);

				// Remove the X after changing in the direction
				board_copy.wholeboard[i->second.first - in][i->second.second + in] = '_';
			}
		}

		// N Direction
		for (int in = 1; in < 10; ++in)
		{
			if (i->second.first - in < 0)
				break;

			if (board_copy.wholeboard[i->second.first - in][i->second.second] != '_')
				break;
			else
			{
				// Place an X to the location
				board_copy.wholeboard[i->second.first - in][i->second.second] = 'X';

				gamestates.push_back(board_copy);

				// Remove the X after changing in the direction
				board_copy.wholeboard[i->second.first - in][i->second.second] = '_';
			}
		}

		// E Direction
		for (int in = 1; in < 10; ++in)
		{
			if (i->second.second + in > 9)
				break;

			if (board_copy.wholeboard[i->second.first][i->second.second + in] != '_')
				break;
			else
			{
				// Place an X to the location
				board_copy.wholeboard[i->second.first][i->second.second + in] = 'X';

				gamestates.push_back(board_copy);

				// Remove the X after changing in the direction
				board_copy.wholeboard[i->second.first][i->second.second + in] = '_';
			}
		}

		// W Direction
		for (int in = 1; in < 10; ++in)
		{
			if (i->second.second - in < 0)
				break;

			if (board[i->second.first][i->second.second - in] != '_')
				break;
			else
			{
				// Place an X to the location
				board_copy.wholeboard[i->second.first][i->second.second - in] = 'X';

				gamestates.push_back(board_copy);

				// Remove the X after changing in the direction
				board_copy.wholeboard[i->second.first][i->second.second - in] = '_';
			}
		}

		// S Direction
		for (int in = 1; in < 10; ++in)
		{
			if (i->second.first + in > 9)
				break;

			if (board_copy.wholeboard[i->second.first + in][i->second.second] != '_')
				break;
			else
			{
				// Place an X to the location
				board_copy.wholeboard[i->second.first + in][i->second.second] = 'X';

				gamestates.push_back(board_copy);

				// Remove the X after changing in the direction
				board_copy.wholeboard[i->second.first + in][i->second.second] = '_';
			}
		}

		// Reverse the changes for the next move
		{
			// reverse the move location
			board_copy.wholeboard[i->second.first][i->second.second] = '_';

			// reverse the piece from where it moved
			board_copy.wholeboard[i->first.first][i->first.second] = player;
		}
		
	}
	// The end of loop signifies that all possible states(with move played on the GameBoard) have been stored in the list

	WholeBoard temp; // For the removal and checking

	std::list<board_scope> scopeforplayer;
	std::list<board_scope> scopeforopp;
	int opp_scope; // opponent scope after a move

	int count_player;
	int count_opp;

	// TODO: From the list of gamestates, store the scope of each move made
	std::vector<std::pair<WholeBoard, std::pair<int, int>>> board_max_min;
	for(auto val = std::begin(gamestates); val != std::end(gamestates); ++val)
	{
		// Accessing only one GameBoard state
		temp = *val;
		//gamestates.erase(gamestates.begin());

		//count_player = scopeforplayer.size(); // for optimization, please check code below
		//count_opp = scopeforopp.size(); // for optimization, please check below

		int count_player, count_opp;
		bool found_player = false, found_opp = false;
		// Accessing temp
		for (int i = 0; i < 10; i++)
		{
			if (found_player && found_opp) {
				break;
			}

			for (int j = 0; j < 10; j++)
			{
				if (found_player && found_opp) {
					break;
				}
				// To catch the value, the only one that has moved and not the pieces that did not move
				if (!found_player) 
				{
					if (temp.wholeboard[i][j] == player) { //should enter only once for no duplicate values
						count_player = ScopeForPlayer(temp, i, j); // Check for correctness of the conversion function
						found_player = true;
					}
				}
				if (!found_opp) 
				{
					if (temp.wholeboard[i][j] != 'X' && temp.wholeboard[i][j] != player && temp.wholeboard[i][j] != '_') {
						count_opp = ScopeForPlayer(temp, i, j);
						found_opp = true;
					}
				}
				
			}
		}
		board_max_min.push_back(std::make_pair(temp, std::make_pair( count_player, count_opp )));
	}
	// received scopes and player move made into lists, ^

	std::vector<int> maxscope;
	int max = INT_MIN;

	// TODO: Complete the below loop for full functionality and return value
	for (int i = 0; i < board_max_min.size(); ++i) {
		if (board_max_min[i].second.first > max) {
			max = board_max_min[i].second.first;
			maxscope.clear(); // only keep the maximum of values, so clear list if bigger value found
			maxscope.push_back(i);
		}
		else if (board_max_min[i].second.first == max) // all duplicate maximum values
		{
			maxscope.push_back(i);
		}

	} // maxscope has the maximum scope of values there are

	std::list<board_scope> minscope;

	if (maxscope.size() > 1)
	{
		int low = INT_MAX;
		int index = 0;
		for (int& i : maxscope) {
			
			if (board_max_min[i].second.second < low) {
				low = board_max_min[i].second.second;
				index = i;
			}
		}
		
		WholeBoard& temp = board_max_min[index].first;

		//for (auto val = std::begin(gamestates); val != std::end(gamestates); ++val)
		{
		
			for (int i = 0; i < 10; i++)
			{
				for (int j = 0; j < 10; j++)
				{

					// Copy the wholeboard to be the exact board to display the player move
					board[i][j].mark_value = temp.wholeboard[i][j];

				}
			}
		}
	}

	
	else // (maxscope.size() <= 1)
	{
		// TODO: Try and gather the original piece position from the orignal board and and then send it in the return value, Done Maybe
		WholeBoard& temp = board_max_min[*maxscope.begin()].first;

		
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				// Copy the wholeboard to be the exact board to display the player move
				board[i][j].mark_value = temp.wholeboard[i][j];
			}
		}
		
	}

}

piecepos FindSecondPlayer(GameBoard** board)
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (board[i][j].mark_value == '2')
				return std::make_pair(i, j);
		}
	}
}

piecepos FindFirstPlayer(GameBoard** board)
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (board[i][j].mark_value == '1')
				return std::make_pair(i, j);
		}
	}
}

int main(int argc, char** argv)
{
	std::cout << "******** -- Amazons Project MAT-564 -- *********" << std::endl;

	// 10 X 10 Matrix for amazon's board
	GameBoard** gameboard;
	gameboard = new GameBoard* [10]; // dynamic array (size 10) of pointers to GameBoard*

	for(int i = 0; i < 10; ++i)
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
		std::cerr << "Error: Unable to open File" << filename << "to read"<<std::endl;
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