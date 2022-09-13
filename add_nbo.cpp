#include <stdio.h>
#include <stdint.h>
#include <netinet/in.h>

int main(){
	FILE *fp1, *fp2;

	fp1 = fopen("thousand.bin", rb);

	uint32_t buffer1;
	uint32_t a1 = fread(&buffer1, 1, 1, fp1);

	fp2 = fopen("five-hundred.bin", rb);

	uint32_t buffer2;
	uint32_t a2 = fread(&buffer2, 1, 1, fp2);

	a1 = uint32_t ntohl(a1);
	a2 = uint32_t ntohl(a2);

	printf("%d\n", a1+a2);
}

