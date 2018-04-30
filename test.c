#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int sum(int i);

struct test {
	char keyword[1024];
	int size;
	struct test* next;
};

int main() {
	printf("pid: %d\n", getpid());
	printf("Press a key to start the test: \n");
	getchar();
	getchar();
	
	printf("Heap Test...\n");
	int i = 0;
	
	struct test* head = (struct test*)malloc(sizeof(struct test));
	struct test* tmp = head;
	printf("i = ");
	while(1) {
		tmp->next = (struct test*)malloc(sizeof(struct test));
		tmp = tmp->next;
		i++;
		if(i%100 == 0) {
			printf("%d", i);
			printf("\nPress '*' to stop heap test, any other key to continue: ");
			char x = getchar();
			if(x == '*')
				break;
			i = 0;
			printf("Next cycle...\n");
			printf("i = ");
		}
		else 
			printf("%d, ", i);
		
		sleep(0.3);
	}
	
	tmp->next = NULL;
	tmp = head;
	while(tmp != NULL) {
		struct test* tmp2 = tmp->next;
		free(tmp);
		tmp = tmp2;
	}
	getchar();
	
	printf("Stack Test...\n");
	getchar();
	getchar();
	sum(50000);
	
	
	printf("Press a key to exit: ");
	getchar();

	return 0;
}

int sum(int a) {
	if(a == 0) {
		printf("Press a key to restore the stack: \n");
		getchar();
		getchar();
		
		return 0;
	}
	else
		return 1 + sum(a - 1);
}
