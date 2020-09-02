#include <stdio.h>
#include <unistd.h>
#include <errno.h>


int main() {
	char buffer[]  = "Hello, World!\n";
	size_t count =  sizeof(buffer) / sizeof(char);
	if (write(1, buffer, count) != count) { // 1 - stdout, buffer - string, count - number of characters
		perror("Write error!\n");
		return errno;
	};
	return 0;
}
