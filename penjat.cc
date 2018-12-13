#include <stdlib.h>
#include <time.h>

#include <fstream>

#include <iostream>
#include <string>
#include <vector>

#include <algorithm>

using namespace std;




int random(int min, int max) {   
    srand(time(NULL));
    return rand() % (max - min) + min;
}

string getRandomWord() {
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
	
	return paraula;
}

string getPlayerSprite(int errors){
	string sprite = "0033333330\n0020000040\n0020000050\n0020000768\n0020000060\n002000090a\n1111100000";
	for(int i = 0; i < 11; i++){
		bool showPart = i <= errors;
		switch(i){
			case 0:
				replace( sprite.begin(), sprite.end(), '0', ' ');
				break;
			case 1:
				replace( sprite.begin(), sprite.end(), '1', showPart ? '-' : ' ');
				break;
			case 2:
				replace( sprite.begin(), sprite.end(), '2', showPart ? '|' : ' ');
				break;
			case 3:
				replace( sprite.begin(), sprite.end(), '3', showPart ? '-' : ' ');
				break;
			case 4:
				replace( sprite.begin(), sprite.end(), '4', showPart ? '|' : ' ');
				break;
			case 5:
				replace( sprite.begin(), sprite.end(), '5', showPart ? 'o' : ' ');
				break;
			case 6:
				replace( sprite.begin(), sprite.end(), '6', showPart ? '|' : ' ');
				break;
			case 7:
				replace( sprite.begin(), sprite.end(), '7', showPart ? '/' : ' ');
				break;
			case 8:
				replace( sprite.begin(), sprite.end(), '8', showPart ? '\\' : ' ');
				break;
			case 9:
				replace( sprite.begin(), sprite.end(), '9', showPart ? '/' : ' ');
				break;
			case 10:
				replace( sprite.begin(), sprite.end(), 'a', showPart ? '\\' : ' ');
				break;
		}
	}
	
	return sprite;
}

int main() {
}
