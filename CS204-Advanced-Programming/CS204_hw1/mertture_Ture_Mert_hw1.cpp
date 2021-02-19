//Mert Ture 26800
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <istream>
#include <string>
#include <iomanip> 
#include "strutils.h"
using namespace std;

void readFiles(string &boardName, string &constName, ifstream &inputPuzzle, ifstream &inputConst, vector<vector<string>> &puzzle, vector<string> &temp, int &puzzleSize, vector<vector<int>> &Const, vector<int> &tempC, int &constSize);
void PrintPuzzle(vector<vector<string>> & puzzle);
bool sizeCheckerPuzzle(int &puzzleSize, vector<vector<string>> &puzzle);
bool intervalCheckerPuzzle(vector<vector<string>> &puzzle, int &puzzleSize);
bool countCheckerPuzzle(vector<vector<string>> &puzzle);
bool sizeCheckerConst(int &constSize, vector<vector<int>> &Const);
bool intervalCheckerConst(vector<vector<int>> &Const, int &puzzleSize);
bool repeatCheckerPuzzle(vector<vector<string>> &puzzle);
void findingValues(vector<vector<string>> &puzzle, vector<vector<int>> &Const);
void constCheckerForNormal(vector<vector<string>> &puzzle, vector<vector<int>> &Const);
bool constCheckerForHarderPuzzle(vector<vector<string>> &puzzle, vector<vector<int>> &Const);
int main() {
	ifstream inputPuzzle, inputConst;
	vector<vector<string>> puzzle;
	vector<string> temp;
	vector<vector<int>> Const;
	vector<int> tempC;
	string boardName, constName;
	int puzzleSize, constSize;  
	readFiles(boardName, constName, inputPuzzle, inputConst, puzzle, temp, puzzleSize, Const, tempC, constSize);
	sizeCheckerPuzzle(puzzleSize, puzzle);
	intervalCheckerPuzzle(puzzle, puzzleSize);
	repeatCheckerPuzzle(puzzle);
	sizeCheckerConst(constSize, Const);
	intervalCheckerConst(Const, puzzleSize);
	findingValues(puzzle, Const);
	PrintPuzzle(puzzle);
	return 0;
}


void readFiles(string &boardName, string &constName, ifstream &inputPuzzle, ifstream &inputConst, vector<vector<string>> &puzzle, vector<string> &temp, int &puzzleSize, vector<vector<int>> &Const, vector<int> &tempC, int &constSize) {
	//Taking file names and opening them
	
	cout << "Enter the board file:  ";
	cin >> boardName;

	cout << "Enter the constraint file:  ";
	cin >> constName;

	inputPuzzle.open(boardName);

  if(inputPuzzle.fail())
    {
      cout<< "Could not open the file "<< boardName << endl;
	  exit(0);
    }
	inputConst.open(constName);

  if(inputConst.fail()) 
	{
		cout << "Could not open the file " << constName << endl;
		exit(0);
    }

  //reading the board file and load it to puzzle vector
    string line;
	int i = 0;
	
	while (!inputPuzzle.eof()) {
		string num;
		getline(inputPuzzle, line);
		stringstream ss(line);
		temp.clear();
		if (line != ""){
			while (ss >> num){
				if(i == 0) {
					puzzleSize = atoi(num);
					i++;
				
				}
				else {
				
					temp.push_back(num);
				
				}
				i++;
			}
				if (i > 2) {
					puzzle.push_back(temp);
				
				}
		
			}
	}

	//reading the const file and load it to const vector
	string lineC;
	int a = 0;
	
	while (!inputConst.eof()) {
		int num;
		getline(inputConst, lineC);
		stringstream ss(lineC);
		tempC.clear();
		if (lineC != ""){
			while (ss >> num){
				if(a == 0) {
					constSize = num;
					a++;
				
				}
				else {
				
					tempC.push_back(num);
				
				}
				a++;
			}
				if (a > 2) {
					Const.push_back(tempC);
				
				}
		
			}
	}
	inputPuzzle.close();
	inputConst.close();
}

//Printing the completed boards
void PrintPuzzle(vector<vector<string>> &puzzle) {
	for (int j=0; j < puzzle.size(); j++)
    {   
		
      for (int k=0; k < puzzle[0].size(); k++)
        {   
			if (puzzle[j][k] == "X") {
				exit(0);
			}
	  }
	}
	cout << "Files are in correct format" << endl;
	for (int j=0; j < puzzle.size(); j++)
    {   
		
      for (int k=0; k < puzzle[0].size(); k++)
        {   
		  cout << puzzle[j][k] << " ";
			}
		  cout << endl;
		}

}


// Giving an error and stopping the program if the first line of file is not equal to size of matrix
bool sizeCheckerPuzzle(int &puzzleSize, vector<vector<string>> &puzzle) {
	if (puzzleSize == puzzle.size() && puzzleSize == puzzle[0].size()){
		return true;
	}
	else {
		cout << "File contains more lines than size" << endl;
		exit(0);
		return false;
	}
}

// Looks if the numbers are between the matrix's size (1 - puzzleSize)
bool intervalCheckerPuzzle(vector<vector<string>> &puzzle, int &puzzleSize) {
	for (int j=0; j < puzzle.size(); j++) {   
		
      for (int k=0; k < puzzle[0].size(); k++) {   
	    if (puzzle[j][k] != "X") {
			if (atoi(puzzle[j][k]) != string::npos &&  (atoi(puzzle[j][k]) <= puzzleSize && atoi(puzzle[j][k]) > 0)) {
			
			}
			else {
				cout << "Value "  << puzzle[j][k] << " is out of matrix size" << endl;
				exit(0);
				return false;
			}

	    }
      }
	}

	return true;
}

// Look if any number is repeating in a row and column
bool repeatCheckerPuzzle(vector<vector<string>> &puzzle) {
	for (int j = 0; j < puzzle.size(); j++) {
		
		for (int k = 0; k < puzzle[0].size(); k++) {
			int i = 0;
			for (int m = 0; m < puzzle[0].size() ;m++){
				if (puzzle[j][k] == puzzle[j][m]){
					if (puzzle[j][k] != "X"){
						i++;
					}
				}
			}
			if (i > 1) {
				cout << "There is a value repeating in same row" << endl;
				exit(0);
				return false;
				}
			int a = 0;
			for (int t = 0; t < puzzle.size(); t++) {
				if (puzzle[t][k] == puzzle[j][k]){
					if (puzzle[j][k] != "X") {
						a++;
					}
				}
			}
			if (a > 1) {
				cout << "There is a value repeating in same column" << endl;
				exit(0);
				return false;
			}
		}
	}
	return true;
}

// Looks if the X's in the board have other X's in the same rows and columns
// If there are in all of them, it is hard to solve --> So, next functions will check can it be fixed by looking the constraints
bool countCheckerPuzzle(vector<vector<string>> &puzzle){
	int countX = 0;
	int countXways = 0;
	for (int j = 0; j < puzzle.size(); j++) {
		for (int k = 0; k < puzzle[0].size(); k++) {
			if (puzzle[j][k] == "X"){
				countX++;
				int i = 0;
				for (int m = 0; m < puzzle[0].size(); m++){
					
					if (puzzle[j][m] == "X") {
						i++;
					}
				}
				if (i > 1) {
					int b = 0;
					for (int t = 0; t < puzzle.size(); t++) {
						if (puzzle[t][k] == "X"){
							b++;
						}
					}
					if (b > 1) {
						countXways++;

					}
					else {
						
						return false;
					}
				}
				else {			
					return false;
				}
				
			}
		}
	}
	if (countX == countXways) {
		return true;
	}
	return false;

}

// Checks if the first line of the const file is valid for the size of the Const
bool sizeCheckerConst(int &constSize, vector<vector<int>> &Const) {
	if (constSize == Const.size()){
		return true;
	}
	else {
		cout << "There are more constraints then previously stated" << endl;
		exit(0);
		return false;
	}
}

// Checks if the Constraints are in the correct interval for boards
bool intervalCheckerConst(vector<vector<int>> &Const, int &puzzleSize) {
	for (int j=0; j < Const.size(); j++) {   
		
      for (int k=0; k < Const[0].size(); k++) {   
	    
			if (Const[j][k] != string::npos &&  (Const[j][k] <= puzzleSize && Const[j][k] > 0)) {
			
			}
			else {
				cout << "Value "  << Const[j][k] << " is out of matrix size" << endl;
				exit(0);
				return false;
			}

	    
      }
	}

	return true;
}

// Checks are the board[x1][y1] > board[x2][y2] for the ones which passed the CountCheckerPuzzle Function
void constCheckerForNormal(vector<vector<string>> &puzzle, vector<vector<int>> &Const){
	for (int i = 0; i < Const.size(); i++) {
		if (puzzle[(Const[i][0])-1][(Const[i][1])-1] != "X" && puzzle[(Const[i][2]) - 1][(Const[i][3]) - 1] != "X") {
			if (atoi(puzzle[(Const[i][0])-1][(Const[i][1])-1]) > atoi(puzzle[(Const[i][2]) - 1][(Const[i][3]) - 1])) {
			
			}
			else {

				cout << "Constraints do not match board" << endl;
				exit(0);
			}
		}
	}


}

// Checks are the board[x1][y1] > board[x2][y2] for the ones which could not pass the CountCheckerPuzzle Function
// So it is a hard board
bool constCheckerForHarderPuzzle(vector<vector<string>> &puzzle, vector<vector<int>> &Const){
	for (int i = 0; i < Const.size(); i++) {
		if (puzzle[(Const[i][0])-1][(Const[i][1])-1] != "X" && puzzle[(Const[i][2]) - 1][(Const[i][3]) - 1] != "X") {
			if (atoi(puzzle[(Const[i][0])-1][(Const[i][1])-1]) > atoi(puzzle[(Const[i][2]) - 1][(Const[i][3]) - 1])) {
			
			}
			else {
				return false;
			}
		}
	}
	return true;

}

// Finds and fills the X's in the boards, with the previous functions, checks if there is a problem with properties (rules)
// First divides the board if it is Normal or Hard
void findingValues(vector<vector<string>> &puzzle, vector<vector<int>> &Const) {
	int sumRC = puzzle.size() * (puzzle.size() + 1) / 2;
	if (countCheckerPuzzle(puzzle) != true){
		for (int j = 0; j < puzzle.size(); j++) {
			
			for (int k = 0; k < puzzle[0].size(); k++) {
					int sumRow = 0;

					if (puzzle[j][k] == "X"){
						int m = 0;
						for (int i = 0; i < puzzle[0].size(); i++) {
							if (puzzle[j][i] != "X"){
								sumRow += atoi(puzzle[j][i]);
								m++;
							}
						}
						int Xcounter = puzzle[0].size() - m;
					
						if (Xcounter == 1) {
							puzzle[j][k] = itoa(sumRC - sumRow);
						}
						
						int sumCol = 0;
						int a = 0;
						for (int t = 0; t < puzzle.size(); t++) {
							if (puzzle[t][k] != "X") {
								sumCol += atoi(puzzle[t][k]);
								a++;						
							}
						}
						Xcounter = puzzle.size() - a;
						if (Xcounter == 1){
							if (puzzle[j][k] == "X"){
							puzzle[j][k] = itoa(sumRC - sumCol);
							}
							else {
								for (int i = 0; i < puzzle.size(); i++) {
									if (puzzle[i][k] == "X") {
										puzzle[i][k] = itoa(sumRC - sumCol);
									}
								}
							}
							sumRow = 0;
							m = 0;
							for (int i = 0; i < puzzle[0].size(); i++) {
								if (puzzle[j][i] != "X"){
									sumRow += atoi(puzzle[j][i]);
									m++;
								}
							}
								int Xcounter = puzzle[0].size() - m;
					
								if (Xcounter == 1) {
										for (int i = 0; i < puzzle.size(); i++) {
											if (puzzle[j][i] == "X") {
												puzzle[j][i] = itoa(sumRC - sumRow);
											}
										}
									
									
								}
					
						}
				}
			}
		}
		constCheckerForNormal(puzzle, Const);
	}
	

	else {
		// For hard puzzles, finds the not occurred numbers and tries, checks consts. If there are more than one possible valye after checks, prints error
		/*vector<vector<int>> triedValues;*/
		vector<int> listRow;
		for (int i = 1; i < puzzle.size()+1; i++) {
			listRow.push_back(i);
		}
		
		for (int j = 0; j < puzzle.size(); j++) {
			
			for (int k = 0; k < puzzle[0].size(); k++) {
				
				if (puzzle[j][k] == "X") {
					for (int i = 0; i < listRow.size(); i++) {
						int x = 0;
						for (int m = 0; m < puzzle[0].size(); m++){
							if (itoa(listRow[i]) == (puzzle[j][m])){
								x++;
							}
						}
						if (x < 1) {
							int a = 0;
							for (int t = 0; t < puzzle.size(); t++) {
								if (itoa(listRow[i]) == (puzzle[t][k])) {
									a++;
								}
							}
							if (a < 1) {
								if (puzzle[j][k] == "X") {
									puzzle[j][k] = itoa(listRow[i]);
									if (constCheckerForHarderPuzzle(puzzle, Const) == false) {
										puzzle[j][k] = "X";
									}
									
										
										/*vector<int> temp1;
										temp1.push_back(j);
										temp1.push_back(k);
										triedValues.push_back(temp1);*/
									
									
								}
							}
						}
					}
				}
			}
		}
		/*for (int i = 0; i < triedValues.size(); i++) {
			for(int s = 0; s < triedValues[0].size(); s++) {
				cout << "a" << endl;
				cout << triedValues[i][s] << " " << endl;
			}
		}
*/
		for (int j = 0; j < puzzle.size(); j++) {
			for (int k = 0; k < puzzle[0].size(); k++) {
				if (puzzle[j][k] == "X") {
					cout << "X's can have more than 1 value" << endl;
					exit(0);
				}
			}
		}

	}
}



