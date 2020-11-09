/*

George Babã, Cocan Alexandru

Format de intrare:
- Simbol de start
- Neterminale
- Terminale
- Productii

*/

#include <iostream>
#include <stdio.h>
#include <string>
#include <stack>
#include <time.h>
#include <vector>
#include <fstream>

using namespace std;

#define MAXCHARS 20

struct P {
	string from;
	string to;
};

struct Actiune {
	string index;
	string with;
	string to;
};

struct Salt {
	string index;
	string with;
	string to;
};

// Split String by Delimiter
vector<string> split(string s, string delimiter) {
	int pos_start = 0, pos_end, delim_len = delimiter.length();
	string token;
	vector<string> res;

	while ((pos_end = s.find(delimiter, pos_start)) != string::npos) {
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}

	res.push_back(s.substr(pos_start));
	return res;
}

int main() {
	srand(time(NULL));

	/* Vars */
	int random = 0, lineNrG = 0, productiiIterator = 1;
	string lambda = "#", keyChar, from, valuesWithDelimiter, valuesDelimiter = ",";
	string startSymbol;
	string neterminale;
	string terminale;
	P productii[10];

	string stiva = "$0", cuvIntrare, index, with, to, cuvIntrareAction, productieNr;
	int lineNrAPD = 0;
	Actiune actiuni[50];
	Salt salturi[50];

	/* Input Gramatica */
	cout << "---------------------- Input Gramatica -----------------------" << endl;
	ifstream inputG("inputG.txt");
	for (string line; getline(inputG, line);) {
		if (lineNrG == 0) {
			startSymbol = line;
			cout << "Simbol de start: " << startSymbol << endl;
		} else if (lineNrG == 1) {
			neterminale = line;
			cout << "Neterminale: " << neterminale << endl;
		} else if (lineNrG == 2) {
			terminale = line;
			cout << "Terminale: " << terminale << endl;
			cout << "Productii: " << endl;
		} else {
			// Set From
			from = line.substr(0, line.find(" "));
			productii[productiiIterator].from = from;
			cout << "\t" << from << ": ";

			// Set To
			productii[productiiIterator].to = line.substr(line.find(" ") + 1, line.size());
			cout << productii[productiiIterator].to << endl;

			productiiIterator++;
		}

		lineNrG++;
	}

	/* Input APD */
	cout << "---------------------- Input APD -----------------------" << endl;
	ifstream inputAPD("inputAPD.txt");
	for (string line; getline(inputAPD, line);) {
		if (lineNrAPD == 0) {
			cuvIntrare = line;
		} else if(lineNrAPD >= 1 && lineNrAPD <= 36) {
			index = line.substr(0, line.find(","));
			actiuni[lineNrAPD].index = index;
			cout << actiuni[lineNrAPD].index << ",";

			with = line.substr(line.find(",") + 1, 1);
			actiuni[lineNrAPD].with = with;
			cout << actiuni[lineNrAPD].with << ":";

			to = line.substr(line.find(":") + 1, line.size());
			actiuni[lineNrAPD].to = to;
			cout << actiuni[lineNrAPD].to << endl;
		} else {
			index = line.substr(0, line.find(","));
			salturi[lineNrAPD].index = index;

			with = line.substr(line.find(",") + 1, 1);
			salturi[lineNrAPD].with = with;

			to = line.substr(line.find(":") + 1, line.size());
			salturi[lineNrAPD].to = to;
		}

		lineNrAPD++;
	}

	/* Procesare Gramatica */
	cout << "---------------------- Procesare Gramatica -----------------------" << endl;
	while ((startSymbol.length() < MAXCHARS) && (startSymbol.find_first_of(neterminale) != string::npos)) {
		int pos = startSymbol.find_first_of(neterminale);
		keyChar = startSymbol.substr(pos, 1);

		for (int i = 0; i < sizeof(productii) / sizeof(productii[0]); i++) {
			if (productii[i].from == keyChar) {
				if (productii[i].to != lambda) {
					startSymbol.replace(pos, 1, productii[i].to);
				} else {
					startSymbol.erase(pos, 1);
				}
				cout << startSymbol << endl;
			}
		}
	}

	/* Procesare APD */
	cout << "---------------------- Procesare APD -----------------------" << endl;
	for (int i = 1; i <= 36; i++) {
		cuvIntrareAction = cuvIntrare.substr(0, 1); // q
		if (cuvIntrareAction.length() != 0) {
			if ((actiuni[i].index == stiva.substr(stiva.size() - actiuni[i].index.length(), actiuni[i].index.length())) && (actiuni[i].with == cuvIntrareAction)) {
				if (actiuni[i].to.size() <= 3) {
					productieNr = actiuni[i].to.substr(1, actiuni[i].to.size() - 1); // 5
					printf("S: %-20s     CI: %-8s     A: %s", stiva.c_str(), cuvIntrare.c_str(), actiuni[i].to.c_str());

					if (actiuni[i].to.substr(0, 1) == "d") {
						stiva.append(cuvIntrareAction); // $0q
						cuvIntrare.erase(0, 1);
						stiva.append(productieNr); // $0q5

						i = 1;
					} else if (actiuni[i].to.substr(0, 1) == "r") {
						int pos = (stiva.size() - (2 * productii[stoi(productieNr)].to.length()) - (actiuni[i].index.length() - 1));
						string stringAtPos = productii[stoi(productieNr)].from; // F
						stiva.replace(pos, actiuni[i].index.length(), stringAtPos); // $0F5
						stiva.erase(pos + 1); // $0F
						string index = stiva.substr(pos - 1, 1);
						string with = stringAtPos;

						for (int j = 37; j <= 45; j++) {
							if ((salturi[j].index == index && salturi[j].with == with)) {
								stiva.append(salturi[j].to);
							}
						}
						cout << " => " << productii[stoi(productieNr)].from << "+TS(" << index << "," << with << ")";
						i = 1;
					}

					cout << endl;
				} else {
					printf("S: %-20s     CI: %-8s     A: %s\n", stiva.c_str(), cuvIntrare.c_str(), actiuni[i].to.c_str());
					// acceptare
					i = 36;
				}
			}

		}
	}
}