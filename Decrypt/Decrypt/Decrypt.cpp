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
	
	string plaintext = text;
	for (int i = 0; i < text.size(); i++) {
		plaintext[i] = (text[i] - shift + 65) % 26 + 65;
	}
	//tolower
	for (int i = 0; i < plaintext.size(); i++) {
		plaintext[i] = tolower(plaintext[i]);
	}
	cout << plaintext << endl;
}

void playfair(string key, string text) {
	vector<bool> used(26, false);
	char matrix[5][5];
	int row = 0;
	int col = 0;

	//put key into matrix
	for (int i = 0; i < key.size(); i++) {
		if (col == 5) {
			row++;
			col = 0;
		}
		if (!used[key[i] - 65]) {
			matrix[row][col++] = key[i];
			used[key[i] - 65] = true;
		}
	}
	//put other alphbet into matrix
	for (char i = 'A'; i <= 'Z'; i++) {
		if (col == 5) {
			row++;
			col = 0;
		}
		if (i != 'J') {
			if (!used[i - 65]) {
				matrix[row][col++] = i;
				used[i - 65] = true;
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
	string plaintext = "";
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
			if (col1 - 1 >= 0 || col2 - 1 >= 0) {
				plaintext += matrix[row1][(col1 - 1) % 5];
				plaintext += matrix[row2][(col2 - 1) % 5];
			}
			else {
				plaintext += matrix[row1][4];
				plaintext += matrix[row2][4];
			}
		}
		else if (col1 == col2) {
			if (row1 - 1 >= 0 || row2 - 1 >= 0) {
				plaintext += matrix[(row1 - 1) % 5][col1];
				plaintext += matrix[(row2 - 1) % 5][col2];
			}
			else {
				plaintext += matrix[4][col1];
				plaintext += matrix[4][col2];
			}
		}
		else {
			plaintext += matrix[row1][col2];
			plaintext += matrix[row2][col1];
		}
	}
	//tolower
	for (int i = 0; i < plaintext.size(); i++) {
		plaintext[i] = tolower(plaintext[i]);
	}
	cout << plaintext << endl;

}

void vernam(string key, string text) {
	int n = key.size();
	string plaintext = "";
	string temp = "";
	string k = key;
	int time;
	if (text.size() % key.size() == 0)
		time = text.size() / key.size();
	else
		time = text.size() / key.size() + 1;

	int count = 0;
	for (int i = 0; i < time; i++) {
		for (int j = 0; j < n && count < text.size() ; j++, count++) {
			temp += ((k[j] - 65) ^ (text[count] - 65)) % 26 + 65;
		}
		k = temp;
		plaintext += temp;
		temp = "";
	}
	for (int i = 0; i < plaintext.size(); i++) {
		plaintext[i] = tolower(plaintext[i]);
	}
	cout << plaintext << endl;
}

void row(string key, string text) {
	//n*m
	int n = key.size();//col
	int m;//row
	if (text.size() % n == 0)
		m = text.size() / n;
	else
		m = text.size() / n + 1;

	char **a = new char*[m];
	for (int i = 0; i < m; i++) {
		a[i] = new char[n]();
	}
	int index = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (index < text.size())
				a[j][i] = text[index++];
		}
	}
	string plaintext = "";
	char *key_order = new char[n];
	for (int i = 0; i < n; i++) {
		key_order[key[i] - 48 - 1] = i;
	}
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			if (a[i][key_order[j]] != NULL)
				plaintext += a[i][key_order[j]];
		}
	}
	for (int i = 0; i < plaintext.size(); i++) {
		plaintext[i] = tolower(plaintext[i]);
	}
	cout << plaintext << endl;
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
	
	bool down;
	int row = 0, col = 0;

	// mark the text position with '*' 
	for (int i = 0; i < text.size(); i++) {

		if (row == 0)
			down = true;
		if (row == n - 1)
			down = false;//up

		rail[row][col++] = '*';
		if (down)
			row++;
		else
			row--;
	}
	//replace '*' with text
	int index = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < text.size(); j++) {
			if (rail[i][j] == '*' && index < text.size())
				rail[i][j] = text[index++];
		}
	}
	string plaintext = "";
	row = 0, col = 0;
	for (int i = 0; i < text.size(); i++) {
		if (row == 0)
			down = true;
		if (row == n - 1)
			down = false;

		if (rail[row][col] != '-')
			plaintext += rail[row][col++];
		if (down)
			row++;
		else
			row--;
	}
	for (int i = 0; i < plaintext.size(); i++) {
		plaintext[i] = tolower(plaintext[i]);
	}
	cout << plaintext << endl;
}