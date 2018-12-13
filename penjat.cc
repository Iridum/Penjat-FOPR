#include <stdlib.h>
#include <time.h>

#include <fstream>

#include <iostream>
#include <string>
#include <vector>

using namespace std;

 
int random(int min, int max) {   
    srand(time(NULL));
    return rand() % (max - min) + min;
}

string get_random_word() {
	ifstream fin("paraules.txt");
	int x;
	fin >> x;
	int paraulaId = random(0, x);
	string s;
	string paraula;
	
	for (int i=0; i < x and paraula == ""; ++i) {
		fin >> s;
		if(i == paraulaId) paraula = s;
	}

	fin.close();
	
	return s;
}

int main() {
	while(true){
		
	}
}
