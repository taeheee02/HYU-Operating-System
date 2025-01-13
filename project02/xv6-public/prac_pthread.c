//prac_pthread.c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_THREADS 4
#define MAX_NUM 40000000

int arr[MAX_NUM];
int result[MAX_NUM];

int check_sorted(int n)
{
    int i;
    for (i = 0; i < n; i++)
        if (arr[i] != i)
            return 0;
    return 1;
}

// Implement your solution here
void merge(int left, int mid, int right){
  int n1 = left;
  int n2 = mid + 1;
  int k = left;

  while(n1 <= mid && n2 <= right){
    if(arr[n1] <= arr[n2])
      result[k++] = arr[n1++];
    else
      result[k++] = arr[n2++];
  }

  if(n1 > mid){
    for(int i = n2; i <= right; i++)
      result[k++] = arr[i];
  }else{
    for(int i = n1; i <= mid; i++)
      result[k++] = arr[i];
  }

  for(int i = left; i <= right; i++)
    arr[i] = result[i];
}

void merge_sort(int left, int right){
  int mid;
  
  if(left < right){
    mid = (left + right) / 2;
    merge_sort(left, mid);
    merge_sort(mid+1, right);
    merge(left, mid, right);
  }
}

//thread function
void* thread_merge(void* arg){
  long tid = (long) arg;

  int left = (tid - 1) * (MAX_NUM / 4);
  int right = tid * (MAX_NUM / 4) -1;
  
  merge_sort(left, right);

  pthread_exit(NULL);
}

///////////////////////////////

int main(void)
{
    srand((unsigned int)time(NULL));
    const int n = MAX_NUM;
    int i;

    for (i = 0; i < n; i++)
        arr[i] = i;
    for (i = n - 1; i >= 1; i--)
    {
        int j = rand() % (i + 1);
        int t = arr[i];
        arr[i] = arr[j];
        arr[j] = t;
    }

    printf("Sorting %d elements...\n", n);

    // Create threads and execute.
    long t1 = 1, t2 = 2, t3 = 3, t4 = 4;
    pthread_t threads[NUM_THREADS];
    pthread_create(&threads[0], NULL, thread_merge, (void*) t1);
    pthread_create(&threads[1], NULL, thread_merge, (void*) t2);
    pthread_create(&threads[2], NULL, thread_merge, (void*) t3);
    pthread_create(&threads[3], NULL, thread_merge, (void*) t4);

    for(i = 0; i < NUM_THREADS; i++){
      pthread_join(threads[i], NULL);
    }
    
    merge(0, MAX_NUM / 4 - 1, 2 * (MAX_NUM / 4) -1);
    merge(2 * (MAX_NUM / 4), 3 * (MAX_NUM / 4) -1, MAX_NUM -1);
    
    merge(0, 2 * (MAX_NUM / 4) -1, MAX_NUM -1);
    //////////////////////////////

    if (!check_sorted(n))
    {
        printf("Sort failed!\n");
        return 0;
    }

    printf("Ok %d elements sorted\n", n);
    return 0;
}

