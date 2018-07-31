#ifndef NODE_REF_H
#define NODE_REF_H

#include "Oned_topk.h"
using namespace std;

class MyRDAG_node;

struct node_ref{
  FourTopk_second_oned* A1;
  vector<int>* list1;
  map<pair<int, int>, MyRDAG_node*>* node_map;
  int ply, pry;
  bool from_left;
  int* weight_bits;
};

#endif
