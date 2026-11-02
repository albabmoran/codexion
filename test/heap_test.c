
#include "codexion.h"
#include <stdio.h>

static void insert_request(t_heap *heap, int id,
        uint64_t request_time, uint64_t deadline)
{
    t_request req;

    req.coder = NULL;
    req.request_time = request_time;
    req.deadline = deadline;

    if (!heap_push(heap, req))
        printf("Error inserting request %d\n", id);
    else
        printf("Inserted request %d\n", id);
}

static void extract_requests(t_heap *heap)
{
    t_request req;

    while (heap_pop(heap, &req))
        printf("request_time: %llu | deadline: %llu\n",
            (unsigned long long)req.request_time,
            (unsigned long long)req.deadline);

    printf("Heap empty: ");
    if (!heap_pop(heap, &req))
        printf("OK\n");
}

int main(void)
{
    t_heap heap;

    printf("\n--- FIFO TEST ---\n");
    heap.size = 0;
    heap.capacity = 4;
    heap.scheduler = FIFO;
    heap.requests = malloc(sizeof(t_request) * heap.capacity);

    insert_request(&heap, 1, 10, 100);
    insert_request(&heap, 2, 20, 30);
    insert_request(&heap, 3, 5, 80);
    insert_request(&heap, 4, 15, 50);

    extract_requests(&heap);
    free(heap.requests);

    printf("\n--- EDF TEST ---\n");
    heap.size = 0;
    heap.capacity = 4;
    heap.scheduler = EDF;
    heap.requests = malloc(sizeof(t_request) * heap.capacity);

    insert_request(&heap, 1, 10, 100);
    insert_request(&heap, 2, 20, 30);
    insert_request(&heap, 3, 5, 80);
    insert_request(&heap, 4, 15, 50);

    extract_requests(&heap);
    free(heap.requests);

    return (0);
}
