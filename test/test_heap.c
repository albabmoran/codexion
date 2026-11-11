
#include "codexion.h"

static void test_scheduler(t_scheduler scheduler, const char *name)
{
    t_heap      heap;
    t_request   storage[3];
    t_request   request;
    int         i;

    heap.requests = storage;
    heap.size = 0;
    heap.capacity = 3;
    heap.scheduler = scheduler;

    request.coder = NULL;
    request.request_time = 300;
    request.deadline = 900;
    heap_push(&heap, request);

    request.request_time = 100;
    request.deadline = 700;
    heap_push(&heap, request);

    request.request_time = 200;
    request.deadline = 500;
    heap_push(&heap, request);

    printf("\n--- %s ---\n", name);
    i = 0;
    while (i < 3)
    {
        if (!heap_pop(&heap, &request))
            break;
        printf("request_time: %lu | deadline: %lu\n",
            (unsigned long)request.request_time,
            (unsigned long)request.deadline);
        i++;
    }
}

int main(void)
{
    test_scheduler(FIFO, "FIFO");
    test_scheduler(EDF, "EDF");
    return (0);
}
