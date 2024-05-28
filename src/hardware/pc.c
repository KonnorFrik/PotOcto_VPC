#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h> 
#include <unistd.h>
#include <fcntl.h>

#include "pc.h"
#include "instructions_executors.h"
#include "../common/error_codes.h"

void usage(const char* prog_name) {
    fprintf(stderr, "Usage: %s <bin file>\n", prog_name);
}

void executor(PC* vpc) {
    int execute = 1;

    while (execute) {
        word instr_code = vpc->memory[vpc->cpu.IP];
        vpc->cpu.IP++;
        word operand_1 = vpc->memory[vpc->cpu.IP];
        vpc->cpu.IP++;
        word operand_2 = vpc->memory[vpc->cpu.IP];
        vpc->cpu.IP++;

        execute = do_instruction(vpc, instr_code, operand_1, operand_2);
    }
}

int main(const int argc, const char** argv) {
    if (argc < 2) {
        usage(argv[0]);
        exit(WRONG_ARGS);
    }

    struct stat file_stat;
    int fd = open(argv[1], O_RDONLY);

    if (fd < 0) {
        perror("Can't open file: ");
        exit(FILE_ERROR);
    }

    if (fstat(fd, &file_stat) < 0) {
        perror("File Stat's Error: ");
        exit(FILE_ERROR);
    }

    void* file_map = NULL;
    file_map = mmap(0, file_stat.st_size, PROT_READ, MAP_SHARED, fd, 0);

    if (file_map == MAP_FAILED) {
        perror("MAP Fail: ");
        exit(MAP_ERROR);
    }

    if (MEM_SIZE < file_stat.st_size) {
        fprintf(stderr, "File too large for memory\n");
        exit(FILE_ERROR);
    }

    PC vpc = {0};
    memcpy(vpc.memory, file_map, file_stat.st_size);
    executor(&vpc);

    printf("PC: STATS:\n");
    printf("IP: dec(%d):hex(%x)\n", vpc.cpu.IP, vpc.cpu.IP);
    printf("MP: dec(%d):hex(%x)\n", vpc.cpu.MP, vpc.cpu.MP);
    printf("Registers:\n");

    for (int i = 0; i < 10/*REG_COUNT*/; ++i) {
        printf("%d - dec(%d):hex(%x)\n", i, vpc.cpu.reg[i], vpc.cpu.reg[i]);
    }

    printf("\nMemory Dump:\n");
    mem_dump(stdout, vpc.memory, 256);

    return OK;
}
