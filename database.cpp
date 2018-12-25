#include "database.h"
#include <functional>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <set>
#include <cctype>
using namespace std;

//this load factor determines when to rehash the table. It is .7 because if 
//it is too low, then the hash table will rehash too often which will make the
//program run slower, but if it is too high, then the hash table will get too
//full and searches will take longer.
const double LOAD_FACTOR=0.7;

//the default constructor takes in a directory and indexes it
dataBase::dataBase(string directory){
	numFilled = 0;
	size = 10000;
	table.resize(size);
	FSTree tree(directory);
	traverseIndex(tree.getRoot(), directory);
}

//this function traverses the directory tree and opens the files and 
//indexes words. it loops through every file in the current directory, reads
//and indexes all its files, then recursively goes through each subdirectory of
//the current node and does the same as above. It takes in a pointer to an
//instance of a DirNode, that serves as the root,and a string that is the path
void dataBase::traverseIndex(DirNode *currNode, string directory){
	if(!currNode)
		return;
	else{
		int numOfFiles = currNode -> numFiles();
		for(int i = 0; i < numOfFiles; i++){
			readInData(currNode -> getFile(i), directory);
		}
		int numOfDirs = currNode -> numSubDirs();
		for(int i = 0; i < numOfDirs; i++){
			DirNode *subDirs = currNode -> getSubDir(i);
			traverseIndex(subDirs, directory+"/"+subDirs -> 
			getName());
		}
	}
}

//this function prints out words that are exactly identical(case included) to 
//the indexed word. It uses the search function to search for the word and if
//it is found, goes through the set of indices and prints every sentence with
//such indices. else it prompts the user
//to try searching for the word using the insensitive print. It takes in a 
//string to be searched for and returns nothing
void dataBase::sensitivePrint(string input){
	word *query = search(input);	
	
	if(query){
		set <size_t>::iterator x;
		for (x = query->locationIndex.begin();
		x != query->locationIndex.end(); x++)
			cout << pathNameLineNumContent[*x] << endl;
	}
	else
		cout << input << " Not Found. Try with @insensitive or @i";
		cout << endl; 
		
}

//this function prints out instances of a word that are similar to its parameter
//regardless of case. It does this by using the casePermute function to 
//create a vector containing all case permutations of the word, then searches
//for each permutation. If one of the cases is found, it goes through the 
//duplicateCheck set and prints out all the paths and filenames, and numbers
//using the indices  in the duplicate check set and sets the bool found to true.
//If no instance is found, it 
//prints out a message to let the client know and leaves found as false
//It takes in a string whose instances are to be searched for, 
//and returns nothing
void dataBase::insensitivePrint(string input){
	//bool is false
	bool found = false;
	vector <string> casePermute;
	casePermutations(casePermute, input, 0);
	set <size_t> duplicateCheck;
	for(unsigned int i = 0; i < casePermute.size(); i++){
		word *query = search(casePermute[i]);
		if(query){
			set <size_t>::iterator x;
			//found should be true
			found = true;
			for(x = query->locationIndex.begin();
			x != query->locationIndex.end(); x++)
				duplicateCheck.insert(*x);	
		}
	}
	if(!found){
		for(unsigned int i = 0; i < input.size(); i++)
			input[i] = tolower(input[i]);
		cout << input << " Not Found" << endl;
	}	
	
	else{
		set <size_t>::iterator x;
		for(x = duplicateCheck.begin(); x!= duplicateCheck.end(); x++)
			cout << pathNameLineNumContent[*x] << endl;
	}
}

//this function checks if a hash table needs to be expanded by comparing how
//filled up it is to the load factor and if the load factor has been exceeded,it
//creates a new vector to temporarily hold all the elements, resizes the 
//hashtable, and then rehashes all the elements back into the hashtable
//It takes in no parameters and returns nothing
void dataBase::expandAndRehash(){
	if (static_cast <double> (numFilled)/size > LOAD_FACTOR){
		vector <emptyOrNah> newTable;
		for(int i = 0; i < size; i++){
			if(table[i].full)
				newTable.push_back(table[i]);
		}
		
		size = (size * 2);
		table.resize(size);
		for(unsigned int i = 0; i < table.size(); i++)
			table[i].full = false;
		for(unsigned int i = 0; i < newTable.size(); i++)
			addToTable(newTable[i].data);
	}	
}

//this function adds a word to the hashtable. It checks if the table needs
//yto be rehashed and if the index the word is to be added isn't filled. If that
//index is filled, it uses linear probing to find a spot to add the word to.
//if it finds that a slot filled contains a duplicate of the word to be 
//added, it goes through all the path indices of the word and inserts them 
//into the path indices set of the word occupying the slot. The function takes
//in an instance of the struct word to be added to the hashtable and returns
//nothing
void dataBase::addToTable(word addWord){
	expandAndRehash();
	int index = hash(addWord.newWord)%size;
	if(!table[index].full){
		table[index].data = addWord;
		table[index].full = true;
		numFilled++;
	}
	else if((table[index].full) && (table[index].data.newWord ==
		addWord.newWord)){
		set <size_t>::iterator x;
		for(x = addWord.locationIndex.begin();
		x != addWord.locationIndex.end(); x++)
			table[index].data.locationIndex.insert(*x);
	}
	else{
		int newIndex = (index + 1) % size;
		while((table[newIndex].full) && 
	     (table[newIndex].data.newWord != addWord.newWord))
		     newIndex = (newIndex + 1) % size;
		if(table[newIndex].full){
			set <size_t>::iterator x;
			for(x = addWord.locationIndex.begin();
			    x != addWord.locationIndex.end(); x++)
				table[newIndex].data.locationIndex.insert(*x);
		}
		
		else{
			table[newIndex].data = addWord;
			table[newIndex].full = true;
			numFilled++;
		}
	}
}

//this function searches for a word in a hashtable and returns a pointer to
//the word if it is found. In the case that it isn't found, the pointer is
//set to NULL. It takes in a string to be searched for, and returns a
//pointer of type word
word * dataBase::search(string toFind){
	word *temp = NULL;
	size_t index = hash(toFind) % size;
	if((table[index].full) && (table[index].data.newWord != toFind)){
		size_t newIndex = (index + 1) % size;
		while(table[newIndex].full){
			if(table[newIndex].data.newWord == toFind){
				temp = &table[newIndex].data;
				break;
			}
			else{
				newIndex = (newIndex + 1) % size;
			}
		}	
	}
	else if((table[index].full) && (table[index].data.newWord == toFind))
		temp = &table[index].data;
	return temp;
}

//this function puts all case permutations of a string into a vector by using
//the tolower and toupper functions to create different case permutations of
//the string and push it to the vector. It takes in a vector to save the 
//permutations, a string that is the word to be permuted, and an unsigned int
//to keep track of what letter in the string is being converted to upper or
//lower case. It returns nothing
void casePermutations(vector <string> &x, string y, unsigned int currPos){
	if(currPos == y.size() - 1){
		if(isalpha(y[currPos])){
			y[currPos] = tolower(y[currPos]);
			x.push_back(y);
			y[currPos] = toupper(y[currPos]);
			x.push_back(y);
		}
		else
			x.push_back(y);
	}
	else{
		if (isalpha(y[currPos]))
		{
			y[currPos] = tolower(y[currPos]);
			casePermutations(x,y,currPos + 1);
			y[currPos] = toupper(y[currPos]);
			casePermutations(x,y,currPos + 1);
		}
		else
			casePermutations(x,y,currPos + 1);
	}
}

//this function opens a file and indexes the words in the file. it also 
//concatenates the path, file name, line number, and line content into a 
//vector. It takes in a string that is the filename and a string that is 
//the path of the file. It returns nothing
void dataBase::readInData(string fileName, string path){
	string line;
	ifstream inputStream;
	inputStream.open(path+ "/" + fileName);
	if(inputStream.fail())
		cout << "Failed to open " << path + "/" + fileName << endl;
	int lineCount(1);
	while(getline(inputStream, line)){
		pathNameLineNumContent.push_back(path + "/" + fileName +
		":" + to_string(lineCount) + ": " + line);
		string save;
		istringstream read_word(line);
		//break up strings into words separated by whithespace into
		//words and then adds that word to the hash table
		while(read_word >> save){
			word secondNewWord;
			secondNewWord.newWord = save;
			secondNewWord.locationIndex.insert
			(pathNameLineNumContent.size() - 1);
			addToTable(secondNewWord);
		}
		lineCount++;
	}
	inputStream.close();
}





















