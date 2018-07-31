
#include "Oned_topk.h"

using namespace std;
using namespace sdsl;

FourTopk_second_oned::FourTopk_second_oned(){

}
FourTopk_second_oned::FourTopk_second_oned(vector<int>& input,int k){
        bit_vector SS;
        int ele = 0;
        int n = input.size();
        int delta = 0;
        int sigma = 0;
        int sigma_prev = 0;
        int pivot = 0;
        SS.resize((k+1)*n);
        for(int i=0;i<n;i++){            
            for(int j=0;j<=i;j++){
                for(int q=j+1;q<=i;q++){
                    if(input[j]<input[q])
                        ele++;
                }
                ele = (ele >= k ? k : ele);
                sigma+=ele;
                ele=0;
            }
            if(i >= 1){
                delta = sigma - sigma_prev;
                int_to_unary(SS,delta,&pivot);
                //cout<<"delta"<<i<<" : "<<delta<<" sigma"<<i+1<<" : "<<sigma<<" sigma"<<i<<" : "<<sigma_prev;
                sigma_prev = sigma;
                sigma = 0;
            }
            //cout<<endl;
        }
        SS.resize(pivot);
        rrr_vector<255> rrr_S(SS);
        S=rrr_S;        
}
vector<int> FourTopk_second_oned::getTopkPos(int i,int j,int k){
    //void getTopkPos(int i,int j,int k){
        vector<int> result;
        list<int> order;
        list<int>::iterator iterInsertPos;
        vector<int> removeindex;
        vector<int> Sj;
        int pivot = 0;
        int delta = 0;
        int pushednum = 0;
        Sj.reserve(j+1);
        order.push_front(0);
        Sj[0] = 0;
        int loop = 0;
        for(int q = 0; q < j;q++){
            Sj[q+1]=0;
            delta = unary_to_int(&pivot);
            iterInsertPos = order.begin();
            for(int r = 0; r < delta;r++,++iterInsertPos);
            order.insert(iterInsertPos,q+1);
            iterInsertPos = order.begin();
            for(int r=0; r<delta;r++,++iterInsertPos){
                int index = *iterInsertPos;
                Sj[index]++;
                if(Sj[index]==k){//go to inactive
                    removeindex.push_back(index);
                }
            }
            for(int r = 0;r<removeindex.size();r++){
                order.remove(removeindex[r]);
            }
            removeindex.clear();
        }

        for(auto it = order.crbegin();it!=order.crend();++it){
            if(*it>=i){
                result.push_back(*it);
                pushednum++;
                if(pushednum==k)
                    break;                    
            }
        }
        return result;
        
}
void FourTopk_second_oned::size(){
        cout<<size_in_bytes(S)<<endl;
}

int FourTopk_second_oned::byte_size(){
     return size_in_bytes(S);
}

int FourTopk_second_oned::unary_to_int(int* pivot){
        int result = 0;
        for(;S[*pivot]==1;(*pivot)++){
            result++;
        }
        (*pivot)++;
        return result;
}
void FourTopk_second_oned::int_to_unary(bit_vector& S,int input,int* pivot){
        for(int i=0;i<input;i++){
            S[*pivot] = 1;
            (*pivot)++;
        }
        S[*pivot] = 0;
        (*pivot)++;
}

int FourTopk_second_oned::total_bits(){
   return size_in_bytes(S)*8;
}

void FourTopk_second_oned::file_save(const char* f_name){
  sdsl::store_to_file(S, f_name);
}

void FourTopk_second_oned::file_load(const char* f_name){
  sdsl::load_from_file(S, f_name);
}

void FourTopk_second_oned::test(std::ofstream& fout){
  std::cout << S.size() << std::endl;
  sdsl::store_to_file(S, "fileWrite.txt");
  rrr_vector<255> S2;
  sdsl::load_from_file(S2, "fileWrite.txt");
  std::cout << S2.size() << std::endl;

}
