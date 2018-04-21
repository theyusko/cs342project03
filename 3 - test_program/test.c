#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int sum(int i);

int main() {
	printf("pid: %d\n", getpid());
	printf("Press a key to start the test...\n");
	getchar();
	
	printf("Heap Test...\n");
	int* yaprak = (int*)malloc(sizeof(int) * 342);
	
	printf("Press a key to restore the heap...\n");
	getchar();
	getchar();
	free(yaprak);
	
	printf("Stack Test...\n");
	sum(3245);

	return 0;
}

int sum(int a) {
	if(a == 0) {
		printf("Press a key to restore the stack...\n");
		getchar();
		return 0;
	}
	else
		return 1 + sum(a - 1);
}
