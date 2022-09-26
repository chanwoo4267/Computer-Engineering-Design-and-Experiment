#include <iostream>
#include <cstdlib>
#include <ctime>

#define MAXH 100
#define MAXW 100

enum { N, E, W, S};

char visit_flag[MAXH][MAXW];
char hor_flag[MAXH][MAXW];
char ver_flag[MAXH][MAXW];

int adjacent(int dir_flag[], int x, int y, int h, int w)
{
    int dir_count = 0;
    
    if (y < h-1 && visit_flag[y+1][x] == 0) dir_flag[dir_count++] = S;
    if (y > 0 && visit_flag[y-1][x] == 0) dir_flag[dir_count++] = N;
    if (x < w-1 && visit_flag[y][x+1] == 0) dir_flag[dir_count++] = E;
    if (x > 0 && visit_flag[y][x-1] == 0) dir_flag[dir_count++] = W;
    
    return dir_count;
}

void makeMaze(int x, int y, int h, int w) // x,y : 현재좌표, h,w: 미로 높이,폭
{
    int dir_flag[4];
    int dir_count = 0;
    dir_count = adjacent(dir_flag,x,y,h,w); //방문 안한 방향,수 확인
    
    visit_flag[y][x] = 1; //현재위치 : 방문
    while(dir_count != 0)
    {
        int random_dir;
        random_dir = rand() % dir_count; //랜덤한 방향 방문
        
        switch(dir_flag[random_dir]) {
            case N:
                ver_flag[y-1][x]=1;
                makeMaze(x,y-1,h,w);
                break;
            case E:
                hor_flag[y][x]=1;
                makeMaze(x+1,y,h,w);
                break;
            case S:
                ver_flag[y][x]=1;
                makeMaze(x,y+1,h,w);
                break;
            case W:
                hor_flag[y][x-1]=1;
                makeMaze(x-1,y,h,w);
                break;
        } //방향으로 이동해서 재귀적호출
        
        dir_count = adjacent(dir_flag,x,y,h,w);
    }
}

void saveMaze(int h,int w)
{
    FILE *fp = fopen("20171645.maz","w");
    
    for(int x=0; x<w; x++)
    {
        fprintf(fp,"+");
        fprintf(fp,"-");
    }
    
    fprintf(fp,"+");
    fprintf(fp,"\n");
    for(int y=0; y<h; y++)
    {
        fprintf(fp,"|");
        for(int x=0; x<w; x++)
        {
            fprintf(fp," ");
            if (x < w-1 && hor_flag[y][x])
                fprintf(fp," ");
            else
                fprintf(fp,"|");
        }
        fprintf(fp,"\n");
        fprintf(fp,"+");
        for(int x=0; x<w; x++)
        {
            if (y<h-1 && ver_flag[y][x])
                fprintf(fp," ");
            else
                fprintf(fp,"-");
            fprintf(fp,"+");
        }
        fprintf(fp,"\n");
    }
    fclose(fp);
}

int main()
{
    srand(time(NULL));
    
    int h,w;
    scanf("%d%d",&w,&h);
    
    makeMaze(0,0,h,w);
    saveMaze(h,w);
    return 0;
}
