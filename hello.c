#include <stdio.h>
#include <string.h>

int main() {
    char name[10];

    printf("What is your name? ");

    scanf("%s", name);

    printf("Hello %s, this is just for testing purposes.", name);

    return 0;
}
