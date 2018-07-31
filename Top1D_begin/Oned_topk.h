//Oned_topk.h

#ifndef ONED_TOPK_H
#define ONED_TOPK_H

#include <iostream>
#include <sdsl/bit_vectors.hpp>
#include <list>
#include <fstream>
using namespace std;
using namespace sdsl;

class FourTopk_second_oned{
	private:
		rrr_vector<255> S;
	public:
		FourTopk_second_oned();
		FourTopk_second_oned(vector<int>& input, int k);
		vector<int> getTopkPos(int i, int j, int k);
		void size();
		int byte_size();
		int unary_to_int(int* pivot);
		void int_to_unary(bit_vector& S, int input, int* pivot);
        int total_bits();
		void file_save(const char* outFile);
		void file_load(const char* inFile);
		void test(std::ofstream& fout);
};

#endif
