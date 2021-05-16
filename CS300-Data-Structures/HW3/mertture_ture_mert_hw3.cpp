#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <list>
#include <algorithm>
#include "hashtable.h"
#include <iomanip>
using namespace std;

//Mert Ture 26800

// For looking if a is occurred in b, if any char of a is in b, returns false
bool Oppcontains(vector<string> a, vector<string> b) {
	
	for (int i = 0; i < b.size(); i++) {
		for (int j = 0; j < a.size(); j++) {
			if (b[i] == a[j]) {
				return false;
			}
		}
	}
	return true;
}

// Again looks if a is occurred in b but in a different way, if it is completely in b, returns false
bool contains(vector<string> a, vector<string> b) {
	for (int i = 0; i < a.size(); i++) {
		if(find(b.begin(), b.end(), a[i]) == b.end()) {
			return false;
		}
	}
	return true;

	
}

// calculating the confidence
double confidence(vector<string> a, vector<string> b, vector<vector<string>> movies) {
	
	vector<string> lhs;
	
	for (int i = 0; i < a.size(); i++) {
		lhs.push_back(a[i]);
		//cout << "a: " << a[i] << " ";
	}
	for (int i = 0; i < b.size(); i++) {
		lhs.push_back(b[i]);
		//cout << "b: " << b[i] << " ";
	}

	double countLhs = 0, countRhs = 0;
	for (int i = 0; i < movies.size(); i++) {
		if (contains(lhs, movies[i])) {
			countLhs++;
		}
		if (contains(a, movies[i])) {
			countRhs++;
		}
	}
	//cout << countLhs << " " << countRhs << endl;
	double ans = (1.0*countLhs / movies.size()) / (1.0*countRhs / movies.size()); 
	//cout << endl << ans << endl;
	return ans;
}





// calculating the support values
double support(vector<string> a, vector<vector<string>> x, int size) {
	double counter = 0;

	for (int i = 0; i < x.size(); i++) {
		if(contains(a,x[i])) {
			counter++;
		}	
	}
	double ans = double(counter) / size;
	
	return ans;
}

// making the item string for hashing and searching
string makeString(vector<string> a) {
	string ans = "";
	for (int i = 0; i < a.size(); i++) {
		ans += a[i] + ",";
	}
	ans.substr(0, ans.length()-1);

	return ans;
}


struct str {
	vector<string> vec;
	int counter;
	string key;


};


int main() {
	string file;
	double supTh, confTh;
	cout << "Please enter the transaction file name: ";
	cin >> file;

	cout << "Please enter support and confidence values between 0 and 1: ";
	cin >> supTh >> confTh;

	vector<string> tmp;
	tmp.push_back("...");
	HashTable<vector<string>> lookUpTable(tmp);


	ifstream input;
	input.open(file);
	string line;
	vector<vector<string>> movies;
	vector<vector<string>> miniMovies;
	
	//reading all the file and push to a vector
	while(getline(input, line)) {
		istringstream sLine(line);
		string movie;
		vector<string> temp;
		while (sLine >> movie) {
			
			temp.push_back(movie);
		}
		movies.push_back(temp);
	}
	

	// iterating the vector and inserting only the items which's support values are higher than threshold
	for (int i = 0; i < movies.size(); i++) {
		for (int j = 0; j < movies[i].size(); j++) {
			vector<string> singleTmpp;
			singleTmpp.push_back(movies[i][j]);
			if (support(singleTmpp, movies, movies.size()) >= supTh) {
				if (support(singleTmpp, movies, movies.size()) >= supTh && (lookUpTable.find(singleTmpp) == tmp)) {
					
					miniMovies.push_back(singleTmpp);
				}
				lookUpTable.insert(singleTmpp);
			}
		}
	}
	
	// iterating again and calculating pairs, first look if it is in hash table in O(1) by time power of hash table
	// because I just added only the items which's support values are higher than threshold 
	for (int i = 0; i < movies.size(); i++) {
		
		for (int j = 0; j < movies[i].size(); j++) {
			
			for (int k = j; k < movies[i].size(); k++) {
				vector<string> ftmp, stmp;
				ftmp.push_back(movies[i][j]); stmp.push_back(movies[i][k]);
				// checking if both items of the pair is in hash table (if they are higher than threshold)
				if (j != k && lookUpTable.find(ftmp) != tmp && lookUpTable.find(stmp) != tmp) {
					vector<string> pairTmp;
					pairTmp.push_back(movies[i][j]);
					pairTmp.push_back(movies[i][k]);
					sort(pairTmp.begin(), pairTmp.end());
					string ans = makeString(pairTmp);
					if (support(pairTmp, movies, movies.size()) >= supTh && (lookUpTable.find(pairTmp) == tmp)) {
						//cout << "aaaaa";
						miniMovies.push_back(pairTmp);
					}

					// if both items are in hashtable, calculates support and if it is higher than threshold again, inserts to hash table
						if (support(pairTmp, movies, movies.size()) >= supTh) {
							lookUpTable.insert(pairTmp);
							//cout << "bbbbbb";
						}
				}
				else {
				
				}
			}
		}
	}
	// at the end of the iteration, our hashtable is containing only the single items and pairs which's support values are higher than the threshold


	// now, I added all the items in hash table to a miniMovies vector. Hence I will not look all the items, just look the items in the hashtable and calculate their confidence level
	string resFile = "results.txt";
	ofstream output;
	output.open(resFile);
	int counter = 0;
	for (int i = 0; i < miniMovies.size(); i++) {
		for (int j = 0; j < miniMovies.size(); j++) {
			if (i != j && Oppcontains(miniMovies[i], miniMovies[j])) {
				double conf = confidence(miniMovies[i], miniMovies[j], movies);
				if (conf >= confTh) {
					counter++;
					for (int k = 0; k < miniMovies[i].size(); k++) {
						// writing to results.txt
						if (miniMovies[i].size() == 1) {
							output << miniMovies[i][k] << ";";
						}
						else {
							if (k != miniMovies[i].size()-1) {
								output << miniMovies[i][k] << ",";
							}
							else {
								output << miniMovies[i][k] << ";";
							}
						}
					}
					for (int k = 0; k < miniMovies[j].size(); k++) {
						if (miniMovies[j].size() == 1) {
							output << miniMovies[j][k] << ";";
						}
						else {
							if (k != miniMovies[j].size()-1) {
								output << miniMovies[j][k] << ",";
							}
							else {
								output << miniMovies[j][k] << ";";
							}
						}
					}
					// to make confidence 2 digits
					output << fixed << setprecision(2) << conf << endl;
				}
			}
		}
	}
	if (counter == 0) {
		cout << "There is no rule for the given support and confidence values." << endl;
	}
	else {
		cout << counter << " rules are written to results.txt" << endl;
	}


	/*for (int i = 0; i < miniMovies.size(); i++) {
		for (int j = 0; j < miniMovies[i].size(); j++) {
			cout << miniMovies[i][j] << " ";
		}
		cout << endl;
	}*/


	/*for (int i = 0; i < movies.size(); i++) {
		
		for (int j = 0; j < movies[i].size(); j++) {
			
			for (int k = 0; k < movies[i].size(); k++) {
				vector<string> pairTmp;
				pairTmp.push_back(movies[i][j]);
				if (j != k) {
						pairTmp.push_back(movies[i][k]);
						sort(pairTmp.begin(), pairTmp.end());
						if (lookUpTable.find(pairTmp) != tmp) {
							vector<string> x = lookUpTable.find(pairTmp);
							for (int z = 0; z < x.size(); z++) {
								cout << x[z] << " ";
							}
							cout << endl;
						}		
				}
				else {
				
				}
			}
		}
	}*/



	/*for (int i = 0; i < movies.size(); i++) {
		for (int j = 0; j < movies[i].size(); j++) {
			vector<string> tempp; tempp.push_back(movies[i][j]);
			if (lookUpTable.find(tempp) != tmp) {
				vector<string> x = lookUpTable.find(tempp);
				cout << x[0] << endl;
			}
		}
	}*/


	return 0;
}