#include <iostream>
#include <string>
#include <cstdio>
using namespace std;

int numJewelsInStones(string J, string S) {
        int count = 0;
	    for (unsigned int i = 0; i < J.length(); i++) {
		    for (unsigned int j = 0; j < S.length(); j++) {
			    if (J[i] == S[j]) {
			    	count++;
			    }
		    }
	    }
	    return count;
    }



int main(int argc, char const *argv[]) {
	std::string hello="hello word!!";
	std::cout<<hello<<std::endl;
	std::string input_j="z";
	std::string input_s="ZZ";
	int test_reslut=numJewelsInStones(input_j,input_s);
	std::cout<<test_reslut<<std::endl;
	return 0;
}