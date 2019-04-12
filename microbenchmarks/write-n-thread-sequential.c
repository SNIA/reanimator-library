#include <fcntl.h>
#include <getopt.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define TEST_BUF_SIZE 4096
#define MAX_WRITES 1024 * 256
#define FILE_NAME "myfile"

bool fill_random_data(char *randomBuffData, int random_fd) {
  int randomDataLen = 0;
  while (randomDataLen < TEST_BUF_SIZE) {
    ssize_t result = read(random_fd, randomBuffData + randomDataLen,
                          TEST_BUF_SIZE - randomDataLen);
    if (result < 0) {
      // printf("Cannot fill complete 4k data into randomBuffData\n");
      return false;
    }
    randomDataLen += result;
  }
  return true;
}

void *threadFunc(void *arg) {
  char buf[TEST_BUF_SIZE];
  char threadIdBuf[15];
  int fd = -1;
  int numbytes;
  int position = 0;
  int random_fd = -1;
  int64_t error = 0;
  uint64_t threadId = (uint64_t)arg;

  snprintf(threadIdBuf, sizeof(threadIdBuf), "%s%d", FILE_NAME, threadId);
  // printf("Thread with %s started\n", threadIdBuf);

  fd = open(threadIdBuf, O_WRONLY | O_CREAT);
  if (fd < 0) {
    pthread_exit((void *)-1);
  }

  random_fd = open("/dev/urandom", O_RDONLY);
  if (random_fd < 0) {
    // printf("Cannot get file descriptor for /dev/random\n");
    close(fd);
    pthread_exit((void *)-2);
  }

  for (int i = 0; i < MAX_WRITES; i++) {
    position = lseek(fd, 0, SEEK_CUR);
    if (!fill_random_data(buf, random_fd)) {
      // printf("Cannot fill random data buffer\n");
      error = -3;
      break;
    }
    numbytes = write(fd, buf, TEST_BUF_SIZE);
    if (numbytes < 0) {
      // printf("Cannot write random data into file\n");
      error = -4;
      break;
    }
    if (numbytes < TEST_BUF_SIZE) {
      // printf("Seek count=%d\n", i);
      i--;
      if (lseek(fd, position, SEEK_SET) < 0) {
        // perror("lseek could not change pointer");
        error = -6;
        break;
      }
    }
  }

  close(fd);
  close(random_fd);
  // printf("Thread %s terminated with error %d\n", threadIdBuf, error);
  pthread_exit((void *)error);
}

int main(int argc, char *argv[]) {
  char c;
  uint64_t nthreads = 0;

  while ((c = getopt(argc, argv, "n:")) != -1) {
    switch (c) {
    case 'n': {
      nthreads = atoi(optarg);
      break;
    }
    default:
      break;
    }
  }
  if (nthreads == 0) {
    nthreads = 1;
  }
  // printf("number of threads %d\n", nthreads);
  if (nthreads == 1) {
    threadFunc((void *)nthreads);
  } else {
    pthread_t *array = malloc(sizeof(pthread_t) * nthreads);
    for (uint64_t i = 0; i < nthreads; i++) {
      pthread_create(&array[i], NULL, threadFunc, (void *)(i + 1));
    }
    for (int i = 0; i < nthreads; i++) {
      int threadRetVal = pthread_join(array[i], NULL);
    }
  }

  exit(0);
}