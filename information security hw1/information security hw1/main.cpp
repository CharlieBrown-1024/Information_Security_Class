#include<iostream>
#include<string>
using namespace std;

void caesar(string, string);
void playfair(string, string);
void vernam(string, string);
void rowTransposition(string, string);
void railFence(string, string);

int main() {
	string file, cipher, key, text;
	while (cin >> file >> cipher >> key >> text) {
		if (cipher == "caesar") {
			caesar(key, text);
		}
	}
}


void caesar(string k, string t) {
	//shift
	int shift = stoi(k);
	//encrypt
	if (islower(t[0])) {
		string ciphertext = t;
		for (int i = 0; i < t.size(); i++) {
			ciphertext[i] = (t[i] + shift - 97) % 26 + 97;
		}
		//toupper
		for (int i = 0; i < ciphertext.size(); i++) {
			ciphertext[i] = toupper(ciphertext[i]);
		}
		cout << ciphertext << endl;
	}
	//decrypt
	else {
		string plaintext = t;
		for (int i = 0; i < t.size(); i++) {
			plaintext[i] = (t[i] - shift + 65) % 26 + 65;
			//cout << plaintext[i] << endl;
		}
		//tolower
		for (int i = 0; i < plaintext.size(); i++) {
			plaintext[i] = tolower(plaintext[i]);
		}
		cout << plaintext << endl;
	}
}