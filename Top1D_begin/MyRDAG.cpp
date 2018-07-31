#include "MyRDAG.h"
#include "Ref.h"
#include <assert.h>
#include <stack>
#include <fstream>

char MyRDAG::bool_to_bit(bool bit, int loc){
	if(!bit){
		return 0b00000000;
	}
	else{
		switch(loc){
			case 0: return 0b10000000;
					break;
			case 1: return 0b01000000;
					break;
			case 2: return 0b00100000;
					break;
			case 3: return 0b00010000;
					break;
			case 4: return 0b00001000;
					break;
			case 5: return 0b00000100;
					break;
			case 6: return 0b00000010;
					break;
			case 7: return 0b00000001;
					break;
			default: return 0b00000000;
		}
	}
}

void MyRDAG::bool_into_char(char* encoded, bool encode[], int size){
	assert(*encoded == 0b00000000);
	for(int i = 0; i < size; i++){
		*encoded |= bool_to_bit(encode[i], i);	
	}
}
/******************************************************************************/

void MyRDAG::char_into_bool(char encoded, bool encode[]){
  char result = 0b11111111 & (encoded);
  if( (result & 0b10000000) == 0b10000000 )
	  encode[0] = true;
  if( (result & 0b01000000) == 0b01000000 )
	  encode[1] = true;
  if( (result & 0b00100000) == 0b00100000 )
	  encode[2] = true;
  if( (result & 0b00010000) == 0b00010000 )
	  encode[3] = true;
  if( (result & 0b00001000) == 0b00001000 )
	  encode[4] = true;
  if( (result & 0b00000100) == 0b00000100 )
	  encode[5] = true;
  if( (result & 0b00000010) == 0b00000010 )
	  encode[6] = true;
  if( (result & 0b00000001) == 0b00000001 )
	  encode[7] = true;

}
/*****************************************************************************************************/

void MyRDAG::_byte_encode(int* bit_count, char* encoded_num, bool encode[], ofstream& fileOut,
		bool* moreBit, bool addBit){
         bool_into_char(encoded_num, encode, 8);
         *bit_count = 8;
         fileOut.put(*encoded_num);
         *encoded_num = 0b00000000;
         for(int j = 0; j < 8; j++){
             encode[j] = false;
         }
         if(*moreBit){
             encode[8 - (*bit_count)] = addBit;
             (*bit_count)--;
             *moreBit = false;
         }
 #ifdef DEBUG
         cerr << "Write: " <<  fileOut.tellp() << endl;
 #endif
}
/****************************************************************************************************/
/***
void MyRDAG::_byte_decode(){

}
***/

MyRDAG::MyRDAG(){
	root = new MyRDAG_node();
	isBuild = false;
	A1_obj = FourTopk_second_oned();
	ref = {0};
	total_weight_bit = 0;
}

MyRDAG::MyRDAG(int l, int r,  vector<int>* list1){
	A1_obj = FourTopk_second_oned(*list1, 2);
	total_weight_bit = 0;
	ref = {&A1_obj, list1, &node_map, -1, -1, false, &total_weight_bit};
	root = new MyRDAG_node(l, r, NULL, NULL, ref);
	isBuild = false;
}

MyRDAG::MyRDAG(int l, int r, vector<int>* list1, bool obj_build){
	if(obj_build){
		A1_obj = FourTopk_second_oned(*list1, 2);
	}
	total_weight_bit = 0;
	ref = {&A1_obj, list1, &node_map, -1, -1, false, &total_weight_bit};
	root = new MyRDAG_node(l, r, NULL, NULL, ref);
	isBuild = false;
}

/**MyRDAG::MyRDAG(int l, int r, std::ifstream fileIn){

}**/

MyRDAG::~MyRDAG(){
   node_map.clear();
}

void MyRDAG::build(){
	root->build();
	isBuild = true;
}

vector<int>* MyRDAG::top2Q(int y1, int y2){
	assert(isBuild);
	if(y2 - y1 < 1){
		cerr << "Range query for top2 requires range of more than 1 element!" << endl;
	}

	return root->top2Q(y1, y2);
}


/**
 * class : MyRDAG
 * Member function name : encode_to_file
 * parameter : [const char*] outFile
 * return type : boolean
 * Description : This function is default version of "encode_to_file" with 2 more variables.
 *               It just calls "encode_to_file(outFile, NULL, NULL)".
 *               Save file name and load file name will be determined by default name. 
 *
 **/
bool MyRDAG::encode_to_file(const char* outFile){
	return encode_to_file(outFile, NULL);
}

bool MyRDAG::encode_to_file(const char* outFile, const char* a1){
	assert(this->isBuild);

	char encoded_num = 0b00000000;
	bool encode[8] = {false};
	int bit_count = 8;
	std::stack<MyRDAG_node*> ite_stack;
	std::vector<MyRDAG_node*> node_vec;
	ite_stack.push(this->root);
	MyRDAG_node* temp = this->root;
	ofstream fileOut(outFile);
	/************************************************************/
	fileOut << total_weight_bit;
	while( !ite_stack.empty() ){
		temp = ite_stack.top();
        	bool isExist = false;
		bool isLExist = false;
		bool isRExist = false;
		bool moreBit = false;
		if(node_vec.size() > 0){
			for(int i = 0; i < node_vec.size(); i++){
				if(node_vec[i] == temp){
					isExist = true;
				}
				if(node_vec[i] == temp->get_left_child() )
					isLExist = true;
				if(node_vec[i] == temp->get_right_child() )
					isRExist = true;
			}
		}
		if(!isExist){
#ifdef DEBUG
			cerr << temp->get_weight() << endl;
#endif
			if(temp->get_weight()){
			    encode[8 - bit_count] = temp->get_weight_bit();
				bit_count--;
				if(bit_count == 0) moreBit = true;
			}//inner2 if end
			else{
			}//inner2 else end

			if(bit_count == 0){
				_byte_encode(&bit_count, &encoded_num, encode, fileOut, &moreBit, temp->get_weight_bit() );
			}//inner2 if end2
			
		}//if end
//		cerr << endl << temp << "- left:  " << temp->get_left_child() << ", right: " << temp->get_right_child() << endl;
//		cerr << "Left range: " << temp->get_left_range() << ", Right range: " << temp->get_right_range() << endl;
//		if(node_vec.size() > 0) cerr << "      " << node_vec.back() << endl;
//		cerr << "stack size: " <<ite_stack.size() << endl;
		if(!temp->get_left_child() || isLExist ){
			if( !temp->get_right_child() || isRExist ){
				ite_stack.pop();
				if(!isExist) node_vec.push_back(temp);
			}//nest if end
			else{
				if(!isRExist){
					ite_stack.push(temp->get_right_child() );
					if(!isExist) node_vec.push_back(temp);
				}//nnest if end
				else{
					ite_stack.pop();
				}//nnest if-else end
			}//nest if-else end
		}//if end
		else{
			if(isExist){
				if( !temp->get_right_child() || isRExist ){
					ite_stack.pop();
				}
				else{
					if(!isRExist){
						ite_stack.push(temp->get_right_child() );
					}
					else{
						ite_stack.pop();
					}
				}
			}//nest if end2
			else{
				if(!isLExist){
					ite_stack.push(temp->get_left_child() );
					node_vec.push_back(temp);
				}//nnest if end2
				else{
					if( !temp->get_right_child() || isRExist ){
						ite_stack.pop();
					}
					else{
						if(!isRExist){
							ite_stack.push(temp->get_right_child() );
							node_vec.push_back(temp);
						}
						else{
							ite_stack.pop();
						}
					}
				}//nnest if-else end2
			}//nest if-else end2
		}//if-else end
		isExist = false;
	}
	if(bit_count > 1 ){
		bool_into_char(&encoded_num, encode, 8);
		fileOut.put(encoded_num);
	}
#ifdef DEBUG
	cerr << "node_vec size: " << node_vec.size() << endl;
#endif
	if(!a1){
		A1_obj.file_save("A1_save.dat");
	}
	else{
		A1_obj.file_save(a1);
	}
	fileOut.close();
	/**********************************************************/
	return true;
}
/***********************************************************************************/
/***********************************************************************************/





bool MyRDAG::read_from_file(const char* inFile){
	return read_from_file(inFile, NULL);
}


/**
 * class : MyRDAG
 * function name: read_from_file
 * param: const char* inFile
 */
bool MyRDAG::read_from_file(const char* inFile, const char* a1){
	stack<MyRDAG_node*> ite_stack;
	bool decode[8] = {false};
	char decoding = 0b00000000;
	int bit_count = -1;
	int remain_bit = -1;
	int total_bit = -1;
	MyRDAG_node* cur_node;
        bool is_wed = false;
	bool is_lcheck = false;
	bool is_rcheck = false;
	int opt_l, opt_r;
	opt_l = opt_r = -1;
	ifstream in_stream(inFile);
	if(!a1 ){
    		A1_obj.file_load("A1_save.dat");
	}
	else{
		A1_obj.file_load(a1);
	}

	if(!in_stream){
		exit(-3);
	}//check validity of input file
	in_stream >> total_bit;
	total_weight_bit = total_bit;
	remain_bit = total_bit - (total_bit/8)*8;
	ite_stack.push(this->root);

	while(in_stream){
		decoding = in_stream.get();
		char_into_bool(decoding, decode);
		int iteration = 8;
		if(remain_bit != 0 && in_stream.tellg() == (total_bit/8 + 3)  )
			iteration = remain_bit;
		//8 weight bit decoding...
		//cerr << iteration << endl;
		if( !in_stream.eof() ){
			for(int i = 0 ; i < iteration; i++){

				/***Checking cur_node and remaining bit***/

                //Stack iterate start
				if(ite_stack.empty()){
					break;
				}
				cur_node = ite_stack.top();

				if(cur_node == root || cur_node->check_real_weight() ){
					is_wed = true;
					cur_node->set_weight(true);
					cur_node->set_weight_bit(decode[i]);
				}

		//		cerr << "file read address: " << cur_node << "l_range" << cur_node->get_left_range() << "r_range" << cur_node->get_right_range() <<endl;
				//Checking cur_node if it has been visited and ,if so, check visits of children
				if( this->node_map.count( pair<int, int>(cur_node->get_left_range(), cur_node->get_right_range()) ) > 0){
		//			cerr << "poping check" << endl;
					is_lcheck = false;
					is_rcheck = false;
					cur_node->get_opt_cols(&opt_l, &opt_r);

					if(cur_node->get_left_child() && this->node_map.count(pair<int, int>(cur_node->get_left_child()->get_left_range(), 
								                               cur_node->get_left_child()->get_right_range()) ) > 0){
						is_lcheck = true;
					}
					if(cur_node->get_right_child() &&
							this->node_map.count( pair<int, int>(cur_node->get_right_child()->get_left_range(),
								                               cur_node->get_right_child()->get_right_range()) ) > 0){
						is_rcheck = true;
					}

					if(is_rcheck && (cur_node->get_left_range() >= opt_r) ){
						ite_stack.pop();
						i--;
						continue;
					}
					else if(is_lcheck && !is_rcheck && (opt_l < cur_node->get_right_range() ) ){
   					    	MyRDAG_node* right_child = new MyRDAG_node(opt_l + 1, cur_node->get_right_range(), false, cur_node, NULL, cur_node->get_ref() );
					    	cur_node->set_right_child(right_child);
					   	ite_stack.push(right_child);
					    	i--;
					    	continue;
					}
					else if(is_lcheck && !is_rcheck && (opt_l >= cur_node->get_right_range() ) ||
							(is_lcheck && is_rcheck)){
						ite_stack.pop();
						i--;
						continue;
					}


				}//if end

				/*********************************************/

				this->node_map[pair<int, int>(cur_node->get_left_range(), cur_node->get_right_range())] = cur_node;

				//cur_node = ite_stack.top();

				if( cur_node->get_right_range() == cur_node->get_left_range() ){
					ite_stack.pop();
					continue;
				}
				cur_node->get_opt_cols(&opt_l, &opt_r);
		//		cerr << opt_l << ", " << opt_r << endl;
				if(!cur_node->get_left_child() && cur_node->get_left_range() < opt_r ){
					bool comp = this->node_map.count(pair<int, int>(cur_node->get_left_range(), opt_r - 1) ) > 0;
					if(!comp){
						MyRDAG_node* left_child = new MyRDAG_node(cur_node->get_left_range(), opt_r - 1, false, NULL, cur_node, cur_node->get_ref() );
						cur_node->set_left_child(left_child);
						ite_stack.push(left_child);
						continue;
					}
					else{
						cur_node->set_left_child(this->node_map[pair<int, int>(cur_node->get_left_range(), opt_r - 1)]);
						cur_node->get_left_child()->set_right_parent(cur_node);
					}
				}
				if(!cur_node->get_right_child() && opt_l < cur_node->get_right_range() ){
					MyRDAG_node* right_child = new MyRDAG_node(opt_l + 1, cur_node->get_right_range(), false, cur_node, NULL, cur_node->get_ref() );
					cur_node->set_right_child(right_child);
					if(this->node_map.count(pair<int, int>(opt_l + 1, cur_node->get_right_range()) ) == 0){
						ite_stack.push(right_child);
					}
				}
				is_wed = false;
			}//for loop end
			
			for(int j = 0; j < 8; j++){
				decode[j] = false;
			}
		}
		/***********************************/
	
	}//main while end
    isBuild = true;	
	return true;
}


int MyRDAG::get_total_weight(){
	return total_weight_bit;
}

int MyRDAG::A1_total_bits(){
  return A1_obj.total_bits();
}

