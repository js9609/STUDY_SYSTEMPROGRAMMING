#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

// DB 포맷 확인용
#define APPNAME "MYDB"

// ANSI 코드 
// 스크린 지우기
#define SCR_CLEAR printf("^[[2J")
// x,y 좌표로 커서이동하기
#define MOVE_CURSOR(x, y) printf("^[[%d;%dH", x, y)

// 메시지를 출력하고 사용자 입력을 기다린다. 
// 스크린 지우기 전에 메시지를 확인할 목적으로 
// 사용된다. 
#define WAIT_INPUT(x) printf("%s", x);getchar() 

// 개행문자 제거
#define chop(str) str[strlen(str)-1] = '\0'; 

// 헤더의 크기 정의  
// 헤더는 recode 를 제외한 파일의 가장앞에 있는 
// 정보이다. 

// DB 포맷 정보 크기 
#define DBINFO_SIZE strlen(APPNAME) 
// R_NUM,INC_NUM 크기
#define INDEX_SIZE sizeof(int)*2 
// 전체 헤더 크기
#define HEADER_SIZE INDEX_SIZE + DBINFO_SIZE

// 메인메뉴 
char *menu =
"
데이타수 : %d 
====================
1. 리스트 보기
2. 리스트 추가
3. 리스트 삭제 
4. 종료
==================== 
input : ";

// 레코드 입력 메뉴
char *input_menu = 
"
번   호  :
이   름  :
전화번호 : 
";

// 레코드 구조체
typedef struct _data
{
    int  num;          // 일련번호
    char name[16];       // 이름
    char tel_num[16];  // 전화번호
} Data;

//  R_NUM, INC_NUM
typedef struct _index_num
{
    int datanum;    // R_NUM   : 데이타 총갯수
    int incnum;     // INC_NUM : 데이타 일련번호 
} Index_num; 


// Index_num 값 즉 R_NUM 과 INC_NUM 
// 을 얻어온다.  
Index_num get_indexnum(int fd)
{
    Index_num index_num;
    lseek(fd, DBINFO_SIZE, SEEK_SET);
    read(fd, (void *)&index_num, HEADER_SIZE);
    return index_num;
}

// DB 파일을 체크한다. 
// 파일의 처음 4바이트 문자가 APPNAME 과 같으면 참 
int dbcheck(fd)
{
    char dbname[8];
    memset(dbname,0x00,8); 
    read(fd, dbname, 8);
    if (strncmp(dbname, APPNAME, DBINFO_SIZE) ==0) 
        return 1;
    else
        return -1;
}

// 최초에 DB파일이 생성되지 
// 않았을때 DB 파일을 초기화 시켜준다. 
// DB 포멧정보(APPNAME)이 들어가고 R_NUM, INC_NUM
// 은 0으로 초기화 된다. 
int init_datanum(int fd)
{
    Index_num index_num;
    write(fd, APPNAME, DBINFO_SIZE);
    memset((void *)&index_num, 0x00, INDEX_SIZE);
    write(fd, (void *)&index_num, INDEX_SIZE); 
}

// 레코드가 insert 되었을 경우
// R_NUM과 INC_NUM 을 증가시킨다. 
int inc_indexnum(int fd)
{
    int datanum;
    Index_num index_num;
    index_num = get_indexnum(fd);
    index_num.datanum++;
    index_num.incnum++;
    lseek(fd, DBINFO_SIZE, SEEK_SET);
    write(fd, (void *)&index_num, INDEX_SIZE);
    return 1;
}

// 메인 메뉴를 출력한다. 
void print_main_menu(fd)
{
    Index_num index_num;

    index_num = get_indexnum(fd);
    printf(menu,index_num.datanum);
}

// 서브메뉴를 출력한다. 
void print_menu(char *sub_menu)
{
    printf(sub_menu);
}

// 레코드를 삽입한다. 
// 레코드 삽입위치는 파일의 마지막이다. 
void input_data(Data mydata, int fd)
{
    // 파일의 마지막으로 이동 
    lseek(fd, 0, SEEK_END);
    write(fd, (void *)&mydata, sizeof(Data));
    inc_indexnum(fd);
}

// 레코드 리스트를 출력한다. 
void print_data(int fd)
{
    int i;
    int offset = 0;
    Data list;
    Index_num index_num;
    index_num = get_indexnum(fd);

    // 레코드의 시작위치로 이동한다. 
    lseek(fd, HEADER_SIZE, SEEK_SET);
    for (i = 0; i < index_num.datanum; )
    {
        read(fd, (void *)&list, sizeof(Data));
        if (list.num > 0) 
        {
            i++;
            printf("%3d %16s %16s\n", list.num, list.name, list.tel_num); 
        }
    }    
}

// 레코드를 삭제한다. 
// 실제로 데이타를 삭제하지는 않으며 
// Data.num 에 (-1)을 곱해준다. 
int del_data(int fd,int num)
{
    int offset;
    int del_flag;
    Data list;
    Index_num index_num;

    index_num = get_indexnum(fd);
    printf("delete num is %d\n", num);

    // 입력된 번호가 1 보다 작거나 레코드 수보다 클경우
    if ((index_num.incnum-1) > index_num.incnum || num < 1)
        return -1;

    // 삭제하고자 하는 레코드의 위치로 이동한다. 
    offset = (sizeof(Data)*(num-1)) + HEADER_SIZE;
    lseek(fd, offset, SEEK_SET);

    read(fd, (void *)&list, sizeof(Data));
    if (list.num < 0) 
    {
        printf("list.num is : %d\n", list.num);
        return -2;
    }

    del_flag = list.num*(-1);    

    // 삭제하고자 하는 레코드의 위치로 이동해서 
    // list.num*(-1) 값을 입력한다. 
    lseek(fd, offset, SEEK_SET);
    write(fd, (void *)&(del_flag), sizeof(int));

    // R_NUM 을 1 감소시킨다. 
    lseek(fd, DBINFO_SIZE, SEEK_SET);
    index_num.datanum--;
    write(fd, (void *)&(index_num), INDEX_SIZE);
    return 1;
}

// 메뉴선택에 대한 처리
void sel_menu(fd)
{
    char menu_num; 
    Index_num index_num;

    // R_NUM과 INC_NUM 을 구해온다. 
    index_num = get_indexnum(fd);

    while(1)
    {
        Data mydata;
        char buf[11];
        char num[11];
        int  state;
        char data[16];

        SCR_CLEAR;            // 화면 clear    
        MOVE_CURSOR(1,1);     // 커서이동
        print_main_menu(fd);  // Main 메뉴출력
        fgets(num, 11, stdin);

        // 입력번호에 따라 분기한다. 
        switch(atoi(num))
        {
            // 리스트 출력
            case 1 :
                print_data(fd);
                WAIT_INPUT("Press any key!!");
                break;
            // 입력 
            case 2 :
                SCR_CLEAR;

                MOVE_CURSOR(1,1);
                print_menu(input_menu);

                MOVE_CURSOR(2,12);
                printf("%d", ++index_num.incnum);
                mydata.num = index_num.incnum;

                MOVE_CURSOR(3,12);
                fgets(mydata.name, 16, stdin);
                chop(mydata.name);

                MOVE_CURSOR(4,12);
                fgets(mydata.tel_num, 16, stdin);
                chop(mydata.tel_num);
                input_data(mydata, fd);

                WAIT_INPUT("Press any key!!");
                break;

            // 삭제 
            case 3 :
                MOVE_CURSOR(10,1);
                printf("삭제번호 ");
                fgets(buf, 11, stdin);
                state = del_data(fd, atoi(buf));
                if (state < 0)
                {
                    printf("잘못된번호 선택\n");
                }
                WAIT_INPUT("Press any key!!");
                break;
            case 4 :
                printf("bye bye\n");    
                exit(0);
            default :
                break;
        }
    }
}



int main(int argc, char **argv)
{
    int data_num = 0;
    int is_fileok = 0;
    int fd;
    Data mydata;

    if (argc != 2)
    {
        printf("Usage : ./seek_db dbfile\n");
        exit(0);
    } 

    if ((access(argv[1], F_OK) == 0))
    {
        is_fileok = 1;
    }

    fd = open(argv[1], O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
    if (fd < 0)
    {
        perror("error : ");
        exit(0);
    }
    if (is_fileok == 0)
    {
        printf("FILE INIT\n");
        init_datanum(fd);
    }
    else
    {
        if (dbcheck(fd) != 1)
        {
            fprintf(stderr, "%s 는 잘못된 DB 파일입니다\n", argv[1]);  
            exit(0);
        }
    }

    sel_menu(fd);

    close(fd);
    return 1;
}
			
