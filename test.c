#include <stdio.h>
#include <windows.h>

int main() {
    int value = 100;  // this is what you'll scan for

    printf("Test program running...\n");
    printf("PID: %d\n", GetCurrentProcessId());

    while (1) {
        printf("\nCurrent value: %d\n", value);
        printf("Enter new value (or -1 to keep same): ");

        int input;
        scanf("%d", &input);

        if (input != -1) {
            value = input;
        }

        Sleep(1000); // small delay to keep things stable
    }

    return 0;
}