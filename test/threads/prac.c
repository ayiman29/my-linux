#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

void *funcThread1(void *arg);
void *funcThread2(void *arg);

int main() {
    int n, m;
    pthread_t t1, t2;
    int *list1;

    printf("Enter the term of fibonacci sequence:\n");
    scanf("%d", &n);

    pthread_create(&t1, NULL, funcThread1, &n);
    pthread_join(t1, (void**)&list1);

    for (int i = 0; i <= n; i++) {
        printf("a[%d] = %d\n", i, list1[i]);
    }

    printf("How many numbers you are willing to search?:\n");
    scanf("%d", &m);

    for (int i = 1; i <= m; i++) {
        int idx;
        printf("Enter search %d: \n", i);
        scanf("%d", &idx);

        long *arg = malloc(3 * sizeof(long));
        arg[0] = idx; 
        arg[1] = (long)list1;
        arg[2] = n;

        int *res;
        pthread_create(&t2, NULL, funcThread2, arg);
        pthread_join(t2, (void**)&res);

        printf("Result of search #%d = %d\n", i, *res);

        free(res);
        free(arg);
    }

    free(list1);
    return 0;
}


void *funcThread1(void *arg) {
    int n = *(int*)arg;
    int *a = malloc((n + 1) * sizeof(int));

    if (n >= 0) a[0] = 0;
    if (n >= 1) a[1] = 1;

    for (int i = 2; i <= n; i++) {
        a[i] = a[i - 1] + a[i - 2];
    }

    return a;
}


void *funcThread2(void *arg) {
    long *args = (long*)arg;

    int idx = args[0];
    int *list = (int*)args[1];
    int size = args[2];

    int *result = malloc(sizeof(int));

    if (idx < 0 || idx > size) {
        *result = -1;
    } else {
        *result = list[idx];
    }

    return result;
}
