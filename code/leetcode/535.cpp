#include <iostream>
#include <string>
#include <cstdio>
using namespace std;

typedef unsigned char byte;
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


    // Encodes a URL to a shortened URL.
    //16位编解码
 byte toHex(const byte &x){
 	return x>9?x-10+'A':x+'0';
 }
 byte fromHex(const byte &y){
 	return isdigit(y)?y-'0':y-'A';
 }
string encode(string longUrl) {
        return longUrl;
    }

    // Decodes a shortened URL to its original URL.
string decode(string shortUrl) {
        return shortUrl+"sss";
    }

int main(int argc, char const *argv[]) {
	std::string encodec_result_string="https://leetcode.com/problems/design-tinyurl";
	std::cout<<encodec_result_string<<std::endl;
	std::string decodec_result_string="http://tinyurl.com/4e9iAk";
	std::cout<< decodec_result_string<<std::endl;
	// std::string input_j="z";
	// std::string input_s="ZZ";
	//if(encode(encodec_result_string)==);

	std::cout<<toHex(fromHex(decodec_result_string[1]));
	//std::cout<<test_reslut<<std::endl;
	return 0;
}