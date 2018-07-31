//MyRDAG_node.h
#ifndef MYRDAG_NODE_H
#define MYRDAG_NODE_H

#include "Oned_topk.h"
#include "Ref.h"
#include <vector>

struct IndexPair{
  int index;
  int elem;

  IndexPair(){
	  index = -1;
	  elem = -1;
  }

  IndexPair(int i, int e){
	  index = i;
	  elem = e;
  }

  bool operator<(IndexPair p1){
	  return elem < p1.elem;
  }

  bool operator==(IndexPair p1){
	  return elem == p1.elem;
  }
  
  bool operator>(IndexPair p1){
	  return elem > p1.elem;
  }

};

class MyRDAG_node{
	private:
		/**Fields**/
		int left_range;
		int right_range;
		bool weight;  //true : weight exist, false : weight does not exist
		bool weight_bit;
		int level;
		int opt_l, opt_r;
		MyRDAG_node* l_child;
		MyRDAG_node* r_child;
		MyRDAG_node* l_parent;
		MyRDAG_node* r_parent;
		node_ref ref;
		bool leaf;
		//std::vector<IndexPair> range_heap1;
		//std::vector<IndexPair> range_heap2;

		/**Functions**/
		vector<int>* _top2Q();
		bool check_real_weight_left();
		bool check_real_weight_right();
	public:
		int total_weight;

		MyRDAG_node();
		MyRDAG_node(int l, int r);
		MyRDAG_node(int l, int r, bool weight, MyRDAG_node* l_pa, MyRDAG_node* r_pa, node_ref refer);
		MyRDAG_node(int l, int r, MyRDAG_node* l_pa, MyRDAG_node* r_pa, node_ref refer);
		~MyRDAG_node();
		void build();
		vector<int>* top2Q(int y1, int y2);
		bool check_real_weight();
		MyRDAG_node* expect_duplicate(int l, int r);
        /****/
		void get_range(int* left, int* right);
		void get_opt_cols();
		void get_opt_cols(int* opt_left, int* opt_right);
		int get_left_range();
		int get_right_range();
		bool get_weight();
		bool get_weight_bit();
		int get_level();
		bool get_leaf();
		node_ref get_ref();
		vector<int> getTop2Int(int num);
		MyRDAG_node* get_left_parent();
		MyRDAG_node* get_right_parent();
		MyRDAG_node* get_left_child();
		MyRDAG_node* get_right_child();
		void set_left_range(int l);
		void set_right_range(int r);
		void set_weight(bool w);
		void set_weight_bit(bool w);
		void set_left_parent(MyRDAG_node* node);
		void set_right_parent(MyRDAG_node* node);
		void set_left_child(MyRDAG_node* node);
		void set_right_child(MyRDAG_node* node);
};

#endif
