#include "header.h"

void calc(int page, int* count, int mul)    //set_to_nine과 set_to_zero에서 연산할때 변화되는 연산만큼 count횟수를 변경,이때 자릿수 곱하기
{                                           //ex) 100~199일때 위 공식에따라 1의자리는 0~9가 10번씩, 10의자리는 10~19를 비교해 1-1+1 =1 이때 실제 횟수는 10의자리이므로 *10을 해줘야함
	while(1)
	{
		if(page <= 0)break;                 //최대자릿수를 넘으면 break
		count[page%10] += mul;              //각 숫자의 갯수를 mul(자릿수에 따른 곱) 만큼 더해줌
		page/=10;                           //다음자릿수로 이동
	}
}

int set_to_nine(int end,int start, int mul, int* count)     //마지막 페이지의 끝자리가 9가 되게 변경
{
	while(1)
	{
		if (end %10 == 9) break;                            //끝자리가 9면 break
		if (start > end) break;                             //예외처리
		calc(end,count,mul);
		end--;                                              //끝자리가 9가 될때까지 1씩 빼주며 각 수에 나오는 숫자를 count에 더함 (calc)
	}
	return end;
}

int set_to_zero(int end,int start, int mul, int* count)     //시작 페이지의 끝자리가 9가 되게 변경
{
	while(1)
	{
		if (start %10 == 0) break;                          //끝자리가 0이면 break
		if (start > end) break;                             //예외처리
		calc(start,count,mul);
		start++;                                            //끝자리가 0이 될때까지 1씩 더하며 각 수에 나오는 숫자를 count에 더함 (calc)
	}
    return start;
}
