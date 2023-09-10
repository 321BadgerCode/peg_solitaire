// Badger
#include <iostream>
#include <vector>
#include <climits>
#include <cstring>

// Dynamic character array to represent the board
char** board;

// Initialize the dynamic board with your board configuration
// NOTE: UNCOMMENT WHICH BOARD TO USE
/*// Simple example
const int BOARD_SIZE = 1;
const char* initialBoard[BOARD_SIZE] = {
	"11010"
};*/
// Triangle(used in Cracker Barrel)
const int BOARD_SIZE = 5;
const char* initialBoard[BOARD_SIZE] = {
	"11111",
	"1111",
	"111",
	"11",
	"0"
};
/*// Diamond
const int BOARD_SIZE = 5;
const char* initialBoard[BOARD_SIZE] = {
	"00100",
	"01110",
	"11011",
	"01110",
	"00100"
};*/
/*const int BOARD_SIZE = 7;
const char* initialBoard[BOARD_SIZE] = {
	"0011100",
	"0111110",
	"1110111",
	"1111111",
	"1111111",
	"0111110",
	"0011100"
};*/
/*const int BOARD_SIZE = 5;
const char* initialBoard[BOARD_SIZE] = {
	"11111",
	"11111",
	"11111",
	"11111",
	"01111"
};*/

struct Move {
	int fromX, fromY;
	int toX, toY;
	int middleX, middleY;

	Move(int fx = 0, int fy = 0, int tx = 0, int ty = 0) : fromX(fx), fromY(fy), toX(tx), toY(ty) {
		int* middle = getMiddle();
		middleX = middle[0];
		middleY = middle[1];
	}

	// Function to get the middle peg's x & y values in a 2D array
	int* getMiddle() {
		return new int[2]{(fromX + toX) / 2, (fromY + toY) / 2};
	}

	// Function to get the x & y values of from and to in a 2D array
	operator int*() const {
		return new int[4]{fromX, fromY, toX, toY};
	}

	// Function to output the move object as a string
	operator std::string() const {
		return "(" + std::to_string(fromX) + "," + std::to_string(fromY) + ")->(" + std::to_string(toX) + "," + std::to_string(toY) + ") over (" + std::to_string(middleX) + "," + std::to_string(middleY) + ")";
	}

	// Function to check if a move is valid
	operator bool() const {
		// Check if the starting and ending positions are within the bounds of the board
		if (fromX < 0 || fromX >= BOARD_SIZE || fromY < 0 || fromY >= std::strlen(board[0]) ||
			toX < 0 || toX >= BOARD_SIZE || toY < 0 || toY >= std::strlen(board[0])) {
			return false;
		}

		// Check if the middle position is equal to starting or ending position
		if ((middleX == fromX && middleY == fromY) ||
			(middleX == toX && middleY == toY)) {
			return false;
		}

		// Check if the starting position has a peg, the ending position is empty, and the middle position has a peg to jump over
		if (board[fromX][fromY] != '1' ||
			board[toX][toY] != '0' ||
			board[middleX][middleY] != '1') {
			return false;
		}

		// Check if the move is diagonally or to the side of the starting peg
		int dx = std::abs(fromX - toX);
		int dy = std::abs(fromY - toY);
		return (dx == 0 && dy == 2) || (dx == 0 && dy == 0) || (dx == 2 && dy == 0)  || (dx == 2 && dy == 2);
	}

	bool operator ==(const Move move) const {
		return 	(fromX == move.fromX) &&
			(toX == move.toX);
	}
	bool operator !=(const Move move) const {
		return !((*this)==move);
	}
};

std::vector<Move> moves;

bool isOnePegLeft() {
	int pegs = 0;
	for (int i = 0; i < BOARD_SIZE; ++i) {
		for (int j = 0; j < std::strlen(board[i]); ++j) {
			if(board[i][j] == '1'){
				pegs++;
				if(pegs >= 2){
					return false;
				}
			}
		}
	}
	return true;
}

// Function to perform a valid move
void doMove(const Move& move) {
	board[move.fromX][move.fromY] = '0';
	board[move.toX][move.toY] = '1';
	board[move.middleX][move.middleY] = '0';
	moves.push_back(move);
}

// Function to undo a move
void undoMove(const Move& move) {
	board[move.fromX][move.fromY] = '1';
	board[move.toX][move.toY] = '0';
	board[move.middleX][move.middleY] = '1';
	moves.pop_back();
}

// Function to print the board with colors based on the move
void printBoard(const Move& currentMove) {
	for (int i = 0; i < BOARD_SIZE; ++i) {
		for (int j = 0; j < BOARD_SIZE - 1; ++j) {
			std::cout << " ";
		}
		for (int j = 0; j <= std::strlen(board[i]); ++j) {
			char peg = board[i][j];
			if ((int*)currentMove != new int[4]{0, 0, 0, 0}) {
				if (i == currentMove.fromX && j == currentMove.fromY) {
					std::cout << "\033[33m" << peg << "\033[0m "; // Yellow for the starting peg
				} else if (i == currentMove.toX && j == currentMove.toY) {
					std::cout << "\033[32m" << peg << "\033[0m "; // Green for the moved peg
				} else if (i == (currentMove.fromX + currentMove.toX) / 2 && j == (currentMove.fromY + currentMove.toY) / 2) {
					std::cout << "\033[31m" << peg << "\033[0m "; // Red for the jumped-over peg
				} else {
					std::cout << peg << " ";
				}
			} else {
				printBoard(*new Move((int)(BOARD_SIZE / 2), (int)(std::strlen(board[BOARD_SIZE / 2]) / 2), (int)(BOARD_SIZE / 2), (int)(std::strlen(board[BOARD_SIZE / 2]) / 2)));
				//std::cout << ((i == (int)(BOARD_SIZE / 2) && j == (int)(std::strlen(board[BOARD_SIZE / 2]) / 2)) ? "\033[33m" + std::string(1, peg) + "\033[0m" : std::string(1, peg)) << " ";
			}
		}
		std::cout << std::endl;
	}
	std::cout << "------------------------------" << std::endl;
}

// Function to solve the peg solitaire board
bool solvePegSolitaire() {
	// Find all possible moves and try them
	for (int fromX = 0; fromX < BOARD_SIZE; ++fromX) {
		for (int fromY = 0; fromY < std::strlen(board[fromX]); ++fromY) {
			for (int toX = 0; toX < BOARD_SIZE; ++toX) {
				for (int toY = 0; toY < std::strlen(board[toX]); ++toY) {
					Move move(fromX, fromY, toX, toY);
					if ((bool)move) {
						doMove(move);
						//std::cout << "Move(" + (std::string)move + "):" << std::endl;
						//printBoard(move);
						if (isOnePegLeft() || solvePegSolitaire()) {
							return true;
						}
						undoMove(move);
						//std::cout << "Undo(" + (std::string)move + "):" << std::endl;
						//printBoard(move);
					}
					/*else{
						std::cout << "Invalid Move: " + (std::string)move << std::endl;
					}*/
				}
			}
		}
	}

	// If no more valid moves are possible, check if there is only one peg left (solution found)
	return isOnePegLeft();
}
// NOTE: ATTEMPT TO MAKE ALGORITHM MORE EFFICIENT BY USING BIDIRECTIONAL BREADTH-FIRST IDA(ITERATIVE DEEPENING A*)* (BFIDA*) GRAPH TRAVERSAL AND PATH SEARCHING ALGORITHM
/*int heuristic() {
	int pegCount = 0;
	for (int i = 0; i < BOARD_SIZE; ++i) {
		for (int j = 0; j < std::strlen(board[i]); ++j) {
			if (board[i][j] == '1') {
				pegCount++;
			}
		}
	}
	return pegCount;
}

// IDA* search function
bool idaStar(int bound, int depth) {
	if (depth == bound) {
		// Reached the depth limit, evaluate the heuristic and return true if the goal is reached
		if (heuristic() == 1) {
			return true; // Goal state reached
		}
		return false; // Not a goal state at this depth
	}

	// Iterate over possible moves and perform depth-first searches
	for (int fromX = 0; fromX < BOARD_SIZE; ++fromX) {
		for (int fromY = 0; fromY < std::strlen(board[fromX]); ++fromY) {
			for (int toX = 0; toX < BOARD_SIZE; ++toX) {
				for (int toY = 0; toY < std::strlen(board[toX]); ++toY) {
					Move move(fromX, fromY, toX, toY);
					std::cout << "Move:" << std::endl;
					printBoard(move);
					if ((bool)move) {
						doMove(move); // Apply the move
						if (idaStar(bound, depth + 1)) {
							return true; // Solution found
						}
						undoMove(move); // Undo the move for backtracking
						std::cout << "Undo:" << std::endl;
						printBoard(move);
					}
				}
			}
		}
	}

	return false; // No solution found at this depth
}

// Function to solve the peg solitaire board using IDA*
bool solvePegSolitaire() {
	int bound = heuristic(); // Initialize the initial bound using the heuristic
	while (true) {
		bool foundSolution = idaStar(bound, 0);
		if (foundSolution) {
			return true; // Solution found
		}

		// If no solution was found at the current bound, update the bound
		// The updated bound should be based on the minimum cost estimate from failed searches
		int minCost = INT_MAX; // Initialize with a large value
		for (const Move& move : moves) {
			doMove(move); // Apply the move
			int cost = heuristic(); // Evaluate the heuristic
			undoMove(move); // Undo the move
			minCost = std::min(minCost, cost);
		}

		// Update the bound to the minimum cost estimate
		bound = minCost;

		if (bound == INT_MAX) {
			// No more moves to explore, and no solution found
			return false;
		}
	}
}*/

void resetBoard() {
	for (int i = 0; i < BOARD_SIZE; ++i) {
		for (int j = 0; initialBoard[i][j] != '\0'; ++j) {
			board[i][j] = initialBoard[i][j];
		}
	}
}

int main() {
	// Allocate memory for the dynamic board
	board = new char*[BOARD_SIZE];
	for (int i = 0; i < BOARD_SIZE; ++i) {
		board[i] = new char[BOARD_SIZE];
	}

	resetBoard();

	std::cout << "Initial board:" << std::endl;
	printBoard(*new Move());

	if (solvePegSolitaire()) {
		resetBoard();
		std::cout << "Solution found:" << std::endl;
		printBoard(*new Move());
		int a = 1;
		for (const Move& move : moves) {
			std::cout << std::to_string(a) + ". " + (std::string)move << std::endl;
			doMove(move);
			printBoard(move);
			a++;
		}
	} else {
		std::cout << "No solution found." << std::endl;
	}

	// Deallocate memory for the dynamic board
	for (int i = 0; i < BOARD_SIZE; ++i) {
		delete[] board[i];
	}
	delete[] board;

	return 0;
}
// TODO: CLEANUP
// TODO: OPTIMIZE
// TODO: FUNC. TO TAKE IN FILENAME AND INTERPRET BOARD MADE IN FILE AND RETURN ARRAY FOR `initialBoard`
	// NOTE: FILENAME WOULD BE PASSED IN AS CMD. ARG.