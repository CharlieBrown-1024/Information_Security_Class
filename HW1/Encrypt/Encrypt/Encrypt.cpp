#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
using namespace std;

void caesar(string, string);
void playfair(string, string);
void vernam(string, string);
void row(string, string);
void rail_fence(string, string);

int main(int argc, char *argv[]) {

	string file, cipher, key, text;
	cipher = argv[1];
	key = argv[2];
	text = argv[3];
	
	if (cipher == "caesar") {
		caesar(key, text);
	}
	else if (cipher == "rail_fence") {
		rail_fence(key, text);
	}
	else if (cipher == "vernam") {
		vernam(key, text);
	}
	else if (cipher == "row") {
		row(key, text);
	}
	else if (cipher == "playfair") {
		playfair(key, text);
	}
}


void caesar(string key, string text) {
	//shift
	int shift = stoi(key);

	string ciphertext = text;
	for (int i = 0; i < text.size(); i++) {
		ciphertext[i] = (text[i] + shift - 97) % 26 + 97;
	}
	//toupper
	for (int i = 0; i < ciphertext.size(); i++) {
		ciphertext[i] = toupper(ciphertext[i]);
	}
	cout << ciphertext << endl;
}

void playfair(string key, string text) {
	vector<bool> used(26, false);
	char matrix[5][5];
	int row = 0;
	int col = 0;

	//lower key
	for (int i = 0; i < key.size(); i++) {
		key[i] = tolower(key[i]);
	}
	//put key into matrix
	for (int i = 0; i < key.size(); i++) {
		if (col == 5) {
			row++;
			col = 0;
		}
		if (!used[key[i] - 97]) {
			matrix[row][col++] = key[i];
			used[key[i] - 97] = true;
		}
	}
	//put other alphabet into matrix
	for (char i = 'a'; i <= 'z'; i++) {
		if (col == 5) {
			row++;
			col = 0;
		}
		if (i != 'j') {
			if (!used[i - 97]) {
				matrix[row][col++] = i;
				used[i - 97] = true;
			}
		}
	}

	/*for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			cout << matrix[i][j] << " ";
		}
		cout << endl;
	}*/

	int col1, row1, col2, row2;
	string ciphertext = "";
	for (int i = 0; i < text.size(); i += 2) {
		for (int j = 0; j < 5; j++) {//row
			for (int k = 0; k < 5; k++) {//col
				if (matrix[j][k] == text[i]) {
					row1 = j;
					col1 = k;
				}
				if (matrix[j][k] == text[i + 1]) {
					row2 = j;
					col2 = k;
				}
			}
		}
		if (row1 == row2) {
			ciphertext += matrix[row1][(col1 + 1) % 5];
			ciphertext += matrix[row2][(col2 + 1) % 5];
		}
		else if (col1 == col2) {
			ciphertext += matrix[(row1 + 1) % 5][col1];
			ciphertext += matrix[(row2 + 1) % 5][col2];
		}
		else {
			ciphertext += matrix[row1][col2];
			ciphertext += matrix[row2][col1];
		}
	}
	//toupper
	for (int i = 0; i < ciphertext.size(); i++) {
		ciphertext[i] = toupper(ciphertext[i]);
	}
	cout << ciphertext << endl;
		
}

void vernam(string key, string text) {
	for (int i = 0; i < key.size(); i++) {
		key[i] = tolower(key[i]);
	}
	string sub = text.substr(0, text.size() - key.size());
	key += sub;
	//cout << key;

	string ciphertext = "";
	for (int i = 0; i < text.size(); i++) {
		ciphertext += (((text[i] - 97) ^ (key[i] - 97)) % 26 + 97);
	}
	//toupper
	for (int i = 0; i < ciphertext.size(); i++) {
		if (isalpha(ciphertext[i]))
			ciphertext[i] = toupper(ciphertext[i]);
	}
	cout << ciphertext << endl;
}

void row(string key, string text) {
	//n*m
	int n = key.size();//col
	int m;//row
	if (text.size() % n == 0)
		m = text.size() / n;
	else
		m = text.size() / n + 1;

	//create matrix
	char **a = new char*[m];
	for (int i = 0; i < m; i++) {
		a[i] = new char[n]();
	}

	int index = 0;
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			if (index < text.size())
				a[i][j] = text[index++];
		}
	}
	string ciphertext = "";
	char *key_order = new char[n];
	for (int i = 0; i < n; i++) {
		key_order[key[i] - 48 - 1] = i;
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (a[j][key_order[i]] != NULL)
				ciphertext += a[j][key_order[i]];
		}
	}
	//toupper
	for (int i = 0; i < ciphertext.size(); i++) {
		ciphertext[i] = toupper(ciphertext[i]);
	}
	cout << ciphertext << endl;
}

void rail_fence(string key, string text) {
	int n = stoi(key);
	char **rail = new char*[n];
	for (int i = 0; i < n; i++) {
		rail[i] = new char[text.size()];
	}
	//mark all matrix '-'
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < text.size(); j++) {
			rail[i][j] = '-';
		}
	}
	//find the direction 
	bool down;
	int row = 0, col = 0;

	for (int i = 0; i < text.size(); i++) {
		//decide direction
		if (row == 0)
			down = true;
		if (row == n - 1)
			down = false;//up
		//fill text
		rail[row][col++] = text[i];
		if (down)
			row++;
		else
			row--;
		//down ? row++ : row--;
	}

	string ciphertext = "";
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < text.size(); j++) {
			if (rail[i][j] != '-')
				ciphertext += rail[i][j];
		}
	}
	for (int i = 0; i < ciphertext.size(); i++) {
		ciphertext[i] = toupper(ciphertext[i]);
	}
	cout << ciphertext << endl;
}