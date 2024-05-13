#include "bch.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int             seed;
int             numerr, errpos[10], decerror = 0;


void int_to_bit_table(int source,unsigned char *data){

  union{
    unsigned char  chr[4];
    unsigned int u32;
  }pice;
  // pice.u32 = source & 0x7FFFFFFF;
  pice.u32 = source & 0x1FFFFF;
  unsigned char temp[32] = {0};

  // printf("source: %2x \r\n",pice.u32);
  // printf("chr[0] = %2x ",pice.chr[3]);
  // printf("chr[1] = %2x ",pice.chr[2]);
  // printf("chr[2] = %2x ",pice.chr[1]);
  // printf("chr[3] = %2x ",pice.chr[0]);
  // printf("\r\n");
  int k = 0;
  for(int i = 0;i < 4;i++){
    for(int j = 0;j < 8;j++){
      if(pice.chr[3 - i] & (0x01 << (7 - j))){
        temp[k] = 1;
      }
      else{
        temp[k] = 0;
      }
      k++;
    }
  }
  // for(int i = 0;i < 32;i++){
  //   printf("%d",temp[i]);
  // }
  // printf("\r\n");

  // for(int i = 0;i < 26;i++){
  for(int i = 0;i < 21;i++){
    // printf("%d",temp[32-21+i]);
    data[i] = temp[32-21+i];
  }
  // printf("\r\n");
}

int bit_to_int(unsigned char *data){
  int result = 0;

  unsigned char temp[32] = {0};
  temp[0] = 0;
  for(int i = 0;i < 31;i++){
    temp[1 + i] = data[i];
  }

  for(int i = 0;i < 32;i++){
      if(temp[i] == 1)
        result += (temp[i] << (31 - i));
  }

  return result;
}



void main(int argc,char * argv[]){
  // unsigned char recd[31], bb[5];
  unsigned char recd[31], bb[31 - 21];
  char str[10];

  int length = 31;
  int m = 5;
  int t = 2;
	int i = 0;
  unsigned char data[21] = {0};


  // int n = 0x3BFFFFF;
  int n = 0x1FFFFF;

  if(argc == 2){
    printf("input epc number is %s\n",argv[1]);
    n = (int)strtol(argv[1],NULL,16);
    // if (n > 0x3FFFFFF){
    if(n > 0x1FFFFF){
      return;
    }
  }

	read_p(m,length);               /* Read m */
	generate_gf();          /* Construct the Galois Field GF(2**m) */
	gen_poly(t);             /* Compute the generator polynomial of BCH code */
  int k = get_K_value();
  printf("k = %d\r\n",k);

  FILE *fp = NULL;
  fp = fopen("./epclist.epc_config","w");


  int_to_bit_table(n,data);
  encode_bch(bb,data);           /* encode data */

  for (i = 0; i < length - k; i++)
    recd[i] = bb[i];
  for (i = 0; i < k; i++)
    recd[i + length - k] = data[i];


  printf("c(x) = ");
  fputs("c(x) = ",fp);

  for (i = 0; i < length; i++){
    printf("%1d", recd[i]);
    sprintf(str,"%d",recd[i]);
    fputs(str,fp);

  }


  int ff = bit_to_int(recd);

  printf(",[EPC_ID = ");
  printf("%2x",ff);

  fputs(",[EPC_ID = ",fp);
  sprintf(str,"%2x",ff);
  fputs(str,fp);

  printf("]\n");
  fputs("]\n",fp);

  fclose(fp);

	printf("Enter the number of errors: ");
	scanf("%d", &numerr);	/* CHANNEL errors */
    printf("Enter error locations (integers between");
    printf(" 0 and %d): ", length-1);
	/*
	 * recd[] are the coefficients of r(x) = c(x) + e(x)
	 */
	for (i = 0; i < numerr; i++)
		scanf("%d", &errpos[i]);
	if (numerr)
		for (i = 0; i < numerr; i++)
			recd[errpos[i]] ^= 1;
	printf("r(x) = ");
	for (i = 0; i < length; i++) {
		printf("%1d", recd[i]);
	}
	printf("\n");





	int er = decode_bch(recd);             /* DECODE received codeword recv[] */
	/*
	 * print out original and decoded data
	 */
	printf("Results: [%d]\n",er);
	printf("original data  = ");
	for (i = 0; i < k; i++) {
		printf("%1d", data[i]);
	}
	printf("\nrecovered data = ");
	for (i = length - k; i < length; i++) {
		printf("%1d", recd[i]);
	}
	printf("\n");
	/*
	 * DECODING ERRORS? we compare only the data portion
	 */
	for (i = length - k; i < length; i++)
		if (data[i - length + k] != recd[i])
			decerror++;
	if (decerror)
	   printf("There were %d decoding errors in message positions\n", decerror);
	else
	   printf("Succesful decoding\n");


}