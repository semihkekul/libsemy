
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
using namespace std;
#define BYTE unsigned char

struct 
{
	BYTE Header[3];
	BYTE CAPHeaderInfo[85];
	BYTE RFU[40];
}CAPHeader;


struct
{
	unsigned int  HexLength;
	BYTE HexLengthBuffer[4];
	BYTE* Hex;
}Hex_BLK;

struct
{
	BYTE EncryptedKencLen[2];
	BYTE EncryptedKenc[256];
}KEY_BLK;

struct
{
	BYTE SignatureLength[2];
	BYTE Signature[256];
}SIGN_BLK;

struct
{
	BYTE HeaderBlockLen[2];
	BYTE FileHeader[1280];
	BYTE FileHeaderSign[256];

}HDR_BLK;

struct
{
	BYTE Header;
	BYTE BlockFormat;
	BYTE APname[21];
	BYTE APpriority;
	BYTE APflag;
	BYTE APtype;
}FileHeader;

void ParseFileHeader(BYTE* fileHeaderBuffer)  // 1280 size
{
	int idx = 0;

	FileHeader.Header = fileHeaderBuffer[idx++];
	FileHeader.BlockFormat = fileHeaderBuffer[idx++];

	printf("header = 0x%02x ; BlockFormat = 0x%02x\n", FileHeader.Header, FileHeader.BlockFormat);

	memcpy(FileHeader.APname, &fileHeaderBuffer[idx],20);
	FileHeader.APname[20] = '\0';
	printf("APname %s\n", FileHeader.APname);
	idx += 20;

	FileHeader.APpriority = fileHeaderBuffer[idx++];
	FileHeader.APflag = fileHeaderBuffer[idx++];
	FileHeader.APtype = fileHeaderBuffer[idx++];
	printf("Priority = %d ;Flag = %d ;Type %d\n", FileHeader.APpriority, FileHeader.APflag, FileHeader.APtype);
}

void ParseCAP(vector<char>& buffer)
{
	unsigned int idx = 0;
	unsigned int step = 3;

	memcpy(CAPHeader.Header, &buffer[idx], step);
	idx += step;

	step = 85;
	memcpy(CAPHeader.CAPHeaderInfo, &buffer[idx], step);
	idx += step;

	step = 40;
	memcpy(CAPHeader.CAPHeaderInfo, &buffer[idx], step);
	idx += step;

	//Hex_BLK
	step = 4;
	memcpy(Hex_BLK.HexLengthBuffer, &buffer[idx], step);
	

	Hex_BLK.HexLength = 0;
	// 4 bytes
	Hex_BLK.HexLength = (Hex_BLK.HexLength << 8) + Hex_BLK.HexLengthBuffer[0];
	Hex_BLK.HexLength = (Hex_BLK.HexLength << 8) + Hex_BLK.HexLengthBuffer[1];
	Hex_BLK.HexLength = (Hex_BLK.HexLength << 8) + Hex_BLK.HexLengthBuffer[2];
	Hex_BLK.HexLength = (Hex_BLK.HexLength << 8) + Hex_BLK.HexLengthBuffer[3];
	
	
	printf("hex lenght is %d\n", Hex_BLK.HexLength);

	idx += step;

	step = Hex_BLK.HexLength;
	Hex_BLK.Hex = (BYTE*) malloc(sizeof(BYTE) * step);
	
	memcpy(Hex_BLK.Hex, &buffer[idx], step);
	idx += step;

	//KEY_BLK
	step = 2;
	memcpy(KEY_BLK.EncryptedKencLen, &buffer[idx], step);
	idx += step;

	printf("EncryptedKencLen is 0x%02x 0x%02x\n", KEY_BLK.EncryptedKencLen[0], KEY_BLK.EncryptedKencLen[1]);

	step = 256;
	memcpy(KEY_BLK.EncryptedKenc, &buffer[idx], step);
	idx += step;

	//SIGN_BLK
	step = 2;
	memcpy(SIGN_BLK.SignatureLength, &buffer[idx], step);
	idx += step;

	printf("SignatureLength is 0x%02x 0x%02x\n", SIGN_BLK.SignatureLength[0], SIGN_BLK.SignatureLength[1]);

	step = 256;
	memcpy(SIGN_BLK.Signature, &buffer[idx], step);
	idx += step;

	//HDR_BLK
	step = 2;
	memcpy(HDR_BLK.HeaderBlockLen, &buffer[idx], step);
	idx += step;

	printf("HeaderBlockLen is 0x%02x 0x%02x\n", HDR_BLK.HeaderBlockLen[0], HDR_BLK.HeaderBlockLen[1]);

	step = 1280;
	memcpy(HDR_BLK.FileHeader, &buffer[idx], step);
	idx += step;

	step = 256;
	memcpy(HDR_BLK.FileHeaderSign, &buffer[idx], step);
	

	ParseFileHeader(HDR_BLK.FileHeader);
}
void handleErrors(void)
{
  ERR_print_errors_fp(stderr);
  abort();
}
int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
  unsigned char *iv, unsigned char *ciphertext)
{
  EVP_CIPHER_CTX *ctx;

  int len;

  int ciphertext_len;

  /* Create and initialise the context */
  if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

  /* Initialise the encryption operation. IMPORTANT - ensure you use a key
   * and IV size appropriate for your cipher
   * In this example we are using 256 bit AES (i.e. a 256 bit key). The
   * IV size for *most* modes is the same as the block size. For AES this
   * is 128 bits */
  if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
    handleErrors();

  /* Provide the message to be encrypted, and obtain the encrypted output.
   * EVP_EncryptUpdate can be called multiple times if necessary
   */
  if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
    handleErrors();
  ciphertext_len = len;

  /* Finalise the encryption. Further ciphertext bytes may be written at
   * this stage.
   */
  if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
  ciphertext_len += len;

  /* Clean up */
  EVP_CIPHER_CTX_free(ctx);

  return ciphertext_len;
}

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
  unsigned char *iv, unsigned char *plaintext)
{
  EVP_CIPHER_CTX *ctx;

  int len;

  int plaintext_len;

  /* Create and initialise the context */
  if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

  /* Initialise the decryption operation. IMPORTANT - ensure you use a key
   * and IV size appropriate for your cipher
   * In this example we are using 256 bit AES (i.e. a 256 bit key). The
   * IV size for *most* modes is the same as the block size. For AES this
   * is 128 bits */
  if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
    handleErrors();

  /* Provide the message to be decrypted, and obtain the plaintext output.
   * EVP_DecryptUpdate can be called multiple times if necessary
   */
  if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
    handleErrors();
  plaintext_len = len;

  /* Finalise the decryption. Further plaintext bytes may be written at
   * this stage.
   */
  if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) handleErrors();
  plaintext_len += len;

  /* Clean up */
  EVP_CIPHER_CTX_free(ctx);

  return plaintext_len;
}

void SetUpEnc()
{
	/* Set up the key and iv. Do I need to say to not hard code these in a
   * real application? :-)
   */

  /* A 256 bit key */
  unsigned char *key = (unsigned char *)"01234567890123456789012345678901";

  /* A 128 bit IV */
  unsigned char *iv = (unsigned char *)"0123456789012345";

  /* Message to be encrypted */
  unsigned char *plaintext =
                (unsigned char *)"The quick brown fox jumps over the lazy dog";

  /* Buffer for ciphertext. Ensure the buffer is long enough for the
   * ciphertext which may be longer than the plaintext, dependant on the
   * algorithm and mode
   */
  unsigned char ciphertext[128];

  /* Buffer for the decrypted text */
  unsigned char decryptedtext[128];

  int decryptedtext_len, ciphertext_len;

  /* Initialise the library */
  ERR_load_crypto_strings();
  OpenSSL_add_all_algorithms();
  OPENSSL_config(NULL);

  /* Encrypt the plaintext */
  ciphertext_len = encrypt (plaintext, strlen ((char *)plaintext), key, iv,
                            ciphertext);

  /* Do something useful with the ciphertext here */
  printf("Ciphertext is:\n");
  BIO_dump_fp (stdout, (const char *)ciphertext, ciphertext_len);

  /* Decrypt the ciphertext */
  decryptedtext_len = decrypt(ciphertext, ciphertext_len, key, iv,
    decryptedtext);

  /* Add a NULL terminator. We are expecting printable text */
  decryptedtext[decryptedtext_len] = '\0';

  /* Show the decrypted text */
  printf("Decrypted text is:\n");
  printf("%s\n", decryptedtext);

  /* Clean up */
  EVP_cleanup();
  ERR_free_strings();
}



int main()
{
	std::ifstream input("bin.CAP", std::ios::binary);
	std::vector<char> buffer((std::istreambuf_iterator<char>(input)),(std::istreambuf_iterator<char>()));

	ParseCAP(buffer);
	SetUpEnc();
	//printf("press any key to exit!\n");
	//getchar();
	
    return 0;
}

