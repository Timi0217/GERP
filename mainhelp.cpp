//PROJECT 2
//main.cpp: main program receive command line arguments
//to index that directory and prepare for query

#include "HashTable.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;

//run wrapper function do appropriate actions based on commands from user
void run(string directory);

int main(int argc, char *argv[])
{
	//if number of arguments is wrong, cerr this message and end program
	if (argc!=2)
	{
		cerr<<"Usage: gerp directory\n";
		cerr<<"            where: directory is a valid directory\n";
		exit(1);
	}
	string directory=string(argv[1]);
	run(directory);
	return 0;
}

//run wrapper function do appropriate actions based on commands from user
void run(string directory)
{
	//create an object of type HashTable with directory
	HashTable table(directory);
	string command;
	cout<<"Query? ";
	while (cin>>command)
	{
		//exit the program if command is @q or @quit
		if ((command=="@q")||(command=="@quit"))
			break;
		//if command is @i or @insensitve, print out all words 
		//in indexed directory with same ordered characters
		else if ((command=="@i")||(command=="@insensitive"))
		{
			cin>>command;
			table.printInsensitive(command);			
		}
		//print out exact words in indexed directory
		else
			table.printSensitive(command);
		cout<<"Query? ";
	};
	cout<<endl;
	cout<<"Goodbye! Thank you and have a nice day.\n";
}
