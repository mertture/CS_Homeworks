// Mert Ture 26800

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include "sketch.hpp"
#include "hasher.hpp"

#define BOOK_SIZE 3506329
#define COUNT 1
#define NO_SKETCHES 9

void read_freqs(std::pair<std::string,int> freqs[]) 
{ 
  std::fstream fin; 
  fin.open("top1000.txt", std::ios::in); 
  
  std::string line, word;
  int t = 0;
  
  while (getline(fin,line)) {
    std::stringstream s(line); 

   int elem = 0;
    while (getline(s, word, ',')) {
      if(!elem)
	freqs[t].first = word;
      else
	freqs[t].second = std::stoi(word);
      elem++;
    }
    t++;
  }
}

int main(){

  std::fstream file;
  std::string word, t, q, filename;
  filename = "do_androids_dream_of_electric_sheep.txt";
  file.open(filename.c_str());

  std::pair<std::string, int>freqs[1000];
  read_freqs(freqs);

  //Created all the Sketches one by one with array form, dynamically
  Sketch *sketch_arr[9];
  sketch_arr[0] = new CountMinSketch(0.01, 0.003);
  sketch_arr[1] = new CountMinMeanSketch(0.01, 0.003);
  sketch_arr[2] = new CountSketch(0.01, 0.003);

  sketch_arr[3] = new CountMinSketch(0.001, 0.003);
  sketch_arr[4] = new CountMinMeanSketch(0.001, 0.003);
  sketch_arr[5] = new CountSketch(0.001, 0.003);

  sketch_arr[6] = new CountMinSketch(0.001, 0.00001);
  sketch_arr[7] = new CountMinMeanSketch(0.001, 0.00001);
  sketch_arr[8] = new CountSketch(0.001, 0.00001);

  //You need to figure out how to construct sketch_arr and use in the loop below
  
  
  while(file >> word){
    for(int i = 0; i < NO_SKETCHES; i++){
      for(int j = 0; j < COUNT; j++){
	auto begin = std::chrono::steady_clock::now();
	sketch_arr[i]->insert(word);
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double>duration = end - begin;
	sketch_arr[i]->add_to_time(duration.count());
      }
    }
  }
  
 
  //Report
  std::cout << "\n\nREPORT\n";
  std::cout << "---------------------------------------------------" << std::endl;
  std::cout << "Table Size: " << sketch_arr[0]->get_no_rows() << " Rows x " << sketch_arr[0]->get_no_cols() << " Cols" << std::endl;
  std::cout << "Errors: " << sketch_arr[0]->name() << ": " << sketch_arr[0]->getError(freqs) << " -- " << sketch_arr[1]->name() << ": " << sketch_arr[1]->getError(freqs) << " -- " << sketch_arr[2]->name() << ": " << sketch_arr[2]->getError(freqs) << std::endl;
  std::cout << "Times: " << sketch_arr[0]->name() << ": " << sketch_arr[0]->get_time() << " -- " << sketch_arr[1]->name() << ": " << sketch_arr[1]->get_time() << " -- " << sketch_arr[2]->name() << ": " << sketch_arr[2]->get_time() << std::endl;
  std::cout << "---------------------------------------------------" << std::endl;
  std::cout << "Table Size: " << sketch_arr[3]->get_no_rows() << " Rows x " << sketch_arr[3]->get_no_cols() << " Cols" << std::endl;
  std::cout << "Errors: " << sketch_arr[3]->name() << ": " << sketch_arr[3]->getError(freqs) << " -- " << sketch_arr[4]->name() << ": " << sketch_arr[4]->getError(freqs) << " -- " << sketch_arr[5]->name() << ": " << sketch_arr[5]->getError(freqs) << std::endl;
  std::cout << "Times: " << sketch_arr[3]->name() << ": " << sketch_arr[3]->get_time() << " -- " << sketch_arr[4]->name() << ": " << sketch_arr[4]->get_time() << " -- " << sketch_arr[5]->name() << ": " << sketch_arr[5]->get_time() << std::endl;
  std::cout << "---------------------------------------------------" << std::endl;
  std::cout << "Table Size: " << sketch_arr[6]->get_no_rows() << " Rows x " << sketch_arr[6]->get_no_cols() << " Cols" << std::endl;
  std::cout << "Errors: " << sketch_arr[6]->name() << ": " << sketch_arr[6]->getError(freqs) << " -- " << sketch_arr[7]->name() << ": " << sketch_arr[7]->getError(freqs) << " -- " << sketch_arr[8]->name() << ": " << sketch_arr[8]->getError(freqs) << std::endl;
  std::cout << "Times: " << sketch_arr[6]->name() << ": " << sketch_arr[6]->get_time() << " -- " << sketch_arr[7]->name() << ": " << sketch_arr[7]->get_time() << " -- " << sketch_arr[8]->name() << ": " << sketch_arr[8]->get_time() << std::endl;
  std::cout << "---------------------------------------------------" << std::endl;

  std::cout << "\n\nTOP3 FREQUENCIES\n"; 
  std::cout << "---------------------------------------------------" << std::endl;
  std::cout << "REAL-->  WORD: " << freqs[0].first << "   COUNT: "<< freqs[0].second << std::endl;
  std::cout << "-----" << std::endl;
  std::cout << "Table Size: " << sketch_arr[0]->get_no_rows() << " Rows x " << sketch_arr[0]->get_no_cols() << " Cols" << std::endl;
  std::cout << sketch_arr[0]->name() << ": " << sketch_arr[0]->query(freqs[0].first) << " -- " << sketch_arr[1]->name() << ": " << sketch_arr[1]->query(freqs[0].first) << " -- " << sketch_arr[2]->name() << ": " << sketch_arr[2]->query(freqs[0].first) << std::endl;
  std::cout << "-----" << std::endl;
  std::cout << "Table Size: " << sketch_arr[3]->get_no_rows() << " Rows x " << sketch_arr[3]->get_no_cols() << " Cols" << std::endl;
  std::cout << sketch_arr[3]->name() << ": " << sketch_arr[3]->query(freqs[0].first) << " -- " << sketch_arr[4]->name() << ": " << sketch_arr[4]->query(freqs[0].first) << " -- " << sketch_arr[5]->name() << ": " << sketch_arr[5]->query(freqs[0].first) << std::endl;
  std::cout << "-----" << std::endl;
  std::cout << "Table Size: " << sketch_arr[6]->get_no_rows() << " Rows x " << sketch_arr[6]->get_no_cols() << " Cols" << std::endl;
  std::cout << sketch_arr[6]->name() << ": " << sketch_arr[6]->query(freqs[0].first) << " -- " << sketch_arr[7]->name() << ": " << sketch_arr[7]->query(freqs[0].first) << " -- " << sketch_arr[8]->name() << ": " << sketch_arr[8]->query(freqs[0].first) << std::endl;
  std::cout << "---------------------------------------------------" << std::endl;
  std::cout << "REAL-->  WORD: " << freqs[1].first << "   COUNT: "<< freqs[1].second << std::endl;
  std::cout << "-----" << std::endl;
  std::cout << "Table Size: " << sketch_arr[0]->get_no_rows() << " Rows x " << sketch_arr[0]->get_no_cols() << " Cols" << std::endl;
  std::cout << sketch_arr[0]->name() << ": " << sketch_arr[0]->query(freqs[1].first) << " -- " << sketch_arr[1]->name() << ": " << sketch_arr[1]->query(freqs[1].first) << " -- " << sketch_arr[2]->name() << ": " << sketch_arr[2]->query(freqs[1].first) << std::endl;
  std::cout << "-----" << std::endl;
  std::cout << "Table Size: " << sketch_arr[3]->get_no_rows() << " Rows x " << sketch_arr[3]->get_no_cols() << " Cols" << std::endl;
  std::cout << sketch_arr[3]->name() << ": " << sketch_arr[3]->query(freqs[1].first) << " -- " << sketch_arr[4]->name() << ": " << sketch_arr[4]->query(freqs[1].first) << " -- " << sketch_arr[5]->name() << ": " << sketch_arr[5]->query(freqs[1].first) << std::endl;
  std::cout << "-----" << std::endl;
  std::cout << "Table Size: " << sketch_arr[6]->get_no_rows() << " Rows x " << sketch_arr[6]->get_no_cols() << " Cols" << std::endl;
  std::cout << sketch_arr[6]->name() << ": " << sketch_arr[6]->query(freqs[1].first) << " -- " << sketch_arr[7]->name() << ": " << sketch_arr[7]->query(freqs[1].first) << " -- " << sketch_arr[8]->name() << ": " << sketch_arr[8]->query(freqs[1].first) << std::endl;
  std::cout << "---------------------------------------------------" << std::endl;
  std::cout << "REAL-->  WORD: " << freqs[2].first << "   COUNT: "<< freqs[2].second << std::endl;
  std::cout << "-----" << std::endl;
  std::cout << "Table Size: " << sketch_arr[0]->get_no_rows() << " Rows x " << sketch_arr[0]->get_no_cols() << " Cols" << std::endl;
  std::cout << sketch_arr[0]->name() << ": " << sketch_arr[0]->query(freqs[2].first) << " -- " << sketch_arr[1]->name() << ": " << sketch_arr[1]->query(freqs[2].first) << " -- " << sketch_arr[2]->name() << ": " << sketch_arr[2]->query(freqs[2].first) << std::endl;
  std::cout << "-----" << std::endl;
  std::cout << "Table Size: " << sketch_arr[3]->get_no_rows() << " Rows x " << sketch_arr[3]->get_no_cols() << " Cols" << std::endl;
  std::cout << sketch_arr[3]->name() << ": " << sketch_arr[3]->query(freqs[2].first) << " -- " << sketch_arr[4]->name() << ": " << sketch_arr[4]->query(freqs[2].first) << " -- " << sketch_arr[5]->name() << ": " << sketch_arr[5]->query(freqs[2].first) << std::endl;
  std::cout << "-----" << std::endl;
  std::cout << "Table Size: " << sketch_arr[6]->get_no_rows() << " Rows x " << sketch_arr[6]->get_no_cols() << " Cols" << std::endl;
  std::cout << sketch_arr[6]->name() << ": " << sketch_arr[6]->query(freqs[2].first) << " -- " << sketch_arr[7]->name() << ": " << sketch_arr[7]->query(freqs[2].first) << " -- " << sketch_arr[8]->name() << ": " << sketch_arr[8]->query(freqs[2].first) << std::endl;
  std::cout << "---------------------------------------------------" << std::endl;
}
















  
  

