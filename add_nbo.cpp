
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

	a1 = ntohl(a1);
	a2 = ntohl(a2);
	
	fclose(fp1);
	fclose(fp2);

	printf("%d(0x%x) + %d(0x%x) = %d(0x%x) \n", a1, a1, a2, a2, a1+a2);
}

