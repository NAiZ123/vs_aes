#include "aes.h"

/*
�d���̍팸_OK
�w�b�_�[�t�@�C���̗��p_OK
192bit_OK
256bit_OK
�n�b�V���ɂ�錮��������ߑ΍� 256bit�͕����������Ȃ�_OK
��d�g���q�ɂ��g���q�T���̖��_OK

CBC���[�h�@�\�̒ǉ�
�n�b�V���֐��̗��p�i�����������������ǂ���)
�������̍팸�ƌx���̃`�F�b�N
*/

int KeyExpansion(char *passward, unsigned char Roundkey[15][16], int nk, int nr);
int padding(unsigned char *in_data, int flag, int in_length);

int main() {//�@�t�@�C���͏㏑�����[�h�ɕύX����悤�ɁB

	unsigned char *in;				//���̓f�[�^�i�[�p�z��
	unsigned char *out;				//�o�̓f�[�^�i�[�p�z��
	int in_len;						//���̓f�[�^��
	int out_len;					//�o�̓f�[�^��
	char file_name[256];			//���̓t�@�C����
	int or ;						//�Í���������
	FILE *fp;
	int k_size;						//�����i�[
	int nk;							//[128,192,256bit]=[4,6,8]
	int nr;							//���E���h���@[128,192,256bit]=[10,12,14]
	int mode;

	unsigned char RoundKey[15][16]; //�g����̃��E���h��
	char passward[256];				//�p�X���[�h�̊i�[

	/*�Í�������������*/
	printf("________AES__________\n\n");

	printf("�Í� = 0 or ���� = 1:\n");//enc = 0 or dec = 1
	scanf("%d", &or );
	if (or < 0 || 1 < or ) {
		printf("error�@���������l����͂��Ă�������\n");
		return -1;
	}

	printf("EBC=0 or CBC=1");
	scanf("%d", &mode );
	if (mode < 0 || 1 < mode) {
		printf("error�@���������l����͂��Ă�������\n");
		return -1;
	}
	

	if (or==0) {
		printf("�w�Í����[�h�ł��B�x\n");
	}
	else if (or==1) {
		printf("�w�������[�h�ł��B�x\n");
	}

	printf("����=128 or 192 or 256\n");
	scanf("%d", &k_size);

	switch (k_size) {
	case 128:
		nk = 4;
		nr = nk + 6;
		break;
	case 192:
		nk = 6;
		nr = nk + 6;
		break;
	case 256:
		nk = 8;
		nr = nk + 6;
		break;
	default:
		printf("error");
		break;
	}

	/* ���̓t�@�C�������擾�@*/
	printf("Input File Name: ");
	scanf_s("%s", &file_name, 255);

	const char *ext = strrchr(file_name, '.');//�g���q������̊i�[
	/*�g���q�̔���*/

	if (or==0 && strcmp(".aes", ext) == 0) {
		printf("�Í�������Ă���t�@�C���ł��B�V�X�e�����I�����܂��B\n");
		return -1;
	}
	else if (or==1) {
		char fe[5];
		for (int i = 4; i > 0; i--) {
			fe[4 - i] = file_name[strlen(file_name) - i];
		}
		fe[4] = '\0';
		if (strcmp(".aes", fe) != 0) {
			//printf("%s\n", fe);
			printf("�Í����t�@�C���ł͂���܂���B�V�X�e�����I�����܂��B\n");
			return -1;
		}
	}

	/* �t�@�C�����I�[�v�����Ē������擾�@*/
	fopen_s(&fp, file_name, "rb");
	if (fp == NULL) {
		printf("%s�̃I�[�v���Ɏ��s���܂����B\n", file_name);
		return -1;
	}
	fseek(fp, 0, SEEK_END);
	in_len = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	/* in��out�̗̈���m��*/
	in = (char *)malloc(in_len + 16);
	if (in == NULL) {
		printf("�������m�ۃG���[\n");
		return -1;
	}
	out = (char *)malloc(in_len + 16);
	if (out == NULL) {
		printf("�������m�ۃG���[\n");
		return -1;
	}

	/* �t�@�C���̃f�[�^��in�֊i�[ */
	fread(in, 1, in_len, fp);
	*(in + in_len) = '\0';
	fclose(fp);

	if (k_size < 256) {
		/* �p�X���[�h����́@*/
		printf(" pass(%d����) = : ", k_size / 8);
		scanf("%s", passward);
		if (strlen(passward) != k_size / 8) {
			printf("\n password�̒������Ⴂ�܂� \n");
			printf("�I�����܂�\n\n");
			return 0;
		}
		KeyExpansion(passward, RoundKey, nk, nr);
	}
	else {
		/* �p�X���[�h����́@*/
		printf(" pass(1~256�����܂�) = : ");
		unsigned char passwd[256];
		scanf("%s", passwd);
		hash(passwd);	//hash�֐���32byte�ɒ���
		KeyExpansion(passwd, RoundKey, nk, nr);
	}

	if (or == 0) {
		in_len = padding(in, 16, in_len);//padding�Ăяo��
		out_len = chipher_aes_enc(in, out, in_len, RoundKey, nr, mode);
		printf("�Í����������܂����B\n");
	}
	else if (or == 1) {
		out_len = invCipher_aes_dec(in, out, in_len, RoundKey, nr, mode);
		/* padding�O������ */
		int sub = 0;
		sub = out[out_len - 1];
		out_len = out_len - sub;
		printf("�������������܂����B\n");
	}

	/* �o�͗p�̃t�@�C������p�ӂ��ďo�͗p�̃t�@�C�����I�[�v���@*/
	if (or == 0) {
		strcat_s(file_name, 255, ".aes"); //�A�ڌ�̕�����̍ő咷���w��
		fopen_s(&fp, file_name, "wb");
		if (fp == NULL) {
			printf("%s�̃I�[�v���Ɏ��s���܂����B\n", file_name);
			return -1;
		}
	}
	else if (or == 1) {
		const int len = strlen(file_name);
		file_name[len - 4] = '\0';
		fopen_s(&fp, file_name, "wb");
		if (fp == NULL) {
			printf("%s�̃I�[�v���Ɏ��s���܂����B\n", file_name);
			return -1;
		}
	}

	/* ���k�����f�[�^�̃t�@�C���������݁@*/
	fwrite(out, 1, out_len, fp);
	fclose(fp);

	/* in��out�̗̈������@*/
	free(in);
	free(out);

	return 0;
}