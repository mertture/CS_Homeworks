#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "strutils.h"
#include "Stack.cpp"
#include <stdlib.h>
#include "randgen.h"
#include "time.h"
using namespace std;
// Mert Ture 26800

// With asking TA, I made bool visited, vector visited because 
// when I made somewhere visited true and came back and come there from another way, it should be not visited. Due to that, it should be vector.
// For Knight visited places I added visitedK bool. It is bool because knight cannot go same place it visited.
struct StackNode {
	int x_coordinate;
	int y_coordinate;
	vector<StackNode> visited;
	bool has_pawn;
	bool visitedK;
};


// Looking if a stackNode (cell) is in vector<stackNode> visited item of struct stackNode. If it is, it will not visit it again.
bool inVisited(StackNode &pawnBef, StackNode &pawnAft, vector<vector<StackNode>> &board) {
	for (int i = 0; i < pawnBef.visited.size(); i++) {
		if (pawnBef.visited[i].x_coordinate == pawnAft.x_coordinate && pawnBef.visited[i].y_coordinate == pawnAft.y_coordinate) {
			return false;
		}
	}
	return true;
}

// It checks both row, column, both diagonal ways for 3 pawns limit.
bool rowColDiagCheck(StackNode& pawn, vector<vector<StackNode>> &board) {
	int rowCounter = 0;
	int colCounter = 0;
	int RdiagCounter = 0;
	int LdiagCounter = 0;
	int x = pawn.x_coordinate, y = pawn.y_coordinate;
	
	
	// Row and Col check
	for (int i = 0; i < board.size(); i++) {
		if (board[i][x-1].has_pawn == 1) {
			colCounter++;
		}
		if (board[y-1][i].has_pawn == 1) {
			rowCounter++;
		}
		
		if (colCounter == 3 || rowCounter == 3) {
			return false;
		}
	}

	//Left Bottom to Right top diagonal check
	if (x > y) {
		for (int i = 0; i < board.size()+y-x; i++) {
			if (board[i][x-y+i].has_pawn == 1) {
				RdiagCounter++;
			}
			if (RdiagCounter == 3) {
				return false;
			}
		}
	}
	else {
		for (int i = 0; i < board.size()+x-y; i++) {
			if (board[y-x+i][i].has_pawn == 1) {
				RdiagCounter++;
			}
			if (RdiagCounter == 3) {
				return false;
			}
		}
	}


	// Left top to Right bottom diagonal check
	if (y+x-2 > (board.size()-1)) {
		for (int i = board.size()-1; i > y+x-board.size()-2; i--) {
			if (board[i][y+x-2-i].has_pawn == 1) {
				LdiagCounter++;
			}
			if (LdiagCounter == 3) {
				return false;
			}
		}
	}
	else {
		for (int i = y+x-2; i >= 0; i--) {
			if (board[i][y+x-2-i].has_pawn == 1) {
				LdiagCounter++;
			}
			if (LdiagCounter == 3) {
				return false;
			}
		
		}
	
	}

	
	return true;
	
}



// Putting pawn, details are in it.
void putPawn(StackNode& pawn, int &counter, vector<vector<StackNode>> &board, int &pawnsL, Stack<StackNode> &stack) {
	RandGen random;

	int ix = 0;
	int rx;
	int ry;
	int x = pawn.x_coordinate; int y = pawn.y_coordinate;
	// Each ran is one cell neighbour.
	int ran00 = 0, ran01 = 0, ran02 = 0, ran10 = 0, ran11 = 0, ran12 = 0, ran20 = 0, ran21 = 0, ran22 = 0;
	while (counter != pawnsL) {
		// randomly chooses number - to 2. Because a cells neighbours are -1, 0, 1 coordinate from them but vector index starts from 0, cell from 1. Hence, 0,1,2 returns to -1,0,1.
		rx =  random.RandInt(3);
		ry = random.RandInt(3);
		pair<int,int> rxy;
		rxy.first = rx; rxy.second = ry;
		int ranx = x-2 + rx;
		int rany = y-2 + ry;

		// After randomly choose, to see which cells were visited, counts the random selections. Because we should be sure that all neighbor cells are visited before popping item from stack.
		if (rxy.first == 0 && rxy.second == 0) ran00++;
		if (rxy.first == 0 && rxy.second == 1) ran01++;
		if (rxy.first == 0 && rxy.second == 2) ran02++;
		if (rxy.first == 1 && rxy.second == 0) ran10++;
		if (rxy.first == 1 && rxy.second == 1) ran11++;
		if (rxy.first == 1 && rxy.second == 2) ran12++;
		if (rxy.first == 2 && rxy.second == 0) ran20++;
		if (rxy.first == 2 && rxy.second == 1) ran21++;
		if (rxy.first == 2 && rxy.second == 2) ran22++;
		//cout << endl << rany << " " << ranx << " ";

		// Looks if that random cell is visited, row, col, diags are okay to 3 pawns lastly looks the chessboard's cell limits.
		if (ranx+1 > 0 && rany+1 > 0 && rany < board.size() && ranx < board.size() && inVisited(board[y-1][x-1], board[rany][ranx], board) && board[rany][ranx].has_pawn == 0 && rowColDiagCheck(board[rany][ranx], board)) {
			
			// if conditions are okay, pushes to visited vector.
			board[y-1][x-1].visited.push_back(board[rany][ranx]);
			board[rany][ranx].has_pawn = 1;
			stack.push(board[rany][ranx]);
			counter++;
			x = ranx+1; y = rany+1;
			// after pushing, makes neighbors empty.
			ran00 = 0, ran01 = 0, ran02 = 0, ran10 = 0, ran11 = 0, ran12 = 0, ran20 = 0, ran21 = 0, ran22 = 0;

		}
		// if conditions are not applied and all the neighbor cells are visited
		else if (ran00 != 0 && ran01 != 0 && ran02 != 0 && ran10 != 0 && ran11 != 0 && ran12 != 0 && ran20 != 0 && ran21 != 0 && ran22 != 0){	
			int xp;
			int yp;
			// pops the item from stack then saves the top of the stack
			StackNode p = stack.topAndpop();
			xp = p.x_coordinate;
			yp = p.y_coordinate;
			if (ix > 0) {
				board[yp-1][xp-1].visited.clear();
			}
			board[yp-1][xp-1].has_pawn = 0;
				
			ix++;
			counter--;
			StackNode t = stack.top();
			x = t.x_coordinate; y = t.y_coordinate;
			ran00 = 0, ran01 = 0, ran02 = 0, ran10 = 0, ran11 = 0, ran12 = 0, ran20 = 0, ran21 = 0, ran22 = 0;


	}

		// Gulsen hoca said doing without randomness will not be a problem. Also I did random too in the upper but turned to sequentially after. If random has a problem this works really fine but I did not wanted to got 0 due to not doing random. You just have to comment 121 to 177. Not comment 181 to 262


		/*if (x-1 > 0 && y-1 > 0 && y-1 < board.size() && x-1 < board.size() && inVisited(board[y-1][x-1], board[y-2][x-2], board) && board[y-2][x-2].has_pawn == 0 && rowColDiagCheck(board[y-2][x-2], board)) {
			board[y-1][x-1].visited.push_back(board[y-2][x-2]);
			board[y-2][x-2].has_pawn = 1;
			stack.push(board[y-2][x-2]);
			counter++;
			x = x-1; y = y-1;
		}
		else if (x-1 > 0 && y > 0 && y-1 < board.size() && x-2 < board.size() && inVisited(board[y-1][x-1], board[y-1][x-2], board) && board[y-1][x-2].has_pawn == 0 && rowColDiagCheck(board[y-1][x-2], board)) {
			board[y-1][x-1].visited.push_back(board[y-1][x-2]);
			board[y-1][x-2].has_pawn = 1;
			stack.push(board[y-1][x-2]);
			counter++;
			x = x-1; y = y;
		}

		else if (x-1 > 0 && y+1 > 0 && y < board.size() && x-2 < board.size() && inVisited(board[y-1][x-1], board[y][x-2], board) && board[y][x-2].has_pawn == 0 && rowColDiagCheck(board[y][x-2], board)) {
			board[y-1][x-1].visited.push_back(board[y][x-2]);
			board[y][x-2].has_pawn = 1;
			stack.push(board[y][x-2]);
			counter++;
			x = x-1; y = y+1;
		}

		else if (x > 0 && y+1 > 0 && y < board.size() && x-1 < board.size() && inVisited(board[y-1][x-1], board[y][x-1], board) && board[y][x-1].has_pawn == 0 && rowColDiagCheck(board[y][x-1], board)) {
			board[y-1][x-1].visited.push_back(board[y][x-1]);
			board[y][x-1].has_pawn = 1;
			stack.push(board[y][x-1]);
			counter++;
			y = y+1;
		}

		else if (x+1 > 0 && y+1 > 0 && y < board.size() && x < board.size() && inVisited(board[y-1][x-1], board[y][x], board) && board[y][x].has_pawn == 0 && rowColDiagCheck(board[y][x], board)) {
			board[y-1][x-1].visited.push_back(board[y][x]);
			board[y][x].has_pawn = 1;
			stack.push(board[y][x]);
			counter++;
			x = x+1; y = y+1;
		}

		else if (x+1 > 0 && y > 0 && y-1 < board.size() && x < board.size() && inVisited(board[y-1][x-1], board[y-1][x], board) && board[y-1][x].has_pawn == 0 && rowColDiagCheck(board[y-1][x], board)) {
			board[y-1][x-1].visited.push_back(board[y-1][x]);
			board[y-1][x].has_pawn = 1;
			stack.push(board[y-1][x]);
			counter++;
			x = x+1; 
		}

		else if (x+1 > 0 && y-1 > 0 && y-2 < board.size() && x < board.size() && inVisited(board[y-1][x-1], board[y-2][x], board) && board[y-2][x].has_pawn == 0 && rowColDiagCheck(board[y-2][x], board)) {
			board[y-1][x-1].visited.push_back(board[y-2][x]);
			board[y-2][x].has_pawn = 1;
			stack.push(board[y-2][x]);
			counter++;
			x = x+1; y = y-1;
		}

		else if (x > 0 && y-1 > 0 && y-2 < board.size() && x-1 < board.size() && inVisited(board[y-1][x-1], board[y-2][x-1], board) && board[y-2][x-1].has_pawn == 0 && rowColDiagCheck(board[y-2][x-1], board)) {
			board[y-1][x-1].visited.push_back(board[y-2][x-1]);
			board[y-2][x-1].has_pawn = 1;
			stack.push(board[y-2][x-1]);
			counter++;
			y = y-1;
		}*/
		//else { //(ran00 != 0 && ran01 != 0 && ran02 != 0 && ran10 != 0 && ran11 != 0 && ran12 != 0 && ran20 != 0 && ran21 != 0 && ran22 != 0){
		//	int xp;
		//	int yp;
		//	StackNode p = stack.topAndpop();
		//	xp = p.x_coordinate;
		//	yp = p.y_coordinate;
		//	if (ix > 0) {
		//		board[yp-1][xp-1].visited.clear();
		//	}
		//	
		//	//if (y != 1 && x != 1) {
		//		board[yp-1][xp-1].has_pawn = 0;
		//		
		//		ix++;
		//	//}
		//	counter--;
		//	StackNode t = stack.top();
		//	x = t.x_coordinate; y = t.y_coordinate;
		//	//ran00 = 0, ran01 = 0, ran02 = 0, ran10 = 0, ran11 = 0, ran12 = 0, ran20 = 0, ran21 = 0, ran22 = 0;
		//}
		
	
	}
	


	
}

// Reaching King function by starting from left bottom cell and looks with clockwise way.
bool reachKing(pair<int,int> knight, pair<int,int> king, vector<vector<StackNode>> board, Stack<StackNode> &rescue) {
	//Stack<StackNode> rescue;
	int knX = knight.first; int knY = knight.second;
	int kX = king.first; int kY = king.second;
	while ((knX != kX || knY != kY) && !(rescue.isEmpty())) {
		// left bottom cell first then clockwise way
		if (knX-1 > 0 && knY-2 > 0 && knY-3 < board.size() && knX-2 < board.size() && board[knY-3][knX-2].has_pawn == 0 && board[knY-3][knX-2].visitedK == 0) {
			board[knY-3][knX-2].visitedK = 1;
			rescue.push(board[knY-3][knX-2]);
			knX = knX-1; knY = knY-2;
		}
		else if (knX-2 > 0 && knY-1 > 0 && knY-2 < board.size() && knX-3 < board.size() && board[knY-2][knX-3].has_pawn == 0 && board[knY-2][knX-3].visitedK == 0) {
			board[knY-2][knX-3].visitedK = 1;
			rescue.push(board[knY-2][knX-3]);
			knX = knX-2; knY = knY-1;
		}

		else if (knX-2 > 0 && knY+1 > 0 && knY < board.size() && knX-3 < board.size() && board[knY][knX-3].has_pawn == 0 && board[knY][knX-3].visitedK == 0) {
			board[knY][knX-3].visitedK = 1;
			rescue.push(board[knY][knX-3]);
			knX = knX-2; knY = knY+1;
		}

		else if (knX-1 > 0 && knY+2 > 0 && knY+1 < board.size() && knX-2 < board.size() && board[knY+1][knX-2].has_pawn == 0 && board[knY+1][knX-2].visitedK == 0) {
			board[knY+1][knX-2].visitedK = 1;
			rescue.push(board[knY+1][knX-2]);
			knX = knX-1; knY = knY+2;
		}

		else if (knX+1 > 0 && knY+2 > 0 && knY+1 < board.size() && knX < board.size() && board[knY+1][knX].has_pawn == 0 && board[knY+1][knX].visitedK == 0) {
			board[knY+1][knX].visitedK = 1;
			rescue.push(board[knY+1][knX]);
			knX = knX+1; knY = knY+2;
		}

		else if (knX+2> 0 && knY+1 > 0 && knY < board.size() && knX+2 < board.size() && board[knY][knX+1].has_pawn == 0 && board[knY][knX+1].visitedK == 0) {
			board[knY][knX+1].visitedK = 1;
			rescue.push(board[knY][knX+1]);
			knX = knX+2; knY = knY+1;
		}
		
		else if (knX+2 > 0 && knY-1 > 0 && knY-2 < board.size() && knX+1 < board.size() && board[knY-2][knX+1].has_pawn == 0 && board[knY-2][knX+1].visitedK == 0) {
			board[knY-2][knX+1].visitedK = 1;
			rescue.push(board[knY-2][knX+1]);
			knX = knX+2; knY = knY-1;
		}

		else if (knX+1> 0 && knY-2 > 0 && knY-3 < board.size() && knX < board.size() && board[knY-3][knX].has_pawn == 0 && board[knY-3][knX].visitedK == 0) {
			board[knY-3][knX].visitedK = 1;
			rescue.push(board[knY-3][knX]);
			knX = knX+1; knY = knY-2;
		}
		// if all cells are not okay, pops from the stack.
		else {
			int xp;
			int yp;
			StackNode p = rescue.topAndpop();
			xp = p.x_coordinate;
			yp = p.y_coordinate;
			if (!rescue.isEmpty()) {
				StackNode t = rescue.top();
				knX = t.x_coordinate; knY = t.y_coordinate;
			}
			
		}

	}
	// if it arrives the king, it returns true, otherwise false.
	if ((knX == kX) && (knY == kY)) {
		cout << "Path found, see the output file!" << endl;
		return true;
	}
	else {
		cout << "No way!" << endl;
		return false;
	}

}



int main() {
	Stack<StackNode> stack;
	vector<vector<StackNode>> board;
	// size input
	int N;
	cout << "Please enter the size of the board (n): ";
	cin >> N;
	while (N <= 4 || N >= 11) {
		cout << "Board size must be 4 < N < 11" << endl;
		cout << "Please enter the size of the board (n): ";
		cin >> N;
	}
	// pawn number input
	int pawnsL;
	cout << "Please enter the number of the pawns (p): ";
	cin >> pawnsL;
	while (3*N < pawnsL) {
		cout << "The number of pawns can be maximum 3 times of N (the board size)." << endl;
		cout << "Please enter the number of the pawns (p): ";
		cin >> pawnsL;
	}
	// empties all cell nodes
	for (int i = 1; i <= N; i++) {
		vector<StackNode> temp;
		for (int j = 1; j <= N; j++) {
			StackNode empty;
			empty.x_coordinate = j;
			empty.y_coordinate = i;
			empty.visitedK = 0;
			empty.has_pawn = 0;
			
			temp.push_back(empty);
			
		}
		board.push_back(temp);
	}




	// pushes first 1.1 node
	board[0][0].has_pawn = 1;
	stack.push(board[0][0]);
	int counter = 1;
	// putting all pawns
	putPawn(board[0][0], counter, board, pawnsL, stack);
	
	cout << "The matrix:" << endl;
	// after putting prints board
	for (int i = board.size()-1; i >= 0; i--) {
				for (int j = 0; j < board[0].size(); j++) {
					cout << board[i][j].has_pawn << " ";
				}	
				cout << endl;
			}	


	int knX, knY;
	int kX, kY;
	// knigt and king position input
	cout << "Enter the position of knight(x,y): ";
	cin >> knX >> knY;
	pair<int,int> knight(knX, knY);
	cout << "Enter the position of king(x,y): ";
	cin >> kX >> kY;
	pair<int,int> king(kX,kY);

	Stack<StackNode> rescue;
	board[knY-1][knX-1].visitedK = 1;
	rescue.push(board[knY-1][knX-1]);

	// trying to reach to king, if it cannot reach returns false
	bool saved = reachKing(knight, king, board, rescue);

	ofstream output;
	output.open("output.txt");

	output << "N = " << N << endl;
	output << "P = " << pawnsL << endl << endl << "Pawn Locations" << endl;

	// by popping, writes all the pawn locations to output file
	while (!stack.isEmpty()) {
		StackNode quit = stack.topAndpop();
		output << quit.x_coordinate << " " << quit.y_coordinate << endl;
	}
	

	output << endl << "Knight = " << knX << " " << knY << endl;
	output << "King = " << kX << " " << kY << endl;

	// by popping and reversing, writes all the knight locations to output.txt
	Stack<StackNode> rescueReverse;
	if (!rescue.isEmpty()) {
		output << endl;
		while (!rescue.isEmpty()) {
			StackNode quit = rescue.topAndpop();
			rescueReverse.push(quit);
		}


		while (!rescueReverse.isEmpty()) {
			StackNode quit = rescueReverse.topAndpop();
			output << quit.x_coordinate << " " << quit.y_coordinate << endl;
		}
	}
	else {
		output << endl << "No Way!" << endl;
	
	}

	output << endl << "Matrix" << endl;
	// lastly writes matrix board to output and closes output file
	for (int i = board.size()-1; i >= 0; i--) {
			for (int j = 0; j < board[0].size(); j++) {
				output << board[i][j].has_pawn << " ";
			}
			if (i != 0) {
				output << endl;
			}
		}
	
	
	output.close();
	
	return 0;
}