#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <sstream>
#include "BST.cpp"
#include "AVL.cpp"
#include <stdlib.h>

using namespace std;
// Mert Ture 26800


//struct for items
struct item {
	string title;
	string info;

	bool operator < (const item& rhs) const {
		return title < rhs.title;
	}
	bool operator > (const item& rhs) const {
		return title > rhs.title;
	}
	friend ostream & operator << (ostream &out, const item &a);
};

ostream & operator << (ostream &out, const item &a) {
	out << a.title;
	return out;
}

struct BSTsection {
	string title;
	BST<item> items;
	bool operator < (const BSTsection& rhs) const {
		return title < rhs.title;
	}
	bool operator > (const BSTsection& rhs) const {
		return title > rhs.title;
	}
	friend ostream & operator << (ostream &out, const BSTsection &a);
	//BSTsection(): title(""), items() {}
		
};
ostream & operator << (ostream &out, const BSTsection &a) {
	out << a.title;
	return out;
}
struct AVLsection {
	string title;
	AVL<item> items;
	bool operator < (const AVLsection& rhs) const {
		return title < rhs.title;
	}
	bool operator > (const AVLsection& rhs) const {
		return title > rhs.title;
	}
	friend ostream & operator << (ostream &out, const AVLsection &a);
};
ostream & operator << (ostream &out, const AVLsection &a) {
	out << a.title;
	return out;
}


//creating BST by reading file
template <class T>
void createBST(BST<T> &bst, ifstream &data, vector<pair<long long, string>> &time) {
	string line;
	string word;
	string sect;
	item ix;
	BSTsection sec;
	int x = 0;
	long long timer;
	auto start = chrono::high_resolution_clock::now();


	//was trying something else

	//getline(data, line);
	//while (line.length() > 0) {
	//	istringstream sLine(line);
	//	
	//	BST<item>* items = new BST<item>();
	//	
	//	//sec.items.makeEmpty();
	//	sect = "";
	//	while (sLine >> word) {
	//		sect += word + " ";
	//	}
	//	sect = sect.substr(0, sect.length()-1);
	//	//BSTsection sec;
	//	//sec.title = sect;
	//	line = "";
	//	long long m = data.tellg();
	//	getline(data, line);
	//	
	//	do{
	//		istringstream xLine(line);
	//		item it;
	//		it.title = "";
	//		it.info = "";
	//		int dash = 0;
	//		while(xLine >> word) {
	//			if (dash == 0) {
	//				dash++;
	//				string m = word.substr(1,word.length()-1);
	//				int idx = m.find('-');
	//				it.title = m.substr(0, idx);
	//				it.info += m.substr(idx+1, m.length()-1) + " ";
	//			}
	//			else {
	//				it.info += word + " ";
	//			}
	//		}
	//		it.info = it.info.substr(0, it.info.length()-1);
	//		items->insert(it);
	//		line = "";
	//	}
	//		while(getline(data, line) && line[0] == '-') ;
	//			bst.insert(BSTsection(sect, items));
	//			auto end = chrono::high_resolution_clock::now();
	//			timer = ((end-start).count() / 1000.0);
	//			time.push_back(make_pair(timer, sec.title));
	//
	//			start = chrono::high_resolution_clock::now();




	//}
	
	

	//reads line by line, fills section, then inserts it into BST
	while (getline(data,line)) {
		istringstream sLine(line);
		
		if (line[0] != '-') {
		
			if (x != 0) {
				BSTsection cloneSec;
				cloneSec.items.root = sec.items.clone(sec.items.root);
				cloneSec.title = sec.title;
				
				bst.insert(cloneSec);
				auto end = chrono::high_resolution_clock::now();
				timer = ((end-start).count() / 1000.0);
				time.push_back(make_pair(timer, sec.title));

				start = chrono::high_resolution_clock::now();
			}
		}
		if (line[0] != '-') {
			x++;
			//BSTsection sec;
			sec.title = "";
			//sec.items.makeEmpty();
			sect = "";
			while (sLine >> word) {
				sect += word + " ";
			}
			sect = sect.substr(0, sect.length()-1);
			sec.title = sect;
			
			
		}

		// filling the section with items
		if (line[0] == '-') {
			item it;
			it.title = "";
			it.info = "";
			int dash = 0;
			while(sLine >> word) {
				if (dash == 0) {
					dash++;
					string m = word.substr(1,word.length()-1);
					int idx = m.find('-');
					it.title = m.substr(0, idx);
					it.info += m.substr(idx+1, m.length()-1) + " ";
				}
				else {
					it.info += word + " ";
				}
			}
			it.info = it.info.substr(0, it.info.length()-1);
			sec.items.insert(it);
		}
	}
	
	// inserting the last section
	BSTsection cloneSec;
	cloneSec.items.root = sec.items.clone(sec.items.root);
	cloneSec.title = sec.title;
				
	bst.insert(cloneSec);
	auto end = chrono::high_resolution_clock::now();
	timer = (end-start).count() / 1000.0;
	time.push_back(make_pair(timer, sec.title));
	
}

// creating AVL tree
template <class T>
void createAVL(AVL<T> &avl, ifstream &avlData, vector<pair<long long, string>> &time) {
	string line;
	string word;
	string sect;
	item ix;
	AVLsection sec;
	int x = 0;
	long long timer;
	auto start = chrono::high_resolution_clock::now();

	// was trying smt else

	//getline(avlData, line);
	//while (line.length() > 0) {
	//	istringstream sLine(line);
	//	//const item NOT_FOUND = item("NoSuchItemFound_999", "NoSuchItem");
	//	AVL<item>* items = new AVL<item>();

	//	
	//	//sec.items.makeEmpty();
	//	sect = "";
	//	while (sLine >> word) {
	//		sect += word + " ";
	//	}
	//	sect = sect.substr(0, sect.length()-1);
	//	//AVLsection sec;
	//	//sec.title = sect;
	//	line = "";
	//	long long m = avlData.tellg();
	//	getline(avlData, line);
	//	
	//	do{
	//		istringstream xLine(line);
	//		string title = "", info = "";
	//		int dash = 0;
	//		while(xLine >> word) {
	//			if (dash == 0) {
	//				dash++;
	//				string m = word.substr(1,word.length()-1);
	//				int idx = m.find('-');
	//				title = m.substr(0, idx);
	//				info += m.substr(idx+1, m.length()-1) + " ";
	//			}
	//			else {
	//				info += word + " ";
	//			}
	//		}
	//		info = info.substr(0, info.length()-1);
	//		items->insert(item(title, info));
	//		line = "";
	//	}
	//		while(getline(avlData, line) && line[0] == '-') ;
	//			avl.insert(AVLsection(sect, items));
	//			auto end = chrono::high_resolution_clock::now();
	//			timer = ((end-start).count() / 1000.0);
	//			time.push_back(make_pair(timer, sect));
	//
	//			start = chrono::high_resolution_clock::now();
	//
	//		
	//		
		
		
		
		
		
		
		
		
		
		
		
		// reading file, filling sections then inserting them
		while (getline(avlData,line)) {
		istringstream sLine(line);
		
			if (line[0] != '-') {
		
				if (x != 0) {
					AVLsection cloneSec;
					cloneSec.items.root = sec.items.clone(sec.items.root);
					cloneSec.title = sec.title;
				
					avl.insert(cloneSec);
				
					auto end = chrono::high_resolution_clock::now();
					timer = ((end-start).count() / 1000.0);
					time.push_back(make_pair(timer, sec.title));
	
					start = chrono::high_resolution_clock::now();
				}
				x++;
				//AVLsection sec;
				sec.title = "";
				sec.items.makeEmpty();
				sect = "";
				while (sLine >> word) {
					sect += word + " ";
				}
				sect = sect.substr(0, sect.length()-1);
				sec.title = sect;
			
			
			}
			// filling the sections with items
			if (line[0] == '-') {
				item it;
				it.title = "";
				it.info = "";
				int dash = 0;
				while(sLine >> word) {
					if (dash == 0) {
						dash++;
						string m = word.substr(1,word.length()-1);
						int idx = m.find('-');
						it.title = m.substr(0, idx);
						it.info += m.substr(idx+1, m.length()-1) + " ";
					}
					else {
						it.info += word + " ";
					}
				}
				it.info = it.info.substr(0, it.info.length()-1);
				sec.items.insert(it);
			}
	}
	// inserting last section
	AVLsection cloneSec;
	cloneSec.items.root = sec.items.clone(sec.items.root);
	cloneSec.title = sec.title;
				
	avl.insert(cloneSec);
	auto end = chrono::high_resolution_clock::now();
	timer = (end-start).count() / 1000.0;
	time.push_back(make_pair(timer, sec.title));

}

int main() {

	BST<BSTsection> bst;
	AVL<AVLsection> avl;
	vector<pair<long long, string>> timesAVL;
	vector<pair<long long, string>> timesBST;
	ifstream data;
	string file = "data.txt";
	data.open(file);
	ifstream avlData;
	avlData.open(file);

	// creating functions, I am reading file twice
	createAVL(avl, avlData, timesAVL);
	createBST(bst, data, timesBST);
	


	// printing the section reading times
	for (int i = 0; i < timesAVL.size(); i++) {

		cout << "Section " << timesAVL[i].second << " has been inserted into the AVL notebook." << endl 
					<< "[AVL] Elapsed time: " << timesAVL[i].first << " microseconds" << endl;
		cout << "Section " << timesBST[i].second << " has been inserted into the BST notebook." << endl 
				<< "[BST] Elapsed time: " << timesBST[i].first << " microseconds" << endl << endl;
	}

	cout <<  "MENU" << endl << "Please enter an input between [1 - 6]: " << endl
		<< "1- Display the sections [AVL]" << endl << "2- Display the sections [BST]" << endl << "3- Select a section" <<
		endl << "4- Add new section" << endl << "5- Delete a section" << endl << "6- Exit" << endl;
	int input;
	
	bool flag = true;
	while (flag) {
		cout << "Input: ";
		cin >> input;
		
		if (input == 1) {
			cout << endl;
			cout << "*****" << endl;
			avl.printTree();
			cout << "*****" << endl;
		}
		else if (input == 2) {
			cout << endl;
			cout << "*****" << endl;
			bst.printTree();
			cout << "*****" << endl;
		}
		else if (input == 3) {
			string section;
			cout << "Enter the title of the section: ";
			cin >> section;
			BSTsection search;
			search.title = section;
			AVLsection searchV;
			searchV.title = section;
			// finding sections
			BSTsection fin = bst.find(search);
			AVLsection finb = avl.find(searchV);
			if (fin.title != section) {
				cout << "Invalid title!" << endl;
			}

			// going into section
			else {
				cout << endl;
				cout << "Selected section -> " << section << endl;
				cout << "Please enter an input between [1 - 7]: " << endl << "1- Display the items [AVL]" << endl << "2- Display the items [BST]" << endl 
					<< "3- Display the information of a item" << endl << "4- Add new item" << endl << "5- Update the information of a item" << endl << "6- Delete an item" 
					<< endl << "7- Return to main menu" << endl;
				int inputS;
				bool flagS = true;

				while (flagS) {
					cout << "Input: ";
					cin >> inputS;
					
					if (inputS == 1) {
						cout << endl;
						cout << "*****" << endl;
						finb.items.printItemTree();
						cout << "*****" << endl;
						cout << endl;
					}
					else if (inputS == 2) {
						cout << endl;
						cout << "*****" << endl;
						fin.items.printItemTree();
						cout << "*****" << endl;
						cout << endl;
					}
					else if (inputS == 3) {
						cout << "Enter the title of the item: ";
						string findIt;
						cin >> findIt;
						// Just finding in AVL because if it is in AVL, it has to be in BST too
						item ibv;
						ibv.title = findIt;
						item ibbv;
						auto start = chrono::high_resolution_clock::now();
						ibbv = fin.items.find(ibv);
						auto end = chrono::high_resolution_clock::now();
						long long timer = ((end-start).count() / 1000.0);
						cout << "[AVL] Elapsed time: " << timer << " microseconds" << endl;
						item ib;
						ib.title = findIt;
						item ibb;
						start = chrono::high_resolution_clock::now();
						ibb = finb.items.find(ib);
						end = chrono::high_resolution_clock::now();
						timer = ((end-start).count() / 1000.0);
						cout << "[BST] Elapsed time: " << timer << " microseconds" << endl;
						

						if (ibb.title == ib.title) {
							cout << ibb.info << endl;
						}
						else {
							cout << "Invalid title." << endl;
						}
						cout << endl;
					}
					// adding item to section
					else if (inputS == 4) {
						cout << "Enter a title for the item: ";
						string addIt;
						cin >> addIt;

						item iav;
						iav.title = addIt;

						item ia;
						ia.title = addIt;
						item iaa;
						iaa = fin.items.find(ia);
						if (iaa.title == ia.title) {
							cout << "Item \"" << iaa.title << "\" already exists in the \"" << fin.title << "\"."  << endl;
						}
						else {
							//deciding description
							cout << "Enter a description for the item: ";
							string itemA;
							cin >> itemA;
							ia.info = itemA;
							iav.info = itemA;
							auto start = chrono::high_resolution_clock::now();
							finb.items.insert(iav);
							auto end = chrono::high_resolution_clock::now();
						    long long timer = ((end-start).count() / 1000.0);
						    cout << "[AVL] Elapsed time: " << timer << " microseconds" << endl;


							start = chrono::high_resolution_clock::now();
							fin.items.insert(ia);
							end = chrono::high_resolution_clock::now();
						    timer = ((end-start).count() / 1000.0);
						    cout << "[BST] Elapsed time: " << timer << " microseconds" << endl;

							cout << "The new item \"" << addIt << "\" has been inserted." << endl;

						}
						cout << endl;
					}
					else if (inputS == 5) {
						// updating the item in section
						cout << "Enter a title for the item: ";
						string upIt;
						cin >> upIt;
						item upV;
						upV.title = upIt;
						item uppV;
						auto start = chrono::high_resolution_clock::now();
						uppV = finb.items.find(upV);
						auto end = chrono::high_resolution_clock::now();
						long long timerA = ((end-start).count() / 1000.0);
						

						item up;
						up.title = upIt;
						item upp;
						start = chrono::high_resolution_clock::now();
						upp = fin.items.find(up);
						end = chrono::high_resolution_clock::now();
						long long timer = ((end-start).count() / 1000.0);
						
						cout << "[AVL] Elapsed time: " << timerA << " microseconds" << endl;
						cout << "[BST] Elapsed time: " << timer << " microseconds" << endl;
						// updating the item by first removing, changing then inserting again.
						if (upp.title == up.title) {
							cout << "Enter new information: ";
							string upInf;
							cin >> upInf;
							//auto start = chrono::high_resolution_clock::now();
							finb.items.remove(upV);
							upV.info = upInf;
							finb.items.insert(upV);
							//auto end = chrono::high_resolution_clock::now();
						    //long long timer = ((end-start).count() / 1000.0);
						    //cout << "[AVL] Elapsed time: " << timer << " microseconds" << endl;

							//start = chrono::high_resolution_clock::now();
							fin.items.remove(upp);
							upp.info = upInf;
							fin.items.insert(upp);
							//end = chrono::high_resolution_clock::now();
						    //timer = ((end-start).count() / 1000.0);
						    //cout << "[BST] Elapsed time: " << timer << " microseconds" << endl;


							cout << "The content " << fin.title << " has updated." << endl;

						}
						else {
							

							cout << "Item \"" << upIt << "\" does not exist in the \"" << fin.title << "\"."  << endl;
						}
						cout << endl;
					}
					// deleting item from section
					else if (inputS == 6) {
						cout << "Enter a title for the item: ";
						string delIt;
						cin >> delIt;
						
						item delV;
						delV.title = delIt;
					
						item del;
						del.title = delIt;
						item dele;
						dele = fin.items.find(del);

						if (dele.title == del.title) {

							auto start = chrono::high_resolution_clock::now();
							finb.items.remove(delV);
							auto end = chrono::high_resolution_clock::now();
							long long timer = ((end-start).count() / 1000.0);
							cout << "[AVL] Elapsed time: " << timer << " microseconds" << endl;

							start = chrono::high_resolution_clock::now();
							fin.items.remove(dele);
							end = chrono::high_resolution_clock::now();
							timer = ((end-start).count() / 1000.0);
							cout << "[BST] Elapsed time: " << timer << " microseconds" << endl;


							cout << "The item \"" << delIt << "\" has been deleted." << endl;
						}
						else {
							cout << "Item \"" << delIt << "\" does not exist in the \"" << fin.title << "\"." << endl;
						}
						cout << endl;
					}
					else if (inputS == 7) {
						flagS = false;
						cout << endl;
						cout <<  "MENU" << endl << "Please enter an input between [1 - 6]: " << endl
							<< "1- Display the sections [AVL]" << endl << "2- Display the sections [BST]" << endl << "3- Select a section" <<
							endl << "4- Add new section" << endl << "5- Delete a section" << endl << "6- Exit";
					}
				
				}
			}
		}
		// creating new section and inserting to notebook trees
		else if (input == 4) {
			string sectI;
			cout << "Enter a title for the section: ";
			cin >> sectI;

			AVLsection insV;
			insV.title = sectI;
			AVLsection inserV;

			BSTsection ins;
			ins.title = sectI;
			BSTsection inser = bst.find(ins);

			if (inser.title == sectI) {
				cout << "Section \"" << sectI << "\" already exists." << endl;
			}
			else {
				avl.insert(insV);

				bst.insert(ins);
				cout << "The new section \"" << sectI << "\" has been inserted." << endl;
			}
		}
		// deleting the section from notebooks
		else if (input == 5) {
			string sectD;
			cout << "Enter a title for the section: ";
			cin >> sectD;
			BSTsection del;
			del.title = sectD;
			BSTsection delet = bst.find(del);

			AVLsection delV;
			delV.title = sectD;
			if (delet.title != sectD) {
				cout << "Section \"" << sectD << "\" does not exist." << endl;
			}
			else {
				avl.remove(delV);
				bst.remove(delet);
				cout << "The section has been deleted." << endl;
			}
		}

		// finishing the program
		else if (input == 6) {
			flag = false;
			cout << endl << "Terminating...";
		}
		cout << endl;
	}
	
	return 0;
}