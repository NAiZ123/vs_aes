#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int  padding(unsigned char *in_data, int flag, int in_length) {
	if (in_length % 16 != 0) {//in_length‚©‚çin_data‚ª16byte‚Ì”{”‚É‚È‚Á‚Ä‚¢‚é‚©‚Ç‚¤‚©’²‚×‚é
		int result_aes, pad_aes;//–‚½‚È‚¢•ª‚Ìbyte”

		if ((result_aes = in_length % 16) != 0) {
			result_aes = in_length % 16;
			pad_aes = 16 - result_aes;

			for (int i = 0; i < pad_aes; i++) {
				in_data[in_length + i] = pad_aes;
			}
			in_length += pad_aes;
		}
	}
	else {
		for (int i = 0; i < 16; i++) {
			in_data[in_length + i] = 16;
		}
		in_length += 16;
	}
	return in_length;
}