#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	pid_t pid = fork();
	if (pid < 0) {
		return errno;
	}

	else if (pid == 0) {
		/* child instructions*/
		long long i = atoi(argv[1]);
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
	}

	else {
		/* parent instructions */
		wait(NULL);
		printf("\nChild %d finished\n", pid);
	}

	return 0;
}
