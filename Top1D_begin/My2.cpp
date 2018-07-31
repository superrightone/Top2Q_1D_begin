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
	A2_obj = FourTopk_second_oned();
	ref = {0};
	total_weight_bit = 0;
}

MyRDAG::MyRDAG(int l, int r,  vector<int>* list1,  vector<int>* list2){
	A1_obj = FourTopk_second_oned(*list1, 2);
	A2_obj = FourTopk_second_oned(*list2, 2);
	total_weight_bit = 0;
    ref = {&A1_obj, &A2_obj, list1, list2, &node_map, -1, -1, -1, -1, false, &total_weight_bit};
	root = new MyRDAG_node(l, r, false, NULL, NULL, ref);
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

vector<int>* MyRDAG::top2Q(int x1, int y1, int x2, int y2){
	assert(isBuild);
    return root->top2Q(x1, y1, x2, y2);
}

bool MyRDAG::encode_to_file(const char* outFile){
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
	//int debug = 50000;
	fileOut << total_weight_bit;
	while( !ite_stack.empty() ){
		//if(debug <= 0) exit(-2);
        //debug--;
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
		if(	!isExist){
#ifdef DEBUG
			cerr << temp->get_weight() << endl;
#endif
			/***encoding the number of weight bits first***/
			encode[8 - bit_count] = temp->get_weight();
			bit_count--;
			  /***Writing a byte, when the byte is filled before encoding actual weight bits***/
			if(bit_count == 0){
            	moreBit = true;
				_byte_encode(&bit_count, &encoded_num, encode, fileOut, &moreBit, temp->get_weight1());
				if(temp->get_weight() ){
					encode[8 - bit_count] = temp->get_weight2();
					bit_count--;
				}
			}//inner if end
			/***Encoding weight bits when a byte is not filled with***/
            else{
				if(temp->get_weight()){
				    encode[8 - bit_count] = temp->get_weight1();
					bit_count--;
					if(bit_count == 0) moreBit = true;
					else{
						encode[8 - bit_count] = temp->get_weight2();
						bit_count--;
					}
				}//inner2 if end
				else{
					encode[8 - bit_count] = temp->get_weight2();
					bit_count--;
				}//inner2 else end
				if(bit_count == 0){
					_byte_encode(&bit_count, &encoded_num, encode, fileOut, &moreBit, temp->get_weight2() );
            		/**bool_into_char(&encoded_num, encode, 8);
					bit_count = 8;
					fileOut.put(encoded_num);
					encoded_num = 0b00000000;
					for(int j = 0; j < 8; j++){
						encode[j] = false;
					}
					if(moreBit){
						encode[8 - bit_count] = temp->get_weight2();
						bit_count--;
						moreBit = false;
					}
#ifdef DEBUG 
					cerr << "Write: " <<  fileOut.tellp() << endl;
#endif
					**/
				}//inner2 if end2
			}//inner else end
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
	A1_obj.file_save("A1_save.dat");
	A2_obj.file_save("A2_save.dat");
/****
	ifstream reOpen1("A1_save.dat");
	ifstream reOpen2("A2_save.dat");
	fileOut << reOpen1.rdbuf() << reOpen2.rdbuf();
	remove("A1_save.dat"); remove("A2_save.dat");**/
	fileOut.close();
	/**********************************************************/
	return true;
}
/***********************************************************************************/
/***********************************************************************************/


bool MyRDAG::read_from_file(const char* inFile){
	stack<MyRDAG_node*> ite_stack;
	vector<MyRDAG_node*> node_vec;
    bool decode[8] = {false};
	char decoding = 0b00000000;
	int bit_count = -1;
    int remain_bit = -1;
	int total_bit = -1;
    bool turn1, turn2, turn3;
    turn1 = turn2 = turn3 = false;
	ifstream in_stream(inFile);
    A1_obj.file_load("A1_save.dat");
	A2_obj.file_load("A2_save.dat");

	if(!in_stream){
		exit(-3);
	}//check validity of input file
	in_stream >> total_bit;
	remain_bit = total_bit - (total_bit/8)*8;
	turn1 = true;
	while(!in_stream){
		decoding = in_stream.get();
		char_into_bool(decoding, decode);
		//8 weight bit decoding...
		for(int i = 0 ; i < 8; i++){
			if(turn1){

			}
			else if(turn2){

			}
		}

		if( in_stream.eof() ){
	       //read remaining bits
		  decoding = 0b00000000;

		  char_into_bool(decoding, decode);
		}
	
	}//main while end
	
	return false;
}


int MyRDAG::get_total_weight(){
	return total_weight_bit;
}

int MyRDAG::A1_total_bits(){
  return A1_obj.total_bits();
}

int MyRDAG::A2_total_bits(){
  return A2_obj.total_bits();
}
