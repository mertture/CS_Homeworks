#ifndef CSKECTH_H
#define CSKECTH_H

#include <iostream>
#include <algorithm>
#include <cstring>
#include <math.h>
#include "hasher.hpp"


class Sketch{

 protected:
  unsigned no_rows;
  unsigned no_cols;
  StrHash* hashes;
  long long int* table;
  double time;
  
  Sketch(double epsilon, double delta){
    no_rows = (log(1 / delta) / log(2));
    no_cols = (2 / epsilon);
    
    //prime column count
    bool found = false;
    
    while(!found){
      found = true;
      
      for(unsigned i = 2; i <= sqrt(no_cols); i++){
	if(no_cols % i == 0){
	  found = false;
	  no_cols++;
	}
      }
    }

    table = new long long int[this->no_rows * this->no_cols];
    
    hashes = new StrHash[no_rows];
    for(unsigned i = 0; i < no_rows; i++){
      hashes[i].set_parameters(no_cols);
    }

    time = 0;
    
    std::cout << "A sketch with " << this->no_rows << " rows and " << this->no_cols << " columns is created" << std::endl;    
  }

 public:

  long long int get(const unsigned& i, const unsigned& j) const {
    return table[i * no_cols + j];
  }

  void add(const unsigned& i, const unsigned& j, const int& val){
    table[i * no_cols + j] += (long long int)val;
  }

  virtual void insert_to_row(const std::string& data, unsigned row_id) = 0;
  virtual void insert(const std::string& data) = 0;
  virtual long long int query(const std::string& data) const = 0;

  virtual std::string name(){
    return "Sketch";
  }

  void reset(){
    memset(table, 0, sizeof(unsigned)*no_rows*no_cols);
  }

  unsigned get_no_rows(){
    return no_rows;
  }

  unsigned get_no_cols(){
    return no_cols;
  }

  double getError(std::pair<std::string, int> freqs[]){
    double err = 0.0;

    for(int i = 0; i < 1000; i++){
      std::string curr = freqs[i].first;
      long long int actual = freqs[i].second;
      long long int guess = query(curr);
	  
      double cerr = ((double)(abs(guess-actual)) / actual);
      err += cerr * cerr;
    }
	
    return sqrt(err / 1000);
  }
  
  void print() {
    for(unsigned i = 0; i < no_rows; i++) {
      for(unsigned j = 0; j < no_cols; j++) {
	std::cout << get(i, j) << " ";
      }
      std::cout << std::endl;
    }
  }
  

  void add_to_time(double partial){
    this->time += partial;
  }

  double get_time(){
    return this->time;
  }

  void reset_time(){
    this->time = 0;
  }
  
  ~Sketch(){
	delete[] table;
    delete[] hashes;
  }
};
//Freq Sketch as given in real cpp which is derived class of Sketch class
class FreqSketch: public Sketch{
protected:
	// It has id which shows which sketch is this
  FreqSketch(double epsilon, double delta, int id):Sketch(epsilon,delta){
	// I reseted table because in debug mode of Vscode, table values are -3453435 something but in release mode they are zero
	// So, to be sure I added reset part, it just after creating table, makes everything zero, so program works well
	  memset(table, 0, sizeof(long long int)*no_rows*no_cols);
	
	  // As i said,with id numbers it prints correct name of sketches
	if (id == 1) {
		std::cout << "--> It is a Count Sketch!\n";
	}
	else if (id == 2) {
		
		std::cout << "--> It is a Count-Min Sketch!\n";
	}
	else if (id == 3) {
		std::cout << "--> It is a Count-Min-Mean Sketch!\n";
	
	}
	
	
}
};



    //TO DO: IMPLEMENT DERIVED SKETCHES

// CountSketch is derived class of FreqSketch, then it goes to Sketch's constructor
class CountSketch: public FreqSketch {
private:
	// Same as in homework file, creating results and hash
	StrHash* g_hashes;
	int* results;
public:
	// Constructor
	CountSketch(double epsilon, double delta):FreqSketch(epsilon, delta, 1) {
		
		results = new int[this->no_rows];
		g_hashes = new StrHash[this->no_rows];
	
		for (unsigned i = 0; i < this->no_rows; i++) {
			g_hashes[i].set_parameters(this->no_cols);
			
		}
	}
	// Virtual because it is defined as pure virtual function in base class Sketch
	virtual void insert_to_row (const std::string& data, unsigned row_id) {
		
		unsigned col_id = hashes[row_id].hash(data);
		unsigned contrib = ((2 * (g_hashes[row_id].hash(data)%2)) - 1);
		add(row_id, col_id, contrib);
	}

	virtual void insert(const std::string& data) {
		for (unsigned i = 0; i < this->no_rows; i++) {
			insert_to_row(data, i);
		}
	}
	// I am first resetting the results, then adding, sorting and return middle value in get_error function of Sketch
	virtual long long int query(const std::string& data) const {
		unsigned h_col;
		unsigned coef;
		memset(results, 0, sizeof(int)*this->no_rows);
		for (unsigned i = 0; i < this->no_rows; i++) {
			h_col = hashes[i].hash(data);
			coef = (2 * (g_hashes[i].hash(data) % 2)) - 1;
			results[i] = (get(i, h_col) * coef);
			if (results[i] < 0) {
				results[i] = 0;
			}
		}
		std::sort(results, results + this->no_rows);
		
		return results[this->no_rows/2];
	}

	virtual std::string name(){
		return "CS";
    }
	// Destructor of it, deleting dynamic ones by delete
	~CountSketch() {
		delete[] results;
		delete[] g_hashes;
	}

};



// Derived class of FreqSketch again
class CountMinSketch: public FreqSketch {

public:
	// This is first constructor, for CountMinSketch in main, it first goes FreqSketch and then Sketch constructor, later prints the id = 2's cout
	CountMinSketch(double epsilon, double delta):FreqSketch(epsilon, delta, 2) {}
	
	// For minmean freqsketch cout process, id created extra constructor because otherwise i cannot differ min and minmean
	// It is for CountMinMean because it is derived class of CountMin so I have to give another id
	CountMinSketch(double epsilon, double delta, int id):FreqSketch(epsilon, delta, 3){}
	virtual void insert_to_row (const std::string& data, unsigned row_id) {
		unsigned col_id = hashes[row_id].hash(data);
		add(row_id, col_id, 1);
		
	}
	// inserting same as all 3 derived classes
	virtual void insert(const std::string& data) {
		for (unsigned int i = 0; i < this->no_rows; i++) {
			insert_to_row(data, i);
		}
		
	}
	// Compares r_Freq and freq, takes the lowest r_Freq and returns as freq
	virtual long long int query(const std::string& data) const {
		long long int r_freq = std::numeric_limits<long long int>::max();
		long long int freq = std::numeric_limits<long long int>::max();
		long long int h_col = 0;
		
		for (unsigned int i = 0; i < this->no_rows; i++) {
			h_col = hashes[i].hash(data);
			r_freq = get(i, h_col);
			if (r_freq < freq) {
				freq = r_freq;
			}
		}
		
		return freq;
	}
	// overridden name function
	virtual std::string name(){
		return "CMS";
    }
	// Not deleting anything because did not created anything dynamically
	~CountMinSketch() {

	}

};

// Derived class of CountMinSketch
class CountMinMeanSketch: public CountMinSketch {
private:
	int* results;
public:
	// Initializing CountMinSketch constructor which I created for MinMean, then dynamically allocate results
	CountMinMeanSketch(double epsilon, double delta):CountMinSketch(epsilon, delta, 3) {
		
		results = new int[no_rows];
		
	}
	// same sa MinSketch, adds values
	virtual void insert_to_row (const std::string& data, unsigned row_id) {
		unsigned col_id = hashes[row_id].hash(data);
		add(row_id, col_id, 1);
	}
	// same as other 2 derived classes, class insert_to_row
	virtual void insert(const std::string& data) {
		for (unsigned i = 0; i < no_rows; i++) {
			insert_to_row(data, i);
		}
		
	}
	// Close to CountSketch query but different
	// first resets the results, then hash, adds values to no_Stream integer. Lastly calculates noise and compares with r_Freq adds to results
	// at the end after sorting results, returns the value to get_Errors function
	virtual long long int query(const std::string& data) const {
		long long int no_stream = 0; 
		long long int r_freq = 0;
		long long int noise = 0;
		long long int h_col = 0;
		memset(results, 0, sizeof(int)*no_rows);

		for (unsigned i = 0; i < no_rows; i++) {
			h_col = hashes[i].hash(data);
			r_freq = get(i, h_col);
			no_stream = 0;
			for (unsigned j = 0; j < no_cols; j++) {
				no_stream += get(i, j);
			}
			noise =  ((no_stream - r_freq) / (this->no_cols - 1));
			if (r_freq > noise) {
				results[i] = r_freq - noise;
			}
		}
		std::sort(results, results + this->no_rows);
		
		return results[this->no_rows / 2];
	}
	// overridden name function
	virtual std::string name(){
		return "CMMS";
    }
	// deletes results which allocated dynamically
	~CountMinMeanSketch() {
		delete[] results;
		
	}

};



#endif CSKECTH_H

