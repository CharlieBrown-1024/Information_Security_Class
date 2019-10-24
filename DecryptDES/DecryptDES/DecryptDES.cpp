#include<iostream>
#include<string>
using namespace std;

string CharToString(char* in, int size);
void DES_setkey(const char key[8]);
void DES_run(char out[8], char in[8]);
void S_func(bool Out[32], const bool In[48]);

int main(int argc, char* argv[]) {
	string P = "", K = "";
	K = argv[1];
	P = argv[2];
	const char key[8] = K.c_str;	//將String轉char array
	char* plaintext = &P[0];
	DES_setkey(key);
	DES_run(plaintext, plaintext);
	string output = CharToString(plaintext, sizeof(plaintext));
	cout << output << endl;
	system("pause");
}

string CharToString(char* in, int size) {
	string s = "";
	for (int i = 0; i < size; i++) {
		s = s + in[i];
	}
	return s;
}

void ByteToBit(bool* Out, const char* In, int bits)
{
	for (int i = 0; i < bits; i++)
	{
		Out[i] = (In[i / 8] >> (i % 8)) & 1;
	}
}

void BitToByte(char* Out, const bool* In, int bits)
{
	memset(Out, 0, (bits + 7) / 8);
	for (int i = 0; i < bits; i++)
	{
		Out[i / 8] |= In[i] << (i % 8);
	}
}

const static char PC1_table[56] = {	//PC1,密鑰奇偶較驗除去(8,16,24,32,40,48,56,64),64=>56位
	57,49,41,33,25,17,9,
	1,58,50,42,34,26,18,
	10,2,59,51,43,35,27,
	19,11,3,60,52,44,36,
	63,55,47,39,31,23,15,
	7,62,54,46,38,30,22,
	14,6,61,53,45,37,29,
	21,13,5,28,20,12,4
};

const static char PC2_table[48] = {	//PC2,生成子密鑰,48位
	14,17,11,24,1,5,
	3,28,15,6,21,10,
	23,19,12,4,26,8,
	16,7,27,20,13,2,
	41,52,31,37,47,55,
	30,40,51,45,33,48,
	44,49,39,56,34,53,
	46,42,50,36,29,32
};

void Trans16_64(bool* out, const char* in) {	//16進制轉成64位
	for (int i = 0; i < 64; i++) {
		out[i] = (in[i / 8] >> (i % 8)) & 1;
	}
}


const static char Left_table[16] = {	//左移table
	1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1
};

void Rotateleft(bool* in, int len, int table) {
	static bool temp[100];
	memcpy(temp, in, table);
	memcpy(in, in + table, len - table);
	memcpy(in + len - table, temp, table);
}

static bool SubKey[16][48];	//子金鑰

void Transform(bool* out, bool* in, const char* table, int len) {	//用於table轉換
	static bool temp[100];
	for (int i = 0; i < len; i++) {
		temp[i] = in[table[i] - 1];
	}
	memcpy(out, temp, len);	//將temp拷貝覆蓋上out
}

void DES_setkey(const char key[8]) {
	static bool K[64], * C = &K[0], * D = &K[28];
	Trans16_64(K, key);	//先將16進制轉換
	Transform(K, K, PC1_table, 56);	//將原始Key從64位轉到56位
	for (int i = 0; i < 16; i++) {
		Rotateleft(C, 28, Left_table[i]);	//C左移
		Rotateleft(D, 28, Left_table[i]);	//D左移
		Transform(SubKey[15 - i], K, PC2_table, 48);	//用PC2轉為48位並存入16個子密鑰(Decrypt子密鑰與Encrypt順序剛好顛倒)
	}
}

const static char IP_table[64] = {
	58,50,42,34,26,18,10,2,
	60,52,44,36,28,20,12,4,
	62,54,46,38,30,22,14,6,
	64,56,48,40,32,24,16,8,
	57,49,41,33,25,17,9,1,
	59,51,43,35,27,19,11,3,
	61,53,45,37,29,21,13,5,
	63,55,47,39,31,23,15,7
};

const static char IP1_table[64] = {
	40,8,48,16,56,24,64,32,
	39,7,47,15,55,23,63,31,
	38,6,46,14,54,22,62,30,
	37,5,45,13,53,21,61,29,
	36,4,44,12,52,20,60,28,
	35,3,43,11,51,19,59,27,
	34,2,42,10,50,18,58,26,
	33,1,41,9,49,17,57,25
};

void XOR(bool* a, const bool* b, int lenth) {
	for (int i = 0; i < lenth; i++) {
		a[i] ^= b[i];
	}
}

static const char E_table[48] = {
	32,1,2,3,4,5,
	4,5,6,7,8,9,
	8,9,10,11,12,13,
	12,13,14,15,16,17,
	16,14,18,19,20,21,
	20,21,22,23,24,25,
	24,25,26,27,28,29,
	28,29,30,31,32,1
};

const static char S_box[8][4][16] = {
	//S1
	14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,
	0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,
	4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
	15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13,
	//S2
	15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,
	3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,
	0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,
	13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9,
	//S3
	10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,
	13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,
	13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,
	1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12,
	//S4
	7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,
	13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,
	3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14,
	//S5
	2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,
	14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,
	4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,
	11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3,
	//S6
	12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,
	10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,
	9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,
	4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13,
	//S7
	4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,
	13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,
	1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,
	6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12,
	//S8
	13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,
	1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,
	7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,
	2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11
};

const static char P_table[32] = {	//Permutation的table
	16,7,20,21,29,12,28,17,
	1,15,23,26,5,18,31,10,
	2,8,24,14,32,27,3,9,
	19,13,30,6,22,11,4,25
};

void F_function(bool in[32], const bool key[48]) {
	static bool expansion[48];
	Transform(expansion, in, E_table, 48);	//先由32位轉48位(E_table)
	XOR(expansion, key, 48);
	S_func(in, expansion);	//S盒48位再轉為32位
	Transform(in, in, P_table, 32);	//Permutation
}

void S_func(bool Out[32], const bool In[48]) {	//S盒代替,輸入6位,輸出4位(1,6位作為行2,3,4,5位作為列)
	for (char i = 0, j, k; i < 8; i++, In += 6, Out += 4)
	{
		j = (In[0] << 1) + In[5];
		k = (In[1] << 3) + (In[2] << 2) + (In[3] << 1) + In[4];
		ByteToBit(Out, &S_box[i][j][k], 4);	// out每次前進4位
	}
}

void DES_run(char out[8], char in[8]) {
	static bool M[64], Tmp[32], * Li = &M[0], * Ri = &M[32];
	Trans16_64(M, in);
	Transform(M, M, IP_table, 64);
	for (int i = 0; i < 16; i++) {
		memcpy(Tmp, Ri, 32);
		F_function(Ri, SubKey[i]);
		XOR(Ri, Li, 32);
		memcpy(Li, Tmp, 32);
	}
	Transform(M, M, IP1_table, 64);
	BitToByte(out, M, 64);
}