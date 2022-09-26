#include "header.h"

int main()
{
    int end;                                         //마지막 페이지
    int start;                                       //시작 페이지, 기본적으로 1
    int count[10];                                   //숫자의 횟수를 저장할 배열
    int mul;                                         //현 자리수를 나타낼 변수
    int N;                                           //입력받을 횟수
    int *testcase;                                   //입력받은 testcase를 저장할 배열
    
    scanf("%d",&N);
    testcase = (int*)malloc(sizeof(int)*N);
    for(int i=0; i<N; i++)
        scanf("%d",&testcase[i]);                    //입력받을 횟수 N과 N개의 testcase 입력
    
    for(int i=0; i<N; i++)
    {
        start = 1;                                      //시작페이지 = 1로 초기화
        end = testcase[i];                              //마지막페이지 = 입력받은 testcase로 초기화
        mul = 1;                                        //자릿수 = 1로 초기화
        for(int j=0; j<10; j++)
            count[j] = 0;                               //0~9 숫자 횟수 초기화
        
        while(1)
        {
            if (start > end) break;                         //예외처리
            end=set_to_nine(end,start,mul,count);           //마지막 페이지 끝자리를 9로
            if (end < start) break;                         //예외처리
            start=set_to_zero(end,start,mul,count);         //시작 페이지 끝자리를 0으로
        
            start/=10;                                      //자리수 변경
            end/=10;
        
            for(int i=0; i<10; i++)                         //위 공식 B/10 - A/10 + 1 이용, 각 숫자 횟수에 더해줌
                count[i] += (end - start + 1) * mul;
        
            mul *= 10;                                      //자리수 변경
        }
        
        Print(count);
        printf("\n");                                       //출력
    }

    return 0;
}
