#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <random>

using namespace std;

void genRandomTest(int ncols, int range, char* fileName);

int main(int argc, char** args){
        
	if(argc != 4){
		cerr << "3 arguments are required" << endl;
		return -1;
	}

    genRandomTest( atoi(args[1]), atoi(args[2]), args[3]);

	return 0;
}

void genRandomTest(int ncols, int range, char* fileName){
  ofstream f_out(fileName);
  f_out << ncols << endl;

  srand(time(NULL));
  int elem = -1;
  for(int i = 0; i < ncols; i++){
	  elem = rand()%range;
	  f_out << elem << " ";
  }
  f_out << endl;
  
  int l, r;
  l = r = 0;
  for(int i = 0; i < 10; i++){
	l = rand() % ncols;
	r = rand() % ncols;
	if(r < l) swap(l, r);
	//f_out << (ncols * i / 10) << " " << (ncols * (i+1) / 10 - 1 );
	f_out << l << " " << r;
	f_out << endl;
  }
  f_out << (-1);
  f_out.close();

}
