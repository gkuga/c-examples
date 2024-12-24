#include <stdio.h>
#include <stdlib.h>

struct queue_node {
  int data;
  struct queue_node *next;
};

struct queue {
  struct queue_node *head;
  struct queue_node *tail;
};

void init_queue(struct queue *q) {
  q->head = NULL;
  q->tail = NULL;
}

void enqueue(struct queue *q, int value) {
  struct queue_node *new_node =
      (struct queue_node *)malloc(sizeof(struct queue_node));
  if (new_node == NULL) {
    printf("Memory allocation failed\n");
    return;
  }
  new_node->data = value;
  new_node->next = NULL;

  if (q->tail == NULL) {
    q->head = new_node;
    q->tail = new_node;
  } else {
    q->tail->next = new_node;
    q->tail = new_node;
  }
}

int dequeue(struct queue *q) {
  if (q->head == NULL) {
    printf("Queue is empty\n");
    return -1;
  }

  struct queue_node *temp = q->head;
  int value = temp->data;

  q->head = q->head->next;
  if (q->head == NULL) {
    q->tail = NULL;
  }

  free(temp);
  return value;
}

int main() {
  struct queue q;
  init_queue(&q);

  enqueue(&q, 10);
  enqueue(&q, 20);
  enqueue(&q, 30);

  printf("Dequeued: %d\n", dequeue(&q));
  printf("Dequeued: %d\n", dequeue(&q));

  enqueue(&q, 40);

  printf("Dequeued: %d\n", dequeue(&q));
  printf("Dequeued: %d\n", dequeue(&q));
  printf("Dequeued: %d\n", dequeue(&q));

  return 0;
}
