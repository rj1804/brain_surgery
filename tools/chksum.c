#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include <sys/stat.h>

static uint32_t checksum(const uint8_t *src, int size)
{
	uint32_t sum = 0;
	
	int i;
	for( i = 0; i < size; i++ ) {
		sum += *src++;
	}

	return sum;
}

int main( int argc, char **argv)
{
	if( argc < 3 ) {
		fprintf(stderr, "chk <infile.bin> <outfile.bin>\n");
		return 1;
	}

	struct stat st;
	stat(argv[1], &st);
	uint32_t src_size = st.st_size;	

	FILE *src = fopen(argv[1], "rb");

	printf("infile:   %s  size %d\n", argv[1], src_size);
	
	if( !src ) {
		fprintf(stderr, "cannot open infile!\n");
		exit(1);
	}

	if( !src_size > 262144 ) {
		fprintf(stderr, "file too big!\n");
		exit(1);
	}

	// file must be at least 65k
	uint32_t out_size = src_size > 65536 ? src_size : 65536;
	
	uint8_t data[262144];
	memset(data, 0, out_size );
	fread(data, src_size  , 1, src);

	uint32_t chk = checksum(data, out_size);
	uint32_t cafe = 0xEFACEFAC;
	uint32_t null = 0x0;

	printf("checksum: %08X\n", chk );

	fclose(src);

	FILE *dst = fopen(argv[2], "wb");

	if( !dst ) {
		fprintf(stderr, "cannot open outfile!\n");
		exit(1);
	}

	printf("outfile:  %s  size %d\n", argv[2], out_size);

	fwrite(&out_size, 4, 1, dst);
	fwrite(&chk,  4, 1, dst);
	fwrite(&cafe, 4, 1, dst);
	fwrite(&null, 4, 1, dst);

	fwrite(data, out_size, 1, dst);

	fclose(dst);

	return 0;
}
