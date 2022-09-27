
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <netinet/in.h>

int main(int argc, char* argv[]){

	if(argc != 3) return 0;

	FILE *fp1, *fp2;

	fp1 = fopen(argv[1], "rb");

	uint32_t buffer1;
	uint32_t a1 = fread(&buffer1, 1, sizeof(buffer1), fp1);

	fp2 = fopen(argv[2], "rb");

	uint32_t buffer2;
	uint32_t a2 = fread(&buffer2, 1, sizeof(buffer2), fp2);

	buffer1 = ntohl(buffer1);
	buffer2 = ntohl(buffer2);
	
	fclose(fp1);
	fclose(fp2);

	printf("%d(0x%x) + %d(0x%x) = %d(0x%x) \n", buffer1, buffer1, buffer2, buffer2, buffer1+buffer2);
}

