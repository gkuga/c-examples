#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *worker(void *arg) {
  sem_t *semaphore = (sem_t *)arg;

  printf("Worker waiting...\n");
  sem_wait(semaphore);
  printf("Worker proceeding...\n");
  sem_post(semaphore);

  return NULL;
}

int success_func() {
  pthread_t threads[2];
  sem_t *semaphore;

  semaphore = sem_open("/example_semaphore", O_CREAT | O_EXCL, 0644, 1);
  if (semaphore == SEM_FAILED) {
    perror("sem_open");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < 2; i++) {
    pthread_create(&threads[i], NULL, worker, (void *)semaphore);
  }

  for (int i = 0; i < 2; i++) {
    pthread_join(threads[i], NULL);
  }

  sem_close(semaphore);
  sem_unlink("/example_semaphore");

  return 0;
}

int counter = 0; // shared resouce at error_func

void *worker2() {
  for (int i = 0; i < 1000; i++) {
    usleep(rand() % 10);
    counter++;
  }
  return NULL;
}

int error_func() {
  pthread_t threads[100];

  for (int i = 0; i < 100; i++) {
    pthread_create(&threads[i], NULL, worker2, NULL);
  }

  for (int i = 0; i < 100; i++) {
    pthread_join(threads[i], NULL);
  }

  printf("Final counter value: %d\n", counter);
  return 0;
}

int main() {
  printf("Running success func...\n");
  success_func();

  printf("Running error func...\n");
  error_func();

  return 0;
}
