//PROJECT 2
//11/24/2016
//HashTable.cpp: implementation file
//includes the implementation of 
//member functions of class HashTable

#include "HashTable.h"
#include <functional>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <set>
#include <cctype>
using namespace std;

//if the load factor of hash table 
//exceeds this number, rehash
//NOTE: if you set the load factor
//too low, you will have to rehash
//often and it will run much slower
//and memory will run out faster
const double LOAD_FACTOR=0.7;

//default constructor take in a directory
//traverse that directory and index it
HashTable::HashTable(string directory)
{	
	//resize the table to value of member variable size
	filledNum=0;
	size=10000;
	table.resize(size);
	FSTree tree(directory);
	TraverseIndex(tree.getRoot(),directory);
}

//print out exact words from the indexed words
void HashTable::printSensitive(string word)
{
	//search hash table for word
	Word *query=SearchHashTable(word);
	//if query isnt NULL, go through set of indices
	//and print out all the sentences with such indices
	if (query)
	{
		set<size_t>::iterator p;
		for (p=query->pathIndex.begin();
		p!=query->pathIndex.end();p++)
			cout<<PathNameLineNumContent[*p]<<endl;
	}
	//if query is NULL, the table doesnt contain that word
	else
		cout<<word<<" Not Found. Try with @insensitive or @i.\n";	
}

//print out words that are similar except
//for cases from the indexed words
void HashTable::printInsensitive(string word)
{
	vector<string>permute;
	bool found=false;
	//find all permutations of word and
	//put them in the vector permute
	permutations(permute, word, 0);
	//go through each case-permutation and
	//search for that permutation in the indexed
	//hashtable. if found, then add the indices to the combination 
	//set to make sure there is no duplicate
	set<size_t>combination;
	for (unsigned int i=0;i<permute.size();i++)
	{
		//search hash table for word
		Word *query=SearchHashTable(permute[i]);
		//if query isnt NULL, go through set of indices
		//and put the indice into the set to make
		//sure there is no duplicate and set found to true
		if (query)
		{
			set<size_t>::iterator p;
			found=true;
			for (p=query->pathIndex.begin();
			p!=query->pathIndex.end();p++)
				combination.insert(*p);
		}
	}
	//if found is false, the table doesnt 
	//contain any permutation of that word
	if (!found)
	{
		for (unsigned int i=0;i<word.size();i++)
			word[i]=tolower(word[i]);
		cout<<word<<" Not Found."<<endl;
	}
	//if found is true, go through the combination set
	//and print out all paths and files names and numbers
	//using the indices in the combination set
	else
	{
		set<size_t>::iterator p;
		for (p=combination.begin();p!=combination.end();p++)
			cout<<PathNameLineNumContent[*p]<<endl;
	}
}

//increase the size of the hash table
//and rehash all the elements
void HashTable::re_hash()
{
	//if ratio of filledNum and size bigger than LOAD_FACTOR
	if (static_cast<double>(filledNum)/size>LOAD_FACTOR)
	{
		//create a new vector to temporarily hold all the elements
		vector<Slot>new_table;
		//put all slots in hashtable in new table
		for (int i=0;i<size;i++)
		{
			if (table[i].filled)
				new_table.push_back(table[i]);
		}
		//resize hashtable and set all filled slots to false
		size*=2;
		table.resize(size);
		for (unsigned int i=0;i<table.size();i++)
			table[i].filled=false;
		//add each element from temporary new_table back
		//to the old table and hash it to its proper place
		for (unsigned int i=0;i<new_table.size();i++)
			AddToTable(new_table[i].data);
	}
}

//add a word to the hash table
void HashTable::AddToTable(Word new_word)
{
	//check if the table needs to be rehashed
	re_hash();
	//hash index of the word
	int index=hash(new_word.word)%size;
	//if slot at index isnt filled, put new_word there and set it to filled
	if (!table[index].filled)
	{
		table[index].data=new_word;
		table[index].filled=true;
		//increase number of filled slots
		filledNum++;
	}
	//if slot at index is filled but by the same word
	else if ((table[index].filled)&&
		(table[index].data.word==new_word.word))
	{
		//go through all the path indices of new_word
		//and insert them into path indices set of word
		//occupying index slot
		set<size_t>::iterator p;
		for (p=new_word.pathIndex.begin();
		p!=new_word.pathIndex.end();p++)
			table[index].data.pathIndex.insert(*p);
	}
	else
	{
		//linear probing until find an empty slot
		//or a slot filled by the same word
		int new_index=(index+1)%size;
		while ((table[new_index].filled)&&
			(table[new_index].data.word!=new_word.word))
			new_index=(new_index+1)%size;
		//if slot at new_index is filled but by the same word
		if (table[new_index].filled)
		{
			//go through all the path indices of new_word
			//and insert them into path indices set of word
			//occupying new_index slot
			set<size_t>::iterator p;
			for (p=new_word.pathIndex.begin();
			p!=new_word.pathIndex.end();p++)
				table[new_index].data.pathIndex.insert(*p);
		}
		//if slot at new_index isnt filled,
		//put new_word there and set it to filled
		else
		{
			table[new_index].data=new_word;
			table[new_index].filled=true;
			//increase number of filled slots
			filledNum++;
		}	
	}
}

//search for a word in the hash table
//return a pointer to that word, NULL
//if cant find the word in the table
Word* HashTable::SearchHashTable(string word)
{
	Word *wrd=NULL;
	//get the hashed index of the string
	size_t index=hash(word)%size;
	//if the slot at hashed index doesnt
	//hold that word
	if ((table[index].filled)&&(table[index].data.word!=word))
	{
		size_t new_index=(index+1)%size;
		//linear probe until either finds the word
		//or get to an empty slot, which means that
		//the word is not in the table
		while (table[new_index].filled)
		{
			if (table[new_index].data.word==word)
			{
				wrd=&table[new_index].data;
				break;
				
			}
			else
				new_index=(new_index+1)%size;
		}
			
	}
	//if the slot at hashed index holds that word,
	//gives pointer the address of that word in the table
	else if ((table[index].filled)&&(table[index].data.word==word))
		wrd=&table[index].data;
	return wrd;	
}

//traverse the directory tree, open file and index words
void HashTable::TraverseIndex (DirNode *currNode, string directory)
{
	//if currNode is NULL return
	if (!currNode)
		return;
	else
	{
		int number_of_file=currNode->numFiles();
		//loop through every file in current directory
		//and read and index all those files
		for (int i=0;i<number_of_file;i++)
			readInFile(currNode->getFile(i),directory);
		int number_of_directories=currNode->numSubDirs();
		//recursively go through each subdirectory of
		//current directory and read in file
		for (int i=0;i<number_of_directories;i++)
		{
			DirNode *subDir=currNode->getSubDir(i);
			TraverseIndex(subDir,directory+"/"+subDir->getName());
		}
	}
}

//open file and index words in that file
void HashTable::readInFile(string FileName, string path)
{
	string line;
	ifstream fin;
	//open file, check if it fails to open file
	fin.open(path+"/"+FileName);
	if (fin.fail())
		cout<<"cant open "<<path+"/"+FileName<<endl;
	int lineCount(1);
	//take in lines while haven't reached end of file
	while (getline(fin,line))
	{
		//put the concatenation of path, file name line number
		//and line content into the vector
		PathNameLineNumContent.push_back
		(path+"/"+FileName+":"+to_string(lineCount)+": "+line);
		string word;
		istringstream read_word(line);
		//break string into words seperated by
		//whitespaces using istringstream and
		//then add that word to the hash table		
		while(read_word>>word)
		{
			Word new_word;
			new_word.word=word;
			//give the new_word set of pathIndex 
			//the index of the current line added
			//to the vector of lines
			new_word.pathIndex.insert
			(PathNameLineNumContent.size()-1);
			AddToTable(new_word);
		}
		lineCount++;
	}
	fin.close();
}

//put all cases-permutations of string into a vector
void permutations(vector<string>& lst, string a, unsigned int curr_pos)
{
	//if current position is the final letter of string
	//check if it is alphabet and if it is, change current 
	//letter to lower and uppercase and push back them
	//else just push back current string
	if (curr_pos==a.size()-1)
	{
		if (isalpha(a[curr_pos]))
		{
			a[curr_pos]=tolower(a[curr_pos]);
			lst.push_back(a);

			a[curr_pos]=toupper(a[curr_pos]);
			lst.push_back(a);
		}
		else
			lst.push_back(a);
	}
	//change current letter to lower and uppercase if
	//current letter is an alphabet character and 
	//recursively call them again with next position
	//else just recursively call without changing string
	else
	{
		if (isalpha(a[curr_pos]))
		{
			a[curr_pos]=tolower(a[curr_pos]);
			permutations(lst,a,curr_pos+1);
			a[curr_pos]=toupper(a[curr_pos]);
			permutations(lst,a,curr_pos+1);
		}
		else
			permutations(lst,a,curr_pos+1);
	}
	
}

