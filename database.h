/*Oluwatimilehin Dayo-Kayode
 *Project 2
 *4/14/2017
 *database.h
 *contains definition of the database class and the word and emptyOrNah structs
*/

#ifndef DATABASE_H
#define DATABASE_H
#include "DirNode.h"
#include "FSTree.h"
#include <string>
#include <functional>
#include <fstream>
#include <set>
#include <vector>
using namespace std;

//struct that contains the word saved and a set of ints that saves 
//the line it appears in
struct word{
	string newWord;
	set <size_t> locationIndex;
};

//this struct contains an instance of the word struct and a bool that saves
//whether or not that slot is filled
struct emptyOrNah{
	word data;
	bool full = false;
};

class dataBase{

public:
	//dataBase constructor that takes in a directory and indexes all the 
	//words in that directory
	dataBase(string directory);
	
	//prints out the exact words that are indexed
	void sensitivePrint(string x);
	
	//prints out the index words without taking into consideration their
	//cases.
	void insensitivePrint(string x);
	
private:
	//expands the size of the hash table and then rehashes all the elements
	//in it.
	void expandAndRehash();
	
	//adds a new word(an instance of struct word) to the hash table 
	void addToTable(word newWord);
	
	//searches the hashtable for a word and returns a pointer to the word.
	//if not found, the pointer points to null
	word * search(string toFind);
	
	//traverses the directory tree, opens a file, and indexes words.
	void traverseIndex(DirNode *currNode, string x);
	
	//opens a file and indexes the words in that file
	void readInData(string fileName, string path);
	
	//the hash function
	std::hash <string> hash;
	
	//put all cases-permutations of string into a vector
	void permute(vector <string> &lst, string x, unsigned currPosition);
	
	//private data members
	int size;
	int numFilled;
	vector <emptyOrNah> table;
	vector <string> pathNameLineNumContent;	
	
	

};

//puts all case-permutations of a string into a vector
void casePermutations(vector<string>& x, string y, unsigned z);

#endif

