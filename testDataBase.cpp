/*
 *Oluwatimilehin.cpp
 *Comp 15
 *proj2part2
 *testdatabase.cpp
 *
 */

/*
 *I use the functions in this file to test my hashTable
 *
 */

#include "database.h"
#include <iostream>
#include <string>
#include <functional>
using namespace std;

void testAdd(dataBase &test);
void testAddOverload(dataBase &test);

int main()
{	
	dataBase test;
	testAdd(test);
	test.print();
	testAddOverload(test);
	test.print();
	return 0;
}



void testAdd(dataBase &test)
{
	for(int i = 0; i < 14; i++)
	{
		word structTest;
		structTest.newWord = "Timi";
		test.addToTable(structTest);
	}	
}

//fuction to test expandandrehash
void testAddOverload(dataBase &test){
	word structTest;
	string testWords[55] = {"timi", "boo", "fart", "poop", "comp40?", 
	"scared", "excited", "so", "many", "emotions", "aghhhhh", "whaaaa",
	"ayo ma", "slide in them dm's", "this", "song", "is", "lit", "af", 
	"bruh", "yeah yeah yeah", "yoooo", "saaasve", "meeeee", "I ", "just"
	"wanna", "graduate", "and", "its", "only", "been", "1.5", "semesters",
	"I ", "hope", "I", "pass", "these", "tests", "I", "really", "need", 
	"a", "good", "grade", "on", "this", "ya", "feel?"
	for(int i = 0; i < 50; i++){
		structTest.newWord = testWords[i];
		test.addToTable(structTest);
	}
}

//print out contents of hashtable
void print(test){
	int size = test.size();
	for(int i = 0; i < size; i++){
		cout << test[i].data;
	}
}
