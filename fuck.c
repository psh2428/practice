#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <Windows.h>
#include <time.h>

void login(); 
void registerUser();
void mainpage();


#define MAX_BOOKS 100
#define MAX_KEYWORD_LEN 50
#define MAX_CHECKOUT_LIMIT 3
#define MAX_DAYS 14
#define FILENAME "users.txt"
#define MAX_USERS 100
#define filename "books.txt"
#define FEE_PER_HOUR 100
#define HOURS_PER_DAY 24
#define HOURS_PER_WEEK 168  



//유저 아이디 구조체 정의
typedef struct {
    int id;
}UserID;

// 책 관련 구조체
typedef struct {
    char title[50];
    char author[50];
    char publisher[50];
    int available;  // 0: 대출 중, 1: 대출 가능
    int favorite;   // 0: 즐겨찾기하지 않음, 1: 즐겨찾기
    int lateFee;
    UserID borrow;
    struct tm loanDate;
} Book;

//유저 구조체 정의
typedef struct {
    UserID userID;
    char name[10];
    char studentID[11];
    char password[16];
    char major[20];
}User;
Book books[MAX_BOOKS];
int numBooks = 0;

// 도서 정보를 파일에서 불러오기
void loadBooks() {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("파일을 열 수 없습니다.\n");
        return;
    }

    while (fread(&books[numBooks], sizeof(Book), 1, file)) {
        numBooks++;
    }

    fclose(file);
}

// 도서 정보를 파일에 저장하기
void saveBooks() {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("파일을 열 수 없습니다.\n");
        return;
    }

    fwrite(books, sizeof(Book), numBooks, file);

    fclose(file);
}

// 대출 가능 여부 확인
bool isAvailable(const char* title) {
    for (int i = 0; i < numBooks; i++) {
        if (strcmp(books[i].title, title) == 0) {
            return books[i].available;
        }
    }
    return false;
}


//전역 변수로 유저 배열 및 아이디 변수 선언
User users[MAX_USERS];
int numUsers = 0;
int nextUserID = 1;

//파일에서 유저 정보 불러오기
void loadUsers() {
    FILE* file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf("파일을 열 수 없습니다.\n");
        return;
    }

    while (fread(&users[numUsers], sizeof(User), 1, file)) {
        numUsers++;
    }

    fclose(file);
}

//파일에서 유저 정보 저장하기
void saveUsers() {
    FILE* file = fopen(FILENAME, "w");
    if (file == NULL) {
        printf("파일을 열 수 없습니다.\n");
        return;
    }
    
    fwrite(users, sizeof(User), numUsers, file);

    fclose(file);
}

// 도서 대출 함수
void borrowBook(User* user) {
    system("cls");
    printf("도서 대출 페이지입니다.\n");

    char keyword[50];
    printf("대출할 도서의 도서명을 입력하세요: ");
    scanf("%s", keyword);

    int foundIndex = -1;
    for (int i = 0; i < numBooks; i++) {
        if (strcmp(books[i].title, keyword) == 0) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex != -1) {
        if (!books[foundIndex].available) {
            printf("도서가 이미 대출되어져 있습니다.\n");
            return;
        }

        int checkoutCount = 0;
        for (int i = 0; i < numBooks; i++) {
            if (!books[i].available && books[i].favorite) {
                checkoutCount++;
            }
        }

        if (checkoutCount >= MAX_CHECKOUT_LIMIT) {
            printf("인당 %d권까지 대출할 수 있습니다. 더 이상 대출할 수 없습니다.\n", MAX_CHECKOUT_LIMIT);
            return;
        }

        if (books[foundIndex].lateFee > 0) {
            int paymentChoice;
            printf("연체 금액이 %d원 있습니다. 납부하시겠습니까? Y(0),N(1): ", books[foundIndex].lateFee);
            scanf("%d", &paymentChoice);

            if (paymentChoice == 1) {
                books[foundIndex].lateFee = 0;
                printf("연체 금액이 납부되었습니다.\n");
            }
            else {
                printf("연체 금액을 납부하지 않았기 때문에 도서를 대출할 수 없습니다.\n");
                return;
            }
        }

        

        printf("도서가 대출되었습니다.\n");
        books[foundIndex].available = 0;  // 대출 중 상태로 변경
        books[foundIndex].favorite = 0;   // 즐겨찾기 해제
        books[foundIndex].borrow = user->userID;
        //대출일 기록
        time_t now; 
        struct tm* loanDate;
        time(&now);
        loanDate = localtime(&now);
        memcpy(&books[foundIndex].loanDate, loanDate, sizeof(struct tm));
        
        saveBooks();
    }
    else {
        printf("도서가 존재하지 않습니다.\n");
    }
}

// 대출일로부터 경과한 시간 계산
void displayRemainDays(struct tm loanDate) {
    time_t now;
    struct tm* currentDate;
    time(&now);
    currentDate = localtime(&now);

    time_t loanTime = mktime(&loanDate);
    time_t currentTime = mktime(currentDate);
    double diffTime = difftime(currentTime, loanTime);

    int remainingDays = MAX_DAYS - (int)(diffTime / (60 * 60 * 24));

    printf("반납까지 남은 일자: %d일\n", remainingDays);
}

// 도서 반납 함수
void returnBook(User *user) {
    system("cls");
    printf("도서 반납 페이지입니다.\n");

    for (int i = 0; i < numBooks; i++) {
        if (!books[i].available && books[i].borrow.id == user->userID.id) {
            printf("%s ", books[i].title);
            displayRemainDays(books[i].loanDate);
        }
    }

    char keyword[50];
    printf("반납할 도서의 도서명을 입력하세요: ");
    scanf("%s", keyword);

    int foundIndex = -1;
    for (int i = 0; i < numBooks; i++) {
        if (strcmp(books[i].title, keyword) == 0 && !books[i].available) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex != -1) {
        printf("%s 도서가 반납되었습니다.\n", books[foundIndex].title);
        books[foundIndex].available = 1;  // 대출 가능 상태로 변경
        saveBooks();
    }
    else {
        printf("도서를 반납할 수 없습니다.\n");
    }
}

// 도서 등록 함수
void registerBook() {
    system("cls");
    printf("도서 등록 페이지입니다.\n");

    char title[50];
    char author[50];
    char publisher[50];

    printf("도서명을 입력하세요: ");
    scanf("%s", title);

    for (int i = 0; i < numBooks; i++) {
        if (strcmp(books[i].title, title) == 0) {
            printf("이미 존재하는 도서명입니다.\n");
            return;
        }
    }

    printf("저자를 입력하세요: ");
    scanf("%s", author);

    printf("출판사를 입력하세요: ");
    scanf("%s", publisher);

    strcpy(books[numBooks].title, title);
    strcpy(books[numBooks].author, author);
    strcpy(books[numBooks].publisher, publisher);
    books[numBooks].available = 1;  // 대출 가능 상태로 초기화
    books[numBooks].favorite = 0;   // 즐겨찾기 해제
    numBooks++;

    saveBooks();
    printf("도서가 등록되었습니다.\n");
}

// 도서 검색 함수
void searchBook() {
    system("cls");
    printf("도서 검색 페이지입니다.\n");

    printf("1. 도서명으로 검색\n");
    printf("2. 출판사로 검색\n");
    printf("3. 저자로 검색\n");
    printf("0. 이전으로 돌아가기\n");
    printf("선택: ");

    int choice;
    scanf("%d", &choice);

    char keyword[MAX_KEYWORD_LEN];
    bool found = false;

    switch (choice) {
    case 1:
        printf("도서명 또는 키워드를 입력하세요: ");
        scanf("%s", keyword);
        for (int i = 0; i < numBooks; i++) {
            if (strstr(books[i].title, keyword) != NULL) {
                printf("%s - %s, %s\n", books[i].title, books[i].author, books[i].publisher);
                found = true;
            }
        }
        break;
    case 2:
        printf("출판사를 입력하세요: ");
        scanf("%s", keyword);
        for (int i = 0; i < numBooks; i++) {
            if (strcmp(books[i].publisher, keyword) == 0) {
                printf("%s - %s, %s\n", books[i].title, books[i].author, books[i].publisher);
                found = true;
            }
        }
        break;
    case 3:
        printf("저자를 입력하세요: ");
        scanf("%s", keyword);
        for (int i = 0; i < numBooks; i++) {
            if (strcmp(books[i].author, keyword) == 0) {
                printf("%s - %s, %s\n", books[i].title, books[i].author, books[i].publisher);
                found = true;
            }
        }
        break;
    case 0:
        return;
    default:
        printf("잘못된 선택입니다.\n");
        return;
    }

    if (!found) {
        printf("검색 결과가 없습니다.\n");
    }
}

// 도서 즐겨찾기 기능
void toggleFavorite() {
    system("cls");
    printf("즐겨찾기 도서 페이지입니다.\n");

    printf("1. 즐겨찾기한 도서 목록 출력\n");
    printf("2. 도서 즐겨찾기/해제\n");
    printf("0. 이전으로 돌아가기\n");
    printf("선택: ");

    int choice;
    scanf("%d", &choice);

    switch (choice) {
    case 1:
        for (int i = 0; i < numBooks; i++) {
            if (books[i].favorite) {
                printf("%s - %s, %s\n", books[i].title, books[i].author, books[i].publisher);
            }
        }
        break;
    case 2: {
        char keyword[50];
        printf("즐겨찾기할 도서명을 입력하세요: ");
        scanf("%s", keyword);

        int foundIndex = -1;
        for (int i = 0; i < numBooks; i++) {
            if (strcmp(books[i].title, keyword) == 0) {
                foundIndex = i;
                break;
            }
        }

        if (foundIndex != -1) {
            books[foundIndex].favorite = !books[foundIndex].favorite;
            saveBooks();
            if (books[foundIndex].favorite) {
                printf("%s 도서를 즐겨찾기 했습니다.\n", books[foundIndex].title);
            }
            else {
                printf("%s 도서의 즐겨찾기를 해제했습니다.\n", books[foundIndex].title);
            }
        }
        else {
            printf("도서를 찾을 수 없습니다.\n");
        }
        break;
    }
    case 0:
        return;
    default:
        printf("잘못된 선택입니다.\n");
        return;
    }
}

//연체비 계산 함수
int calculateLateFee(int bookindex) {
    if (!books[bookindex].available) {
        time_t now;
        struct tm* currentDate;
        time(&now);
        currentDate = localtime(&now);

        int hoursPassed = difftime(mktime(currentDate), mktime(&books[bookindex].loanDate)) / (60 * 60);
        int weeksPassed = hoursPassed / (HOURS_PER_WEEK * 2);  // 2주(14일) 기준으로 주 단위 계산

        if (weeksPassed > 0) {
            int overdueHours = hoursPassed - (weeksPassed * HOURS_PER_WEEK * 2);
            int lateFee = (overdueHours / 8) * FEE_PER_HOUR;
            return lateFee;
        }
    }

    return 0;
}

void displayLateFee(User* user) {
    system("cls");
    printf("연체비용을 출력하는 페이지입니다. \n");
    for (int i = 0; i < numBooks; i++) {
        if (!books[i].available) {
            int lateFee = calculateLateFee(i);
            printf("도서명: %s\n", books[i].title);
            printf("연체비용: %d원\n", lateFee);
            printf("------------------------\n");
            printf("3초 후 메인페이지로 넘어갑니다. ");
            Sleep(3000);
            mainpage(user);
        }
    }
}



//메인 페이지 함수
void mainpage(User* user) {
    loadBooks();

    while (1) {
        system("cls");
        printf("도서 관리 프로그램\n");
        printf("1. 도서 대출\n");
        printf("2. 도서 반납\n");
        printf("3. 도서 등록\n");
        printf("4. 도서 검색\n");
        printf("5. 즐겨찾기 도서\n");
        printf("6. 현재 내 연체 금액\n");
        printf("0. 프로그램 종료\n");
        printf("선택: ");

        int choice;
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            borrowBook(user);
            break;
        case 2:
            returnBook(user);
            break;
        case 3:
            registerBook();
            break;
        case 4:
            searchBook();
            break;
        case 5:
            toggleFavorite();
            break;
        case 6:
            displayLateFee(user);
        case 0:
            saveBooks();
            printf("프로그램을 종료합니다.\n");
            exit(1);
        default:
            printf("잘못된 선택입니다.\n");
            break;
        }

        system("pause");
    }

    
}

//로그인 함수
void login() {
    char studentID[11];
    char password[16];
    int login_check = 0;
    int check;
    int regisTer;
    int carry;
    system("cls");
    printf("로그인 페이지입니다.\n");
    while (1) {
        printf("회원가입 페이지로 가시겠습니까? Y(0),N(1)");
        scanf("%d", &regisTer);
        if (regisTer == 0 || regisTer == 1)
            break;
    }
    if (regisTer == 0) {
        registerUser();
    }
    
    while (1) {
        printf("아이디: ");
        scanf("%s", studentID);
        printf("비밀번호: ");
        scanf("%s", password);


        int i;
        for (i = 0; i < numUsers; i++) {
            if (strcmp(users[i].studentID, studentID) == 0 && strcmp(users[i].password, password) == 0) {
                printf("로그인 성공!\n");
                login_check = 1;
                carry = i;
                printf("메인페이지로 이동합니다");
                Sleep(2000);
                mainpage(&users[carry]);
                
            }

        }
        if (login_check == 1) {
            break;
        }
        else {

            printf("아이디 혹은 비밀번호가 틀렸습니다. 다시 시도해주세요. \n");
        }
    }
    
}

//회원가입 함수
void registerUser() {
    User newUser;
    char passwordConfirm[16];
    int id_check = 0;
    int pwd_check = 0;
    int yn;
    int check;

    system("cls");
    printf("회원가입 페이지\n");
    while (1) {
        printf("로그인 페이지로 가시겠습니까? Y(0),N(1)");
        scanf("%d", &yn);
        if (yn == 0 || yn == 1)
            break;
    }
    if (yn == 0) {
        login();
    }

    printf("이름: ");
    scanf("%s", newUser.name);
    while (1) {
        printf("학번(아이디): ");
        scanf("%s", newUser.studentID);

        // 중복 검사
        int i;
        for (i = 0; i < numUsers; i++) {
            if (strcmp(users[i].studentID, newUser.studentID) == 0) {
                printf("중복된 학번입니다. 다른 학번을 입력해주세요.\n");
                id_check = 1;
                printf("로그인 페이지로 이동하시겠습니까? Y(0),N(1)");
                scanf("%d", &check);
                if (check == 1) {
                    login();
                }
                break;
            }

        }
        if (id_check != 1) {
            break;
        }
        else
            id_check = 0;
    }
    while (1) {
        printf("비밀번호: ");
        scanf("%s", newUser.password);
        printf("비밀번호 확인: ");
        scanf("%s", passwordConfirm);

        if (strcmp(newUser.password, passwordConfirm) == 0) {
            break;
        }
        else {
            printf("비밀번호와 비밀번호 확인이 일치하지 않습니다.\n");
        }

    }

    printf("전공: ");
    scanf("%s", newUser.major);

    // 유저 아이디 설정
    newUser.userID.id = numUsers;

    // 유저 배열에 새 유저 추가
    users[numUsers] = newUser;
    numUsers++;

    saveUsers();
    system("cls");
    for (int i = 3; i > 0; i--) {
        printf("회원가입이 완료되었습니다.\n%d초 후 로그인 페이지로 이동합니다. ", i);
        Sleep(1000);
        system("cls");
    }
    login();
}



int main() {

    loadUsers();
    while (1) {
        printf("1. 로그인\n");
        printf("2. 회원가입\n");
        printf("3. 종료\n");
        printf("선택: ");

        int choice;
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            login();
            break;
        case 2:
            registerUser();
            break;
        case 3:
            exit(1);
        default:
            printf("잘못된 선택입니다. 다시 선택해주세요.\n");
        }
        break;

        
    }
    return 0;
}