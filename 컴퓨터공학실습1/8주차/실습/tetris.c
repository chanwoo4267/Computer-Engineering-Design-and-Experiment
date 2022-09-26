#include "tetris.h"

static struct sigaction act, oact;
int B,count;

int main(){
	int exit=0;
    // rank list
	createRankList();
	initscr();
	noecho();
	keypad(stdscr, TRUE);	

	srand((unsigned int)time(NULL));
	while(!exit){
		clear();
		switch(menu()){
		case MENU_PLAY:
            {   autoplay = 0;
                play();  } break;
		case MENU_EXIT: exit=1; break;
        case MENU_RANK: rank(); break;
        case MENU_RECOMMEND:
            {   autoplay = 1;
                 recommendedPlay(); } break;
		default: break;
		}
	}
    
	endwin();
	system("clear");
	return 0;
}

void FreeRecNode(RecNode* root)
{
    int i;
    if (root->level == BLOCK_NUM) {
	if (root != NULL)
        free(root);
    }
    else
    {
        for(i=0; i<CHILDREN_MAX; i++)
        {
            if (root->child[i] == NULL) break;
            FreeRecNode(root->child[i]);
        }
	if(root != NULL)
        free(root);
    }
}

void InitTetris(){
	int i,j;

	for(j=0;j<HEIGHT;j++)
		for(i=0;i<WIDTH;i++)
			field[j][i]=0;

    for(i=0; i<BLOCK_NUM; i++)
        nextBlock[i]=rand()%7;
    
	blockRotate=0;
	blockY=-1;
	blockX=WIDTH/2-2;
	score=0;	
	gameOver=0;
	timed_out=0;
    
    
    //recommend initialize
    recRoot=(RecNode*)malloc(sizeof(RecNode));
    for(i=0; i<HEIGHT; i++)
        for(j=0; j<WIDTH; j++)
            recRoot->field[i][j]=0;
    recRoot->level=0;
    recRoot->score=0;
    for(i=0;i<CHILDREN_MAX;i++)
        recRoot->child[i] = NULL;
    
    recommend(recRoot);
    
	DrawOutline();
	DrawField();
    DrawBlockWithFeatures(blockY,blockX,nextBlock[0],blockRotate);
	DrawNextBlock(nextBlock);
	PrintScore(score);
}

void DrawOutline(){	 //수정됨 to make nextblock[2]
	int i,j;
	/* 블럭이 떨어지는 공간의 태두리를 그린다.*/
	DrawBox(0,0,HEIGHT,WIDTH);

	/* next block을 보여주는 공간의 태두리를 그린다.*/
	move(2,WIDTH+10);
	printw("NEXT BLOCK");
	DrawBox(3,WIDTH+10,4,8);
    
    DrawBox(9,WIDTH+10,4,8); //custom code and 아래 15, 16도 수정됨

	/* score를 보여주는 공간의 태두리를 그린다.*/
	move(15,WIDTH+10);
	printw("SCORE");
	DrawBox(16,WIDTH+10,1,8);
}

int GetCommand(){
	int command;
	command = wgetch(stdscr);
	switch(command){
	case KEY_UP:
		break;
	case KEY_DOWN:
		break;
	case KEY_LEFT:
		break;
	case KEY_RIGHT:
		break;
	case ' ':	/* space key*/
		/*fall block*/
		break;
	case 'q':
	case 'Q':
		command = QUIT;
		break;
	default:
		command = NOTHING;
		break;
	}
	return command;
}

int ProcessCommand(int command){
	int ret=1;
	int drawFlag=0;
	switch(command){
	case QUIT:
		ret = QUIT;
		break;
	case KEY_UP:
		if((drawFlag = CheckToMove(field,nextBlock[0],(blockRotate+1)%4,blockY,blockX)))
			blockRotate=(blockRotate+1)%4;
		break;
	case KEY_DOWN:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY+1,blockX)))
			blockY++;
		break;
	case KEY_RIGHT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX+1)))
			blockX++;
		break;
	case KEY_LEFT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX-1)))
			blockX--;
		break;
	default:
		break;
	}
	if(drawFlag) DrawChange(field,command,nextBlock[0],blockRotate,blockY,blockX);
	return ret;	
}

void DrawField(){
	int i,j;
	for(j=0;j<HEIGHT;j++){
		move(j+1,1);
		for(i=0;i<WIDTH;i++){
			if(field[j][i]==1){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(".");
		}
	}
}


void PrintScore(int score){
	move(17,WIDTH+11);
	printw("%8d",score);
}

void DrawNextBlock(int *nextBlock){
	int i, j;
	for( i = 0; i < 4; i++ ){
		move(4+i,WIDTH+13);
		for( j = 0; j < 4; j++ ){
			if( block[nextBlock[1]][0][i][j] == 1 ){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
        if (BLOCK_NUM >= 3) {
        move(10+i,WIDTH+13);
        for( j = 0; j < 4; j++ ){
            if( block[nextBlock[2]][0][i][j] == 1 ){
                attron(A_REVERSE);
                printw(" ");
                attroff(A_REVERSE);
            }
            else printw(" ");
        }
        }
        
	}
}

void DrawBlock(int y, int x, int blockID,int blockRotate,char tile){ //blockX, blockY,nextblock[0],rotate,space
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++){
			if(block[blockID][blockRotate][i][j]==1 && i+y>=0){
				move(i+y+1,j+x+1);
				attron(A_REVERSE);
				printw("%c",tile);
				attroff(A_REVERSE);
			}
		}

	move(HEIGHT,WIDTH+10);
}

void DrawBox(int y,int x, int height, int width){
	int i,j;
	move(y,x);
	addch(ACS_ULCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_URCORNER);
	for(j=0;j<height;j++){
		move(y+j+1,x);
		addch(ACS_VLINE);
		move(y+j+1,x+width+1);
		addch(ACS_VLINE);
	}
	move(y+j+1,x);
	addch(ACS_LLCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_LRCORNER);
}

void play(){
	int command;
	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM,&act,&oact);
	InitTetris();
	do{
		if(timed_out==0){
			alarm(1);
			timed_out=1;
		}
        
		command = GetCommand();
		if(ProcessCommand(command)==QUIT){
			alarm(0);
			DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
			move(HEIGHT/2,WIDTH/2-4);
			printw("Good-bye!!");
			refresh();
			getch();
			return;
		}
	}while(!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
	move(HEIGHT/2,WIDTH/2-4);
	printw("GameOver!!");
	refresh();
	getch();
    newRank(score);
}

char menu(){
	printw("1. play\n");
	printw("2. rank\n");
	printw("3. recommended play\n");
	printw("4. exit\n");
	return wgetch(stdscr);
}

/////////////////////////첫주차 실습에서 구현해야 할 함수/////////////////////////

int CheckToMove(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
    
    int i,j,x,y;
    for(i=0; i<4; i++)
        for(j=0; j<4; j++)
        {
           
            if (block[currentBlock][blockRotate][i][j]==1)
            {
                x=blockX+j;
                y=blockY+i;
                if (y >= HEIGHT)
                    return 0;
                else if (x<0 || x>=WIDTH)
                    return 0;
                else if (f[y][x]==1)
                    return 0;
            }
        }
    return 1;
}

void DrawChange(char f[HEIGHT][WIDTH],int command,int currentBlock,int blockRotate, int blockY, int blockX){
	// user code

	//1. 이전 블록 정보를 찾는다. ProcessCommand의 switch문을 참조할 것
	//2. 이전 블록 정보를 지운다. DrawBlock함수 참조할 것.
	//3. 새로운 블록 정보를 그린다.
    
    int i,j;
    int rotatet=blockRotate;
    int blockxt=blockX;
    int blockyt=blockY;
    switch(command)
    {
        case KEY_UP :
            if (blockRotate != 0) rotatet = blockRotate - 1;
            else rotatet = 3;
            break;
        case KEY_LEFT :
            blockxt = blockX + 1;
            break;
        case KEY_RIGHT :
            blockxt = blockX - 1;
            break;
        case KEY_DOWN :
            blockyt = blockY - 1;
            break;
        default :
            break;
    }
    
    for(i=0; i<4; i++)
        for(j=0; j<4; j++)
        {
            int blockt = block[currentBlock][rotatet][j][i];
            int fieldt = f[blockyt + j][blockxt + i];
            if (block[currentBlock][blockRotate][j][i] == 1 && fieldt == 1) field[blockyt + j][blockxt + i] = 0;
        }
    DrawField();
    DrawBlockWithFeatures(blockY,blockX,nextBlock[0],blockRotate);
    move(17,WIDTH+19); //커서 위치
}

void BlockDown(int sig){
	// user code
    if (autoplay == 0)
        alarm(1);
    else
        ualarm(10000,0);
        
	//강의자료 p26-27의 플로우차트를 참고한다.
    int drawFlag = 0;
    int i,j;
    drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY+1,blockX);
    if (drawFlag == 1)
    {
        blockY++;
        DrawChange(field,KEY_DOWN,nextBlock[0],blockRotate,blockY,blockX);
    }
    else
    {
        if (blockY <= -1) { gameOver = 1; return; }
        
        score += AddBlockToField(field,nextBlock[0], blockRotate, blockY, blockX);
        score += DeleteLine(field);
        for(i=0; i<BLOCK_NUM-1;i++)
            nextBlock[i]=nextBlock[i+1];
        nextBlock[BLOCK_NUM-1]=rand()%7;
        DrawField();
        blockRotate=0;
        blockY=-1;
        blockX=WIDTH/2-2;
        DrawField();
        
        //FreeRecNode(recRoot);
        //recRoot = NULL;
        recRoot=(RecNode*)malloc(sizeof(RecNode));
        recRoot->level=0;
        recRoot->score=0;
        for(i=0; i<HEIGHT; i++)
            for(j=0; j<WIDTH; j++)
                recRoot->field[i][j] = field[i][j];
        for(i=0; i<CHILDREN_MAX; i++)
            recRoot->child[i] = NULL;
        
        recommend(recRoot);
       
        DrawBlockWithFeatures(blockY,blockX,nextBlock[0],blockRotate);
        move(11,WIDTH+19);
        DrawNextBlock(nextBlock);
        PrintScore(score);
    }
}

int AddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	
    int i,j,num=0,x,y,result,k;
    RecNode *temp;
    RecNode *del;
    for(i=0; i<4; i++)
        for(j=0; j<4; j++)
        {
            if (block[currentBlock][blockRotate][i][j] == 1)
            {
                x = blockX + j;
                y = blockY + i;
                f[y][x] = 1;
                if ( y==(HEIGHT-1) || f[y+1][x] == 1) num++;
            }
        }
    
    DrawField();
    return num*10;
}

int DeleteLine(char f[HEIGHT][WIDTH]){
	// user code

	//1. 필드를 탐색하여, 꽉 찬 구간이 있는지 탐색한다.
	//2. 꽉 찬 구간이 있으면 해당 구간을 지운다. 즉, 해당 구간으로 필드값을 한칸씩 내린다.
    
    int i,j,k,delline=0;
    for(j=1; j<HEIGHT;j++)
    {
        int lineisfull = 1;
        for(i=0; i<WIDTH; i++)
        {
            if (!field[j][i])
            {
                lineisfull = 0; break;
            }
        }
        
        if (lineisfull)
        {
            delline++;
            for(k=j; k>0; k--)
                for(i=0;i<WIDTH;i++)
                    field[k][i] = field[k-1][i];
        }
    }
    
    return delline*delline*100;
}

///
void DrawShadow(int y, int x, int blockID,int blockRotate){
	int drawFlag = 0;
    int blockYt = blockY;
    while(1)
    {
        drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockYt+1,blockX);
        if (drawFlag == 1)
        {
            blockYt++;
        }
        else break;
        
    }
    DrawBlock(blockYt,blockX,nextBlock[0],blockRotate,'/');
}

void DrawBlockWithFeatures(int blockY, int blockX, int currentBlock, int blockRotate)
{
    DrawRecommend(currentBlock);
    DrawShadow(blockY,blockX,currentBlock,blockRotate);
    DrawBlock(blockY,blockX,currentBlock,blockRotate,' ');
}

//rank node function
RankNode* newRankNode(int score, char* name)
{
    RankNode* node = (RankNode*)malloc(sizeof(RankNode));
    node->score = score;
    if (name)
    {
        strcpy(node->name,name);
    }
    node->next = NULL;
    return node;
}

void createRankList(){
	// 목적: Input파일인 "rank.txt"에서 랭킹 정보를 읽어들임, 읽어들인 정보로 랭킹 목록 생성
    // 1. "rank.txt"열기
    // 2. 파일에서 랭킹정보 읽어오기
    // 3. LinkedList로 저장
    // 4. 파일 닫기
    FILE *fp;
    int i;
    int score;
    char name[NAMELEN];
    RankNode* node;
    
    head = newRankNode(0,"");
    rankCount = 0;
    node = head;

    //1. 파일 열기
    //fp = fopen("/Users/bagchan-u/Desktop/tt/rank.txt", "r");
    fp = fopen("rank.txt", "r");
    // 2. 정보읽어오기
    /* int fscanf(FILE* stream, const char* format, ...);
    stream:데이터를 읽어올 스트림의 FILE 객체를 가리키는 파일포인터
    format: 형식지정자 등등
    변수의 주소: 포인터
    return: 성공할 경우, fscanf 함수는 읽어들인 데이터의 수를 리턴, 실패하면 EOF리턴 */
    // EOF(End Of File): 실제로 이 값은 -1을 나타냄, EOF가 나타날때까지 입력받아오는 if문
    fscanf(fp,"%d", &rankCount);
    for(i=0; i<rankCount; ++i)
    {
        fscanf(fp,"%s %d\n",name,&score);
        node->next = newRankNode(score,name);
        node=node->next;
    }
    // 4. 파일닫기
    fclose(fp);
}

void rank(){
	//목적: rank 메뉴를 출력하고 점수 순으로 X부터~Y까지 출력함
    //1. 문자열 초기화
    int X=1, Y=5, ch, i, j;
    clear();

    //2. printw()로 3개의 메뉴출력
    printw("1. list ranks from X to Y\n");
    printw("2. list ranks by a specific name\n");
    printw("3. delete a specific rank\n");

    //3. wgetch()를 사용하여 변수 ch에 입력받은 메뉴번호 저장
    ch = wgetch(stdscr);
    
    //rankNode
    RankNode* node;
    node = head;
    
    //4. 각 메뉴에 따라 입력받을 값을 변수에 저장
    //4-1. 메뉴1: X, Y를 입력받고 적절한 input인지 확인 후(X<=Y), X와 Y사이의 rank 출력
    if (ch == '1') {
        printw("X: ");
        echo();
        scanw("%d", &X);
        printw("Y: ");
        scanw("%d", &Y);
        noecho();
        printw("\tname\t\t|   score\n");
        printw("---------------------------------\n");
        if (X > Y || rankCount == 0 || X > rankCount )
            mvprintw(8,0,"search failure : no rank in the list\n");
        else
        {
            if (Y > rankCount) Y = rankCount;
            if (X == 0) X = 1;
            for(i=0; i<X; i++)
                node = node->next;
            for(i=0; i<Y-X+1; i++)
            {
                printw("%-16s\t| %d\n",node->name, node->score);
                node = node->next;
            }
            
        }
    }

    //4-2. 메뉴2: 문자열을 받아 저장된 이름과 비교하고 이름에 해당하는 리스트를 출력
    else if ( ch == '2') {
        char str[NAMELEN];
        int check = 0;
        printw("input the name: ");
        echo();
        scanw("%s",str);
        noecho();
        printw("\tname\t\t|   score\n");
        printw("---------------------------------\n");
        for(i=0; i<rankCount; i++)
        {
            node = node->next;
            if(!strcmp(node->name,str)) {
              printw("%-16s\t| %d\n",node->name, node->score);
                check = 1;
            }
        }
        
        if(!check)
            printw("search failure: no name in the list\n");

    }

    //4-3. 메뉴3: rank번호를 입력받아 리스트에서 삭제
    else if ( ch == '3') {
        int num;
        printw("input the rank: ");
        echo();
        scanw("%d",&num);
        noecho();
        
        RankNode* node_next;
        
        if(num<1 || num>rankCount)
        {
            printw("search failure: the rank not in the list\n");
            getch();
            return;
        }
        
        else if (num==rankCount)
        {
            for(i=0; i<rankCount-1; i++)
                node = node->next;
            node_next = node->next;
            node->next = NULL;
            free(node_next);
            printw("result: the rank deleted\n");
        }
        
        else if (num==1)
        {
            
            node_next = node->next;
            head = head->next;
            free(node);
            printw("result: the rank deleted\n");
            echo();
            noecho();
        }
        
        else {
            RankNode* node_before;
            node = node->next;
            node_before = head;
            node_next = node->next;
            for(i=0; i<num-1; i++)
            {
                node_before = node_before->next;
                node = node->next;
                node_next = node_next->next;
            }
            
            node_before->next = node_next;
            free(node);
            printw("result : the rank deleted\n");
        }
        
        rankCount--;
        writeRankFile();
        

    }
    getch();

}

void writeRankFile(){ //** fopen("r")로 되있는데 이게 맞나?????? **//
	// 목적: 추가된 랭킹 정보가 있으면 새로운 정보를 "rank.txt"에 쓰고 없으면 종료
    int sn, i;
    RankNode* node;
    //1. "rank.txt" 연다
    //FILE *fp = fopen("/Users/bagchan-u/Desktop/tt/rank.txt", "w");
    FILE *fp = fopen("rank.txt", "w");
    fprintf(fp,"%d\n",rankCount);
    
    for(node = head->next; node; node = node->next) //run until node == NULL
        fprintf(fp,"%s %d\n",node->name,node->score);
    
    fclose(fp);
}

void newRank(int score){
	// 목적: GameOver시 호출되어 사용자 이름을 입력받고 score와 함께 리스트의 적절한 위치에 저장
    char str[NAMELEN];
    int i, j;
    clear();
    //1. 사용자 이름을 입력받음
    printw("your name: ");
    echo();
    scanw("%s",str);
    noecho();
    //2. 새로운 노드를 생성해 이름과 점수를 저장, score_number가
    
    RankNode* newNode;
    RankNode* node;
    RankNode* lastNode;
    
    newNode = newRankNode(score,str);
    
    node = head->next;
    lastNode = head;
    while(1)
    {
        if(!node || node->score < score)
        {
            lastNode->next = newNode;
            newNode->next = node;
            break;
        }
        
        lastNode = node;
        node = node->next;
    }
    rankCount++;
    
    writeRankFile();
}

void DrawRecommend(int blockID){
	DrawBlock(recommendY,recommendX,blockID,recommendR,'r');
}

int recommend(RecNode *root){
	int max=0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수
    int block_r, block_x, block_y, temp_score,n=0;
    int i,j,r,x,y,lv,t=0,tmp;
    int freecount = 0;
    int freelv = BLOCK_NUM;
    
    lv = root->level;
    for(r=0; r<4; r++) //1st loop : 회전고려
    {
        for(x=-2; x<WIDTH; x++) // 2nd loop : 위치고려
        {
            y=-1;
            while(CheckToMove(root->field,nextBlock[lv],r,y,x))
                y++;
            if (y==-1)
                continue;
                y--; //블럭이 놓이는 위치 x,y 결정
            RecNode* new_node;
	    new_node = NULL;
            new_node = (RecNode*)malloc(sizeof(RecNode));
            new_node->level = lv+1;
            new_node->score = root->score;
            for(i=0; i<HEIGHT; i++)
                for(j=0; j<WIDTH; j++)
                    new_node->field[i][j] = root->field[i][j];
            for(i=0; i<CHILDREN_MAX; i++)
                new_node->child[i] = NULL; //new_node 초기화
            
            new_node->score += AddBlockToField(new_node->field,nextBlock[lv],r,y,x);
            new_node->score += DeleteLine(new_node->field);
            root->child[n] = new_node;
            n++;
            //블럭놓기, 줄지우기 점수를 계산해서 그 노드를 root의 가지 배열에 차례차례 넣는다
            
            if(new_node->level < BLOCK_NUM)
                temp_score = recommend(new_node);
            else
                temp_score = new_node->score;
             
            if(max < temp_score) //최고기록점수면 x,y,r,score 갱신
            {
                max = temp_score;
                block_r = r;
                block_x = x;
                block_y = y;
            }
            
        }
    }
    
    if (root->level == 0)
    {
        recommendR = block_r;
        recommendX = block_x;
        recommendY = block_y;
    }
    
	return max;
}

int customrecommend(RecNode *root)
{
    int max=0; 
    int block_r, block_x, block_y, temp_score, n=0;
    int i,j,k,r,x,y,lv;
    int bblock=0, empty=0, check=0;
    int height[WIDTH] = {HEIGHT,HEIGHT,HEIGHT,HEIGHT,HEIGHT,HEIGHT,HEIGHT,HEIGHT,HEIGHT,HEIGHT};
    
    lv = root->level;
    
    for(r=0; r<4; r++) 
    {
        for(x=-2; x<WIDTH; x++) 
        {
            y=-1;
            while(CheckToMove(root->field,nextBlock[lv],r,y,x))
                y++;
            if (y==-1)
                continue;
                y--; 
            
            RecNode* new_node=NULL;
            new_node = malloc(sizeof(RecNode));
            for(i=0; i<CHILDREN_MAX; i++)
                new_node->child[i] = malloc(sizeof(RecNode));
            new_node->level = lv+1;
            new_node->score = root->score;
            for(i=0; i<HEIGHT; i++)
                for(j=0; j<WIDTH; j++) {
                    new_node->field[i][j] = root->field[i][j];
                }
            for(i=0; i<CHILDREN_MAX; i++)
                new_node->child[i] = NULL; 
            
            new_node->score += AddBlockToField(new_node->field,nextBlock[lv],r,y,x) / 10 * 4;
            new_node->score += (DeleteLine(new_node->field) / 100) * 5;
            
            for(i=HEIGHT; i>0; i--)
            {
                for(j=0; j<WIDTH; j++)
                {
                    if (new_node->field[i][j] == 1 && height[j] > i) height[j] = i;
                    if (new_node->field[i][j] == 0)
                    {
                        for(k=i-1; k>0; k--)
                        {
                            if (new_node->field[k][j] == 1) {
                                bblock++;
                                check=1;
                            }
                            if (check != 0) empty++;
                            check = 0;
                        }
                    }
                }
            }
            
            new_node->score -= (bblock * 1);
            new_node->score -= (empty * 9);
            
            bblock=0; empty=0; check=0;
            
            for(i=0; i<WIDTH; i++)
                new_node->score -= (22-height[i]) * 4;
            
            for(i=0; i<WIDTH; i++)
                height[i] = HEIGHT;
            
            root->child[n] = new_node;
            n++;
            
            if(new_node->level < BLOCK_NUM)
                temp_score = customrecommend(new_node);
            else
                temp_score = new_node->score;
             
            if(max < temp_score)
            {
                max = temp_score;
                block_r = r;
                block_x = x;
                block_y = y;
            }
            
        }
    }
    
    if (root->level == 0)
    {
        recommendR = block_r;
        recommendX = block_x;
        recommendY = block_y;
    }
    
    return max;
}

void recommendedPlay(){
	
    int command;
    clear();
    act.sa_handler = BlockDown;
    sigaction(SIGALRM,&act,&oact);
    InitTetris();
    do{
        if(timed_out==0){
            ualarm(10000,0);
            timed_out=1;
        }
        
        command = GetCommand();
        if(ProcessCommand(command)==QUIT){
            alarm(0);
            DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
            move(HEIGHT/2,WIDTH/2-4);
            printw("Good-bye!!");
            refresh();
            getch();
            return;
        }
        
        if(recommendR != blockRotate)
            ProcessCommand(KEY_UP);
        if(recommendX < blockX)
            ProcessCommand(KEY_LEFT);
        else if(recommendX > blockX)
            ProcessCommand(KEY_RIGHT);
        
    }while(!gameOver);
    
    alarm(0);
    getch();
    DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
    move(HEIGHT/2,WIDTH/2-4);
    printw("GameOver!!");
    refresh();
    getch();
    newRank(score);

}
