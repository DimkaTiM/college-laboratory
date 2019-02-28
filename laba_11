//База книг на текстовых файлах


#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

void setBook(void);
void getBook(void);
void sortBook(void);
void delBook(void);


char const filename[] = "myfile.txt";
const int len = 255;


struct Book {
    char Name[255];
    char Author[255];
    int PageCount;
    int Rank;
};


int main() {
//    printf("%d", strcmp("sdf" ,"sdfd"));
    int n;
    while(1){
        printf("       \x1b[31mMenu:\x1b[0m      \n");
        printf("1 - Add book      \n");
        printf("2 - Print books   \n");
        printf("3 - Sort books    \n");
        printf("4 - Delete book   \n");
        printf("5 - Exit          \n");
        printf("Enter your choice:\n");


        scanf("%d", &n);
        getchar();
        if (n == 1){
            setBook();
        }
        else if( n == 2){
            getBook();
        }
        else if( n == 3){
            sortBook();
        }
        else if( n == 4){
            delBook();
        }
        else if( n == 5){
            printf("       \x1b[31mEnd\x1b[0m  \n");
            break;
        }
    }
}


void getBook(void){
    FILE *file = fopen(filename, "r");
    char str[len];
    int n, i = 1;
    while(fgets(str, len + 1, file))
    {
        printf("   \x1b[34mBook %d\n\x1b[0m", i);
        printf("\x1b[32mName:\x1b[0m %s", str);
        fgets(str, len + 1, file);
        printf("\x1b[32mAuthor:\x1b[0m %s", str);
        fscanf(file, "%d\n", &n);
        printf("\x1b[32mPage Count:\x1b[0m %d\n", n);
        fscanf(file, "%d\n", &n);
        printf("\x1b[32mRank:\x1b[0m %d\n", n);
        i++;
    }
    fclose(file);
}


void sortBook(void){
    printf("\x1b[34mYour books sorted\x1b[0m \n");
    FILE *file = fopen(filename, "r");
    if(file == NULL){
        file = fopen(filename, "w");
    }
    char str[len];
    int n, i = 0, max = -9999999;
    while (fgets(str, len + 1, file)) //  определяю счетчик на кол-во книг
    {
        fgets(str, len + 1, file);
        fscanf(file, "%d\n", &n);
        fscanf(file, "%d\n", &n);
        i++;
    }
    fclose(file);
    struct Book books[i]; // определяю массив книг по счетчику
    file = fopen(filename, "r"); //переоткрываю для считывания сначала
    int j = 0;
    while (fgets(str, len + 1, file)) //считываю книги из файла в массив структур книг
    {
        strcpy(books[j].Name, str);
        fgets(str, len + 1, file);
        strcpy(books[j].Author, str);
        fscanf(file, "%d\n", &books[j].PageCount);
        fscanf(file, "%d\n", &books[j].Rank);
        if(max < books[j].Rank){
            max = books[i].Rank;
        }
        j++;
    }
    fclose(file);
    file = fopen(filename, "w"); // перезаписываю файл с сортировкой
    for (int h = max + 1; h >= 0; h--) {
        for (int i = 0; i < j; i++) {
            if (books[i].Rank == h) {
                fprintf(file, "%s", books[i].Name);
                fprintf(file, "%s", books[i].Author);
                fprintf(file, "%d\n", books[i].PageCount);
                fprintf(file, "%d\n", books[i].Rank);
                printf("   \x1b[34mBook %d\n\x1b[0m", i + 1);
                printf("Name : %s", books[i].Name);
                printf("Author: %s", books[i].Author);
                printf("PageCount %d\n", books[i].PageCount);
                printf("Rank: %d\n", books[i].Rank);
            }
        }
    }
    fclose(file);
}


void setBook() {
    char str[len];
    int n;
    FILE *file = fopen(filename, "a");
    if(file == NULL)
        file = fopen(filename, "w");
    printf("Enter name of the book:");
    fgets(str, len+1, stdin);
    fprintf(file, "%s", str);
    printf("Enter Author of the book:");
    fgets(str, len+1, stdin);
    fprintf(file, "%s", str);
    printf("Enter Page count:");
    scanf("%d", &n); getchar();
    fprintf(file, "%d\n", n);
    printf("Enter Rank book:");
    scanf("%d", &n); getchar();
    fprintf(file, "%d\n", n);
    fclose(file);
}


void delBook(void) {
    printf("Enter Author of the book which you want to delete:");
    char name[len];
    fgets(name, len + 1, stdin);
    FILE *file = fopen(filename, "r");
    char str[len];
    int n, i = 0;
    while (fgets(str, len + 1, file)) //  определяю счетчик на кол-во книг
    {
        fgets(str, len + 1, file);
        fscanf(file, "%d\n", &n);
        fscanf(file, "%d\n", &n);
        i++;
    }
    fclose(file);
    struct Book books[i]; // определяю массив книг по счетчику
    file = fopen(filename, "r"); //переоткрываю для считывания сначала
    int j = 0;
    while (fgets(str, len + 1, file)) //считываю книги из файла в массив структур книг
    {
        strcpy(books[j].Name, str);
        fgets(str, len + 1, file);
        strcpy(books[j].Author, str);
        fscanf(file, "%d\n", &books[j].PageCount);
        fscanf(file, "%d\n", &books[j].Rank);
        j++;
    }
    fclose(file);
    file = fopen(filename, "w"); // перезаписываю файл без удаленной книги
    int r = 0;
    for (int i = 0; i < j; i++) {
        if (strcmp(books[i].Author, name) ) {
            fprintf(file, "%s", books[i].Name);
            fprintf(file, "%s", books[i].Author);
            fprintf(file, "%d\n", books[i].PageCount);
            fprintf(file, "%d\n", books[i].Rank);
            r++;
        }
    }
    if(r >= 0){
        printf(" \x1b[31mBook deleted \x1b[0m\n");
    }else{
        printf(" \x1b[31mDo't find this book in library \x1b[0m");
    }
    fclose(file);
}
