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
		if     (i == 0) replace( sprite.begin(), sprite.end(), '0', ' ');
		else if(i == 1) replace( sprite.begin(), sprite.end(), '1', showPart ? '-' : ' ');
		else if(i == 2) replace( sprite.begin(), sprite.end(), '2', showPart ? '|' : ' ');
		else if(i == 3) replace( sprite.begin(), sprite.end(), '3', showPart ? '-' : ' ');
		else if(i == 4) replace( sprite.begin(), sprite.end(), '4', showPart ? '|' : ' ');
		else if(i == 5) replace( sprite.begin(), sprite.end(), '5', showPart ? 'o' : ' ');
		else if(i == 6) replace( sprite.begin(), sprite.end(), '6', showPart ? '|' : ' ');
		else if(i == 7) replace( sprite.begin(), sprite.end(), '7', showPart ? '/' : ' ');
		else if(i == 8) replace( sprite.begin(), sprite.end(), '8', showPart ? '\\' : ' ');
		else if(i == 9) replace( sprite.begin(), sprite.end(), '9', showPart ? '/' : ' ');
		else if(i == 10)replace( sprite.begin(), sprite.end(), 'a', showPart ? '\\' : ' ');
	}
	
	return sprite;
}

vector<char> hideWord(string word, vector<char> currentChars){
	vector<char> solved;
	for(unsigned int i = 0; i < word.length(); i++){
		bool trobat = false;
		for(unsigned int j = 0; j < currentChars.size() and not trobat; j++){
			if(word[i] == currentChars[j]) trobat = true;
		}
		if(trobat) solved.push_back(word[i]);
		else solved.push_back('_');
	}
	return solved;
}

bool hasChar(string word, char guess){
	bool trobat = false;
	
	for(unsigned int i = 0; i < word.length() and not trobat; i++)
		if(word[i] == guess) trobat = true;
	
	return trobat;
}

void drawUI(vector<char> hideWord, vector<char> currentChars, int errors, string message, int status){
	cout << "------------------------------------------------------------------------------" << endl;
	cout << "Paraula: " << endl;
	for(unsigned int i = 0; i < hideWord.size(); i++)
		cout << hideWord[i] << " ";
	cout << endl;
	cout << endl;
	cout << "LLetres utilitzades: " << endl;
	for(unsigned int i = 0; i < currentChars.size(); i++)
		cout << currentChars[i] << " ";
	cout << endl;
	cout << endl;
	cout << endl;
	cout << getPlayerSprite(errors) << endl;
	cout << endl;
	cout << "Errors: " << errors << endl;
	cout << endl;
	cout << endl;
	cout << endl;
	cout << message << endl;
	cout << endl;
	cout << "------------------------------------------------------------------------------" << endl;
	if(status == 0)
		cout << "Dona'm una lletra minúscula: ";
	else
		cout << "Vols tornar a jugar? (S/N): ";
}

int main() {
	string jugar = "s";
	while(jugar == "s" or jugar == "S"){
		string word = getRandomWord();
		vector<char> currentChars;
		vector<char> hiddenWord;
		int errors = 0;
		bool acabat = false;
		while(not acabat){
			hiddenWord = hideWord(word, currentChars);
			char actual;
			bool lletraValida = true;
			string missatge = "";
			do{
				drawUI(hiddenWord, currentChars, errors, missatge, 0);
				cin >> actual;
				
			}while(lletraValida);
			currentChars.push_back(actual);
			if(not hasChar(word, actual)) errors++;
			if(errors == 10) acabat = true;
			
			bool trobat = false;
			for(unsigned int i = 0; i < hiddenWord.size() and not trobat; i++)
				if(hiddenWord[i] == '_') trobat = true;
			if(not trobat) acabat = true;
		}
		
		drawUI(hiddenWord, currentChars, errors, "", 1);
		cin >> jugar;
	}
}
