//act_demo.cpp

#include "MyRDAG_node.h"
#include "MyRDAG.h"
#include <chrono>
#include <ctime>
#include <ratio>

using namespace std;
using namespace sdsl;


int main(int argv, char* args){

	/**Init**/
    vector<int> firstRow;
	vector<int> secondRow;
	int n_col = 0;
    int temp = 0;
    int x1, x2, y1, y2;
    ifstream f_in(args[0]);
	ofstream f_out(args[1]);

	/**File input(Row info) **/
    f_in >> n_col;
	for(int i = 0; i < n_col; i++){
		f_in >> temp;
		firstRow.push_back(temp);
	}
	for(int i = 0; i < n_col; i++){
		f_in >> temp;
		secondRow.push_back(temp);
	}

	/**Execution process **/
    chrono::steady_clock::time_point t1 = chrono::steady_clock::now();

    MyRDAG body(0, n_col - 1, &firstRow, &secondRow);
    body.build();

	chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
	f_out << "Build time: " << chrono::duration_cast<chrono::nanoseconds>(t2 - t1).count()
		<< " ns" << endl;

	/**Query**/
	f_in >> x1 >> y1 >> x2 >> y2;
	vector<int>* result;
	t1 = chrono::steady_clock::now();    //timer start(query)
	result = body.top2Q(x1, y1, x2, y2);
    t2 = chrono::steady_clock::now();   //timer end(query)
	f_out << "Query time: " << chrono::duration_cast<chrono::nanoseconds>(t2 - t1).count()
		<<  " ns" << endl;

	f_out << "Result: {";
	for(int i = 0; i < result->size(); i++){
		f_out << (*result)[i];
	    if(i != result->size() - 1)
		   f_out << ", ";
	}
	f_out << "}" << endl;

    f_out << "Total weight bits: " <<body.get_total_weight() << endl;
	f_out << "A1 array: " << body.A1_total_bits() << " bits" << endl;
	f_out << "A2 array: " << body.A2_total_bits() << " bits" <<endl;

	return 0;
}
