#ifndef MY_RDAG_H
#define MY_RDAG_H

#include "MyRDAG_node.h"
#include "Ref.h"
#include <fstream>
#include <map>

class MyRDAG{
  private:
	/**Fields**/
    MyRDAG_node* root;
    bool isBuild;
	FourTopk_second_oned A1_obj;
	//vector<MyRDAG_node*> dup_list;
	map<pair<int, int>, MyRDAG_node*> node_map;
	node_ref ref;
	vector<bool> weight_from_file;
	int total_weight_bit;

	/**Functions**/
	char bool_to_bit(bool bit, int loc);
	void bool_into_char(char* encoded, bool encode[], int size);
	void char_into_bool(char encoded, bool encode[]);
	bool check_contain(std::vector<MyRDAG_node*> node_vec, MyRDAG_node* node);
	void _byte_encode(int* bit_count, char* encoded_num, bool encode[], ofstream& fileOut,
			bool* moreBit, bool addBit);

  public:
	MyRDAG();
	MyRDAG(int l, int r, vector<int>* list1);
	MyRDAG(int l, int r, vector<int>* list1, bool obj_build);
	~MyRDAG();
	void build();
	void build(vector<bool> weight_vec);
	vector<int>* top2Q(int y1, int y2);
	bool encode_to_file(const char* outFile);
	bool encode_to_file(const char* outFile, const char* a1);
	bool read_from_file(const char* inFile);
	bool read_from_file(const char* inFile, const char* a1);
	int get_total_weight();
	int A1_total_bits();
};

#endif
