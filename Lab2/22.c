#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

int main(int argc, char* argv[]) {
    
    char* buf;
    struct stat sb;
    char *readf = argv[1], *writef = argv[2];

    if (stat(readf, &sb)) {
        perror("Error opening file!\n");
        return errno;
    }

    size_t size = sb.st_size;
    int rfdesc = open(readf, O_RDONLY);

    if (rfdesc < 0) {
        perror("Error opening file!\n");
        return errno;
    }

    buf = (char*) malloc(size); // allocating memory for buffer

    if (buf == NULL) {
        perror("Error allocating memory for buffer!\n");
    }

    int nread = read(rfdesc, buf, size);

    if (nread !=  size)  {
        perror("Error reading file!\n");
        free(buf);
        return errno;
    }

    int wfdesc = open(writef, O_WRONLY | O_CREAT | O_TRUNC);
	chmod(writef, 777); // setez permisiuni de modificare a fisierului de scriere pt toti utilizatorii

    if (wfdesc < 0) {
        perror("Error opening / creating file !\n");
        free(buf);
        return errno;
    }

    int nwrite = write(wfdesc, buf, size);

    if (nwrite != size) {
        perror("Error writing file!\n");
        free(buf);
        return errno;
    }

    char buffer[]  = "Program executed successfully!\n";
    size_t count =  sizeof(buffer);

    if (write(1, buffer, count) != count) {
	perror("Write error!\n");
	free(buf);
	return errno;
    };

    free(buf);
    return 0;
}

        perror("Error opening / creating file !\n");
        free(buf);
        return errno;
    }

    int nwrite = write(wfdesc, buf, size);

    if (nwrite != size) {
        perror("Error writing file!\n");
        free(buf);
        return errno;
    }

    char buffer[]  = "Program executed successfully!\n";
    size_t count =  sizeof(buffer);

    if (write(1, buffer, count) != count) {
	perror("Write error!\n");
	free(buf);
	return errno;
    };
    free(buf);
    return 0;
}
