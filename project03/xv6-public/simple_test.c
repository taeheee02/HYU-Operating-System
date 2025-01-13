#include "types.h"
#include "stat.h"
#include "user.h"

thread_t thread;
int status;

void failed()
{
    printf(1, "Test failed!\n");
    exit();
}

void *simple_thread(void *arg)
{
    int val = (int)arg;
    printf(1, "Thread %d start\n", val);
    sleep(100);  // Simulate some work
    status = val;
    printf(1, "Thread %d end\n", val);
    thread_exit(arg);
    return 0;
}

int main(int argc, char *argv[])
{
    int retval;

    printf(1, "Simple Thread Test Start\n");

    if (thread_create(&thread, simple_thread, (void *)42) != 0) {
        printf(1, "Error creating thread\n");
        failed();
    }

    printf(1, "Thread created with TID %d\n", thread);

    if (thread_join(thread, (void **)&retval) != 0) {
        printf(1, "Error joining thread\n");
        failed();
    }

    printf(1, "Thread joined, retval: %d\n", retval);

    if (retval != 42) {
        printf(1, "Thread returned wrong value: %d\n", retval);
        failed();
    }

    if (status != 42) {
        printf(1, "Status not updated correctly: %d\n", status);
        failed();
    }

    printf(1, "Simple Thread Test Passed\n");
    exit();
}

