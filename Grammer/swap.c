#include <stdio.h>

void swap(int *a, int *b);
int main() {
	int a = 3;
	int b = 5;

	swap(&a, &b);

	printf("a:%d  b:%d\n", a, b);

	//a�� b�� swap�ϴ� �Լ��� ȣ��
	
	printf("a:%d  b:%d\n", a, b);
}
void swap(int *a, int *b) {
	int temp = (*b);
	*b = (*a);
	*a = temp;
}