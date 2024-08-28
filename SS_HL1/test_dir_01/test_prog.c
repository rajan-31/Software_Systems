#include <stdio.h>

int main(int argc, char const *argv[]) {
	printf("This line is in test_prog.c\n");

	if(argc > 1) {
		printf("Input/argument given is: %s\n\n", argv[1]);
	}

	return 0;
}
