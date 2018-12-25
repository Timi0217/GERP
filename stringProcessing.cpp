#include <iostream>
#include <cstdlib>
#include <string>
#include "stringProcessing.h"

using namespace std;

string stripNonAlphaNum(string x){
	string stripped;
	int start, end = 0;
	for(size_t i = 0; i < x.length(); i++){
		if (isalnum(x[i])){
			start = i;
			break;
		}
	}
	
	for(size_t i = x.length(); i > 0; i--){
		if (isalnum(x[i])){
			end = i;
			break;
		}
	}
	
	for(int i = start; i < end + 1; i++){
		stripped += x[i];
	}
	
	return stripped;
}
