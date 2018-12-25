#include "FSTree.h"
#include "DirNode.h"
#include <iostream>
#include <string>
//cc#include <limits.h>
#include <stdlib.h>

using namespace std;
//print out the paths of every file in a given directory
void treeTraversal(string x);

//prints out the number of levels in a directory
int findLevels(DirNode * x);

//function declaration to print each level
void printByLevel(DirNode * node, string dir, int level);


int main(int argc, char *argv[]){
	if (argc != 2){
		cerr << "usage: treeTraversal directory" << endl;
		cerr << "where: directory is a valid directory" << endl;
		exit(1);
	}
	string directory = argv[1];
	treeTraversal(directory);

return 0;
}

//function returns the number of levels of the subdirectories
//it take in a pointer to a dirnode and returns an int that is the num of
//levels in the tree whose root is the directory 
int findLevels(DirNode * x){
	//if the node is null, return 0
	if(!x){
		return 0;
	}
	
	else{
		int numDir = x -> numSubDirs();
		
		if(numDir > 0 ){
			int max = findLevels(x -> getSubDir(0));
			for(int i = 1; i < numDir; i++){
				int height = findLevels(x -> getSubDir(i));
				if(max < height)
					max = height;
			}
			return max + 1;
		}
		else
			return 1;
	}

}
//this function takes in a pointer to a node, a directory as a string, and a 
//current level. it prints the elements at that level, and returns nothing
void printByLevel(DirNode * node, string dir, int level){
	//check that the node is not null, and if it is, return.
	if(node == NULL){
		return;
	}
	
	else if(level == 0){
		int numFiles = node -> numFiles();
		
		for(int i = 0; i < numFiles; i++){
			cout << dir + "/" + node -> getFile(i) << endl;
		}
	}
	
	else{
		int numDir = node -> numSubDirs();
		for(int i = 0; i < numDir; i++){
			DirNode *subDir = node -> getSubDir(i);
			printByLevel(subDir, dir+"/"+subDir->getName(), 
			level -1);
		}
	}
}



//prints all the files associated with a given directory. takes in a string
//that is a directory and returns nothing
void treeTraversal(string directory){
	FSTree myTree(directory);
	int height = findLevels(myTree.getRoot());
	for(int i = 0; i < height; i++){
		printByLevel(myTree.getRoot(), directory, i);
	}
		
}

