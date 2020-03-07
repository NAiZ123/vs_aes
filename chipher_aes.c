#include "aes.h"

int nk; //����bit(=4,6,8[128,192,256bit])
int nr; //���E���h��(=10,12,14)

int Sbox[256] = {
  0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76,
  0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0,
  0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15,
  0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75,
  0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84,
  0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf,
  0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8,
  0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2,
  0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73,
  0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb,
  0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79,
  0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08,
  0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a,
  0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e,
  0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf,
  0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16
};
int invSbox[256] = {
  0x52,0x09,0x6a,0xd5,0x30,0x36,0xa5,0x38,0xbf,0x40,0xa3,0x9e,0x81,0xf3,0xd7,0xfb,
  0x7c,0xe3,0x39,0x82,0x9b,0x2f,0xff,0x87,0x34,0x8e,0x43,0x44,0xc4,0xde,0xe9,0xcb,
  0x54,0x7b,0x94,0x32,0xa6,0xc2,0x23,0x3d,0xee,0x4c,0x95,0x0b,0x42,0xfa,0xc3,0x4e,
  0x08,0x2e,0xa1,0x66,0x28,0xd9,0x24,0xb2,0x76,0x5b,0xa2,0x49,0x6d,0x8b,0xd1,0x25,
  0x72,0xf8,0xf6,0x64,0x86,0x68,0x98,0x16,0xd4,0xa4,0x5c,0xcc,0x5d,0x65,0xb6,0x92,
  0x6c,0x70,0x48,0x50,0xfd,0xed,0xb9,0xda,0x5e,0x15,0x46,0x57,0xa7,0x8d,0x9d,0x84,
  0x90,0xd8,0xab,0x00,0x8c,0xbc,0xd3,0x0a,0xf7,0xe4,0x58,0x05,0xb8,0xb3,0x45,0x06,
  0xd0,0x2c,0x1e,0x8f,0xca,0x3f,0x0f,0x02,0xc1,0xaf,0xbd,0x03,0x01,0x13,0x8a,0x6b,
  0x3a,0x91,0x11,0x41,0x4f,0x67,0xdc,0xea,0x97,0xf2,0xcf,0xce,0xf0,0xb4,0xe6,0x73,
  0x96,0xac,0x74,0x22,0xe7,0xad,0x35,0x85,0xe2,0xf9,0x37,0xe8,0x1c,0x75,0xdf,0x6e,
  0x47,0xf1,0x1a,0x71,0x1d,0x29,0xc5,0x89,0x6f,0xb7,0x62,0x0e,0xaa,0x18,0xbe,0x1b,
  0xfc,0x56,0x3e,0x4b,0xc6,0xd2,0x79,0x20,0x9a,0xdb,0xc0,0xfe,0x78,0xcd,0x5a,0xf4,
  0x1f,0xdd,0xa8,0x33,0x88,0x07,0xc7,0x31,0xb1,0x12,0x10,0x59,0x27,0x80,0xec,0x5f,
  0x60,0x51,0x7f,0xa9,0x19,0xb5,0x4a,0x0d,0x2d,0xe5,0x7a,0x9f,0x93,0xc9,0x9c,0xef,
  0xa0,0xe0,0x3b,0x4d,0xae,0x2a,0xf5,0xb0,0xc8,0xeb,0xbb,0x3c,0x83,0x53,0x99,0x61,
  0x17,0x2b,0x04,0x7e,0xba,0x77,0xd6,0x26,0xe1,0x69,0x14,0x63,0x55,0x21,0x0c,0x7d
};


int chipher_aes_enc(unsigned char *in_data, unsigned char *out_data, int pt, unsigned char RoundKey[15][16], int nr,int mode)
{

	unsigned char state[4][4];
	int length = 0;
	int i, j, k, l, n;
	int count;

	if (mode == 1) {
		int block = pt / 128;
		aes_CBC_enc_buf(in_data, block, pt);
	}


	n = 0;//n*16
	while (n < (pt / 16)) {
		count = 0;

		for (j = 0; j < 4; j++) {
			for (k = 0; k < 4; k++) {
				state[j][k] = *(in_data + count + (n * 16));
				count++;
			}
		}
		AddRoundKey(state, RoundKey, 0);
		for (l = 1; l < nr; l++) {
			SubBytes(state);
			ShiftRows(state);
			MixColumns(state);
			AddRoundKey(state, RoundKey, l);
		}
		SubBytes(state);
		ShiftRows(state);
		AddRoundKey(state, RoundKey, nr);
		count = 0;
		for (j = 0; j < 4; j++) {
			for (k = 0; k < 4; k++) {
				*(out_data + count + (n * 16)) = state[j][k];
				count++;
			}
		}
		length += 16;
		n++;
	}
	return length;
}


int invCipher_aes_dec(unsigned char * in_data, unsigned char *out_data, int pt, unsigned char RoundKey[15][16], int nr,int mode)
{
	unsigned char state[4][4];
	int length = 0;
	int i, j, k, l, n;
	int count;


	n = 0;//
	while (n < (pt / 16)) {    
		count = 0;
		for (j = 0; j < 4; j++) {
			for (k = 0; k < 4; k++) {
				state[j][k] = *(in_data + count + (n * 16));
				count++;
			}
		}
		AddRoundKey(state, RoundKey, nr);
		for (l = nr-1; l > 0; l--) {
			InvShiftRows(state);
			InvSubBytes(state);
			AddRoundKey(state, RoundKey, l);
			InvMixColumns(state);
		}
		InvShiftRows(state);
		InvSubBytes(state);
		AddRoundKey(state, RoundKey, 0);
		count = 0;
		for (j = 0; j < 4; j++) {
			for (k = 0; k < 4; k++) {
				*(out_data + count + (n * 16)) = state[j][k];
				count++;
			}
		}
		length += 16;
		n++;
	}
	return length;
}


void AddRoundKey(unsigned char state[4][4], unsigned char RoundKey[15][16], int count) {

	unsigned char w[4][4];
	int a = 0;
	int j;
	for (j = 0; j < 16; j++) {
		w[a][j % 4] = RoundKey[count][j];
		if ((j + 1) % 4 == 0) {
			a++;
		}
	}
	//�V����state���쐬
	for (int i = 0; i < 4; i++) {
		for (int k = 0; k < 4; k++) {
			state[i][k] ^= w[k][i];
		}
	}
}


void SubBytes(unsigned char state[4][4]) {
	int i, j;
	int x, y;
	for (i = 0; i < 4; ++i)
	{
		for (j = 0; j < 4; ++j)
		{
			x = (state[j][i] >> 0) & 0x0f;
			y = (state[j][i] >> 4) & 0xff;
			state[j][i] = Sbox[(16 * y) + (x)];//Sbox�ϊ�����
		}
	}
	return 0;
}

void InvSubBytes(unsigned char state[4][4]) {
	int i, j;
	int x, y;
	for (i = 0; i < 4; ++i)
	{
		for (j = 0; j < 4; ++j)
		{
			x = (state[j][i] >> 0) & 0x0f;
			y = (state[j][i] >> 4) & 0xff;
			state[j][i] = invSbox[(16 * y) + (x)];//Sbox�ϊ�����
		}
	}
	return 0;
}


void ShiftRows(unsigned char state[4][4]) {

	int temp;

	//2�s��state�u���b�N1���V�t�g
	temp = state[1][0];
	state[1][0] = state[1][1];
	state[1][1] = state[1][2];
	state[1][2] = state[1][3];
	state[1][3] = temp;

	//3�s��state�u���b�N2���V�t�g
	temp = state[2][0];
	state[2][0] = state[2][2];
	state[2][2] = temp;
	temp = state[2][1];
	state[2][1] = state[2][3];
	state[2][3] = temp;

	//4�s��state�u���b�N3���V�t�g
	temp = state[3][0];
	state[3][0] = state[3][3];
	state[3][3] = state[3][2];
	state[3][2] = state[3][1];
	state[3][1] = temp;
}


void InvShiftRows(unsigned char state[4][4]) {
	int temp;

	//2�s��state�u���b�N1�E�V�t�g
	temp = state[1][3];
	state[1][3] = state[1][2];
	state[1][2] = state[1][1];
	state[1][1] = state[1][0];
	state[1][0] = temp;


	//3�s��state�u���b�N2�E�V�t�g_�ς�炸
	temp = state[2][0];
	state[2][0] = state[2][2];
	state[2][2] = temp;
	temp = state[2][1];
	state[2][1] = state[2][3];
	state[2][3] = temp;

	//4�s��state�u���b�N3�E�V�t�g
	temp = state[3][0];
	state[3][0] = state[3][1];
	state[3][1] = state[3][2];
	state[3][2] = state[3][3];
	state[3][3] = temp;


}


unsigned char gf_mult(unsigned char a, unsigned char b) {
	int i;
	int retval = 0;

	/* GF_bitshift*/
	for (i = 0; i < 8; i++) {
		if ((b & 1) == 1)
			retval ^= a;

		if ((a & 0x80) == 0x80) {
			a <<= 1;
			a ^= 0x1b;
		}
		else {
			a <<= 1;
		}

		b >>= 1;
	}

	return (unsigned char)retval;
}


void MixColumns(unsigned char in[4][4]) {
	int c;
	int i, j;
	unsigned char out[4][4];
	for (c = 0; c < 4; ++c) {
		out[0][c] = gf_mult(0x02, in[0][c]) ^ gf_mult(0x03, in[1][c]) ^ in[2][c] ^ in[3][c];
		out[1][c] = in[0][c] ^ gf_mult(0x02, in[1][c]) ^ gf_mult(0x03, in[2][c]) ^ in[3][c];
		out[2][c] = in[0][c] ^ in[1][c] ^ gf_mult(0x02, in[2][c]) ^ gf_mult(0x03, in[3][c]);
		out[3][c] = gf_mult(0x03, in[0][c]) ^ in[1][c] ^ in[2][c] ^ gf_mult(0x02, in[3][c]);
	}
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			in[i][j] = out[i][j];
		}
	}

}

void InvMixColumns(unsigned char in[4][4]) {
	int c;
	int i, j;
	unsigned char out[4][4];
	for (c = 0; c < 4; ++c) {
		out[0][c] = gf_mult(0x0E, in[0][c]) ^ gf_mult(0x0B, in[1][c]) ^ gf_mult(0x0D, in[2][c]) ^ gf_mult(0x09, in[3][c]);
		out[1][c] = gf_mult(0x09, in[0][c]) ^ gf_mult(0x0E, in[1][c]) ^ gf_mult(0x0B, in[2][c]) ^ gf_mult(0x0D, in[3][c]);
		out[2][c] = gf_mult(0x0D, in[0][c]) ^ gf_mult(0x09, in[1][c]) ^ gf_mult(0x0E, in[2][c]) ^ gf_mult(0x0B, in[3][c]);
		out[3][c] = gf_mult(0x0B, in[0][c]) ^ gf_mult(0x0D, in[1][c]) ^ gf_mult(0x09, in[2][c]) ^ gf_mult(0x0E, in[3][c]);
	}
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			in[i][j] = out[i][j];
		}
	}
}

int KeyExpansion(char *passward, unsigned char Roundkey[15][16],int nk,int nr) {//w=���E���h��

	//Roundkey[11][16],Roundkey[13][16],Roundkey[15][16]
	unsigned char temp[4];
	unsigned char temp2;

	int count = 0;
	int i, j, k;
	int c = 0;
	int d = 0;
	int x, y;
	int Rcon[10] = { 0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x1b,0x36 };//���E���h�萔
	unsigned char w[4*(14+1)][4];//4*(nr+1) nr=10,12,14  nk=4,6,8

	for (i = 0; i < 4 * (nr + 1); i++) {
		for (j = 0; j < 4; j++) {
			w[i][j] = 0;
		}
	}

	for (i = 0; i < nk; i++) {
		for (j = 0; j < 4; j++) {
			w[i][j] = *(passward + count);
			printf(" %x ", w[i][j]);
			count++;
		}
	}


	for (i = nk; i < 4 * (nr + 1); i++) {
		for (j = 0; j < 4; j++) {
			temp[j] = w[i - 1][j];
		}
		if ((i % nk) == 0) {// nk != 8 && i % nk == 0
			//1byte���V�t�g:Rotword
			temp2 = temp[0];
			temp[0] = temp[1];
			temp[1] = temp[2];
			temp[2] = temp[3];
			temp[3] = temp2;

			for (k = 0; k < 4; k++)
			{
				x = temp[k] & 0x0f;
				y = (temp[k] >> 4) & 0xff;
				temp[k] = Sbox[(16 * y) + (x)];//Sbox�ϊ�����:Subword
			}
			temp[0] = temp[0] ^ Rcon[(i / nk) - 1];
			/*for (j = 0; j < 4; j++) {
				temp[j] = temp[j] ^ Rcon[(i / 4) - 1];
			}*/
		}
		else if (nk > 6 && (i % nk) == 4) {
			for (k = 0; k < 4; k++)
			{
				x = temp[k] & 0x0f;
				y = (temp[k] >> 4) & 0xff;
				temp[k] = Sbox[(16 * y) + (x)];//Sbox�ϊ�����:Subword
			}
		}

		for (j = 0; j < 4; j++) {
			w[i][j] = w[i - nk][j] ^ temp[j];
			//�ϐ�temp�̒l��w [i-1] [0�`4]�̔r���I�_���a���Ƃ��Ċi�[
		}
	}

	//�ȉ� w[44][4]�� RoundKey[11][16]�ɕϊ��B
	for (i = 0; i < 4 * (nr + 1); i++) {
		Roundkey[i / 16][d] = w[c][i - (c * 4)];

		if ((i + 1) % 4 == 0) {
			c++;
		}
		if ((i + 1) % 16 == 0) {
			d = 0;
		}
		d++;
	}
	


	return 0;
}


void aes_CBC_enc_buf(unsigned char *in_data, int block, int length)
{
	int i,j;
	int count = 0;
	unsigned char Iv[128];
	Iv[0] = 123;

	for (i = 0; i < block; i++) {
		for (j = 0; j < 128; ++j) {
			in_data[j] ^= Iv[j];
		}
		count += 128;
	}

}

void aes_CBC_dec_buf(unsigned char *in_data, int block, int length)
{

}