//Oluwatimilehin Dayo-Kayode
//PROJECT 2
//04/21/2017
//main.cpp: main program receive command line arguments
//to index that directory and prepare for query

#include "database.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;

//umbrella function that runs the gerp program as a whole. It takes in a file
//name in the form of a string that is the directory wants to search for a 
//word in. It returns nothing
void run(string directory);

int main(int argc, char *argv[]){
	//checks that the user put in the right number of arguments and exits
	//the program after printing an error message.
	if (argc != 2){
		cerr << "Usage: gerp directory" << endl;
		cerr <<"             where: directory is a valid directory";
		cerr << endl;
		exit(1);
	}
	string directory = string(argv[1]);
	run(directory);
	return 0;
}


//function definition of umbrella function that runs the whole program based
//on the users inputs.
void run(string directory){
	//creates an object of type dataBase with directory as a parameter
	dataBase table(directory);
	string input;
	cout<< "Query? ";
	while (cin >> input){
		//exit the program if command is @q or @quit
		if ((input == "@q")||(input == "@quit"))
			break;
		//if the input is @i or @insensitve, print out all words 
		//in indexed directory with same ordered characters
		else if ((input == "@i")||(input == "@insensitive"))
		{
			cin >> input;
			table.insensitivePrint(input);			
		}
		//print out exact words in indexed directory
		else
			table.sensitivePrint(input);
		cout << "Query? ";
	};
	cout << endl;
	cout<< "Goodbye! Thank you and have a nice day." << endl;
}
