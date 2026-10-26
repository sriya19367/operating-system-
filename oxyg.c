#include <stdio.h>
#include <pthread.h>

int spo2;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *sensor(void *arg) {
    pthread_mutex_lock(&lock);
    printf("Thread 1 [Sensor]: Current SpO2 reading = %d%%\n", spo2);
    pthread_mutex_unlock(&lock);
    return NULL;
}

void *monitor(void *arg) {
    pthread_mutex_lock(&lock);
    printf("Thread 2 [Monitor]: Status - ");
    if (spo2 >= 95 && spo2 <= 100) {
        printf("NORMAL / SAFE (Healthy oxygenation)\n");
    } else if (spo2 >= 90 && spo2 < 95) {
        printf("ACCEPTABLE / MILD LOW (Monitor closely)\n");
    } else if (spo2 < 90 && spo2 >= 0) {
        printf("CRITICAL ALERT (Hypoxia risk - low SpO2!)\n");
    } else {
        printf("INVALID READING (Enter value between 0-100)\n");
    }
    pthread_mutex_unlock(&lock);
    return NULL;
}

void *alert(void *arg) {
    pthread_mutex_lock(&lock);
    if (spo2 < 90 && spo2 >= 0) {
        printf("Thread 3 [Alert]: * TRIGGERING EMERGENCY ALARM *\n");
    } else {
        printf("Thread 3 [Alert]: System nominal. No alarm triggered.\n");
    }
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main() {
    pthread_t t1, t2, t3;

    printf("Enter SpO2 level (0-100%%): ");
    if (scanf("%d", &spo2) != 1) {
        printf("Invalid input.\n");
        return 1;
    }

    pthread_create(&t1, NULL, sensor, NULL);
    pthread_create(&t2, NULL, monitor, NULL);
    pthread_create(&t3, NULL, alert, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    pthread_mutex_destroy(&lock);
    printf("Monitoring completed.\n");
    return 0;
}
