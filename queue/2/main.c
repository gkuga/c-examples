#define _POSIX_C_SOURCE 200809L
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct queue_node {
  struct queue_node *next;
};

struct response_data {
  struct queue_node node;
  sem_t *complete;
  void *data;
};

struct queue {
  struct queue_node *head;
  struct queue_node *tail;
};

void init_queue(struct queue *q) {
  q->head = NULL;
  q->tail = NULL;
}

void enqueue(struct queue *q, struct queue_node *node) {
  if (q->tail == NULL) {
    q->head = node;
    q->tail = node;
  } else {
    q->tail->next = node;
    q->tail = node;
  }
}

struct queue_node *dequeue(struct queue *q) {
  if (q->head == NULL) {
    printf("Queue is empty\n");
    return NULL;
  }

  struct queue_node *temp = q->head;
  struct queue_node *resp = temp;

  q->head = q->head->next;
  if (q->head == NULL) {
    q->tail = NULL;
  }

  return resp;
}

int main() {
  struct queue q;
  init_queue(&q);

  struct response_data resp1, resp2, resp3;

  resp1.complete = sem_open("/sem_resp1", O_CREAT | O_EXCL, 0644, 0);
  resp2.complete = sem_open("/sem_resp2", O_CREAT | O_EXCL, 0644, 0);
  resp3.complete = sem_open("/sem_resp3", O_CREAT | O_EXCL, 0644, 0);

  if (resp1.complete == SEM_FAILED || resp2.complete == SEM_FAILED ||
      resp3.complete == SEM_FAILED) {
    perror("sem_open");
    exit(EXIT_FAILURE);
  }

  resp1.data = "Data 1";

  resp2.data = "Data 2";

  resp3.data = "Data 3";

  enqueue(&q, &resp1.node);
  enqueue(&q, &resp2.node);
  enqueue(&q, &resp3.node);

  struct response_data *deq_resp;
  while ((deq_resp = (struct response_data *)dequeue(&q)) != NULL) {
    printf("Dequeued Response:\n");
    printf("  Data: %s\n", (char *)deq_resp->data);

    sem_close(deq_resp->complete);
  }
  sem_unlink("/sem_resp1");
  sem_unlink("/sem_resp2");
  sem_unlink("/sem_resp3");

  return 0;
}
