#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

void collatz(long long i) {
     printf ("%lld ", i);
    while (i > 1) {

        if (i % 2) {
            i = 3 * i + 1;
        }

        else {
            i = i / 2;
        }
        printf("%lld ", i);
    }
    printf("\n");
}

int main(int argc, char* argv[]) {
    int k;
    
    if (argc < 2) {
        printf("Invalid arguments\n");
        return 0;
    }

    for (k = 1; k < argc; ++k) {
        pid_t pid = fork();
        if (pid < 0) {
            return errno;
        }

        else if (pid == 0) {
            /* child instructions*/
            long long i = atoi(argv[k]);
            printf ("%lld: ", i);
            collatz(i);
            printf("Done. Parent %d, Me %d\n", getppid(), getpid());

            // exit(0) - terminarea procesului fara nicio eroare

            exit(0); // ies din procesul copil dupa ce am calculat ipoteza collatz pentru numarul desemnat lui, pentru a nu crea o structura arborescenta de procese
        } 
        else { 
            /* parent instructions */
            // wait(NULL); // aceasta 'metoda' face procesul parinte sa astepte fiecare copil in parte, nerespectand paralelizarea
        }
    } 
         for (k = 1; k < argc; ++k) { // aceasta 'metoda' face procesul parinte sa astepte toate procesele copil
                wait(NULL);
        } 
	return 0;
}
