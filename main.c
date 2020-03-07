#include "aes.h"

/*
重複の削減_OK
ヘッダーファイルの利用_OK
192bit_OK
256bit_OK
ハッシュによる鍵長上限超過対策 256bitは文字数制限なし_OK
二重拡張子による拡張子探索の問題_OK

CBCモード機能の追加
ハッシュ関数の利用（復号化が正しいかどうか)
メモリの削減と警告のチェック
*/

int KeyExpansion(char *passward, unsigned char Roundkey[15][16], int nk, int nr);
int padding(unsigned char *in_data, int flag, int in_length);

int main() {//　ファイルは上書きモードに変更するように。

	unsigned char *in;				//入力データ格納用配列
	unsigned char *out;				//出力データ格納用配列
	int in_len;						//入力データ長
	int out_len;					//出力データ長
	char file_name[256];			//入力ファイル名
	int or ;						//暗号か復号か
	FILE *fp;
	int k_size;						//鍵長格納
	int nk;							//[128,192,256bit]=[4,6,8]
	int nr;							//ラウンド数　[128,192,256bit]=[10,12,14]
	int mode;

	unsigned char RoundKey[15][16]; //拡張後のラウンド鍵
	char passward[256];				//パスワードの格納

	/*暗号化か復号化か*/
	printf("________AES__________\n\n");

	printf("暗号 = 0 or 復号 = 1:\n");//enc = 0 or dec = 1
	scanf("%d", &or );
	if (or < 0 || 1 < or ) {
		printf("error　正しい数値を入力してください\n");
		return -1;
	}

	printf("EBC=0 or CBC=1");
	scanf("%d", &mode );
	if (mode < 0 || 1 < mode) {
		printf("error　正しい数値を入力してください\n");
		return -1;
	}
	

	if (or==0) {
		printf("『暗号モードです。』\n");
	}
	else if (or==1) {
		printf("『復号モードです。』\n");
	}

	printf("鍵長=128 or 192 or 256\n");
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

	/* 入力ファイル名を取得　*/
	printf("Input File Name: ");
	scanf_s("%s", &file_name, 255);

	const char *ext = strrchr(file_name, '.');//拡張子文字列の格納
	/*拡張子の判別*/

	if (or==0 && strcmp(".aes", ext) == 0) {
		printf("暗号化されているファイルです。システムを終了します。\n");
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
			printf("暗号化ファイルではありません。システムを終了します。\n");
			return -1;
		}
	}

	/* ファイルをオープンして長さを取得　*/
	fopen_s(&fp, file_name, "rb");
	if (fp == NULL) {
		printf("%sのオープンに失敗しました。\n", file_name);
		return -1;
	}
	fseek(fp, 0, SEEK_END);
	in_len = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	/* inとoutの領域を確保*/
	in = (char *)malloc(in_len + 16);
	if (in == NULL) {
		printf("メモリ確保エラー\n");
		return -1;
	}
	out = (char *)malloc(in_len + 16);
	if (out == NULL) {
		printf("メモリ確保エラー\n");
		return -1;
	}

	/* ファイルのデータをinへ格納 */
	fread(in, 1, in_len, fp);
	*(in + in_len) = '\0';
	fclose(fp);

	if (k_size < 256) {
		/* パスワードを入力　*/
		printf(" pass(%d文字) = : ", k_size / 8);
		scanf("%s", passward);
		if (strlen(passward) != k_size / 8) {
			printf("\n passwordの長さが違います \n");
			printf("終了します\n\n");
			return 0;
		}
		KeyExpansion(passward, RoundKey, nk, nr);
	}
	else {
		/* パスワードを入力　*/
		printf(" pass(1~256文字まで) = : ");
		unsigned char passwd[256];
		scanf("%s", passwd);
		hash(passwd);	//hash関数で32byteに調整
		KeyExpansion(passwd, RoundKey, nk, nr);
	}

	if (or == 0) {
		in_len = padding(in, 16, in_len);//padding呼び出し
		out_len = chipher_aes_enc(in, out, in_len, RoundKey, nr, mode);
		printf("暗号化完了しました。\n");
	}
	else if (or == 1) {
		out_len = invCipher_aes_dec(in, out, in_len, RoundKey, nr, mode);
		/* padding外す操作 */
		int sub = 0;
		sub = out[out_len - 1];
		out_len = out_len - sub;
		printf("復号化完了しました。\n");
	}

	/* 出力用のファイル名を用意して出力用のファイルをオープン　*/
	if (or == 0) {
		strcat_s(file_name, 255, ".aes"); //連接後の文字列の最大長を指定
		fopen_s(&fp, file_name, "wb");
		if (fp == NULL) {
			printf("%sのオープンに失敗しました。\n", file_name);
			return -1;
		}
	}
	else if (or == 1) {
		const int len = strlen(file_name);
		file_name[len - 4] = '\0';
		fopen_s(&fp, file_name, "wb");
		if (fp == NULL) {
			printf("%sのオープンに失敗しました。\n", file_name);
			return -1;
		}
	}

	/* 圧縮したデータのファイル書き込み　*/
	fwrite(out, 1, out_len, fp);
	fclose(fp);

	/* inとoutの領域を解放　*/
	free(in);
	free(out);

	return 0;
}