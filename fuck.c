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



//���� ���̵� ����ü ����
typedef struct {
    int id;
}UserID;

// å ���� ����ü
typedef struct {
    char title[50];
    char author[50];
    char publisher[50];
    int available;  // 0: ���� ��, 1: ���� ����
    int favorite;   // 0: ���ã������ ����, 1: ���ã��
    int lateFee;
    UserID borrow;
    struct tm loanDate;
} Book;

//���� ����ü ����
typedef struct {
    UserID userID;
    char name[10];
    char studentID[11];
    char password[16];
    char major[20];
}User;
Book books[MAX_BOOKS];
int numBooks = 0;

// ���� ������ ���Ͽ��� �ҷ�����
void loadBooks() {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("������ �� �� �����ϴ�.\n");
        return;
    }

    while (fread(&books[numBooks], sizeof(Book), 1, file)) {
        numBooks++;
    }

    fclose(file);
}

// ���� ������ ���Ͽ� �����ϱ�
void saveBooks() {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("������ �� �� �����ϴ�.\n");
        return;
    }

    fwrite(books, sizeof(Book), numBooks, file);

    fclose(file);
}

// ���� ���� ���� Ȯ��
bool isAvailable(const char* title) {
    for (int i = 0; i < numBooks; i++) {
        if (strcmp(books[i].title, title) == 0) {
            return books[i].available;
        }
    }
    return false;
}


//���� ������ ���� �迭 �� ���̵� ���� ����
User users[MAX_USERS];
int numUsers = 0;
int nextUserID = 1;

//���Ͽ��� ���� ���� �ҷ�����
void loadUsers() {
    FILE* file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf("������ �� �� �����ϴ�.\n");
        return;
    }

    while (fread(&users[numUsers], sizeof(User), 1, file)) {
        numUsers++;
    }

    fclose(file);
}

//���Ͽ��� ���� ���� �����ϱ�
void saveUsers() {
    FILE* file = fopen(FILENAME, "w");
    if (file == NULL) {
        printf("������ �� �� �����ϴ�.\n");
        return;
    }
    
    fwrite(users, sizeof(User), numUsers, file);

    fclose(file);
}

// ���� ���� �Լ�
void borrowBook(User* user) {
    system("cls");
    printf("���� ���� �������Դϴ�.\n");

    char keyword[50];
    printf("������ ������ �������� �Է��ϼ���: ");
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
            printf("������ �̹� ����Ǿ��� �ֽ��ϴ�.\n");
            return;
        }

        int checkoutCount = 0;
        for (int i = 0; i < numBooks; i++) {
            if (!books[i].available && books[i].favorite) {
                checkoutCount++;
            }
        }

        if (checkoutCount >= MAX_CHECKOUT_LIMIT) {
            printf("�δ� %d�Ǳ��� ������ �� �ֽ��ϴ�. �� �̻� ������ �� �����ϴ�.\n", MAX_CHECKOUT_LIMIT);
            return;
        }

        if (books[foundIndex].lateFee > 0) {
            int paymentChoice;
            printf("��ü �ݾ��� %d�� �ֽ��ϴ�. �����Ͻðڽ��ϱ�? Y(0),N(1): ", books[foundIndex].lateFee);
            scanf("%d", &paymentChoice);

            if (paymentChoice == 1) {
                books[foundIndex].lateFee = 0;
                printf("��ü �ݾ��� ���εǾ����ϴ�.\n");
            }
            else {
                printf("��ü �ݾ��� �������� �ʾұ� ������ ������ ������ �� �����ϴ�.\n");
                return;
            }
        }

        

        printf("������ ����Ǿ����ϴ�.\n");
        books[foundIndex].available = 0;  // ���� �� ���·� ����
        books[foundIndex].favorite = 0;   // ���ã�� ����
        books[foundIndex].borrow = user->userID;
        //������ ���
        time_t now; 
        struct tm* loanDate;
        time(&now);
        loanDate = localtime(&now);
        memcpy(&books[foundIndex].loanDate, loanDate, sizeof(struct tm));
        
        saveBooks();
    }
    else {
        printf("������ �������� �ʽ��ϴ�.\n");
    }
}

// �����Ϸκ��� ����� �ð� ���
void displayRemainDays(struct tm loanDate) {
    time_t now;
    struct tm* currentDate;
    time(&now);
    currentDate = localtime(&now);

    time_t loanTime = mktime(&loanDate);
    time_t currentTime = mktime(currentDate);
    double diffTime = difftime(currentTime, loanTime);

    int remainingDays = MAX_DAYS - (int)(diffTime / (60 * 60 * 24));

    printf("�ݳ����� ���� ����: %d��\n", remainingDays);
}

// ���� �ݳ� �Լ�
void returnBook(User *user) {
    system("cls");
    printf("���� �ݳ� �������Դϴ�.\n");

    for (int i = 0; i < numBooks; i++) {
        if (!books[i].available && books[i].borrow.id == user->userID.id) {
            printf("%s ", books[i].title);
            displayRemainDays(books[i].loanDate);
        }
    }

    char keyword[50];
    printf("�ݳ��� ������ �������� �Է��ϼ���: ");
    scanf("%s", keyword);

    int foundIndex = -1;
    for (int i = 0; i < numBooks; i++) {
        if (strcmp(books[i].title, keyword) == 0 && !books[i].available) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex != -1) {
        printf("%s ������ �ݳ��Ǿ����ϴ�.\n", books[foundIndex].title);
        books[foundIndex].available = 1;  // ���� ���� ���·� ����
        saveBooks();
    }
    else {
        printf("������ �ݳ��� �� �����ϴ�.\n");
    }
}

// ���� ��� �Լ�
void registerBook() {
    system("cls");
    printf("���� ��� �������Դϴ�.\n");

    char title[50];
    char author[50];
    char publisher[50];

    printf("�������� �Է��ϼ���: ");
    scanf("%s", title);

    for (int i = 0; i < numBooks; i++) {
        if (strcmp(books[i].title, title) == 0) {
            printf("�̹� �����ϴ� �������Դϴ�.\n");
            return;
        }
    }

    printf("���ڸ� �Է��ϼ���: ");
    scanf("%s", author);

    printf("���ǻ縦 �Է��ϼ���: ");
    scanf("%s", publisher);

    strcpy(books[numBooks].title, title);
    strcpy(books[numBooks].author, author);
    strcpy(books[numBooks].publisher, publisher);
    books[numBooks].available = 1;  // ���� ���� ���·� �ʱ�ȭ
    books[numBooks].favorite = 0;   // ���ã�� ����
    numBooks++;

    saveBooks();
    printf("������ ��ϵǾ����ϴ�.\n");
}

// ���� �˻� �Լ�
void searchBook() {
    system("cls");
    printf("���� �˻� �������Դϴ�.\n");

    printf("1. ���������� �˻�\n");
    printf("2. ���ǻ�� �˻�\n");
    printf("3. ���ڷ� �˻�\n");
    printf("0. �������� ���ư���\n");
    printf("����: ");

    int choice;
    scanf("%d", &choice);

    char keyword[MAX_KEYWORD_LEN];
    bool found = false;

    switch (choice) {
    case 1:
        printf("������ �Ǵ� Ű���带 �Է��ϼ���: ");
        scanf("%s", keyword);
        for (int i = 0; i < numBooks; i++) {
            if (strstr(books[i].title, keyword) != NULL) {
                printf("%s - %s, %s\n", books[i].title, books[i].author, books[i].publisher);
                found = true;
            }
        }
        break;
    case 2:
        printf("���ǻ縦 �Է��ϼ���: ");
        scanf("%s", keyword);
        for (int i = 0; i < numBooks; i++) {
            if (strcmp(books[i].publisher, keyword) == 0) {
                printf("%s - %s, %s\n", books[i].title, books[i].author, books[i].publisher);
                found = true;
            }
        }
        break;
    case 3:
        printf("���ڸ� �Է��ϼ���: ");
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
        printf("�߸��� �����Դϴ�.\n");
        return;
    }

    if (!found) {
        printf("�˻� ����� �����ϴ�.\n");
    }
}

// ���� ���ã�� ���
void toggleFavorite() {
    system("cls");
    printf("���ã�� ���� �������Դϴ�.\n");

    printf("1. ���ã���� ���� ��� ���\n");
    printf("2. ���� ���ã��/����\n");
    printf("0. �������� ���ư���\n");
    printf("����: ");

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
        printf("���ã���� �������� �Է��ϼ���: ");
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
                printf("%s ������ ���ã�� �߽��ϴ�.\n", books[foundIndex].title);
            }
            else {
                printf("%s ������ ���ã�⸦ �����߽��ϴ�.\n", books[foundIndex].title);
            }
        }
        else {
            printf("������ ã�� �� �����ϴ�.\n");
        }
        break;
    }
    case 0:
        return;
    default:
        printf("�߸��� �����Դϴ�.\n");
        return;
    }
}

//��ü�� ��� �Լ�
int calculateLateFee(int bookindex) {
    if (!books[bookindex].available) {
        time_t now;
        struct tm* currentDate;
        time(&now);
        currentDate = localtime(&now);

        int hoursPassed = difftime(mktime(currentDate), mktime(&books[bookindex].loanDate)) / (60 * 60);
        int weeksPassed = hoursPassed / (HOURS_PER_WEEK * 2);  // 2��(14��) �������� �� ���� ���

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
    printf("��ü����� ����ϴ� �������Դϴ�. \n");
    for (int i = 0; i < numBooks; i++) {
        if (!books[i].available) {
            int lateFee = calculateLateFee(i);
            printf("������: %s\n", books[i].title);
            printf("��ü���: %d��\n", lateFee);
            printf("------------------------\n");
            printf("3�� �� ������������ �Ѿ�ϴ�. ");
            Sleep(3000);
            mainpage(user);
        }
    }
}



//���� ������ �Լ�
void mainpage(User* user) {
    loadBooks();

    while (1) {
        system("cls");
        printf("���� ���� ���α׷�\n");
        printf("1. ���� ����\n");
        printf("2. ���� �ݳ�\n");
        printf("3. ���� ���\n");
        printf("4. ���� �˻�\n");
        printf("5. ���ã�� ����\n");
        printf("6. ���� �� ��ü �ݾ�\n");
        printf("0. ���α׷� ����\n");
        printf("����: ");

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
            printf("���α׷��� �����մϴ�.\n");
            exit(1);
        default:
            printf("�߸��� �����Դϴ�.\n");
            break;
        }

        system("pause");
    }

    
}

//�α��� �Լ�
void login() {
    char studentID[11];
    char password[16];
    int login_check = 0;
    int check;
    int regisTer;
    int carry;
    system("cls");
    printf("�α��� �������Դϴ�.\n");
    while (1) {
        printf("ȸ������ �������� ���ðڽ��ϱ�? Y(0),N(1)");
        scanf("%d", &regisTer);
        if (regisTer == 0 || regisTer == 1)
            break;
    }
    if (regisTer == 0) {
        registerUser();
    }
    
    while (1) {
        printf("���̵�: ");
        scanf("%s", studentID);
        printf("��й�ȣ: ");
        scanf("%s", password);


        int i;
        for (i = 0; i < numUsers; i++) {
            if (strcmp(users[i].studentID, studentID) == 0 && strcmp(users[i].password, password) == 0) {
                printf("�α��� ����!\n");
                login_check = 1;
                carry = i;
                printf("������������ �̵��մϴ�");
                Sleep(2000);
                mainpage(&users[carry]);
                
            }

        }
        if (login_check == 1) {
            break;
        }
        else {

            printf("���̵� Ȥ�� ��й�ȣ�� Ʋ�Ƚ��ϴ�. �ٽ� �õ����ּ���. \n");
        }
    }
    
}

//ȸ������ �Լ�
void registerUser() {
    User newUser;
    char passwordConfirm[16];
    int id_check = 0;
    int pwd_check = 0;
    int yn;
    int check;

    system("cls");
    printf("ȸ������ ������\n");
    while (1) {
        printf("�α��� �������� ���ðڽ��ϱ�? Y(0),N(1)");
        scanf("%d", &yn);
        if (yn == 0 || yn == 1)
            break;
    }
    if (yn == 0) {
        login();
    }

    printf("�̸�: ");
    scanf("%s", newUser.name);
    while (1) {
        printf("�й�(���̵�): ");
        scanf("%s", newUser.studentID);

        // �ߺ� �˻�
        int i;
        for (i = 0; i < numUsers; i++) {
            if (strcmp(users[i].studentID, newUser.studentID) == 0) {
                printf("�ߺ��� �й��Դϴ�. �ٸ� �й��� �Է����ּ���.\n");
                id_check = 1;
                printf("�α��� �������� �̵��Ͻðڽ��ϱ�? Y(0),N(1)");
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
        printf("��й�ȣ: ");
        scanf("%s", newUser.password);
        printf("��й�ȣ Ȯ��: ");
        scanf("%s", passwordConfirm);

        if (strcmp(newUser.password, passwordConfirm) == 0) {
            break;
        }
        else {
            printf("��й�ȣ�� ��й�ȣ Ȯ���� ��ġ���� �ʽ��ϴ�.\n");
        }

    }

    printf("����: ");
    scanf("%s", newUser.major);

    // ���� ���̵� ����
    newUser.userID.id = numUsers;

    // ���� �迭�� �� ���� �߰�
    users[numUsers] = newUser;
    numUsers++;

    saveUsers();
    system("cls");
    for (int i = 3; i > 0; i--) {
        printf("ȸ�������� �Ϸ�Ǿ����ϴ�.\n%d�� �� �α��� �������� �̵��մϴ�. ", i);
        Sleep(1000);
        system("cls");
    }
    login();
}



int main() {

    loadUsers();
    while (1) {
        printf("1. �α���\n");
        printf("2. ȸ������\n");
        printf("3. ����\n");
        printf("����: ");

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
            printf("�߸��� �����Դϴ�. �ٽ� �������ּ���.\n");
        }
        break;

        
    }
    return 0;
}