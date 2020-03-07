#ifndef AES_H
#define AES_H
//多重インクルード防止

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int chipher_aes_enc(unsigned char * in_data, unsigned char *out_data, int pt, unsigned char RoundKey[15][16], int nr, int mode);
int invCipher_aes_dec(unsigned char * in_data, unsigned char *out_data, int pt, unsigned char RoundKey[15][16], int nr, int mode);
void AddRoundKey(unsigned char state[4][4], unsigned char RoundKey[15][16], int count);
void SubBytes(unsigned char state[4][4]);
void InvSubBytes(unsigned char state[4][4]);
void ShiftRows(unsigned char state[4][4]);
void InvShiftRows(unsigned char state[4][4]);
unsigned char gf_mult(unsigned char a, unsigned char b);
void MixColumns(unsigned char in[4][4]);
void InvMixColumns(unsigned char in[4][4]);
//プロトタイプ宣言

#endif