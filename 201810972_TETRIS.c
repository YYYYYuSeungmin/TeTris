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
int block[7][4][4][4] = { // [紫⑽曖 謙盟], [紫⑽曖 �蛻�], [紫⑽曖 撮煎 譆渠望檜], [紫⑽曖 陛煎 譆渠望檜]
    //中濠                // 寡翮 萌睡碟 [4][4]朝 �蛻�縑 評塭 殖塭雖朝 紫⑽曖 寞щ擊 斜葡.
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

     //羲中濠
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

     // z賅曄
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

     //羲 z賅曄
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

     //匹賅曄
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

     //仃濠 賅曄
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

     //太濠 賅曄
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
enum color {        //夔樂鼻縑憮 餌辨й 儀 雖薑
    BLACK, BLUE, GREEN, IADE, RED, PURPLE, YELLOW, WHITE, GRAY, LIGHTBLUE, LIGHTGREEN, LIGHTIADE, LIGHTRED,
	LIGHTPURPLE, LIGHTYELLOW, LIGHTWHITE
};
void gotoxy(int x, int y) { //夔樂璽 醴憮 薯橫
	COORD Pos = { x - 1, y - 1 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}
void setColor(unsigned short background, unsigned short text) { //setColor(寡唳, 臢蝶お)
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (background >> 4) | text);
}
void removeCursor() { //夔樂璽 醴憮 雖辦晦
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}
void reviveCursor() { //夔樂璽 醴憮 雖辦晦
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = TRUE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void Starting_Screen(int select) { //裔 蟾晦 衛濛 �飛� ( 煎斜檣, �蛾灠㊣�, �蛾躞ㄠ�, 謙猿 )
    int y = 5;

    setColor(BLACK, LIGHTWHITE);
    gotoxy(3, y);
    printf("﹥﹥﹥﹥﹥ ﹥﹥﹥﹥﹥ ﹥﹥﹥﹥﹥ ﹥﹥﹥﹥﹥ ﹥﹥﹥﹥﹥ ﹥﹥﹥﹥﹥", y++);
    gotoxy(3, y);
    printf("    ﹥     ﹥             ﹥     ﹥      ﹥     ﹥     ﹥        ", y++);
    gotoxy(3, y);
    printf("    ﹥     ﹥﹥﹥﹥﹥     ﹥     ﹥﹥﹥﹥﹥     ﹥     ﹥﹥﹥﹥﹥", y++);
    gotoxy(3, y);
    printf("    ﹥     ﹥             ﹥     ﹥    ﹥       ﹥             ﹥", y++);
    gotoxy(3, y);
    printf("    ﹥     ﹥﹥﹥﹥﹥     ﹥     ﹥      ﹥ ﹥﹥﹥﹥﹥ ﹥﹥﹥﹥﹥", y++);


    setColor(BLACK, WHITE);
    if (select == 1)
        setColor(BLACK, LIGHTBLUE);
    y = 20;
    gotoxy(30, y);
    printf("忙式式式式式式式式式忖 ", y++);
    gotoxy(30, y);
    printf("弛  煎斜檣 弛 ", y++);
    gotoxy(30, y);
    printf("戌式式式式式式式式式戎 ", y++);
    setColor(BLACK, WHITE);


    if (select == 2)
        setColor(BLACK, LIGHTBLUE);
    gotoxy(30, y);
    printf("忙式式式式式式式式式忖 ", y++);
    gotoxy(30, y);
    printf("弛 �蛾灠㊣啈� ", y++);
    gotoxy(30, y);
    printf("戌式式式式式式式式式戎 ", y++);
    setColor(BLACK, WHITE);

    if (select == 3)
        setColor(BLACK, LIGHTBLUE);
    gotoxy(30, y);
    printf("忙式式式式式式式式式忖 ", y++);
    gotoxy(30, y);
    printf("弛 �蛾躞ㄠ潀� ", y++);
    gotoxy(30, y);
    printf("戌式式式式式式式式式戎 ", y++);
    setColor(BLACK, WHITE);

    if (select == 4)
        setColor(BLACK, LIGHTBLUE);
    gotoxy(30, y);
    printf("忙式式式式式式式式式忖 ", y++);
    gotoxy(30, y);
    printf("弛   謙猿  弛 ", y++);
    gotoxy(30, y);
    printf("戌式式式式式式式式式戎 ", y++);
    setColor(BLACK, WHITE);

    gotoxy(69, 39);
}
void Next_block_box() { //棻擠 綰楣檜 橫雯 綰楣檣雖 ル晦腎朝 夢蝶 儅撩
    int x, y;
    x = 31; y = 12;
    gotoxy(x, y++);
    printf("﹤﹤﹤﹤﹤﹤");
    for (int i = 0; i < 4; i++) {
        gotoxy(x, y);
        printf("﹤        ﹤", y++);
    }
    gotoxy(x, y);
    printf("﹤﹤﹤﹤﹤﹤");
}
void Game_Screen() { //纔お葬蝶 霞ч腎朝 裘 ル晦
    int x = 7, y = 12;


    setColor(BLACK, LIGHTWHITE);
    gotoxy(x, y);
    printf("﹤﹤﹤﹤﹤﹤﹤﹤﹤﹤﹤﹤", y++);
    for (int i = 0; i < 20; i++) {
        gotoxy(x, y);
        printf("﹤                    ﹤", y++);
    }
    gotoxy(x, y);
    printf("﹤﹤﹤﹤﹤﹤﹤﹤﹤﹤﹤﹤", y++);
}

int overlap_check(char* id) { // id醞犒檜 氈朝雖 羹觼ж朝 л熱
    char check[20];
    FILE* fp;
    int i = 0;
    long size;
    
    fp = fopen("Member_list.txt", "r");
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);

    while (21 * i < size) { //だ橾曖 觼晦虜躑 鬚儀
        fseek(fp, i * 21, SEEK_SET);
        fscanf(fp, "%s", check);
        i += 4;
        if (strcmp(check, id) == 0) { //醞犒嫦儅
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
    printf("嬪 薑爾陛 蜃戲褊棲梱? (Y/N) >> ");
    scanf(" %c", &check);
    
    overlap = overlap_check(&m.ID);
    if (overlap == 1) {
        gotoxy(x, y + 2);
        printf("醞犒脹 ID殮棲棻.");
        Sleep(3000);
        removeCursor();
        system("cls");
        return;
    }
    if (check == 89 || check == 121) { //Y : 89, y : 121 嬴蝶酈囀萄
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
    printf("忙式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式忖 ", y++);
    for (i = 0; i < 10; i++) {
        gotoxy(x, y);
        printf("弛                                    弛 ", y++);
    }
    gotoxy(x, y);
    printf("戌式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式戎 ", y++);

    x = 20; y = 17;
    gotoxy(x, y);
    printf("檜葷 : ", y += 2);
    gotoxy(x, y);
    printf("з廓 : ", y += 2);
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
    printf("忙式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式忖 ", y++);
    for (i = 0; i < 12; i++) {
        gotoxy(x, y);
        printf("弛                                    弛 ", y++);
    }
    gotoxy(x, y);
    printf("戌式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式戎 ", y++);

    gotoxy(20, 17);
    printf("檜葷擊 殮溘п輿撮蹂 >> ");
    scanf("%s", input_name);

    fp = fopen("Member_list.txt", "r");
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);

    i = 2;
    while (21 * i < size) { //だ橾曖 觼晦虜躑 鬚儀
        fseek(fp, i * 21, SEEK_SET);
        fscanf(fp, "%s", store_name);
        if (strcmp(input_name, store_name) == 0) { //翕橾 嬴檜蛤 嫦唯
            fseek(fp, 21 * (i - 2), SEEK_SET); // だ橾ん檣攪蒂 ID嬪纂煎 衡啖 蝶警
            fscanf(fp, "%s", store_id);
            sign = 1;
            break;
        }
        i += 4; //陝 翮葆棻 4偃曖 等檜攪陛 襄營ж嘎煎 棻擠 翮煎 檜翕ж晦 嬪л.
    }
    fclose(fp);
    
    gotoxy(20, 25);
    if (sign == 1)
        printf("嬴檜蛤朝 %s 殮棲棻 ", store_id);
    else
        printf("蛔煙脹 嬴檜蛤陛 橈蝗棲棻.");
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
    printf("忙式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式忖 ", y++);
    for (i = 0; i < 12; i++) {
        gotoxy(x, y);
        printf("弛                                    弛 ", y++);
    }
    gotoxy(x, y);
    printf("戌式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式戎 ", y++);

    gotoxy(20, 17);
    printf("ID蒂 殮溘п輿撮蹂 >> ");
    scanf("%s", input_id);
    gotoxy(20, 18);
    printf("з廓擊 殮溘п輿撮蹂 >> ");
    scanf("%s", input_Snum);

    fp = fopen("Member_list.txt", "r");
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);

    i = 0;
    while (21 * i < size) { //だ橾曖 觼晦虜躑 鬚儀
        fseek(fp, i * 21, SEEK_SET);
        fscanf(fp, "%s", store_id);
        if (strcmp(input_id, store_id) == 0) { //翕橾 id 嫦唯
            fseek(fp, 21 * (i + 3), SEEK_SET);
            fscanf(fp, "%s", store_Snum);
            if (strcmp(input_Snum, store_Snum) == 0) {//з廓紫 偽擊 陽
                fseek(fp, 21 * (i + 1), SEEK_SET); //綠塵廓��
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
        printf("綠塵廓�ㄣ� %s 殮棲棻 ", store_pw);
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
    printf("忙式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式忖 ", y++);
    for (i = 0; i < 10; i++) {
        gotoxy(x, y);
        printf("弛                                    弛 ", y++);
    }
    gotoxy(x, y);
    printf("戌式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式戎 ", y++);
    
    
    x = 20; y = 17;
    gotoxy(x, y);
    printf("1. ID瓊晦 ", y += 2);
    gotoxy(x, y);
    printf("2. PW瓊晦 ", y += 2);
    
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
    printf("忙式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式忖 ");
    for (i = 0; i < 15; i++) {
        gotoxy(x, y++);
        printf("弛                                    弛 ");
    }
    gotoxy(x, y++);
    printf("戌式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式戎 ");
    y = 18;
    gotoxy(x + 15, y);
    printf("啪 歜 螃 幗");
    y += 3;
    gotoxy(x + 5, y++);
    printf("溯漣 : %d ", t.level);
    gotoxy(x + 5, y++);
    printf("薄熱 : %d ", t.score);
    gotoxy(x + 5, y++);;
    printf("衛除 : %02d碟 %02d蟾", (int)gt.total_time / 60, (int)gt.total_time % 60);

    /*strcpy(new.ID, ID);
    new.score = t.score;
    new.time = gt.total_time;

    fp = fopen("Ranking.txt", "r");
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);

    fseek(fp, 0, SEEK_SET);
    for (i = 0; i < size / 35; i++) { // size / 35朝 盪濰脹 楨韁曖 熱蒂 瓊晦 嬪л.
        fseek(fp, 35 * i, SEEK_SET);
        fscanf(fp, "%s", Rank[i].ID);
        fseek(fp, (35 * i) + 21, SEEK_SET);
        fscanf(fp, "%d", &Rank[i].score);
        fseek(fp, (35 * i) + 28, SEEK_SET);
        fscanf(fp, "%d", &Rank[i].time);
    }
    fclose(fp);

    for (i = 0; i < size / 35; i++) {
        if (Rank[i].score < new.score) { //渦 堪擎 晦煙
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
    printf("蹴жм棲棻. 楨韁 %d蛔 殮棲棻", i + 1);
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
    printf("忙式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式忖 ", y++);
    for (i = 0; i < 12; i++) {
        gotoxy(x, y);
        printf("弛                                    弛 ", y++);
    }
    gotoxy(x, y);
    printf("戌式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式戎 ", y++);

    gotoxy(20, 17);
    printf("ID蒂 殮溘п輿撮蹂 >> ");
    scanf("%s", input_id);
    strcpy(ID, input_id);
    gotoxy(20, 18);
    printf("PW蒂 殮溘п輿撮蹂 >> ");
    scanf("%s", input_pw);

    fp = fopen("Member_list.txt", "r");
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);

    i = 0;
    while (21 * i < size) { //だ橾曖 觼晦虜躑 鬚儀
        fseek(fp, i * 21, SEEK_SET);
        fscanf(fp, "%s", store_id);
        if (strcmp(input_id, store_id) == 0) { //翕橾 id 嫦唯
            fseek(fp, 21 * (i + 1), SEEK_SET);
            sign = 1;
            fscanf(fp, "%s", store_pw);
            if (strcmp(input_pw, store_pw) == 0) {//綠塵廓�� 橾纂
                sign = 2;
                break;
            }
        }
        i += 4;
    }
    fclose(fp);

    gotoxy(20, 25);
    if (sign == 0) {
        printf("嬴檜蛤蒂 瓊擊 熱 橈蝗棲棻.");
    }
    else if (sign == 1) {
        printf("綠塵廓�ㄟ� 橾纂ж雖 彊蝗棲棻.");
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
    printf("忙式式式式式式式式式式式式式式式式式式式式式式式式式式忖 ", y++);
    for (i = 0; i < 3; i++) {
        gotoxy(x, y);
        printf("弛 ");
        gotoxy(x + 27, y);
        printf("弛 ", y++);
    }
    gotoxy(x, y);
    printf("戌式式式式式式式式式式式式式式式式式式式式式式式式式式戎 ", y++);
    gotoxy(49, 14);
    printf("M E N U");
    gotoxy(x, y);
    printf("忙式式式式式式式式式式式式式式式式式式式式式式式式式式忖 ", y++);
    for (i = 0; i < 15; i++) {
        gotoxy(x, y);
        printf("弛 ");
        gotoxy(x + 27, y);
        printf("弛 ", y++);
    }
    gotoxy(x, y);
    printf("戌式式式式式式式式式式式式式式式式式式式式式式式式式式戎 ", y++);

    setColor(BLACK, WHITE);
    if (select == 1)
        setColor(BLACK, LIGHTBLUE);
    x = 46; y = 19;
    gotoxy(x, y);
    printf("忙式式式式式式式式式忖 ", y++);
    gotoxy(x, y);
    printf("弛 啪歜ж晦弛 ", y++);
    gotoxy(x, y);
    printf("戌式式式式式式式式式戎 ", y++);
    setColor(BLACK, WHITE);


    if (select == 2)
        setColor(BLACK, LIGHTBLUE);
    gotoxy(x, y);
    printf("忙式式式式式式式式式忖 ", y++);
    gotoxy(x, y);
    printf("弛 啪歜寞徹弛 ", y++);
    gotoxy(x, y);
    printf("戌式式式式式式式式式戎 ", y++);
    setColor(BLACK, WHITE);

    if (select == 3)
        setColor(BLACK, LIGHTBLUE);
    gotoxy(x, y);
    printf("忙式式式式式式式式式忖 ", y++);
    gotoxy(x, y);
    printf("弛 楨韁爾晦弛 ", y++);
    gotoxy(x, y);
    printf("戌式式式式式式式式式戎 ", y++);
    setColor(BLACK, WHITE);

    if (select == 4)
        setColor(BLACK, LIGHTBLUE);
    gotoxy(x, y);
    printf("忙式式式式式式式式式忖 ", y++);
    gotoxy(x, y);
    printf("弛 啪歜謙猿弛 ", y++);
    gotoxy(x, y);
    printf("戌式式式式式式式式式戎 ", y++);
    setColor(BLACK, WHITE);

}
void Game_Rule() {
    int i;
    int x, y;

    x = 15; y = 12;
    gotoxy(x, y);
    printf("忙式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式忖 ", y++); //式 40偃 忙 忖 1偃噶
    for (i = 0; i < 15; i++) {
        gotoxy(x, y);
        printf("弛                                        弛 ", y++);
    }
    gotoxy(x, y);
    printf("戌式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式戎 ", y++);

    gotoxy(35, 38);
    printf("給嬴陛衛溥賊 嬴鼠酈釭 揚楝輿撮蹂.");

    x = 20; y = 16;
    gotoxy(x, y);
    printf("∠ 謝煎 檜翕  ⊿ 嬴楚煎 檜翕  ", y++);
    gotoxy(x, y);
    printf("⊥ 辦煎 檜翕  ∟ 綰煙 �蛻� ", y++);
    gotoxy(x, y);
    printf("SPACE BAR 綰楣 ж鬼", y++);
    gotoxy(x, y);
    printf("ESC 橾衛 薑雖", y++);
    y++;
    gotoxy(x, y);
    printf("薄熱 500薄渡 陪檜紫 鼻蝓", y++);
    gotoxy(x, y);
    printf("и 塭檣 薯剪渡 50薄", y++);
    gotoxy(x, y);
    printf("罹楝 塭檣 翕衛縑 薯剪衛 爾傘蝶 薄熱", y++);

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
                printf("﹥");
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
void Display_Game_Play() { //啪歜 Ы溯檜 �飛� 轎溘
	int y, x;
	for (y = 0; y < 20; y++) {
		for (x = 0; x < 10; x++) {
			if (Game_Space[y][x] != cpy_Game_Space[y][x]) {  //滲�倆� 睡碟虜 夥紱晦 嬪л.
                    //0 : 綴奢除, 1: 漁(綰楣), 2:鐘お煤 綰楣, 3:紫雜雖薄
                    //綰煙檣 奢除
					if (Game_Space[y][x] == 1 || Game_Space[y][x] == 2) {
						gotoxy((MAP_X + 2) + (x * 2), (MAP_Y + 1) + y);       
						printf("﹥");
						cpy_Game_Space[y][x] = 1;
					}
                    //綴奢除檣 奢除
                    else if (Game_Space[y][x] == 0) {
                        gotoxy((MAP_X + 2) + (x * 2), (MAP_Y + 1) + y);
                        printf("  ");
                        cpy_Game_Space[y][x] = 0;
                    }
                    //紫雜 蕨薑 奢除
                    else {
                        gotoxy((MAP_X + 2) + (x * 2), (MAP_Y + 1) + y);
                        printf("﹤");
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

    //綰煙 儅撩
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
    //儅撩 嬪纂縑 檜嘐 綰煙檜 氈棻賊 啪歜螃幗 蹺陛
}

void rotate_block(Tetris* t) {
    int i, j, sign = 0;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            //�蛻�й 綰楣檜 紫雜ж朝 嬪纂縑 漁(綰楣)檜 氈朝雖 匐餌
            if (block[t->curblock][(t->rotation + 1) % 4][i][j] == 1) {
                if (t->curY + i >= 0 && t->curY + i <= 19){ //嬪, 嬴楚 漁 匐餌
                    if (t->curX + j >= 0 && t->curX + j <= 9) { //謝, 辦 漁 匐餌
                        if (Game_Space[t->curY + i][t->curX + j] == 1) { //檜嘐 綰楣檜 氈朝唳辦
                            sign = 1;
                            break;
                        }
                    }
                }
            }
        }
    }

    if (sign == 0) { //綰楣檜 �蛻�й 熱 氈朝 唳辦

        //氈湍 綰楣擊 薯剪ж晦 嬪л
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                if (block[t->curblock][t->rotation][i][j] == 1) {
                    Game_Space[t->curY + i][t->curX + j] = 0;
                }
            }
        }

        t->rotation = (t->rotation + 1) % 4;
        //億煎檜 綰楣擊 虜菟橫輿晦 嬪л
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                if (block[t->curblock][t->rotation][i][j] == 1) {
                    Game_Space[t->curY + i][t->curX + j] = 2;
                }
            }
        }


    }
}
void break_line(Tetris* t) { //還檜 粕 獺擊 唳辦 雖辦朝 л熱
    int i, j, k, count, broke = 0;
    for (i = 0; i < 20; i++) {
        count = 0;
        for (j = 0; j < 10; j++) {
            if (Game_Space[i][j] == 1) { // 碳縑 綰楣曖 熱蒂 蘋遴お
                count++;
            }
        }
        if (count == 10) { //蘋遴お脹 綰煙檜 10偃 (粕霤)
            for (k = i; k > 1; k--) { //嬴楚憮睡攪 霞ч.
                for (j = 0; j < 10; j++) {
                    if (Game_Space[k - 1][j] != 2) { //鐘お煤醞檣 綰煙檜 嬴棋 奢除縑 иж罹
                        Game_Space[k][j] = Game_Space[k - 1][j]; //и蘊 嬪曖 綰楣擊 嬴楚煎 衡梯
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
    for (i = 3; 0 <= i; i--) { //晦獄 綰楣曖 薑爾陛 4x4曖 薑爾蒂 陛雖堅 氈歷戲棲, 4x4蒂 鬚儀ж貊 橫替 蘊縑 綰楣檜 氈朝雖 羹觼
        for (j = 3; 0 <= j; j--) {
            if (block[t->curblock][t->rotation][j][i] == 1) { //綰楣檜 氈朝 謝ル蒂 瓊懊擊 陽
                count++;
                if (Game_Space[t->curY + j + 1][t->curX + i] != 1 && t->curY + j + 1 <= 19) { //綰楣曖 嬴楚縑 棻艇 綰楣檜 橈堅, 嬴概薹檜 漁檜 嬴棋唳辦
                    sign++;
                }
            }
        }
    }
    if (count == sign) {
        for (i = 3; 0 <= i; i--) {
            for (j = 3; 0 <= j; j--) {
                if (Game_Space[t->curY + j][t->curX + i] == 2) {
                    Game_Space[t->curY + j + 1][t->curX + i] = 2; //п渡 綰楣擊 嬴楚煎
                    Game_Space[t->curY + j][t->curX + i] = 0; //綰楣檜 氈湍 濠葬朝 0戲煎 蟾晦��
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
    case UP : //�蛻�
        rotate_block(t);
        break;
    case LEFT:
        sign = 0;
        count = 0;
        for (i = 0; i < 4; i++) { //晦獄 綰楣曖 薑爾陛 4x4曖 薑爾蒂 陛雖堅 氈歷戲棲, 4x4蒂 鬚儀ж貊 橫替 蘊縑 綰楣檜 氈朝雖 羹觼
            for (j = 0; j < 4; j++) {
                if (block[t->curblock][t->rotation][j][i] == 1) { //綰楣檜 氈朝 謝ル蒂 瓊懊擊 陽
                    count++;
                    if (Game_Space[t->curY + j][t->curX + i - 1] != 1 && (t->curX + i - 1) >= 0) { //綰楣曖 謝難縑 棻艇 綰楣檜 橈堅, 豭薹檜 漁檜 嬴棋唳辦
                        sign++;
                    }
                }
            }
        }
        if (count == sign) { 
            for (i = 0; i < 4; i++) {
                for (j = 0; j < 4; j++) {
                    if (Game_Space[t->curY + j][t->curX + i] == 2) {
                        Game_Space[t->curY + j][t->curX + i - 1] = 2; //п渡 綰楣擊 謝難戲煎
                        Game_Space[t->curY + j][t->curX + i] = 0; //綰楣檜 氈湍 濠葬朝 0戲煎 蟾晦��
                    }
                }
            }
            t->curX--;
        }

        break;

    case RIGHT:
        sign = 0;
        count = 0;
        for (i = 3; 0 <= i; i--) { //晦獄 綰楣曖 薑爾陛 4x4曖 薑爾蒂 陛雖堅 氈歷戲棲, 4x4蒂 鬚儀ж貊 橫替 蘊縑 綰楣檜 氈朝雖 羹觼
            for (j = 3; 0 <= j; j--) {
                if (block[t->curblock][t->rotation][j][i] == 1) { //綰楣檜 氈朝 謝ル蒂 瓊懊擊 陽
                    count++;
                    if (Game_Space[t->curY + j][t->curX + i + 1] != 1 && (t->curX + i + 1) <= 9) { //綰楣曖 辦難縑 棻艇 綰楣檜 橈堅, 辦難檜 漁檜 嬴棋唳辦
                        sign++;
                    }
                }
            }
        }
        if (count == sign) {
            for (i = 3; 0 <= i; i--) { //晦獄 綰楣曖 薑爾陛 4x4曖 薑爾蒂 陛雖堅 氈歷戲棲, 4x4蒂 鬚儀ж貊 橫替 蘊縑 綰楣檜 氈朝雖 羹觼
                for (j = 3; 0 <= j; j--) {
                    if (Game_Space[t->curY + j][t->curX + i] == 2) {
                        Game_Space[t->curY + j][t->curX + i + 1] = 2; //п渡 綰楣擊 辦難戲煎
                        Game_Space[t->curY + j][t->curX + i] = 0; //綰楣檜 氈湍 濠葬朝 0戲煎 蟾晦��
                    }
                }
            }
            t->curX++;
        }
        break;

    case DOWN://и蘊 壽戲煎
        down_block(t);
        break;

    case SPACEBAR: //ж鬼
        y = 0;
        while (TRUE) {
            sign = 0;
            for (i = 0; i < 4; i++) {
                for (j = 0; j < 4; j++) {
                    if (block[t->curblock][t->rotation][j][i] == 1) { //綰煙檜 氈朝 奢除 瓊晦
                        if (Game_Space[t->curY + j + 1][t->curX + i] == 1) { //鐘お煤醞檣 綰煙 嬴楚縑 漁檜 氈朝雖 匐餌
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

void set_gamemap(){ //啪歜 衛濛衛 裘擊 賅舒 0戲煎 蟾晦��
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

        if (new_block_sign == 0) { //褻濛醞檣 綰煙檜 橈朝 唳辦 億煎遴 綰煙 儅撩
            New_Block(&t, t.nextblock);
            t.nextblock = rand() % 7; //綰楣 謙盟唸薑擊 嬪и 楠渾л熱.
            new_block_sign = 1;
            Display_nextblock(&t);
        }

        Display_Game_Play(); //啪歜 �飛� 偵褐
        Display_Game_Info(&t, gametime);

        while (kbhit() != 0) { //酈爾萄 殮溘
            key = _getch();
            
            
            if (key == ESC) {//橾衛薑雖 esc
                gotoxy(30, 38);
                gametime.end_time = time(0);
                gametime.total_time += gametime.end_time - gametime.start_time; //橾衛薑雖 晦除翕寰擎 衛除 羹觼 X
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
        Sleep(33); //1蟾縑 30Щ溯歜
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
        gametime.total_time += gametime.end_time - gametime.start_time; //部陪衛陝 - 衛濛衛陝 = 霞ч衛除
    }
}

int main(void)
{
	system("mode con cols=70 lines=40");
    SetConsoleTitle(TEXT("Tetris"));
	removeCursor();						// 夔樂璽 醴憮 雖辦晦
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
                    case 1: //煎斜檣
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
                                        case 1: //啪歜ж晦
                                            system("cls");
                                            game_start();
                                            break;
                                        case 2: //啪歜寞徹
                                            system("cls");
                                            Game_Rule();
                                            break;
                                        case 3: //楨韁爾晦
                                            break;
                                        case 4: //啪歜謙猿
                                            system("cls");
                                            exit(0);
                                            break;
                                        }
                                    }
                                }
                            }
                        }  
                        break;
                    case 2: //�蛾灠㊣�
                        system("cls");
                        Register_Screen();
                        break;
                    case 3: //�蛾躞ㄠ�
                        system("cls");
                        Find_member();
                        break;
                    case 4: //謙猿
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
    printf("啪歜 謙猿");

	return 0;
}