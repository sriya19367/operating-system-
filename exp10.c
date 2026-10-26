#include <stdio.h>

#define SIZE 5

int main() {
    int buffer[SIZE];
    int front = 0, rear = 0;
    int n, packet, i;

    printf("Network Packet Buffer - FIFO\n");

    printf("Enter number of packets: ");
    scanf("%d", &n);

    for (i = 0; i < n; i++) {
        printf("Enter packet %d: ", i + 1);
        scanf("%d", &packet);

        if (rear < SIZE) {
            buffer[rear] = packet;
            rear++;

            printf("Packet %d added to buffer.\n", packet);
        } 
        else {
            printf("Buffer full! Packet %d removed.\n", buffer[front]);

            buffer[front] = packet;
            front++;

            printf("Packet %d added to buffer.\n", packet);
        }
    }

    printf("\nFinal packets in buffer: ");

    for (i = front; i < rear; i++) {
        printf("%d ", buffer[i]);
    }

    printf("\n");

    return 0;
}
