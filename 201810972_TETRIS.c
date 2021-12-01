#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define SPACEBAR 32
#define ENTER 13
#define ESC 27
#define MAP_X 7
#define MAP_Y 12

typedef struct tetris {
    int level;
    int score;
    int rotation;
    int curX, curY;
    int curblock;
    int nextblock;
}Tetris;
typedef struct member {
    char name[20];
    char Snum[20];
    char ID[20];
    char PW[20];
}Member;
typedef struct gametime {
    time_t start_time;
    time_t end_time;
    time_t total_time;  
}Time;
typedef struct ranking {
    char ID[20];
    int score;
    int time;
}Ranking;

int Game_Space[20][10] = { 0 };
int cpy_Game_Space[20][10] = { 0 };
char ID[20];
int block[7][4][4][4] = { // [도형의 종류], [도형의 회전], [도형의 세로 최대길이], [도형의 가로 최대길이]
    //ㄴ자                // 배열 뒷부분 [4][4]는 회전에 따라 달라지는 도형의 방향을 그림.
    {
     0,1,0,0,
     0,1,1,1,
     0,0,0,0,
     0,0,0,0,
    
     0,0,0,0,
     0,1,1,0,
     0,1,0,0,
     0,1,0,0,
    
     0,0,0,0,
     1,1,1,0,
     0,0,1,0,
     0,0,0,0,
    
     0,0,1,0,
     0,0,1,0,
     0,1,1,0,
     0,0,0,0},

     //역ㄴ자
    {
     0,0,1,0,
     1,1,1,0,
     0,0,0,0,
     0,0,0,0,

     0,1,0,0,
     0,1,0,0,
     0,1,1,0,
     0,0,0,0,

     0,0,0,0,
     0,1,1,1,
     0,1,0,0,
     0,0,0,0,

     0,1,1,0,
     0,0,1,0,
     0,0,1,0,
     0,0,0,0,},

     // z모양
    {
     0,1,1,0,
     0,0,1,1,
     0,0,0,0,
     0,0,0,0,

     0,0,1,0,
     0,1,1,0,
     0,1,0,0,
     0,0,0,0,

     0,1,1,0,
     0,0,1,1,
     0,0,0,0,
     0,0,0,0,

     0,0,1,0,
     0,1,1,0,
     0,1,0,0,
     0,0,0,0},

     //역 z모양
    {
     0,1,1,0,
     1,1,0,0,
     0,0,0,0,
     0,0,0,0,

     0,1,0,0,
     0,1,1,0,
     0,0,1,0,
     0,0,0,0,

     0,1,1,0,
     1,1,0,0,
     0,0,0,0,
     0,0,0,0,

     0,1,0,0,
     0,1,1,0,
     0,0,1,0,
     0,0,0,0},

     //ㅗ모양
    {
     0,1,0,0,
     1,1,1,0,
     0,0,0,0,
     0,0,0,0,

     0,1,0,0,
     0,1,1,0,
     0,1,0,0,
     0,0,0,0,

     0,0,0,0,
     1,1,1,0,
     0,1,0,0,
     0,0,0,0,

     0,1,0,0,
     1,1,0,0,
     0,1,0,0,
     0,0,0,0},

     //ㅁ자 모양
     {
     0,1,1,0,
     0,1,1,0,
     0,0,0,0,
     0,0,0,0,

     0,1,1,0,
     0,1,1,0,
     0,0,0,0,
     0,0,0,0,

     0,1,1,0,
     0,1,1,0,
     0,0,0,0,
     0,0,0,0,

     0,1,1,0,
     0,1,1,0,
     0,0,0,0,
     0,0,0,0 },

     //ㅣ자 모양
     {
     1,1,1,1,
     0,0,0,0,
     0,0,0,0,
     0,0,0,0,

     0,1,0,0,
     0,1,0,0,
     0,1,0,0,
     0,1,0,0,

     1,1,1,1,
     0,0,0,0,
     0,0,0,0,
     0,0,0,0,

     0,1,0,0,
     0,1,0,0,
     0,1,0,0,
     0,1,0,0}
};
int new_block_sign = 0;
int Game_over = 0;
int timer = 1;
enum color {        //콘솔상에서 사용할 색 지정
    BLACK, BLUE, GREEN, IADE, RED, PURPLE, YELLOW, WHITE, GRAY, LIGHTBLUE, LIGHTGREEN, LIGHTIADE, LIGHTRED,
	LIGHTPURPLE, LIGHTYELLOW, LIGHTWHITE
};
void gotoxy(int x, int y) { //콘솔창 커서 제어
	COORD Pos = { x - 1, y - 1 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}
void setColor(unsigned short background, unsigned short text) { //setColor(배경, 텍스트)
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (background >> 4) | text);
}
void removeCursor() { //콘솔창 커서 지우기
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}
void reviveCursor() { //콘솔창 커서 지우기
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = TRUE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void Starting_Screen(int select) { //맨 초기 시작 화면 ( 로그인, 회원가입, 회원찾기, 종료 )
    int y = 5;

    setColor(BLACK, LIGHTWHITE);
    gotoxy(3, y);
    printf("■■■■■ ■■■■■ ■■■■■ ■■■■■ ■■■■■ ■■■■■", y++);
    gotoxy(3, y);
    printf("    ■     ■             ■     ■      ■     ■     ■        ", y++);
    gotoxy(3, y);
    printf("    ■     ■■■■■     ■     ■■■■■     ■     ■■■■■", y++);
    gotoxy(3, y);
    printf("    ■     ■             ■     ■    ■       ■             ■", y++);
    gotoxy(3, y);
    printf("    ■     ■■■■■     ■     ■      ■ ■■■■■ ■■■■■", y++);


    setColor(BLACK, WHITE);
    if (select == 1)
        setColor(BLACK, LIGHTBLUE);
    y = 20;
    gotoxy(30, y);
    printf("┌─────────┐ ", y++);
    gotoxy(30, y);
    printf("│  로그인 │ ", y++);
    gotoxy(30, y);
    printf("└─────────┘ ", y++);
    setColor(BLACK, WHITE);


    if (select == 2)
        setColor(BLACK, LIGHTBLUE);
    gotoxy(30, y);
    printf("┌─────────┐ ", y++);
    gotoxy(30, y);
    printf("│ 회원가입│ ", y++);
    gotoxy(30, y);
    printf("└─────────┘ ", y++);
    setColor(BLACK, WHITE);

    if (select == 3)
        setColor(BLACK, LIGHTBLUE);
    gotoxy(30, y);
    printf("┌─────────┐ ", y++);
    gotoxy(30, y);
    printf("│ 회원찾기│ ", y++);
    gotoxy(30, y);
    printf("└─────────┘ ", y++);
    setColor(BLACK, WHITE);

    if (select == 4)
        setColor(BLACK, LIGHTBLUE);
    gotoxy(30, y);
    printf("┌─────────┐ ", y++);
    gotoxy(30, y);
    printf("│   종료  │ ", y++);
    gotoxy(30, y);
    printf("└─────────┘ ", y++);
    setColor(BLACK, WHITE);

    gotoxy(69, 39);
}
void Next_block_box() { //다음 블럭이 어떤 블럭인지 표기되는 박스 생성
    int x, y;
    x = 31; y = 12;
    gotoxy(x, y++);
    printf("□□□□□□");
    for (int i = 0; i < 4; i++) {
        gotoxy(x, y);
        printf("□        □", y++);
    }
    gotoxy(x, y);
    printf("□□□□□□");
}
void Game_Screen() { //테트리스 진행되는 맵 표기
    int x = 7, y = 12;


    setColor(BLACK, LIGHTWHITE);
    gotoxy(x, y);
    printf("□□□□□□□□□□□□", y++);
    for (int i = 0; i < 20; i++) {
        gotoxy(x, y);
        printf("□                    □", y++);
    }
    gotoxy(x, y);
    printf("□□□□□□□□□□□□", y++);
}

int overlap_check(char* id) { // id중복이 있는지 체크하는 함수
    char check[20];
    FILE* fp;
    int i = 0;
    long size;
    
    fp = fopen("Member_list.txt", "r");
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);

    while (21 * i < size) { //파일의 크기만큼 탐색
        fseek(fp, i * 21, SEEK_SET);
        fscanf(fp, "%s", check);
        i += 4;
        if (strcmp(check, id) == 0) { //중복발생
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
}
void Register() {
    reviveCursor();
    Member m;
    FILE* fp;   
    int x, y;
    int overlap;
    char check;
    x = 27; y = 17;
    gotoxy(x, y);
    scanf("%s", m.name);
    y += 2;
    gotoxy(x, y);
    scanf("%s", m.Snum);
    y += 2;
    gotoxy(x, y);
    scanf("%s", m.ID);
    y += 2;
    gotoxy(x, y);
    scanf("%s", m.PW);

    x = 15; y = 30;
    gotoxy(x, y);
    printf("ID : %s ", m.ID, y++);
    gotoxy(x, y);
    printf("PW : %s ", m.PW, y++);
    gotoxy(x, y);
    printf("위 정보가 맞으십니까? (Y/N) >> ");
    scanf(" %c", &check);
    
    overlap = overlap_check(&m.ID);
    if (overlap == 1) {
        gotoxy(x, y + 2);
        printf("중복된 ID입니다.");
        Sleep(3000);
        removeCursor();
        system("cls");
        return;
    }
    if (check == 89 || check == 121) { //Y : 89, y : 121 아스키코드
        fp = fopen("Member_list.txt", "a");

        fprintf(fp, "%-20s ", m.ID);
        fprintf(fp, "%-20s ", m.PW);
        fprintf(fp, "%-20s ", m.name);
        fprintf(fp, "%-20s ", m.Snum);
        fprintf(fp, "\n");
        fclose(fp);
    }
    removeCursor();
    system("cls");
}
void Register_Screen() {
    int x, y, i;

    x = 17; y = 15;
    
    gotoxy(x, y);
    printf("┌────────────────────────────────────┐ ", y++);
    for (i = 0; i < 10; i++) {
        gotoxy(x, y);
        printf("│                                    │ ", y++);
    }
    gotoxy(x, y);
    printf("└────────────────────────────────────┘ ", y++);

    x = 20; y = 17;
    gotoxy(x, y);
    printf("이름 : ", y += 2);
    gotoxy(x, y);
    printf("학번 : ", y += 2);
    gotoxy(x, y);
    printf("ID   : ", y += 2);
    gotoxy(x, y);
    printf("PW   : ", y += 2);

    Register();
}

void Find_id() {
    FILE* fp;
    char store_name[20], input_name[20], store_id[20];
    int x, y, i, select, sign = 0;
    long size;

    x = 17; y = 15;

    gotoxy(x, y);
    printf("┌────────────────────────────────────┐ ", y++);
    for (i = 0; i < 12; i++) {
        gotoxy(x, y);
        printf("│                                    │ ", y++);
    }
    gotoxy(x, y);
    printf("└────────────────────────────────────┘ ", y++);

    gotoxy(20, 17);
    printf("이름을 입력해주세요 >> ");
    scanf("%s", input_name);

    fp = fopen("Member_list.txt", "r");
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);

    i = 2;
    while (21 * i < size) { //파일의 크기만큼 탐색
        fseek(fp, i * 21, SEEK_SET);
        fscanf(fp, "%s", store_name);
        if (strcmp(input_name, store_name) == 0) { //동일 아이디 발견
            fseek(fp, 21 * (i - 2), SEEK_SET); // 파일포인터를 ID위치로 옮겨 스캔
            fscanf(fp, "%s", store_id);
            sign = 1;
            break;
        }
        i += 4; //각 열마다 4개의 데이터가 존재하므로 다음 열로 이동하기 위함.
    }
    fclose(fp);
    
    gotoxy(20, 25);
    if (sign == 1)
        printf("아이디는 %s 입니다 ", store_id);
    else
        printf("등록된 아이디가 없습니다.");
    Sleep(3000);
    removeCursor();
    system("cls");
}
void Find_pw() {
    FILE* fp;
    char store_Snum[20], input_Snum[20], store_pw[20], store_id[20], input_id[20];
    int x, y, i, select, sign = 0;
    long size;

    x = 17; y = 15;

    gotoxy(x, y);
    printf("┌────────────────────────────────────┐ ", y++);
    for (i = 0; i < 12; i++) {
        gotoxy(x, y);
        printf("│                                    │ ", y++);
    }
    gotoxy(x, y);
    printf("└────────────────────────────────────┘ ", y++);

    gotoxy(20, 17);
    printf("ID를 입력해주세요 >> ");
    scanf("%s", input_id);
    gotoxy(20, 18);
    printf("학번을 입력해주세요 >> ");
    scanf("%s", input_Snum);

    fp = fopen("Member_list.txt", "r");
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);

    i = 0;
    while (21 * i < size) { //파일의 크기만큼 탐색
        fseek(fp, i * 21, SEEK_SET);
        fscanf(fp, "%s", store_id);
        if (strcmp(input_id, store_id) == 0) { //동일 id 발견
            fseek(fp, 21 * (i + 3), SEEK_SET);
            fscanf(fp, "%s", store_Snum);
            if (strcmp(input_Snum, store_Snum) == 0) {//학번도 같을 때
                fseek(fp, 21 * (i + 1), SEEK_SET); //비밀번호
                fscanf(fp, "%s", store_pw);
                sign = 1;
                break;
            }
        }
        i += 4;
    }
    fclose(fp);

    gotoxy(20, 25);
    if (sign == 1)
        printf("비밀번호는 %s 입니다 ", store_pw);
    else
        printf("id : %s, Snum : %s ", store_id, store_Snum);
    Sleep(3000);
    removeCursor();
    system("cls");
}
void Find_member() {
    int x, y, i, select;

    reviveCursor();
    x = 17; y = 15;

    gotoxy(x, y);
    printf("┌────────────────────────────────────┐ ", y++);
    for (i = 0; i < 10; i++) {
        gotoxy(x, y);
        printf("│                                    │ ", y++);
    }
    gotoxy(x, y);
    printf("└────────────────────────────────────┘ ", y++);
    
    
    x = 20; y = 17;
    gotoxy(x, y);
    printf("1. ID찾기 ", y += 2);
    gotoxy(x, y);
    printf("2. PW찾기 ", y += 2);
    
    gotoxy(x, y + 2);
    printf("SELECT >> ");
    scanf("%d", &select);

    if (select == 1) {
        Find_id();
    }
    else if (select == 2) {
        Find_pw();
    }
}

void Gameover(Tetris t, Time gt) {
    int x, y, i, j, select;
    FILE* fp;
    long size;
    Ranking Rank[5];
    Ranking new, tmp;

    x = 15; y = 15;
    gotoxy(x, y++);
    printf("┌────────────────────────────────────┐ ");
    for (i = 0; i < 15; i++) {
        gotoxy(x, y++);
        printf("│                                    │ ");
    }
    gotoxy(x, y++);
    printf("└────────────────────────────────────┘ ");
    y = 18;
    gotoxy(x + 15, y);
    printf("게 임 오 버");
    y += 3;
    gotoxy(x + 5, y++);
    printf("레벨 : %d ", t.level);
    gotoxy(x + 5, y++);
    printf("점수 : %d ", t.score);
    gotoxy(x + 5, y++);;
    printf("시간 : %02d분 %02d초", (int)gt.total_time / 60, (int)gt.total_time % 60);

    /*strcpy(new.ID, ID);
    new.score = t.score;
    new.time = gt.total_time;

    fp = fopen("Ranking.txt", "r");
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);

    fseek(fp, 0, SEEK_SET);
    for (i = 0; i < size / 35; i++) { // size / 35는 저장된 랭킹의 수를 찾기 위함.
        fseek(fp, 35 * i, SEEK_SET);
        fscanf(fp, "%s", Rank[i].ID);
        fseek(fp, (35 * i) + 21, SEEK_SET);
        fscanf(fp, "%d", &Rank[i].score);
        fseek(fp, (35 * i) + 28, SEEK_SET);
        fscanf(fp, "%d", &Rank[i].time);
    }
    fclose(fp);

    for (i = 0; i < size / 35; i++) {
        if (Rank[i].score < new.score) { //더 높은 기록
            for (j = 4; j > i; j--) {
                Rank[j] = Rank[j - 1];
            }
            Rank[i] = new;
            break;
        }
    }

    fp = fopen("Ranking.txt", "w");
    for (i = 0; i < size / 35; i++) {
        fprintf(fp, "%-20s ", Rank[i].ID);
        fprintf(fp, "%06d ", Rank[i].score);
        fprintf(fp, "%04d ", (int)Rank[i].time);
        fprintf(fp, "\n");
    }
    fclose(fp);

    gotoxy(x + 5, 28);
    printf("축하합니다. 랭킹 %d등 입니다", i + 1);
    gotoxy(x + 5, 29);
    printf("Rank[%d].ID : %s, score : %d", i, Rank[i].ID, Rank[i].score);*/
    Sleep(3000);
}

int Login() {
    FILE* fp;
    char store_id[20], input_id[20], store_pw[20], input_pw[20];
    int x, y, i, select, sign = 0;
    long size;

    x = 17; y = 15;

    gotoxy(x, y);
    printf("┌────────────────────────────────────┐ ", y++);
    for (i = 0; i < 12; i++) {
        gotoxy(x, y);
        printf("│                                    │ ", y++);
    }
    gotoxy(x, y);
    printf("└────────────────────────────────────┘ ", y++);

    gotoxy(20, 17);
    printf("ID를 입력해주세요 >> ");
    scanf("%s", input_id);
    strcpy(ID, input_id);
    gotoxy(20, 18);
    printf("PW를 입력해주세요 >> ");
    scanf("%s", input_pw);

    fp = fopen("Member_list.txt", "r");
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);

    i = 0;
    while (21 * i < size) { //파일의 크기만큼 탐색
        fseek(fp, i * 21, SEEK_SET);
        fscanf(fp, "%s", store_id);
        if (strcmp(input_id, store_id) == 0) { //동일 id 발견
            fseek(fp, 21 * (i + 1), SEEK_SET);
            sign = 1;
            fscanf(fp, "%s", store_pw);
            if (strcmp(input_pw, store_pw) == 0) {//비밀번호 일치
                sign = 2;
                break;
            }
        }
        i += 4;
    }
    fclose(fp);

    gotoxy(20, 25);
    if (sign == 0) {
        printf("아이디를 찾을 수 없습니다.");
    }
    else if (sign == 1) {
        printf("비밀번호가 일치하지 않습니다.");
    }
    removeCursor();
    system("cls");

    return sign;
}

void Main_Menu(int select) {
    int x = 38, y = 12;
    int i;
    Game_Screen();

    gotoxy(55, 2);
    printf("ID : %s", ID);
    
    gotoxy(x, y);
    printf("┌──────────────────────────┐ ", y++);
    for (i = 0; i < 3; i++) {
        gotoxy(x, y);
        printf("│ ");
        gotoxy(x + 27, y);
        printf("│ ", y++);
    }
    gotoxy(x, y);
    printf("└──────────────────────────┘ ", y++);
    gotoxy(49, 14);
    printf("M E N U");
    gotoxy(x, y);
    printf("┌──────────────────────────┐ ", y++);
    for (i = 0; i < 15; i++) {
        gotoxy(x, y);
        printf("│ ");
        gotoxy(x + 27, y);
        printf("│ ", y++);
    }
    gotoxy(x, y);
    printf("└──────────────────────────┘ ", y++);

    setColor(BLACK, WHITE);
    if (select == 1)
        setColor(BLACK, LIGHTBLUE);
    x = 46; y = 19;
    gotoxy(x, y);
    printf("┌─────────┐ ", y++);
    gotoxy(x, y);
    printf("│ 게임하기│ ", y++);
    gotoxy(x, y);
    printf("└─────────┘ ", y++);
    setColor(BLACK, WHITE);


    if (select == 2)
        setColor(BLACK, LIGHTBLUE);
    gotoxy(x, y);
    printf("┌─────────┐ ", y++);
    gotoxy(x, y);
    printf("│ 게임방법│ ", y++);
    gotoxy(x, y);
    printf("└─────────┘ ", y++);
    setColor(BLACK, WHITE);

    if (select == 3)
        setColor(BLACK, LIGHTBLUE);
    gotoxy(x, y);
    printf("┌─────────┐ ", y++);
    gotoxy(x, y);
    printf("│ 랭킹보기│ ", y++);
    gotoxy(x, y);
    printf("└─────────┘ ", y++);
    setColor(BLACK, WHITE);

    if (select == 4)
        setColor(BLACK, LIGHTBLUE);
    gotoxy(x, y);
    printf("┌─────────┐ ", y++);
    gotoxy(x, y);
    printf("│ 게임종료│ ", y++);
    gotoxy(x, y);
    printf("└─────────┘ ", y++);
    setColor(BLACK, WHITE);

}
void Game_Rule() {
    int i;
    int x, y;

    x = 15; y = 12;
    gotoxy(x, y);
    printf("┌────────────────────────────────────────┐ ", y++); //─ 40개 ┌ ┐ 1개씩
    for (i = 0; i < 15; i++) {
        gotoxy(x, y);
        printf("│                                        │ ", y++);
    }
    gotoxy(x, y);
    printf("└────────────────────────────────────────┘ ", y++);

    gotoxy(35, 38);
    printf("돌아가시려면 아무키나 눌러주세요.");

    x = 20; y = 16;
    gotoxy(x, y);
    printf("← 좌로 이동  ↓ 아래로 이동  ", y++);
    gotoxy(x, y);
    printf("→ 우로 이동  ↑ 블록 회전 ", y++);
    gotoxy(x, y);
    printf("SPACE BAR 블럭 하강", y++);
    gotoxy(x, y);
    printf("ESC 일시 정지", y++);
    y++;
    gotoxy(x, y);
    printf("점수 500점당 난이도 상승", y++);
    gotoxy(x, y);
    printf("한 라인 제거당 50점", y++);
    gotoxy(x, y);
    printf("여러 라인 동시에 제거시 보너스 점수", y++);

    while (TRUE) {
        if (kbhit() != 0) {
            system("cls");
            break;
        }
    };
}

void tetris_set(Tetris* t) {
    t->level = 1;
    t->score = 0;
    t->curX = 0;
    t->curY = 0;
    t->rotation = 0;
    t->curblock = NULL;
    t->nextblock = rand() % 7;
    new_block_sign = 0;
    Game_over = 0;
    timer = 1;
}
void Display_nextblock(Tetris* t) {
    int i, j;
    gotoxy(33, 13);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            gotoxy(33 + (j * 2), 13 + i);
            if (block[t->nextblock][t->rotation][i][j] == 0) {
                printf("  ");
            }
            else {
                printf("■");
            }
        }
    }
}
void Display_Game_Info(Tetris* t, Time gt) {
    int y = 2;
    gotoxy(55, y++);
    printf("LEVEL : %d", t->level);
    gotoxy(55, y++);
    printf("SCORE : %d", t->score);
    gotoxy(55, y++);
    printf("TIME  : %02d:%02d", (int)gt.total_time / 60, (int)gt.total_time % 60);
}
void Display_Game_Play() { //게임 플레이 화면 출력
	int y, x;
	for (y = 0; y < 20; y++) {
		for (x = 0; x < 10; x++) {
			if (Game_Space[y][x] != cpy_Game_Space[y][x]) {  //변화한 부분만 바꾸기 위함.
                    //0 : 빈공간, 1: 벽(블럭), 2:컨트롤 블럭, 3:도착지점
                    //블록인 공간
					if (Game_Space[y][x] == 1 || Game_Space[y][x] == 2) {
						gotoxy((MAP_X + 2) + (x * 2), (MAP_Y + 1) + y);       
						printf("■");
						cpy_Game_Space[y][x] = 1;
					}
                    //빈공간인 공간
                    else if (Game_Space[y][x] == 0) {
                        gotoxy((MAP_X + 2) + (x * 2), (MAP_Y + 1) + y);
                        printf("  ");
                        cpy_Game_Space[y][x] = 0;
                    }
                    //도착 예정 공간
                    else {
                        gotoxy((MAP_X + 2) + (x * 2), (MAP_Y + 1) + y);
                        printf("□");
                        cpy_Game_Space[y][x] = 3;
                    }
			}
		}
	}
}
void New_Block(Tetris* t, int num) {
    int i, j, sign = 0;
    t->curX = 3;
    t->curY = 0;
    t->curblock = num;
    t->rotation = 0;

    //블록 생성
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (block[num][t->rotation][i][j] == 1) {
                if (Game_Space[t->curY + i][t->curX + j] == 0) {
                    Game_Space[t->curY + i][t->curX + j] = 2;
                }
                else {
                    Game_over = 1;
                    return 0;
                }
            }

        }
    }
    //생성 위치에 이미 블록이 있다면 게임오버 추가
}

void rotate_block(Tetris* t) {
    int i, j, sign = 0;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            //회전할 블럭이 도착하는 위치에 벽(블럭)이 있는지 검사
            if (block[t->curblock][(t->rotation + 1) % 4][i][j] == 1) {
                if (t->curY + i >= 0 && t->curY + i <= 19){ //위, 아래 벽 검사
                    if (t->curX + j >= 0 && t->curX + j <= 9) { //좌, 우 벽 검사
                        if (Game_Space[t->curY + i][t->curX + j] == 1) { //이미 블럭이 있는경우
                            sign = 1;
                            break;
                        }
                    }
                }
            }
        }
    }

    if (sign == 0) { //블럭이 회전할 수 있는 경우

        //있던 블럭을 제거하기 위함
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                if (block[t->curblock][t->rotation][i][j] == 1) {
                    Game_Space[t->curY + i][t->curX + j] = 0;
                }
            }
        }

        t->rotation = (t->rotation + 1) % 4;
        //새로이 블럭을 만들어주기 위함
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                if (block[t->curblock][t->rotation][i][j] == 1) {
                    Game_Space[t->curY + i][t->curX + j] = 2;
                }
            }
        }


    }
}
void break_line(Tetris* t) { //줄이 꽉 찼을 경우 지우는 함수
    int i, j, k, count, broke = 0;
    for (i = 0; i < 20; i++) {
        count = 0;
        for (j = 0; j < 10; j++) {
            if (Game_Space[i][j] == 1) { // 불에 블럭의 수를 카운트
                count++;
            }
        }
        if (count == 10) { //카운트된 블록이 10개 (꽉참)
            for (k = i; k > 1; k--) { //아래서부터 진행.
                for (j = 0; j < 10; j++) {
                    if (Game_Space[k - 1][j] != 2) { //컨트롤중인 블록이 아닌 공간에 한하여
                        Game_Space[k][j] = Game_Space[k - 1][j]; //한칸 위의 블럭을 아래로 옮김
                    }
                }
            }
            i--;
            broke++;
        }
    }
    for (i = 0; i < broke; i++) {
        for (j = i; j < broke; j++) {
            t->score += 50;
        }
    }
}
void down_block(Tetris* t) {
    int sign, count, i, j;
    sign = 0;
    count = 0;
    for (i = 3; 0 <= i; i--) { //기본 블럭의 정보가 4x4의 정보를 가지고 있었으니, 4x4를 탐색하며 어느 칸에 블럭이 있는지 체크
        for (j = 3; 0 <= j; j--) {
            if (block[t->curblock][t->rotation][j][i] == 1) { //블럭이 있는 좌표를 찾았을 때
                count++;
                if (Game_Space[t->curY + j + 1][t->curX + i] != 1 && t->curY + j + 1 <= 19) { //블럭의 아래에 다른 블럭이 없고, 아랫쪽이 벽이 아닌경우
                    sign++;
                }
            }
        }
    }
    if (count == sign) {
        for (i = 3; 0 <= i; i--) {
            for (j = 3; 0 <= j; j--) {
                if (Game_Space[t->curY + j][t->curX + i] == 2) {
                    Game_Space[t->curY + j + 1][t->curX + i] = 2; //해당 블럭을 아래로
                    Game_Space[t->curY + j][t->curX + i] = 0; //블럭이 있던 자리는 0으로 초기화
                }
            }
        }
        t->curY++;
    }
    else {
        new_block_sign = 0;
        for (i = 0; i < 20; i++) {
            for (j = 0; j < 10; j++) {
                if (Game_Space[i][j] == 2) {
                    Game_Space[i][j] = 1;
                }
            }
        }
    }
}
void move_block(Tetris *t, char key) {
    int i, j, sign, count;
    int x, y;
    switch (key) {
    case UP : //회전
        rotate_block(t);
        break;
    case LEFT:
        sign = 0;
        count = 0;
        for (i = 0; i < 4; i++) { //기본 블럭의 정보가 4x4의 정보를 가지고 있었으니, 4x4를 탐색하며 어느 칸에 블럭이 있는지 체크
            for (j = 0; j < 4; j++) {
                if (block[t->curblock][t->rotation][j][i] == 1) { //블럭이 있는 좌표를 찾았을 때
                    count++;
                    if (Game_Space[t->curY + j][t->curX + i - 1] != 1 && (t->curX + i - 1) >= 0) { //블럭의 좌측에 다른 블럭이 없고, 왼쪽이 벽이 아닌경우
                        sign++;
                    }
                }
            }
        }
        if (count == sign) { 
            for (i = 0; i < 4; i++) {
                for (j = 0; j < 4; j++) {
                    if (Game_Space[t->curY + j][t->curX + i] == 2) {
                        Game_Space[t->curY + j][t->curX + i - 1] = 2; //해당 블럭을 좌측으로
                        Game_Space[t->curY + j][t->curX + i] = 0; //블럭이 있던 자리는 0으로 초기화
                    }
                }
            }
            t->curX--;
        }

        break;

    case RIGHT:
        sign = 0;
        count = 0;
        for (i = 3; 0 <= i; i--) { //기본 블럭의 정보가 4x4의 정보를 가지고 있었으니, 4x4를 탐색하며 어느 칸에 블럭이 있는지 체크
            for (j = 3; 0 <= j; j--) {
                if (block[t->curblock][t->rotation][j][i] == 1) { //블럭이 있는 좌표를 찾았을 때
                    count++;
                    if (Game_Space[t->curY + j][t->curX + i + 1] != 1 && (t->curX + i + 1) <= 9) { //블럭의 우측에 다른 블럭이 없고, 우측이 벽이 아닌경우
                        sign++;
                    }
                }
            }
        }
        if (count == sign) {
            for (i = 3; 0 <= i; i--) { //기본 블럭의 정보가 4x4의 정보를 가지고 있었으니, 4x4를 탐색하며 어느 칸에 블럭이 있는지 체크
                for (j = 3; 0 <= j; j--) {
                    if (Game_Space[t->curY + j][t->curX + i] == 2) {
                        Game_Space[t->curY + j][t->curX + i + 1] = 2; //해당 블럭을 우측으로
                        Game_Space[t->curY + j][t->curX + i] = 0; //블럭이 있던 자리는 0으로 초기화
                    }
                }
            }
            t->curX++;
        }
        break;

    case DOWN://한칸 밑으로
        down_block(t);
        break;

    case SPACEBAR: //하강
        y = 0;
        while (TRUE) {
            sign = 0;
            for (i = 0; i < 4; i++) {
                for (j = 0; j < 4; j++) {
                    if (block[t->curblock][t->rotation][j][i] == 1) { //블록이 있는 공간 찾기
                        if (Game_Space[t->curY + j + 1][t->curX + i] == 1) { //컨트롤중인 블록 아래에 벽이 있는지 검사
                            sign = 1;
                            break;
                        } 
                    }
                }
            }
            
            if (sign == 0 && y <= 19) {
                down_block(t);
            }
            else {
                new_block_sign = 0;
                for (i = 0; i < 20; i++) {
                    for (j = 0; j < 10; j++) {
                        if (Game_Space[i][j] == 2) {
                            Game_Space[i][j] = 1;
                        }
                    }
                }
                break;
            }
            y++;
        }
    }
}

void set_gamemap(){ //게임 시작시 맵을 모두 0으로 초기화
    int i, j;
    for (i = 0; i < 20; i++) {
        for (j = 0; j < 10; j++) {
            Game_Space[i][j] = 0;
            cpy_Game_Space[i][j] = 0;
        }
    }
}

void game_start() {
    set_gamemap();
    char key = 0;
    Tetris t;
    Time gametime;
    tetris_set(&t);
    gametime.total_time = 0;
    

    Game_Screen();
    Next_block_box();
    while (TRUE) {
        gametime.start_time = time(0);

        if (new_block_sign == 0) { //조작중인 블록이 없는 경우 새로운 블록 생성
            New_Block(&t, t.nextblock);
            t.nextblock = rand() % 7; //블럭 종류결정을 위한 랜덤함수.
            new_block_sign = 1;
            Display_nextblock(&t);
        }

        Display_Game_Play(); //게임 화면 갱신
        Display_Game_Info(&t, gametime);

        while (kbhit() != 0) { //키보드 입력
            key = _getch();
            
            
            if (key == ESC) {//일시정지 esc
                gotoxy(30, 38);
                gametime.end_time = time(0);
                gametime.total_time += gametime.end_time - gametime.start_time; //일시정지 기간동안은 시간 체크 X
                system("PAUSE");
                gotoxy(30, 38);
                printf("                                           ");
                gametime.start_time = time(0);
            }
            else {
                move_block(&t, key);
            }
        }

        break_line(&t);
        if (t.level <= 15) {
           t.level = 1 + (t.score / 500);
        }
        timer++;
        Sleep(33); //1초에 30프레임
        if (timer == (66 - (t.level * 3))) {
            down_block(&t);
            timer = 0;
        }

        if (Game_over == 1) {
            Gameover(t, gametime);
            system("cls");
            break;
        }

        gametime.end_time = time(0);
        gametime.total_time += gametime.end_time - gametime.start_time; //끝난시각 - 시작시각 = 진행시간
    }
}

int main(void)
{
	system("mode con cols=70 lines=40");
    SetConsoleTitle(TEXT("Tetris"));
	removeCursor();						// 콘솔창 커서 지우기
    srand(time(NULL));

    char key;
    int select = 1, check = 0;;

	while (TRUE) {

        while (TRUE) {
            Starting_Screen(select);

            if (kbhit() != 0) {
                key = _getch();
                if (key == DOWN && select < 4) {
                    select++;
                }
                else if (key == UP && select > 1) {
                    select--;
                }
                else if (key == ENTER) {

                    switch (select) {
                    case 1: //로그인
                        system("cls");
                        check = Login();
                        if (check == 2) {
                                while (TRUE) {
                                Main_Menu(select);
                                while (kbhit() != 0) {
                                    key = _getch();
                                    if (key == DOWN && select < 4) {
                                        select++;
                                    }
                                    else if (key == UP && select > 1) {
                                        select--;
                                    }
                                    else if (key == ENTER) {

                                        switch (select) {
                                        case 1: //게임하기
                                            system("cls");
                                            game_start();
                                            break;
                                        case 2: //게임방법
                                            system("cls");
                                            Game_Rule();
                                            break;
                                        case 3: //랭킹보기
                                            break;
                                        case 4: //게임종료
                                            system("cls");
                                            exit(0);
                                            break;
                                        }
                                    }
                                }
                            }
                        }  
                        break;
                    case 2: //회원가입
                        system("cls");
                        Register_Screen();
                        break;
                    case 3: //회원찾기
                        system("cls");
                        Find_member();
                        break;
                    case 4: //종료
                        exit(0);
                        break;
                    }
                }
            }
        };
		//game_start();
        if (Game_over == 1) {
            system("cls");
            break;
        }
	};

    gotoxy(35, 20);
    printf("게임 종료");

	return 0;
}