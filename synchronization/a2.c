#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

sem_t semBread, semCheese, semLettuce;
sem_t tableEmpty;
pthread_mutex_t mutex;

int remaining;   

void *supplier(void *arg);
void *A(void *arg);
void *B(void *arg);
void *C(void *arg);

int main() {
    pthread_t t[4];
    srand(time(NULL));

    printf("Enter number of times supplier places ingredients: ");
    scanf("%d", &remaining);
    printf("\n");

    pthread_mutex_init(&mutex, NULL);
    sem_init(&semBread, 0, 0);
    sem_init(&semCheese, 0, 0);
    sem_init(&semLettuce, 0, 0);
    sem_init(&tableEmpty, 0, 1);

    pthread_create(&t[0], NULL, supplier, NULL);
    pthread_create(&t[1], NULL, A, NULL);
    pthread_create(&t[2], NULL, B, NULL);
    pthread_create(&t[3], NULL, C, NULL);

    for (int i = 0; i < 4; i++) pthread_join(t[i], NULL);

    pthread_mutex_destroy(&mutex);
    sem_destroy(&semBread);
    sem_destroy(&semCheese);
    sem_destroy(&semLettuce);
    sem_destroy(&tableEmpty);

    return 0;
}

void *supplier(void *arg) {
    char *ingredients[] = {"Bread", "Cheese", "Lettuce"};

    while (1) {
        sem_wait(&tableEmpty);
        pthread_mutex_lock(&mutex);
        if (remaining == 0) {
            pthread_mutex_unlock(&mutex);
            sem_post(&semBread);
            sem_post(&semCheese);
            sem_post(&semLettuce);
            sem_post(&tableEmpty);

            break;
        }
        pthread_mutex_unlock(&mutex);

        int first = rand() % 3;
        int second;
        do {
            second = rand() % 3;
        } while (second == first);

        printf("Supplier places: %s and %s\n",
               ingredients[first], ingredients[second]);

        int hasBread = (first == 0 || second == 0);
        int hasCheese = (first == 1 || second == 1);
        int hasLettuce = (first == 2 || second == 2);
        int missing;

        if (!hasBread) missing = 0;
        else if (!hasCheese) missing = 1;
        else missing = 2;

        if (missing == 0) sem_post(&semBread);
        else if (missing == 1) sem_post(&semCheese);
        else sem_post(&semLettuce);
    }

    return NULL;
}

void *A(void *arg) {
    while (1) {
        sem_wait(&semBread);

        pthread_mutex_lock(&mutex);
        if (remaining == 0) {
            pthread_mutex_unlock(&mutex);
            pthread_exit(NULL);
        }
        remaining--;
        pthread_mutex_unlock(&mutex);

        printf("Maker A picks up Cheese and Lettuce\n");
        printf("Maker A is making the sandwich...\n");
        printf("Maker A finished making the sandwich and eats it\n");
        printf("Maker A signals Supplier\n");
        printf("\n");

        sem_post(&tableEmpty);
    }
}

void *B(void *arg) {
    while (1) {
        sem_wait(&semCheese);

        pthread_mutex_lock(&mutex);
        if (remaining == 0) {
            pthread_mutex_unlock(&mutex);
            pthread_exit(NULL);
        }
        remaining--;
        pthread_mutex_unlock(&mutex);

        printf("Maker B picks up Bread and Lettuce\n");
        printf("Maker B is making the sandwich...\n");
        printf("Maker B finished making the sandwich and eats it\n");
        printf("Maker B signals Supplier\n");
        printf("\n");

        sem_post(&tableEmpty);
    }
}

void *C(void *arg) {
    while (1) {
        sem_wait(&semLettuce);

        pthread_mutex_lock(&mutex);
        if (remaining == 0) {
            pthread_mutex_unlock(&mutex);
            pthread_exit(NULL);
        }
        remaining--;
        pthread_mutex_unlock(&mutex);

        printf("Maker C picks up Bread and Cheese\n");
        printf("Maker C is making the sandwich...\n");
        printf("Maker C finished making the sandwich and eats it\n");
        printf("Maker C signals Supplier\n");
        printf("\n");

        sem_post(&tableEmpty);
    }
}
