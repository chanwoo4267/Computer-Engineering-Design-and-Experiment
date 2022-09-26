#include <iostream>
#include <cstring>          //strlen, strcmp, strcpy 사용
using namespace std;

class Str
{
private:
    char *str;      //문자열
    int len;        //문자열 크기
public:
    Str(int leng)
    {
        str = new char[leng];           //크기만 입력받은경우 동적할당만 해놓음
        len = leng;                     //길이저장
    }
    Str(char *neyong)
    {
        len = (int)strlen(neyong);      //길이저장
        str = new char[len];            //동적할당
        strcpy(str,neyong);             //문자열 입력
    }
    ~Str()
    {
        delete[] str;                   //메모리누수 방지
    }// 소멸자.
    int length(void)
    {
        return len;                     //문자열 길이 반환
    }
    char *contents(void)
    {
        return str;                     //문자열 내용 반환
    }
    int compare(class Str& a)           //같은 Str 클래스를 입력받았을때와 단순문자열을 입력받았을때를 구분
    {                                   //Str클래스를 입력받을경우 클래스 내부의 문자열에 접근해야하므로 strcmp 명령어를 사용할때 a.str로 접근해야함
        int i= strcmp(str,a.str);       //입력받은 Str 클래스 a 내부의 문자열과 이 Str의 str 문자열을 비교
        return i;                       //같으면 0 반환, str이 큰경우 양수, a가 큰경우 음수를 반환
    }
    int compare(char *a)                //단순 문자열을 입력받았으므로 str클래스를 입력받았을때와 구분
    {
        int i=strcmp(str,a);            //입력받은 a문자열과 이 Str내부의  str문자열을 비교
        return i;                       //같으면 0, str이 큰경우 양수, a가 큰경우 음수
    }
    void operator=(char *a)             //같은 Str 클래스를 입력받았을때와 단순문자열을 입력받았을때를 구분
    {
        strcpy(str,a);                  //문자열 입력받은경우 str에 문자열 a 복사
    }
    void operator=(class Str& a)        //같은 Str 클래스를 입력받았을때와 단순문자열을 입력받았을때를 구분
    {
        strcpy(str,a.str);              //Str클래스를 입력받은경우 입력받은 a 내부의 str을 이 Str의 내부 str에 복사
    }
};

int main()
{
    Str a("I'm a girl");
    cout<<a.contents();
    a="I'm a boy\n";
    cout<<a.contents();
    cout<<a.compare("I'm a a")<<endl;
    return 0;
}
