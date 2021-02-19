#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "strutils.h"
#include <chrono>
using namespace std;


// Mert Ture 26800

int maxhops;
// opening the files and first saving them in vector
void addFiles(vector<vector<string>> &Vstops, vector<vector<double>> &costss) {
	string metrofile;
    string costfile;
    
    cout << "Enter the name of the metro line data file: " << endl;
    cin >> metrofile;
    
    cout << "Enter the name of costs file: " << endl;
    cin >> costfile;
    
    cout << "Enter maximum number of hops: " << endl;
    cin >> maxhops;
    
    ifstream input;
	ifstream cost_input;
    input.open(metrofile.c_str());
    cost_input.open(costfile.c_str());
    string ss;
    string cost;
    
    
	
    string price;
	vector<string> tmp;
	vector<string> tempCosts;
	vector<vector<string>> firstCost;
	// opening costs.txt and pushing them to a vector with metroLine names
	while(getline(cost_input, cost, ',')){
		if (cost.find("\n") != string::npos) {
			tempCosts.push_back(cost.substr(0, cost.find("\n")));
			firstCost.push_back(tempCosts);
			tempCosts.clear();
			
			if (!cost_input.eof()) {
				cost = (cost.substr(cost.find("\n")+1, cost.length()-1));
				tempCosts.push_back(cost);
			}
		}
		else {
			tempCosts.push_back(cost);
		}
			

	}
	// opening the lines.txt and saving them to a vector
    while (getline(input, ss, ',')){
		
		int found = ss.find('#');
		if (found != string::npos) {
			string end;
			end = ss.substr(0, found);
			tmp.push_back(end);
			Vstops.push_back(tmp);
			tmp.clear();
			if (!input.eof()) {
				found = found+2;
				ss = ss.substr(found, ss.length()-1);
				tmp.push_back(ss);
				
			}
		}
		else {
			tmp.push_back(ss);
		}
    }

	// in the first cost part, I saved them with metroline names and as string type
	// now I took the metroLine names and turned it to double vector
	vector<double> lastCost;
	for (int i = 0; i < firstCost.size(); i++) {
		for (int k = 1; k < firstCost[i].size(); k++) {
			lastCost.push_back(atof(firstCost[i][k]));
		}
		costss.push_back(lastCost);
		lastCost.clear();
	}
}

// metroStops struct which will be Doubly Linked List
struct metroStop {
    string metroStopName;
    metroStop *left;
    metroStop *right;
};

// metroLine struct which contains doubly linkedlists in each node and it is a single linkedlist
struct metroLine {
    string metroNumber;
    metroLine *next;
    metroStop *metroStops;
    vector<double>costs;
};

// I created the head and tail of single Linked List to use later
struct metroLine *head = nullptr, *tail = nullptr;


// adding all the metroLines to metroLine struct
// then adding the metroStops as doubly linked lists
void add_metroLine (vector<vector<string>> &Vstops, vector<vector<double>> &costss) {
    
	// adding single linkedlist, metroLines
	for (int i = 0; i < Vstops.size(); i++) {
		metroLine *tmp = new metroLine;
		tmp->metroNumber = Vstops[i][0];
		tmp->costs = costss[i];
		tmp->next = nullptr;
		
		if (head == nullptr) {
			head = tmp;
			tail = tmp;
		}
		else {
			tail->next = tmp;
			tail = tail->next;
		}
	}

	
	// first from the metroStops vector (Vstops), assigning each value to tmpp, then create doubly linkedlist step by step
	metroLine *ptr = head;
	while (ptr != nullptr) {
		for (int i = 0; i < Vstops.size(); i++) {
			metroStop *headd = nullptr, *taill = nullptr;
			for (int k = 1; k < Vstops[i].size(); k++) {
				metroStop *tmpp = new metroStop;
				tmpp->metroStopName = Vstops[i][k];
				tmpp->right = nullptr;

				if (headd == nullptr) {
					headd = tmpp;
					headd->left = nullptr;
					taill = tmpp;
				}

				else {
					taill->right = tmpp;
					tmpp->left = taill;
					taill = taill->right;
				}
			}
			// connect it to single linked list's node when each doubly linked list is finished
			ptr->metroStops = headd;
			ptr = ptr->next;
		}
	}
}

// this function is very useful
// everytime I need to go to the head of metroStop doublylinkedlist but I do not have any head
// I used it to go head of doubly LinkedLists
void comeToStopHead(metroLine *curr) {
	while (curr->metroStops->left != nullptr) {
		curr->metroStops = curr->metroStops->left;
	}
}

// The function to print all the metro map
void printmetroLinesDetailed() {
	metroLine *print = head;
	
	// It basically goes all over the linkedlist and prints every lines' doublylinkedlist and costs
	while (print != nullptr) {
		while (print->metroStops->left != nullptr) {
			print->metroStops = print->metroStops->left;
		}

		cout << print->metroNumber << ": ";
		for (int i = 0; i < print->costs.size(); i++) {
			cout << print->metroStops->metroStopName << "<-" << print->costs[i] << "->";
			print->metroStops = print->metroStops->right;
		}
		cout << endl;
		//cout << print->metroStops->metroStopName << endl; BURAYI CIKARINCA DUZELIYOR
		print = print->next;
	}
	delete print;
}

// In addmetroStop function I had to look previous stop to add is in the map to add after it
// if there is no prevstop which had inputted, it returns true as flag, if there is, it returns false. It seems opposite but I arranged it in function
bool prevStopCheck(metroLine *curr, string prevStop, bool flag, string metroLines) {
	curr = head;
	
	while (curr->metroNumber != metroLines) {
		curr = curr->next;
	}
	
	while(curr->metroStops != nullptr && flag == true) {
		
			if (prevStop == curr->metroStops->metroStopName) {
				flag = false;
			}
			else {
				curr->metroStops = curr->metroStops->right;
			}
	}
	return flag;
}

// adding new metroStops
void addmetroStop() {
    string metroLines;
	metroLine *curr = head;
	comeToStopHead(curr);
	// enter the name of metroLine, if does not occur, gives error
	cout <<	"Enter the name of the metro line to insert a new metro stop (0 for main menu)" << endl;
	cin >> metroLines;
	bool flag = true;
	while ((curr != nullptr) && (flag==true)) {
		if (curr->metroNumber == metroLines){
			flag = false;
		}
		else {
		comeToStopHead(curr);
		curr = curr->next;
		
		}
	}
	if (flag == true) {
		// the error of metroLine
		cout << "Metro line cannot be found. Going back to previous menu." << endl;
		return;
	}
	// if no error, it prints the metroLine
	cout << "The metro line information is shown below" << endl;
	cout << curr->metroNumber << ": ";
	comeToStopHead(curr);
	for (int i = 0; i < curr->costs.size(); i++) {
		cout << curr->metroStops->metroStopName << "<-" << curr->costs[i] << "->";
		curr->metroStops = curr->metroStops->right;
	}
	cout << curr->metroStops->metroStopName << endl;
	
	// then takes metroStop to add
	string metroStopp;
	cout << "Enter the name of the new metro stop" << endl;
	cin >> metroStopp;
	comeToStopHead(curr);
	// checks if metroStop to add is there before, if it is, gives error
	while ((curr->metroStops->right != nullptr) && flag == false) {
		if (metroStopp == curr->metroStops->right->metroStopName || metroStopp == curr->metroStops->metroStopName) {
			flag = true;
			cout << metroStopp << " Metro stop already exists. Going back to previous menu.";
			return;
		}
		
		curr->metroStops = curr->metroStops->right;
	}
	// comes to head of doubly to search again
	comeToStopHead(curr);
	
	// after all, taking the costs and prevmetroStop
	string prevStop;
	
		cout << "Enter the name of the previous metro stop for the new one (0 to put the new one as the first metro stop)" << endl;
		cin >> prevStop;

		double cost1, cost2;
		cout << "Enter new cost 1" << endl;
		cin >> cost1;
		cout << "Enter new cost 2" << endl;
		cin >> cost2;
		comeToStopHead(curr);
		// checking is prevstop is there or not
		while(curr->metroStops->right != nullptr && flag == false) {
			if (prevStop == curr->metroStops->metroStopName || prevStop == "0") {
				flag = true;
			}
			else if (prevStop == curr->metroStops->right->metroStopName) {
				flag = true;
			}
			else {
				curr->metroStops = curr->metroStops->right;
			}
			
		}
		
		bool flag1 = true;	
		// if the first time prevstop was not there, asks until it says 0 or enters correct input
		while(flag1){
			if (flag == false) {
				comeToStopHead(curr);
				cout << "Metro stop does not exist. Typo? Enter again (0 for main menu)" << endl;
				cin >> prevStop;
				if (prevStop == "0") {
					return;
				}
				flag1 = prevStopCheck(curr, prevStop, flag1, metroLines);
			}
			else {
				flag1 = false;
			}
		
		}
		// there are 3 different situations

		// if prevstop is 0, add it to start. So head is shifted to one right
		metroStop *a = new metroStop;
		if (prevStop == "0") {
			a->metroStopName = metroStopp;
			a->left = nullptr;
			comeToStopHead(curr);

			curr->metroStops->left = a;
			a->right = curr->metroStops;
			curr->metroStops = curr->metroStops->left;
			curr->costs.insert(curr->costs.begin(), cost1);
		
		}
		
		// 2 situations
		// one of them is adding stop to the end
		else {
			comeToStopHead(curr);
			int x = 0;
			while (curr->metroStops->metroStopName != prevStop) {
				curr->metroStops = curr->metroStops->right;
				x++;
			}
			if (x == curr->costs.size()) {
				a->metroStopName = metroStopp;
				a->left = curr->metroStops;
				a->right = nullptr;
				curr->metroStops->right = a;
				curr->costs.push_back(cost1);
			}
			// otherone is any other place and beginning and end
			else {
			a->metroStopName = metroStopp;
			a->right = curr->metroStops->right;
			curr->metroStops->right->left = a;
			curr->metroStops->right = a;
			a->left = curr->metroStops;

			curr->costs[x] = cost1;
			curr->costs.insert(curr->costs.begin()+x+1, cost2);
			}
		}
		
		// finally print the final MetroLine
		comeToStopHead(curr);
		cout << curr->metroNumber << ": ";
		for (int i = 0; i < curr->costs.size(); i++) {
			cout << curr->metroStops->metroStopName << "<-" << curr->costs[i] << "->";
			curr->metroStops = curr->metroStops->right;
		}
		cout << curr->metroStops->metroStopName << endl;
			
		
}

//deleting the inputted metroStop from the spesific metroLine
// if there is no metroStop left in metroLine, deletes the metroLine

void deletemetroStop() {
	string metroLines;
	metroLine *cur = head;
	comeToStopHead(cur);
	// checks if metroLine is in the map
	cout <<	"Enter the name of the metro line to insert a new metro stop (0 for main menu)" << endl;
	cin >> metroLines;
	bool flagg = true;
	while ((cur != nullptr) && (flagg==true)) {
		if (cur->metroNumber == metroLines){
			flagg = false;
		}
		else {
		comeToStopHead(cur);
		cur = cur->next;		
		}
	}
	// if not, gives error
	if (flagg == true) {
		cout << "Metro line cannot be found. Going back to previous menu." << endl;
		return;
	}
	// else couts line info
	cout << "The metro line information is shown below" << endl;
	cout << cur->metroNumber << ": ";
	comeToStopHead(cur);
	for (int i = 0; i < cur->costs.size(); i++) {
		cout << cur->metroStops->metroStopName << "<-" << cur->costs[i] << "->";
		cur->metroStops = cur->metroStops->right;
	}
	cout << cur->metroStops->metroStopName << endl;
	

	// takes metroStop which will be deleted
	string deletedStop;
	cout << "Enter the name of the metro stop to delete (0 for main menu)" << endl;
	cin >> deletedStop;

	if (deletedStop == "0") {
		return;
	}
	bool flag = true;
	comeToStopHead(cur);
	while (cur->metroStops->right != nullptr && flag) {
		if (cur->metroStops->metroStopName == deletedStop || cur->metroStops->right->metroStopName == deletedStop) {
			flag = false;
		}
		else {
			cur->metroStops = cur->metroStops->right;
		}
	}
	if (flag) {
		while (flag) {
			// checked for metroStop is in map or not by searching each stop in line
			if (cur->metroStops->metroStopName != deletedStop) {
				cout << "Metro stop cannot be found. Enter the name of the metro stop to delete (0 for main menu)" << endl;
				cin >> deletedStop;
				if (deletedStop == "0") {
					return;
				}
				comeToStopHead(cur);
				while (cur->metroStops->right != nullptr && flag) {
					if (cur->metroStops->metroStopName == deletedStop || cur->metroStops->right->metroStopName == deletedStop) {
						flag = false;
					}
					else {
						cur->metroStops = cur->metroStops->right;
					}
				}
			}
			else {
				flag = false;
			}
		}
	}
	comeToStopHead(cur);
	int x = 0;
	// counts what is the index of stop which will be deleted
	while(cur->metroStops->metroStopName != deletedStop) {
		cur->metroStops = cur->metroStops->right;
		x++;
	}

	metroStop *s = cur->metroStops;
	

	// this is important, first I look if this metroLine have one stop, so if I delete the metroStop, line have to be deleted
	if (x == 0 && cur->costs.size() == 0) {
		metroLine *deletedM = cur;
		cur = head;
		while (cur->next->metroNumber != metroLines) {
			cur = cur->next;
		}
		cur->next = cur->next->next;
		delete deletedM->metroStops;
		delete deletedM;
		return;
	}
	// deletes the stop from the beginning because x == 0, so index is 0
	else if (x == 0) {
		cur->costs.erase(cur->costs.begin());
		cur->metroStops = cur->metroStops->right;
		delete s;
		cur->metroStops->left = nullptr;
	}
	// deletes if x == last item of line
	else if(x == cur->costs.size()) {
		cur->costs.pop_back();
		cur->metroStops = cur->metroStops->left;
		delete s;
		cur->metroStops->right = nullptr;
	}
	// else it is anywhere, other than the start and end
	else {
		double sum = 0;
		sum = cur->costs[x] + cur->costs[x-1];
		cur->costs[x-1] = sum;
		cur->costs.erase(cur->costs.begin()+x);

		cur->metroStops->left->right = cur->metroStops->right;
		cur->metroStops->right->left = cur->metroStops->left;
		cur->metroStops = cur->metroStops->right;
		delete s;
		
	}
	
	// prints the final line
	// if there is no line left, it does not print anything
	comeToStopHead(cur);
	cout << cur->metroNumber << ": ";
	for (int i = 0; i < cur->costs.size(); i++) {
		cout << cur->metroStops->metroStopName << "<-" << cur->costs[i] << "->";
		cur->metroStops = cur->metroStops->right;
	}
	cout << cur->metroStops->metroStopName << endl;
	
}



 
  

// It is really important, it shows is there any other transfer point for that stop
bool findTransfer(metroLine *currrr, string transferr, vector<string> &transfer) {
	metroLine *tempHead = currrr;
	comeToStopHead(tempHead);
	while (tempHead->next != nullptr && tempHead->metroStops->metroStopName != transferr){
		tempHead->metroStops = tempHead->metroStops->right;
		if (tempHead->metroStops->right == nullptr && tempHead->metroStops->metroStopName != transferr) {
			tempHead = tempHead->next;
			comeToStopHead(tempHead);
			for (int i = 0; i < transfer.size(); i++) {
				if (tempHead->metroNumber == transfer[i]) {
					tempHead = tempHead->next;
					comeToStopHead(tempHead);
				}
			}
		}
	}
	// last node is bit problematic and tricky, for memory problems I did it seperate
	if (tempHead->next == nullptr) {
		comeToStopHead(tempHead);
		for (int i = 0; i < transfer.size(); i++) {
			if (tempHead->metroNumber == transfer[i]) {
				return false;
			}
		}
		if (tempHead->metroStops->metroStopName != transferr && tempHead->metroStops->right != nullptr){
			tempHead->metroStops = tempHead->metroStops->right;
		}
		if(tempHead->metroStops->metroStopName == transferr) {
			return true;
		}
	}
	if (tempHead->next == nullptr) {
		return false;
	}
	else {
		return true;
	}
	
}

// finds the transfer point and returns the found point
metroLine* findTransferReturn(metroLine *currr, string transferr, vector<string> &transfer) {
	currr = head;
	comeToStopHead(currr);
	while (currr->next != nullptr && currr->metroStops->metroStopName != transferr){
		currr->metroStops = currr->metroStops->right;
		if (currr->metroStops->right == nullptr && currr->metroStops->metroStopName != transferr) {
			currr = currr->next;
			comeToStopHead(currr);
			for (int i = 0; i < transfer.size(); i++) {
				if (currr->metroNumber == transfer[i]) {
					currr = currr->next;
					comeToStopHead(currr);
				}
			}
			
		}
	}
	if (currr->next == nullptr) {
		comeToStopHead(currr);
		while (currr->metroStops->metroStopName != transferr){
			currr->metroStops = currr->metroStops->right;
		}
		
	}

	transfer.push_back(currr->metroNumber);
	return currr;
	
}
// Finds transfer points index for cost calculations in vector index
int Index = 0;
int findTransferReturnIndex(metroLine *currr, string transferr, vector<string> &transfer, int Index) {
	currr = head;
	comeToStopHead(currr);
	Index = 0;
	while (currr != nullptr && currr->metroStops->metroStopName != transferr){
		currr->metroStops = currr->metroStops->right;
		Index++;
		if (currr->metroStops->right == nullptr && currr->metroStops->metroStopName != transferr) {
			currr = currr->next;
			comeToStopHead(currr);
			for (int i = 0; i < transfer.size(); i++) {
				if (currr->metroNumber == transfer[i]) {
					currr = currr->next;
					comeToStopHead(currr);
				}
			}
			Index = 0;
			
		}
	}
	return Index;
}




// Finds the startstop's first occurance
metroLine* findFirst(metroLine *currr, string firstPlace, vector<string> &transfer, int &Index, vector<string> &temps) {
	Index = 0;
	while (currr->metroStops->metroStopName != firstPlace) {
		currr->metroStops = currr->metroStops->right;
		Index++;
		if (currr->metroStops->right == nullptr && currr->metroStops->metroStopName != firstPlace) {
			currr = currr->next;
			comeToStopHead(currr);
			Index = 0;
		}
	}
	if (currr->metroStops->metroStopName == firstPlace) {
			transfer.push_back(currr->metroNumber);
			temps.push_back(currr->metroNumber);
			return currr;
	}
	temps.push_back(currr->metroNumber);
	return currr;
}



double cost = 0;
int leftCount = 0;
int rightCount = 0;

vector<vector<string>> paths;
vector<double> lastCosts;
// Which had to be the recursive function of the pathfinder but I could not implement it
void finder(int hops, metroLine *forCost, metroStop *rotating, double &min, string searchingFirstStop, string &lastPlace, vector<string> &transfer, int Index, vector<string> temps) {
	
	if(hops > maxhops) {
		return;
	}
	if (rotating->metroStopName == lastPlace) {
		temps.push_back(rotating->metroStopName);
		if (min > cost) {
			min = cost;
			lastCosts.push_back(cost);
			if (temps[2].length() <= 3) {
				hops--;
				temps.push_back(itoa(hops));
			}
			else {
				temps.push_back(itoa(hops));
			}
				paths.push_back(temps);
		}
		
		
		
		return;
	}
	
	// first goes right if it did not came from right
	if (rotating->right != nullptr && leftCount == 0) {
		rightCount++;
		if (leftCount > 0) {
			leftCount++;
		}
		cost += forCost->costs[Index];
		temps.push_back(rotating->metroStopName);
		finder(hops, forCost, rotating->right, min, rotating->right->metroStopName, lastPlace, transfer, Index+1, temps);
		temps.pop_back();
		
		cost -= forCost->costs[Index];

	}
	// after going right, goes left if it did not came from left
	if (rotating->left != nullptr && rightCount == 0) {
		leftCount++;
		if (rightCount > 0) {
			rightCount--;
		}
		cost += forCost->costs[Index-1];
		temps.push_back(rotating->metroStopName);
		finder(hops, forCost, rotating->left, min, rotating->left->metroStopName, lastPlace, transfer, Index-1, temps);
		temps.pop_back();
		
		cost -= forCost->costs[Index-1];

	}

	// looks transfer points, if there is, goes there and increases hop counter, if this way is not work, decreases the hopcounter
	if (findTransfer(head, rotating->metroStopName, transfer)){
		Index = findTransferReturnIndex(forCost, searchingFirstStop, transfer, Index);
		metroLine *forCostt = findTransferReturn(forCost, rotating->metroStopName, transfer);
		rightCount = 0;
		leftCount = 0;
		hops++;
		temps.push_back(rotating->metroStopName);
		temps.push_back(forCostt->metroNumber);
		transfer.push_back(forCostt->metroNumber);
		finder(hops, forCostt, forCostt->metroStops, min, rotating->metroStopName, lastPlace, transfer, Index, temps);
		hops--;
		temps.pop_back();
	}
	return;
}




	// pathfinder main function, first checks if the stops are occuring in the map
	void pathfinder() {
    string firstPlace;
	string lastPlace;
	
	cout << "Where are you now?" << endl;
	cin >> firstPlace;

	cout << "Where do you want to go?" << endl;
	cin >> lastPlace;
	auto startt = std::chrono::system_clock::now();
	metroLine *start = head;
	comeToStopHead(start);
	int m = 0, t = 0;
	// checks occurance of stops
	while (start->next != nullptr) {
		while (start->metroStops->right != nullptr) {
			if (firstPlace == start->metroStops->metroStopName || firstPlace == start->metroStops->right->metroStopName) {
				m++;
			}
			if (lastPlace == start->metroStops->metroStopName || lastPlace == start->metroStops->right->metroStopName) {
				t++;
			}
			
			start->metroStops = start->metroStops->right;
		}
		start = start->next;
		comeToStopHead(start);
	}
	// if not, gives error
	if (m < 1 || t < 1) {
		cout <<  "Metro stop does not exist in the table. Going back to previous menu." << endl;
		return;
	}
	vector<string> temps;
	vector<string> transfer;
	int hops = 0;
	double min = DBL_MAX;
	metroLine *currr = new metroLine;
	currr = head;
	// finds the first start stop
	comeToStopHead(currr);
	currr = findFirst(currr, firstPlace, transfer, Index, temps);
	// the recursive function to find cheapest path
	finder(hops, currr, currr->metroStops, min, firstPlace, lastPlace, transfer, Index, temps);

	int lastMin = 0;
	double minn = DBL_MAX;
	// chrono for time
	auto end = std::chrono::system_clock::now();
	chrono::duration<double> elapsed = end - startt;
	// finds the cheapest paths from the path list
	if (paths.size() >= 1) {
		for (int i = 0; i < lastCosts.size(); i++) {
			if (lastCosts[i] < minn) {
				lastMin = i;
			}
		}
		if (paths[lastMin][2].length() <= 3) {
			for (int i = 0; i < 2; i++) {
				paths[lastMin].erase(paths[lastMin].begin());
			}

		}
		//prints the path and everything
				cout << "Best cost path: ";
				for (int k = 0; k < paths[lastMin].size()-1; k++) {
					if (paths[lastMin][k].length() <= 3) {
						cout << endl << paths[lastMin][k] << ": ";
					}
					else {
					cout << paths[lastMin][k] << " ";
					}
				}
				
				cout << endl << "Hops: " << paths[lastMin][paths[lastMin].size()-1] << endl;
				cout << "Cost: " << min << endl;
				cout << "Elapsed time: " << elapsed.count() << "s";
		}
	
	else {
		cout << "No path found between " << firstPlace << " and " << lastPlace << endl;
		cout << "Elapsed time: " << elapsed.count() << "s";
	}
	temps.clear();
	transfer.clear();
}
// main menu
void printMainMenu() {
  cout << endl;
  cout <<"I***********************************************I"<<endl
       <<"I               0 - EXIT PROGRAM                I"<<endl
       <<"I               1 - PRINT LINES                 I"<<endl
       <<"I               2 - ADD METRO STOP              I"<<endl
       <<"I               3 - DELETE METRO STOP           I"<<endl
       <<"I               4 - PATH FINDER                 I"<<endl
       <<"I***********************************************I"<<endl
       <<">>";
  cout << endl;
}
// checks the linkedlists connection
bool consistencyCheck() {
  metroLine* currBL = head;
  while(currBL) {
    metroStop* currBS = currBL->metroStops;
    while(currBS) {
      metroStop* rightBS = currBS->right;
      if(rightBS && rightBS->left != currBS) {
	cout << "Inconsistency for " 
	     << currBL->metroNumber 
	     << " " 
	     << currBS->metroStopName << endl;
	return false;
      }
      currBS = currBS->right;
    }
    currBL = currBL->next;
  }
  return true;
}
// main menu function for asking until 0 is entered
void processMainMenu() {
  char input;
  
  do{
    if(!consistencyCheck()) {
      cout << "There are inconsistencies. Exit." << endl; 
      return;
    }
    printMainMenu();
	
    cout << "Please enter your option " << endl;
    cin >> input;
    switch (input) {
    case '0':
      cout << "Thanks for using our software" << endl;
      return;
    case '1':
      printmetroLinesDetailed();
      break;
    case '2':
      addmetroStop();
      break;
    case '3':
      deletemetroStop();
      break;
    case '4':
      pathfinder();
      break;
    default:
      cout << "Invalid option: please enter again" << endl;
    }
  } while(true);
}





// main 
int main() {
    
	
    vector<vector<string>> Vstops;
	vector<vector<double>> costss;

	addFiles(Vstops, costss);
	add_metroLine(Vstops, costss);
	
	processMainMenu();
	
}
