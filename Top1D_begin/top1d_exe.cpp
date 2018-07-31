//act_demo.cpp

#include "MyRDAG_node.h"
#include "MyRDAG.h"
#include <chrono>
#include <ctime>
#include <ratio>
#include <fstream>

using namespace std;
using namespace sdsl;

bool _main_read(ifstream& file_in, char* r_file_name, int* col, vector<int>& f_row);

int main(int argc, char* argv[]){

	vector<int> firstRow;
	int n_col = 0;
	int temp = 0;
	vector<int> range_input[2];
 	int y1, y2;
    	ifstream f_in;
	ofstream f_out;
	string dag_fname = "buildSave.dat";
	string a1_fname = "A1_save.dat";

	if(argc < 2){
		return -1;
	}
	else if(argc == 3){
		bool isContinue = _main_read(f_in, argv[1], &n_col, firstRow);
		if(!isContinue){
			cout << "Input format error!" << endl;
			return -1;
		}
	}
	else if(argc == 6){
		bool isContinue =  _main_read(f_in, argv[1], &n_col, firstRow);
		dag_fname = argv[3]; a1_fname = argv[4]; 
	}
	else{
		cerr << "To be supported.." << endl;
		return -2;
	}
	f_out.open(argv[2]);

    chrono::steady_clock::time_point t1 = chrono::steady_clock::now();

    MyRDAG body(0, n_col - 1, &firstRow);
    body.build();

	chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
	f_out << "Build time: " << chrono::duration_cast<chrono::nanoseconds>(t2 - t1).count()
		<< " ns" << endl;
	//cerr << "Build end" << endl;
    //body.encode_to_file("buildSave.dat");
	//body.encode_to_file(dag_fname.c_str() , a1_fname.c_str(), a2_fname.c_str() );
    f_out << "Top2 Query Range(y1, y2): " << endl;
	//Get Query input
	while(f_in){
		f_in >> y1;
		if(y1 == -1) break;
		f_in >> y2;
		range_input[0].push_back(y1); range_input[1].push_back(y2);
		if(y1 < -1 || y2 < 0 ){
			cerr << "Required query range is out of possible range" << endl;
			return -1;
		}
	}
	///////////////////////////////////////////////////////////////
/**	
	t2 = chrono::steady_clock::now();
    MyRDAG file_body(0, n_col - 1, &firstRow, &secondRow, false);
	file_body.read_from_file(dag_fname.c_str(), a1_fname.c_str(), a2_fname.c_str() );
    t1 = chrono::steady_clock::now();

	f_out << "File fetching build time: " << chrono::duration_cast<chrono::nanoseconds>(t1 - t2).count() << " ns" << endl;
	vector<int>* file_result[range_input[1].size()];
    for(int i = 0; i < range_input[1].size(); i++){
       t1 = chrono::steady_clock::now();
       file_result[i] = file_body.top2Q(range_input[0][i], range_input[1][i],
                           range_input[2][i], range_input[3][i]);
       t2 = chrono::steady_clock::now();
       f_out << "Query time(file): " << chrono::duration_cast<chrono::nanoseconds>(t2 - t1).count()
           << " ns" << endl;
    }
**/
    //////////////////////////////////////////////////////////////
    int total = 0;	
	vector<int>* result[range_input[1].size()];
	for(int i = 0; i < range_input[1].size(); i++){
		t1 = chrono::steady_clock::now();
		result[i] = body.top2Q(range_input[0][i], range_input[1][i]);
		t2 = chrono::steady_clock::now();
		f_out << "Query time: " << chrono::duration_cast<chrono::nanoseconds>(t2 - t1).count()
			<< " ns" << endl;
		total += (int)chrono::duration_cast<chrono::nanoseconds>(t2 - t1).count();
	}
    f_out << "Average Query time: " << total/range_input[1].size() << " ns" << endl;

	assert(result[0]->size() == 2);

	for(int i = 0; i < range_input[1].size() ; i++){
		f_out << "Query " << (i+1) << endl;
		f_out << "Top1 pos : (" << (*result[i])[0] << ")" << endl;
		f_out << "Top2 pos : (" << (*result[i])[1] << ")" << endl;
		f_out << "-----------------------------------------" << endl;
	}

//	f_out << "Top1 pos(file) : (" << (*file_result[0])[0] << ", " << (*file_result[0])[1] << ")" << endl;
//	f_out << "Top2 pos(file) : (" << (*file_result[0])[2] << ", " << (*file_result[0])[3] << ")" << endl;
    f_out << "Total weight bits: " <<body.get_total_weight() << endl;
	f_out << "A1 array: " << body.A1_total_bits() << " bits" << endl;

	f_in.close();
	f_out.close();
	return 0;
}

bool _main_read(ifstream& file_in, char* r_file_name, int* col, vector<int>& f_row){
	file_in.open(r_file_name);
	if(!file_in.is_open() ){
		return false;
	}
	file_in >> *col;
	int temp = -1;
	for(int i = 0; i < *col; i++){
		if(file_in.eof()){
			return false;
		}
		file_in >> temp;
		f_row.push_back(temp);
	}
	return true;
}
