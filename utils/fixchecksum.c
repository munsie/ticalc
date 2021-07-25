#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main(int argc, char *argv[]) {
  char *progname = strrchr(argv[0], '/');
  progname = progname ? progname + 1 : argv[0];
  
  if(argc != 2) {
    fprintf(stderr, "%s: a TI graphing calculator file must be specified on the command line.\n", progname);
    exit(EXIT_FAILURE);
  }
  char *filename = argv[1];
  int fd = open(filename, O_RDWR);
  if(fd < 0) {
    perror(filename);
    exit(EXIT_FAILURE);
  }
  struct stat buf;
  if(fstat(fd, &buf) < 0) {
    perror(filename);
    exit(EXIT_FAILURE);
  }
  uint8_t *ptr = (uint8_t *)mmap(NULL, buf.st_size, PROT_READ|PROT_WRITE, MAP_FILE|MAP_SHARED, fd, 0);
  if(ptr == MAP_FAILED) {
    perror(filename);
    exit(EXIT_FAILURE);
  }
  close(fd);

  // look for the signature
  if(strncmp((char *)ptr, "**TI83F*", 8)) {
    fprintf(stderr, "%s: signature mismatch\n", filename);
    exit(EXIT_FAILURE);
  }
  // calculate the checksum
  uint16_t checksum = 0;
  for(unsigned i = 0x37; i < buf.st_size - 2; i++) {
    checksum += ptr[i];
  }
  printf("Calculated checksum: 0x%4.4x\n", checksum);
  uint16_t old_checksum = ptr[buf.st_size - 2] | ptr[buf.st_size - 1] << 8;
  printf("Checksum in file:    0x%4.4x\n", old_checksum);
  if(checksum == old_checksum) {
    printf("Checksums match, no change needed.\n");
  } else {
    printf("Checksums differ, updating checksum in the file.\n");
    ptr[buf.st_size - 2] = checksum & 0xff;
    ptr[buf.st_size - 1] = (checksum >> 8) & 0xff;
  }

  munmap(ptr, buf.st_size);
  return 0;
}
