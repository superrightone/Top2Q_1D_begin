#include "Oned_topk.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <random>
#include <cstdlib>

void func1(int* a, int* b, int range1, int range2);

int main(){

	vector<int> test_vec;
	for(int i = 0; i < 10; i++){
		test_vec.push_back(i+1);
	}

	/*****
	std::ofstream f_out;
	f_out.open("testing.txt");
	FourTopk_second_oned enc_ins(test_vec, 2);

    enc_ins.test(f_out);
	f_out.close();
	*****/
    int l, r;
	std::default_random_engine engine;
	std::uniform_int_distribution<int> dist(0, 9);
	FourTopk_second_oned test_obj(test_vec, 2);
    vector<int> res_vec = test_obj.getTopkPos(0, 9, 2);
	l = 0; r = 6;
	for(int j = 0; j < 5; j++){
	//   l = dist(engine); r = dist(engine);
	//   if(l > r) swap(l, r);

	   std::cout <<l << "--"<<j<<"--"<<r << endl;
	   res_vec = test_obj.getTopkPos(l, r, 2);
	   for(int i = 0; i < res_vec.size(); i++){
	   std::cout << res_vec[i] << std::endl;
	   }
	}

    ofstream f_out("write_data.txt");
	test_obj.test(f_out);

	return 0;
}

void func1(int* a, int* b, int range1, int range2){
  std::default_random_engine engine;
  std::uniform_int_distribution<int> dist(range1, range2);
  int x,y;

  engine.seed(time(NULL));
  x = dist(engine);
  y = dist(engine);
  *a = x < y ? x : y;
  *b = x < y ? y : x;
}
