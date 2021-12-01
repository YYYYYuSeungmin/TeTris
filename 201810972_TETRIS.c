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
int block[7][4][4][4] = { // [������ ����], [������ ȸ��], [������ ���� �ִ����], [������ ���� �ִ����]
    //����                // �迭 �޺κ� [4][4]�� ȸ���� ���� �޶����� ������ ������ �׸�.
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

     //������
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

     // z���
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

     //�� z���
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

     //�Ǹ��
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

     //���� ���
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

     //���� ���
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
enum color {        //�ֻܼ󿡼� ����� �� ����
    BLACK, BLUE, GREEN, IADE, RED, PURPLE, YELLOW, WHITE, GRAY, LIGHTBLUE, LIGHTGREEN, LIGHTIADE, LIGHTRED,
	LIGHTPURPLE, LIGHTYELLOW, LIGHTWHITE
};
void gotoxy(int x, int y) { //�ܼ�â Ŀ�� ����
	COORD Pos = { x - 1, y - 1 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}
void setColor(unsigned short background, unsigned short text) { //setColor(���, �ؽ�Ʈ)
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (background >> 4) | text);
}
void removeCursor() { //�ܼ�â Ŀ�� �����
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}
void reviveCursor() { //�ܼ�â Ŀ�� �����
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = TRUE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void Starting_Screen(int select) { //�� �ʱ� ���� ȭ�� ( �α���, ȸ������, ȸ��ã��, ���� )
    int y = 5;

    setColor(BLACK, LIGHTWHITE);
    gotoxy(3, y);
    printf("������ ������ ������ ������ ������ ������", y++);
    gotoxy(3, y);
    printf("    ��     ��             ��     ��      ��     ��     ��        ", y++);
    gotoxy(3, y);
    printf("    ��     ������     ��     ������     ��     ������", y++);
    gotoxy(3, y);
    printf("    ��     ��             ��     ��    ��       ��             ��", y++);
    gotoxy(3, y);
    printf("    ��     ������     ��     ��      �� ������ ������", y++);


    setColor(BLACK, WHITE);
    if (select == 1)
        setColor(BLACK, LIGHTBLUE);
    y = 20;
    gotoxy(30, y);
    printf("���������������������� ", y++);
    gotoxy(30, y);
    printf("��  �α��� �� ", y++);
    gotoxy(30, y);
    printf("���������������������� ", y++);
    setColor(BLACK, WHITE);


    if (select == 2)
        setColor(BLACK, LIGHTBLUE);
    gotoxy(30, y);
    printf("���������������������� ", y++);
    gotoxy(30, y);
    printf("�� ȸ�����Ԧ� ", y++);
    gotoxy(30, y);
    printf("���������������������� ", y++);
    setColor(BLACK, WHITE);

    if (select == 3)
        setColor(BLACK, LIGHTBLUE);
    gotoxy(30, y);
    printf("���������������������� ", y++);
    gotoxy(30, y);
    printf("�� ȸ��ã�⦢ ", y++);
    gotoxy(30, y);
    printf("���������������������� ", y++);
    setColor(BLACK, WHITE);

    if (select == 4)
        setColor(BLACK, LIGHTBLUE);
    gotoxy(30, y);
    printf("���������������������� ", y++);
    gotoxy(30, y);
    printf("��   ����  �� ", y++);
    gotoxy(30, y);
    printf("���������������������� ", y++);
    setColor(BLACK, WHITE);

    gotoxy(69, 39);
}
void Next_block_box() { //���� ���� � ������ ǥ��Ǵ� �ڽ� ����
    int x, y;
    x = 31; y = 12;
    gotoxy(x, y++);
    printf("�������");
    for (int i = 0; i < 4; i++) {
        gotoxy(x, y);
        printf("��        ��", y++);
    }
    gotoxy(x, y);
    printf("�������");
}
void Game_Screen() { //��Ʈ���� ����Ǵ� �� ǥ��
    int x = 7, y = 12;


    setColor(BLACK, LIGHTWHITE);
    gotoxy(x, y);
    printf("�������������", y++);
    for (int i = 0; i < 20; i++) {
        gotoxy(x, y);
        printf("��                    ��", y++);
    }
    gotoxy(x, y);
    printf("�������������", y++);
}

int overlap_check(char* id) { // id�ߺ��� �ִ��� üũ�ϴ� �Լ�
    char check[20];
    FILE* fp;
    int i = 0;
    long size;
    
    fp = fopen("Member_list.txt", "r");
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);

    while (21 * i < size) { //������ ũ�⸸ŭ Ž��
        fseek(fp, i * 21, SEEK_SET);
        fscanf(fp, "%s", check);
        i += 4;
        if (strcmp(check, id) == 0) { //�ߺ��߻�
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
    printf("�� ������ �����ʴϱ�? (Y/N) >> ");
    scanf(" %c", &check);
    
    overlap = overlap_check(&m.ID);
    if (overlap == 1) {
        gotoxy(x, y + 2);
        printf("�ߺ��� ID�Դϴ�.");
        Sleep(3000);
        removeCursor();
        system("cls");
        return;
    }
    if (check == 89 || check == 121) { //Y : 89, y : 121 �ƽ�Ű�ڵ�
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
    printf("���������������������������������������������������������������������������� ", y++);
    for (i = 0; i < 10; i++) {
        gotoxy(x, y);
        printf("��                                    �� ", y++);
    }
    gotoxy(x, y);
    printf("���������������������������������������������������������������������������� ", y++);

    x = 20; y = 17;
    gotoxy(x, y);
    printf("�̸� : ", y += 2);
    gotoxy(x, y);
    printf("�й� : ", y += 2);
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
    printf("���������������������������������������������������������������������������� ", y++);
    for (i = 0; i < 12; i++) {
        gotoxy(x, y);
        printf("��                                    �� ", y++);
    }
    gotoxy(x, y);
    printf("���������������������������������������������������������������������������� ", y++);

    gotoxy(20, 17);
    printf("�̸��� �Է����ּ��� >> ");
    scanf("%s", input_name);

    fp = fopen("Member_list.txt", "r");
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);

    i = 2;
    while (21 * i < size) { //������ ũ�⸸ŭ Ž��
        fseek(fp, i * 21, SEEK_SET);
        fscanf(fp, "%s", store_name);
        if (strcmp(input_name, store_name) == 0) { //���� ���̵� �߰�
            fseek(fp, 21 * (i - 2), SEEK_SET); // ���������͸� ID��ġ�� �Ű� ��ĵ
            fscanf(fp, "%s", store_id);
            sign = 1;
            break;
        }
        i += 4; //�� ������ 4���� �����Ͱ� �����ϹǷ� ���� ���� �̵��ϱ� ����.
    }
    fclose(fp);
    
    gotoxy(20, 25);
    if (sign == 1)
        printf("���̵�� %s �Դϴ� ", store_id);
    else
        printf("��ϵ� ���̵� �����ϴ�.");
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
    printf("���������������������������������������������������������������������������� ", y++);
    for (i = 0; i < 12; i++) {
        gotoxy(x, y);
        printf("��                                    �� ", y++);
    }
    gotoxy(x, y);
    printf("���������������������������������������������������������������������������� ", y++);

    gotoxy(20, 17);
    printf("ID�� �Է����ּ��� >> ");
    scanf("%s", input_id);
    gotoxy(20, 18);
    printf("�й��� �Է����ּ��� >> ");
    scanf("%s", input_Snum);

    fp = fopen("Member_list.txt", "r");
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);

    i = 0;
    while (21 * i < size) { //������ ũ�⸸ŭ Ž��
        fseek(fp, i * 21, SEEK_SET);
        fscanf(fp, "%s", store_id);
        if (strcmp(input_id, store_id) == 0) { //���� id �߰�
            fseek(fp, 21 * (i + 3), SEEK_SET);
            fscanf(fp, "%s", store_Snum);
            if (strcmp(input_Snum, store_Snum) == 0) {//�й��� ���� ��
                fseek(fp, 21 * (i + 1), SEEK_SET); //��й�ȣ
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
        printf("��й�ȣ�� %s �Դϴ� ", store_pw);
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
    printf("���������������������������������������������������������������������������� ", y++);
    for (i = 0; i < 10; i++) {
        gotoxy(x, y);
        printf("��                                    �� ", y++);
    }
    gotoxy(x, y);
    printf("���������������������������������������������������������������������������� ", y++);
    
    
    x = 20; y = 17;
    gotoxy(x, y);
    printf("1. IDã�� ", y += 2);
    gotoxy(x, y);
    printf("2. PWã�� ", y += 2);
    
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
    printf("���������������������������������������������������������������������������� ");
    for (i = 0; i < 15; i++) {
        gotoxy(x, y++);
        printf("��                                    �� ");
    }
    gotoxy(x, y++);
    printf("���������������������������������������������������������������������������� ");
    y = 18;
    gotoxy(x + 15, y);
    printf("�� �� �� ��");
    y += 3;
    gotoxy(x + 5, y++);
    printf("���� : %d ", t.level);
    gotoxy(x + 5, y++);
    printf("���� : %d ", t.score);
    gotoxy(x + 5, y++);;
    printf("�ð� : %02d�� %02d��", (int)gt.total_time / 60, (int)gt.total_time % 60);

    /*strcpy(new.ID, ID);
    new.score = t.score;
    new.time = gt.total_time;

    fp = fopen("Ranking.txt", "r");
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);

    fseek(fp, 0, SEEK_SET);
    for (i = 0; i < size / 35; i++) { // size / 35�� ����� ��ŷ�� ���� ã�� ����.
        fseek(fp, 35 * i, SEEK_SET);
        fscanf(fp, "%s", Rank[i].ID);
        fseek(fp, (35 * i) + 21, SEEK_SET);
        fscanf(fp, "%d", &Rank[i].score);
        fseek(fp, (35 * i) + 28, SEEK_SET);
        fscanf(fp, "%d", &Rank[i].time);
    }
    fclose(fp);

    for (i = 0; i < size / 35; i++) {
        if (Rank[i].score < new.score) { //�� ���� ���
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
    printf("�����մϴ�. ��ŷ %d�� �Դϴ�", i + 1);
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
    printf("���������������������������������������������������������������������������� ", y++);
    for (i = 0; i < 12; i++) {
        gotoxy(x, y);
        printf("��                                    �� ", y++);
    }
    gotoxy(x, y);
    printf("���������������������������������������������������������������������������� ", y++);

    gotoxy(20, 17);
    printf("ID�� �Է����ּ��� >> ");
    scanf("%s", input_id);
    strcpy(ID, input_id);
    gotoxy(20, 18);
    printf("PW�� �Է����ּ��� >> ");
    scanf("%s", input_pw);

    fp = fopen("Member_list.txt", "r");
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);

    i = 0;
    while (21 * i < size) { //������ ũ�⸸ŭ Ž��
        fseek(fp, i * 21, SEEK_SET);
        fscanf(fp, "%s", store_id);
        if (strcmp(input_id, store_id) == 0) { //���� id �߰�
            fseek(fp, 21 * (i + 1), SEEK_SET);
            sign = 1;
            fscanf(fp, "%s", store_pw);
            if (strcmp(input_pw, store_pw) == 0) {//��й�ȣ ��ġ
                sign = 2;
                break;
            }
        }
        i += 4;
    }
    fclose(fp);

    gotoxy(20, 25);
    if (sign == 0) {
        printf("���̵� ã�� �� �����ϴ�.");
    }
    else if (sign == 1) {
        printf("��й�ȣ�� ��ġ���� �ʽ��ϴ�.");
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
    printf("�������������������������������������������������������� ", y++);
    for (i = 0; i < 3; i++) {
        gotoxy(x, y);
        printf("�� ");
        gotoxy(x + 27, y);
        printf("�� ", y++);
    }
    gotoxy(x, y);
    printf("�������������������������������������������������������� ", y++);
    gotoxy(49, 14);
    printf("M E N U");
    gotoxy(x, y);
    printf("�������������������������������������������������������� ", y++);
    for (i = 0; i < 15; i++) {
        gotoxy(x, y);
        printf("�� ");
        gotoxy(x + 27, y);
        printf("�� ", y++);
    }
    gotoxy(x, y);
    printf("�������������������������������������������������������� ", y++);

    setColor(BLACK, WHITE);
    if (select == 1)
        setColor(BLACK, LIGHTBLUE);
    x = 46; y = 19;
    gotoxy(x, y);
    printf("���������������������� ", y++);
    gotoxy(x, y);
    printf("�� �����ϱ⦢ ", y++);
    gotoxy(x, y);
    printf("���������������������� ", y++);
    setColor(BLACK, WHITE);


    if (select == 2)
        setColor(BLACK, LIGHTBLUE);
    gotoxy(x, y);
    printf("���������������������� ", y++);
    gotoxy(x, y);
    printf("�� ���ӹ���� ", y++);
    gotoxy(x, y);
    printf("���������������������� ", y++);
    setColor(BLACK, WHITE);

    if (select == 3)
        setColor(BLACK, LIGHTBLUE);
    gotoxy(x, y);
    printf("���������������������� ", y++);
    gotoxy(x, y);
    printf("�� ��ŷ���⦢ ", y++);
    gotoxy(x, y);
    printf("���������������������� ", y++);
    setColor(BLACK, WHITE);

    if (select == 4)
        setColor(BLACK, LIGHTBLUE);
    gotoxy(x, y);
    printf("���������������������� ", y++);
    gotoxy(x, y);
    printf("�� �������ᦢ ", y++);
    gotoxy(x, y);
    printf("���������������������� ", y++);
    setColor(BLACK, WHITE);

}
void Game_Rule() {
    int i;
    int x, y;

    x = 15; y = 12;
    gotoxy(x, y);
    printf("������������������������������������������������������������������������������������ ", y++); //�� 40�� �� �� 1����
    for (i = 0; i < 15; i++) {
        gotoxy(x, y);
        printf("��                                        �� ", y++);
    }
    gotoxy(x, y);
    printf("������������������������������������������������������������������������������������ ", y++);

    gotoxy(35, 38);
    printf("���ư��÷��� �ƹ�Ű�� �����ּ���.");

    x = 20; y = 16;
    gotoxy(x, y);
    printf("�� �·� �̵�  �� �Ʒ��� �̵�  ", y++);
    gotoxy(x, y);
    printf("�� ��� �̵�  �� ��� ȸ�� ", y++);
    gotoxy(x, y);
    printf("SPACE BAR �� �ϰ�", y++);
    gotoxy(x, y);
    printf("ESC �Ͻ� ����", y++);
    y++;
    gotoxy(x, y);
    printf("���� 500���� ���̵� ���", y++);
    gotoxy(x, y);
    printf("�� ���� ���Ŵ� 50��", y++);
    gotoxy(x, y);
    printf("���� ���� ���ÿ� ���Ž� ���ʽ� ����", y++);

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
                printf("��");
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
void Display_Game_Play() { //���� �÷��� ȭ�� ���
	int y, x;
	for (y = 0; y < 20; y++) {
		for (x = 0; x < 10; x++) {
			if (Game_Space[y][x] != cpy_Game_Space[y][x]) {  //��ȭ�� �κи� �ٲٱ� ����.
                    //0 : �����, 1: ��(��), 2:��Ʈ�� ��, 3:��������
                    //����� ����
					if (Game_Space[y][x] == 1 || Game_Space[y][x] == 2) {
						gotoxy((MAP_X + 2) + (x * 2), (MAP_Y + 1) + y);       
						printf("��");
						cpy_Game_Space[y][x] = 1;
					}
                    //������� ����
                    else if (Game_Space[y][x] == 0) {
                        gotoxy((MAP_X + 2) + (x * 2), (MAP_Y + 1) + y);
                        printf("  ");
                        cpy_Game_Space[y][x] = 0;
                    }
                    //���� ���� ����
                    else {
                        gotoxy((MAP_X + 2) + (x * 2), (MAP_Y + 1) + y);
                        printf("��");
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

    //��� ����
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
    //���� ��ġ�� �̹� ����� �ִٸ� ���ӿ��� �߰�
}

void rotate_block(Tetris* t) {
    int i, j, sign = 0;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            //ȸ���� ���� �����ϴ� ��ġ�� ��(��)�� �ִ��� �˻�
            if (block[t->curblock][(t->rotation + 1) % 4][i][j] == 1) {
                if (t->curY + i >= 0 && t->curY + i <= 19){ //��, �Ʒ� �� �˻�
                    if (t->curX + j >= 0 && t->curX + j <= 9) { //��, �� �� �˻�
                        if (Game_Space[t->curY + i][t->curX + j] == 1) { //�̹� ���� �ִ°��
                            sign = 1;
                            break;
                        }
                    }
                }
            }
        }
    }

    if (sign == 0) { //���� ȸ���� �� �ִ� ���

        //�ִ� ���� �����ϱ� ����
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                if (block[t->curblock][t->rotation][i][j] == 1) {
                    Game_Space[t->curY + i][t->curX + j] = 0;
                }
            }
        }

        t->rotation = (t->rotation + 1) % 4;
        //������ ���� ������ֱ� ����
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                if (block[t->curblock][t->rotation][i][j] == 1) {
                    Game_Space[t->curY + i][t->curX + j] = 2;
                }
            }
        }


    }
}
void break_line(Tetris* t) { //���� �� á�� ��� ����� �Լ�
    int i, j, k, count, broke = 0;
    for (i = 0; i < 20; i++) {
        count = 0;
        for (j = 0; j < 10; j++) {
            if (Game_Space[i][j] == 1) { // �ҿ� ���� ���� ī��Ʈ
                count++;
            }
        }
        if (count == 10) { //ī��Ʈ�� ����� 10�� (����)
            for (k = i; k > 1; k--) { //�Ʒ������� ����.
                for (j = 0; j < 10; j++) {
                    if (Game_Space[k - 1][j] != 2) { //��Ʈ������ ����� �ƴ� ������ ���Ͽ�
                        Game_Space[k][j] = Game_Space[k - 1][j]; //��ĭ ���� ���� �Ʒ��� �ű�
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
    for (i = 3; 0 <= i; i--) { //�⺻ ���� ������ 4x4�� ������ ������ �־�����, 4x4�� Ž���ϸ� ��� ĭ�� ���� �ִ��� üũ
        for (j = 3; 0 <= j; j--) {
            if (block[t->curblock][t->rotation][j][i] == 1) { //���� �ִ� ��ǥ�� ã���� ��
                count++;
                if (Game_Space[t->curY + j + 1][t->curX + i] != 1 && t->curY + j + 1 <= 19) { //���� �Ʒ��� �ٸ� ���� ����, �Ʒ����� ���� �ƴѰ��
                    sign++;
                }
            }
        }
    }
    if (count == sign) {
        for (i = 3; 0 <= i; i--) {
            for (j = 3; 0 <= j; j--) {
                if (Game_Space[t->curY + j][t->curX + i] == 2) {
                    Game_Space[t->curY + j + 1][t->curX + i] = 2; //�ش� ���� �Ʒ���
                    Game_Space[t->curY + j][t->curX + i] = 0; //���� �ִ� �ڸ��� 0���� �ʱ�ȭ
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
    case UP : //ȸ��
        rotate_block(t);
        break;
    case LEFT:
        sign = 0;
        count = 0;
        for (i = 0; i < 4; i++) { //�⺻ ���� ������ 4x4�� ������ ������ �־�����, 4x4�� Ž���ϸ� ��� ĭ�� ���� �ִ��� üũ
            for (j = 0; j < 4; j++) {
                if (block[t->curblock][t->rotation][j][i] == 1) { //���� �ִ� ��ǥ�� ã���� ��
                    count++;
                    if (Game_Space[t->curY + j][t->curX + i - 1] != 1 && (t->curX + i - 1) >= 0) { //���� ������ �ٸ� ���� ����, ������ ���� �ƴѰ��
                        sign++;
                    }
                }
            }
        }
        if (count == sign) { 
            for (i = 0; i < 4; i++) {
                for (j = 0; j < 4; j++) {
                    if (Game_Space[t->curY + j][t->curX + i] == 2) {
                        Game_Space[t->curY + j][t->curX + i - 1] = 2; //�ش� ���� ��������
                        Game_Space[t->curY + j][t->curX + i] = 0; //���� �ִ� �ڸ��� 0���� �ʱ�ȭ
                    }
                }
            }
            t->curX--;
        }

        break;

    case RIGHT:
        sign = 0;
        count = 0;
        for (i = 3; 0 <= i; i--) { //�⺻ ���� ������ 4x4�� ������ ������ �־�����, 4x4�� Ž���ϸ� ��� ĭ�� ���� �ִ��� üũ
            for (j = 3; 0 <= j; j--) {
                if (block[t->curblock][t->rotation][j][i] == 1) { //���� �ִ� ��ǥ�� ã���� ��
                    count++;
                    if (Game_Space[t->curY + j][t->curX + i + 1] != 1 && (t->curX + i + 1) <= 9) { //���� ������ �ٸ� ���� ����, ������ ���� �ƴѰ��
                        sign++;
                    }
                }
            }
        }
        if (count == sign) {
            for (i = 3; 0 <= i; i--) { //�⺻ ���� ������ 4x4�� ������ ������ �־�����, 4x4�� Ž���ϸ� ��� ĭ�� ���� �ִ��� üũ
                for (j = 3; 0 <= j; j--) {
                    if (Game_Space[t->curY + j][t->curX + i] == 2) {
                        Game_Space[t->curY + j][t->curX + i + 1] = 2; //�ش� ���� ��������
                        Game_Space[t->curY + j][t->curX + i] = 0; //���� �ִ� �ڸ��� 0���� �ʱ�ȭ
                    }
                }
            }
            t->curX++;
        }
        break;

    case DOWN://��ĭ ������
        down_block(t);
        break;

    case SPACEBAR: //�ϰ�
        y = 0;
        while (TRUE) {
            sign = 0;
            for (i = 0; i < 4; i++) {
                for (j = 0; j < 4; j++) {
                    if (block[t->curblock][t->rotation][j][i] == 1) { //����� �ִ� ���� ã��
                        if (Game_Space[t->curY + j + 1][t->curX + i] == 1) { //��Ʈ������ ��� �Ʒ��� ���� �ִ��� �˻�
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

void set_gamemap(){ //���� ���۽� ���� ��� 0���� �ʱ�ȭ
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

        if (new_block_sign == 0) { //�������� ����� ���� ��� ���ο� ��� ����
            New_Block(&t, t.nextblock);
            t.nextblock = rand() % 7; //�� ���������� ���� �����Լ�.
            new_block_sign = 1;
            Display_nextblock(&t);
        }

        Display_Game_Play(); //���� ȭ�� ����
        Display_Game_Info(&t, gametime);

        while (kbhit() != 0) { //Ű���� �Է�
            key = _getch();
            
            
            if (key == ESC) {//�Ͻ����� esc
                gotoxy(30, 38);
                gametime.end_time = time(0);
                gametime.total_time += gametime.end_time - gametime.start_time; //�Ͻ����� �Ⱓ������ �ð� üũ X
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
        Sleep(33); //1�ʿ� 30������
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
        gametime.total_time += gametime.end_time - gametime.start_time; //�����ð� - ���۽ð� = ����ð�
    }
}

int main(void)
{
	system("mode con cols=70 lines=40");
    SetConsoleTitle(TEXT("Tetris"));
	removeCursor();						// �ܼ�â Ŀ�� �����
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
                    case 1: //�α���
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
                                        case 1: //�����ϱ�
                                            system("cls");
                                            game_start();
                                            break;
                                        case 2: //���ӹ��
                                            system("cls");
                                            Game_Rule();
                                            break;
                                        case 3: //��ŷ����
                                            break;
                                        case 4: //��������
                                            system("cls");
                                            exit(0);
                                            break;
                                        }
                                    }
                                }
                            }
                        }  
                        break;
                    case 2: //ȸ������
                        system("cls");
                        Register_Screen();
                        break;
                    case 3: //ȸ��ã��
                        system("cls");
                        Find_member();
                        break;
                    case 4: //����
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
    printf("���� ����");

	return 0;
}