#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>

// compilare: gcc 51.c -o 51 -lrt

// in caz de pica - de vazut man 2 mmap

int main(int argc, char* argv[]) {
	char shm_name[] = "collatz";
	int shm_fd;

	shm_fd = shm_open(shm_name, O_CREAT|O_RDWR, S_IRUSR|S_IWUSR); // creez obiectul de memorie partajata

	if (shm_fd < 0) {
		perror(NULL);
		return errno;
	}

	size_t shm_size = getpagesize() * argc; // dimensiunea obiectului
	size_t page_size = getpagesize(); // dimensiunea fiecarei trunchieri

	if (ftruncate(shm_fd, shm_size) == -1) {
		perror(NULL);
		shm_unlink(shm_name);
		return errno;
	}


    int k;
	char* shm_ptr;
    	for (k = 1; k < argc; ++k) {
		shm_ptr = mmap(0, page_size, PROT_WRITE, MAP_SHARED, shm_fd, (k - 1) * page_size);

		if (shm_ptr == MAP_FAILED) {
			perror(NULL);
			shm_unlink(shm_name);
			return errno;
		}

        pid_t pid = fork();
        if (pid < 0) {
            return errno;
        }


        else if (pid == 0) {
            /* child instructions */
            long long i = atoi(argv[k]);
            int print_code = sprintf(shm_ptr, "%lld: %lld ", i, i);

			if (print_code < 0) {
				perror(NULL);
				shm_unlink(shm_name);
				return errno;
			}

			shm_ptr += print_code;
            while (i > 1) {
                if (i % 2) {
                    i = 3 * i + 1;
                }
                else {
                    i = i / 2;
                }

                print_code = sprintf(shm_ptr, "%lld ", i);
				if (print_code < 0) {
					perror(NULL);
					shm_unlink(shm_name);
					return errno;
				}
				shm_ptr += print_code;
            }
			printf("Done Parent %d Me %d\n",  getppid(), getpid());
            exit(0);
        } 
		int munmap_value = munmap(shm_ptr, page_size);

		if (munmap_value < 0) {
			perror(NULL);
			shm_unlink(shm_name);
			return errno;
		}
	}
    

	int i;

	for (i = 1; i < argc; ++i) {
		wait(NULL);
	}

	for (i = 1; i < argc; ++i) {
		shm_ptr = mmap(0, page_size, PROT_READ, MAP_SHARED, shm_fd, (i - 1) * page_size);

		if (shm_ptr == MAP_FAILED) {
			perror(NULL);
			shm_unlink(shm_name);
			return errno;
		}

		printf("%s\n", shm_ptr);

		int munmap_value = munmap(shm_ptr, page_size);

		if (munmap_value < 0) {
			perror(NULL);
			shm_unlink(shm_name);
			return errno;
		}
	}

	shm_unlink(shm_name);

	printf("Done Parent %d Me %d\n",  getppid(), getpid());
	return 0;
}
