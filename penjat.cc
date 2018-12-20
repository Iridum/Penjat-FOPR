#include <stdlib.h>
#include <time.h>

#include <fstream>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <algorithm>

using namespace std;

////////////////////////////////
//          BALENA            //
////////////////////////////////
/*******************************
 *                             *
 *         .                   *
 *        ":"                  *
 *      ___:____     |"\/"|    *
 *    ,'        `.    \  /     *
 *    |  O        \___/  |     *
 *  ~^~^~^~^~^~^~^~^~^~^~^~^~  *
 *                             *
 *******************************/
 
 
// Funció que retorna un nombre aleatori R tal que: min =< R =< max
int random(int min, int max) {   
    srand(time(NULL));
    return rand() % (max - min) + min;
}

// Dona una paraula aleatoria del arxiu paraules.txt
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

// Crea un dibuix del nino segons el nombre d'errors
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

// Oculta la paraula canviant les lletres per '_' si l'usuari no ha trobat la lletra
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

// Torna TRUE si la paraula conté la lletra, si no, torna FALSE
bool hasChar(string word, char guess){
	bool trobat = false;
	
	for(unsigned int i = 0; i < word.length() and not trobat; i++)
		if(word[i] == guess) trobat = true;
	
	return trobat;
}

// Dibuixa la interficie del joc
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
	if(status == 0) cout << "Dona'm una lletra minúscula: ";
	else cout << "Vols tornar a jugar? (S/N): ";
}

// Logica del joc i inici.
int main() {
	string jugar = "!";
	int v = 0;
	int d = 0;
	do {
		string word = getRandomWord();
		vector<char> currentChars;
		vector<char> hiddenWord;
		int errors = 0;
		bool acabat = false;
		string missatge = "";
		
		while(not acabat){
			hiddenWord = hideWord(word, currentChars);
			char actual;
			bool lletraValida = true;
			do{
				drawUI(hiddenWord, currentChars, errors, missatge, 0);
				if(cin >> actual) {} else acabat = true; // Per alguna misteriosa rao el if not no funciona.
				
				if(actual > 96 and actual < 123) lletraValida = true;
				else{
					missatge = "El caracter introduit no es valid.";
					lletraValida = false;
				}
				
				for(unsigned int i = 0; i < currentChars.size() and lletraValida; i++){
					if(currentChars[i] == actual){
						lletraValida = false;
						missatge = "Ja has fet servir aquesta lletra.";
					}
				}
			}while(not lletraValida);
			currentChars.push_back(actual);
			if(not hasChar(word, actual)){
				missatge = "La lletra no esta";
				errors++;
			}else missatge = "";
			if(errors == 10){
				 acabat = true;
				 d++;
			}
			bool trobat = false;
			hiddenWord = hideWord(word, currentChars);
			for(unsigned int i = 0; i < hiddenWord.size() and not trobat; i++)
				if(hiddenWord[i] == '_') trobat = true;
			if(not trobat){
				acabat = true;
				v++;
			}
		}
		ostringstream vss;
		vss << v; 
		string vs = vss.str();
		
		ostringstream dss;
		dss << d;
		string ds = dss.str();
		// c++ es tan estupid que no deixa sumar 2 literals. Perque? Ningú ho sap
		missatge = (errors == 10 ? "Ets dolentissim desinstala el joc buuuuh. Victories " : "Bestia champion ets un maquina!!!!!!!!!!!!!!!!!! Victories ")+ vs + " | Derrotes: " + ds;
		drawUI(hiddenWord, currentChars, errors, missatge, 1);
		if(cin >> jugar) {} else jugar = "!";  // Per alguna misteriosa rao el if not no funciona.
	}while(jugar == "s" or jugar == "S");
}
