#include "header.h"

void Print(int* count)                  //count[]내부의 0~9 숫자들의 횟수 출력
{
	for(int i=0; i<10; i++)
		printf("%d ", count[i]);
}
