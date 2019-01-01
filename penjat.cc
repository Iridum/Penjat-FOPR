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
	// Obrir stream de lectura
	ifstream fin("paraules.txt");
	int x;
	fin >> x;
	// Obtenir nombre aleatori
	int paraulaId = random(0, x);
	string s;
	string paraula;

	// Busca la paraula seleccionada segons el nombre aleatori
	for (int i = 0; i < x and paraula == ""; ++i) {
		fin >> s;
		if (i == paraulaId) paraula = s;
	}

	// Tancar stream de lectura
	fin.close();

	return paraula;
}

// Crea un dibuix del nino segons el nombre d'errors
string getPlayerSprite(int errors) {
	string sprite = "0033333330\n0020000040\n0020000050\n0020000768\n0020000060\n002000090a\n1111100000"; // Dibuix del nino

	// Amaga parts del nino segons el nombre d'errors
	replace(sprite.begin(), sprite.end(), '0', ' ');
	replace(sprite.begin(), sprite.end(), '1', errors > 0 ? '-' : ' '); // Base
	replace(sprite.begin(), sprite.end(), '2', errors > 1 ? '|' : ' '); // Pal
	replace(sprite.begin(), sprite.end(), '3', errors > 2 ? '-' : ' '); // Part superior
	replace(sprite.begin(), sprite.end(), '4', errors > 3 ? '|' : ' '); // Corda
	replace(sprite.begin(), sprite.end(), '5', errors > 4 ? 'o' : ' '); // Cap
	replace(sprite.begin(), sprite.end(), '6', errors > 5 ? '|' : ' '); // Cos
	replace(sprite.begin(), sprite.end(), '7', errors > 6 ? '/' : ' '); // Brac L
	replace(sprite.begin(), sprite.end(), '8', errors > 7 ? '\\' : ' '); // Brac R
	replace(sprite.begin(), sprite.end(), '9', errors > 8 ? '/' : ' '); // Cama L
	replace(sprite.begin(), sprite.end(), 'a', errors > 9 ? '\\' : ' '); // Cama R

	return sprite;
}

// Oculta la paraula canviant les lletres per '_' si l'usuari no ha trobat la lletra
vector<char> hideWord(string word, vector<char> currentChars) {
	vector<char> solved;
	for (unsigned int i = 0; i < word.length(); i++) {
		bool trobat = false;
		for (unsigned int j = 0; j < currentChars.size() and not trobat; j++) {
			if (word[i] == currentChars[j]) trobat = true;
		}
		if (trobat) solved.push_back(word[i]);
		else solved.push_back('_');
	}
	return solved;
}

// Torna TRUE si la paraula conté la lletra, si no, torna FALSE
bool hasChar(string word, char guess) {
	bool trobat = false;

	for (unsigned int i = 0; i < word.length() and not trobat; i++)
		if (word[i] == guess)
			trobat = true;

	return trobat;
}

// Dibuixa la interficie del joc i la mostra per pantalla
// status=0 - Jugant | status=1 - Continuar jugant?
void drawUI(vector<char> hideWord, vector<char> currentChars, int errors, string message, int status) {
	cout << "------------------------------------------------------------------------------" << endl;
	cout << "Paraula: " << endl;
	for (unsigned int i = 0; i < hideWord.size(); i++)
		cout << hideWord[i] << " ";
	cout << endl;
	cout << endl;
	cout << "LLetres utilitzades: " << endl;
	for (unsigned int i = 0; i < currentChars.size(); i++)
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
	if (status == 0) cout << "Dona'm una lletra minuscula: ";
	else cout << "Vols tornar a jugar? (S/N): ";
}

// Logica del joc i inici.
int main() {
	string jugar = "!";
	int v = 0;
	int d = 0;

	// Do while es la millor estructura perque sempre s'ha de jugar com a minim una vegada
	do {
		string word = getRandomWord();
		vector<char> currentChars;
		vector<char> hiddenWord;
		int errors = 0;
		bool acabat = false;
		string missatge = "";
		// Joc
		while (not acabat) {
			hiddenWord = hideWord(word, currentChars);
			char actual;
			bool lletraValida = true;
			// Demanar lletra fins que sigui valida
			do {
				drawUI(hiddenWord, currentChars, errors, missatge, 0);
				cin >> actual;
				// Filtrar chars
				if (actual > 96 and actual < 123) lletraValida = true;
				else {
					missatge = "El caracter introduit no es valid.";
					lletraValida = false;
				}
				// Buscar si s'ha fet servir
				for (unsigned int i = 0; i < currentChars.size() and lletraValida; i++) {
					if (currentChars[i] == actual) {
						lletraValida = false;
						missatge = "Ja has fet servir aquesta lletra.";
					}
				}
			} while (not lletraValida);
			// Afegir el char a la llista d'utilitzats
			currentChars.push_back(actual);
			// Buscar la lletra en la paraula
			if (not hasChar(word, actual)) {
				missatge = "La lletra no esta :(";
				errors++;
			} else missatge = "";
			// Acabar si s'han fet 10 errors
			if (errors == 10) {
				acabat = true;
				d++;
			}
			// Actualiztar la paraula amb la nova lletra introduida
			bool trobat = false;
			hiddenWord = hideWord(word, currentChars);
			for (unsigned int i = 0; i < hiddenWord.size() and not trobat; i++)
				if (hiddenWord[i] == '_') trobat = true;
			// Si ja no queden _ vol dir que l'usuari ha encertat la paraula
			if (not trobat) {
				acabat = true;
				v++;
			}
		}
		// Fi del joc
		// Convertir int a string per a poder enviar el missatge
		ostringstream vss;
		vss << v;
		string vs = vss.str();

		ostringstream dss;
		dss << d;
		string ds = dss.str();
		// Mostrar si has guanyat o perdut i recompte de victories/derrotes
		// c++ no deixa sumar 2 literals. Perque? Ningú ho sap.
		missatge = (
			errors == 10 ?
				"Has perdut, prova de nou!! Victories " :
				"Has guanyat, felicitats!!! Victories "
			) + vs + " | Derrotes: " + ds;
		// Tornar a jugar????
		drawUI(hiddenWord, currentChars, errors, missatge, 1);
		cin >> jugar;
		// jugar=s - tornar a jugar | jugar=altre char - sortir
	} while (jugar == "s" or jugar == "S");
}
