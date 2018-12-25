//PROJECT 2
//11/24/2016
//HashTable.h: header file includes the
//definition of class HashTable and struct
//Word and Slot

#ifndef HASHTABLE_H
#define HASHTABLE_H
#include "DirNode.h"
#include "FSTree.h"
#include <string>
#include <functional>
#include <fstream>
#include <set>
#include <vector>
using namespace std;

//a struct Word is made up of the word itself
//and a set of integer which stores the index
//of the line in which the word appears
struct Word
{
	string word;
	set<size_t>pathIndex;
};

//a struct Slot is made up of a Word and 
//a bool to check if that slot is filled
struct Slot
{
	Word data;
	bool filled=false;
};

//class HashTable is made up of a vector 
//of Slots, two int size which is the size
//of vector and number of filled slots and
//a vector of all lines with its number and
//file name and path
class HashTable
{
public:
	//default constructor take in a directory
	//and index all words in that directory
	HashTable(string directory);

	//print out exact words from the indexed words
	void printSensitive(string word);

	//print out words that are similar except
	//for cases from the indexed words
	void printInsensitive(string word);
private:
	//private helper functions
	
	//increase the size of the hash table
	//and rehash all the elements
	void re_hash();

	//add a word to the hash table
	void AddToTable(Word new_word);
	
	//search for a word in the hash table
	//return a pointer to that word, NULL
	//if cant find the word in the table
	Word* SearchHashTable(string word);
	
	//traverse the directory tree, open file and index words
	void TraverseIndex (DirNode *currNode, string directory);
	
	//open file and index words in that file
	void readInFile(string FileName, string path);
	
	//hash function
	std::hash<string> hash;

	//private member variables
	int size;
	int filledNum;
	vector<Slot>table;
	vector<string> PathNameLineNumContent;		
};

//put all cases-permutations of string into a vector
void permutations(vector<string>& lst, string a, unsigned curr_pos);


#endif //HASHTABLE_H
