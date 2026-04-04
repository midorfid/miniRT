#include <pthread.h>
#include <stdio.h>

void* f(void* arg) {
    return NULL;
}

int main() {
    pthread_t t;
    pthread_create(&t, NULL, f, NULL);
    pthread_join(t, NULL);
    return 0;
}