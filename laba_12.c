#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <fcntl.h>
#include <unistd.h>

char const filename[] = "myfile.bin";
char const filenindex[] = "myfile.bin";
const int len = 255;

void setBook(void);
void getBook(void);
void sortBook(void);
void delBook(void);
void search(void);

struct Book {
    char Name[len];
    char Author[len];
    int PageCount;
    int Rank;
};

void search(void){

}

int main()
{
    int n;
    while(1){
        printf("\n       \x1b[31mMenu:\x1b[0m      \n");
        printf("1 - Add book      \n");
        printf("2 - Print books   \n");
        printf("3 - Sort books    \n");
        printf("4 - Delete book   \n");
        printf("5 - Search:\n");
        printf("6 - Exit          \n");
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
        }else if( n == 5){
            search();
        }
        else if( n == 6){
            printf("       \x1b[31mEnd\x1b[0m  \n");
            break;
        }
        else if( n == 6){
            FILE *f = fopen(filename, "wb");
            fclose(f);
        }
    }

}

void setBook(void){
    struct Book bk;
    FILE* f = NULL;
    f = fopen(filename, "ab");
    printf("Enter name of the book:");
    fgets(bk.Name, len, stdin);
    printf("Enter Author of the book:");
    fgets(bk.Author, len, stdin);
    printf("Enter Page count:");
    scanf("%d", &bk.PageCount);
    printf("Enter Rank book:");
    scanf("%d", &bk.Rank);
    fwrite(&bk, sizeof(struct Book), 1, f);
    fclose(f);
}

void getBook(void){
    FILE *f = fopen(filename, "rb");
    int i = 1;
    struct Book bk;
    while(fread(&bk, sizeof(struct Book), 1, f))
    {
        printf("   \x1b[34mBook %d\n\x1b[0m", i);
        printf("\x1b[32mName:\x1b[0m %s", bk.Name);
        printf("\x1b[32mAuthor:\x1b[0m %s", bk.Author);
        printf("\x1b[32mPage Count:\x1b[0m %d\n", bk.PageCount);
        printf("\x1b[32mRank:\x1b[0m %d\n", bk.Rank);
        i++;
    }
    fclose(f);
}

void sortBook(void){
    FILE *f = fopen(filename, "rb+"); // открываем бинарный файл на чтение, но с возможностью записи
    struct Book bk1;
    struct Book bk2;
    int i = 0;
    while(fread(&bk1, sizeof(struct Book), 1, f))//определяю сколько кник
    {
        i++;
    }
    rewind(f);//обнуляю счетчик
    for( int h = 0; h < (i+1)*i; h++) {
        for (int w = 1; w < i; w++) {     // переставляю местами
            fseek(f, (w - 1) * sizeof(struct Book), SEEK_SET);
            fread(&bk1, sizeof(struct Book), 1, f);
            fseek(f, (w) * sizeof(struct Book), SEEK_SET);
            fread(&bk2, sizeof(struct Book), 1, f);
            if (bk2.Rank > bk1.Rank) {
                fseek(f, (w-1) * sizeof(struct Book), SEEK_SET);
                fwrite(&bk2, sizeof(struct Book), 1, f);
                fwrite(&bk1, sizeof(struct Book), 1, f);
            }
        }
        rewind(f);
    }
    fclose(f); // закрываем файл
}

void delBook(void){ // удаление книги
    struct Book bk1;
    struct Book bk2;
    char name[len];
    FILE *f = fopen(filename, "rb+");
    printf("Enter Author of the book which you want to delete:");
    fgets(name, len + 1, stdin);
    int i = 0;
    while(fread(&bk1, sizeof(struct Book), 1, f))//определяю сколько книг
    {
        i++;
    }
    if (i > 0) { // проверяю, есть ли унига, которую надо удалить
        int k = 0;
        for (int ent = 0; ent < i; ent++) {
            for (int w = 0; w < i; w++) {     // переставляю местами
                fseek(f, (w) * sizeof(struct Book), SEEK_SET);
                fread(&bk1, sizeof(struct Book), 1, f);
                if (strcmp(bk1.Author, name) == 0) {
                    k++;
                }
            }
        }
        if(k>0) {
            rewind(f);//обнуляю указатель, ставлю в начало
            for (int ent = 0; ent < i; ent++) { // пузырьковая сортировка при удалении
                for (int w = 0; w < i; w++) {     // переставляю местами
                    fseek(f, (w) * sizeof(struct Book), SEEK_SET); // ставлю указатель на нужную строчку
                    fread(&bk1, sizeof(struct Book), 1, f); // читаю структуру
                    if (strcmp(bk1.Author, name) == 0) { // если совпадает нужное имя Автора

                        fseek(f, (i - 1) * sizeof(struct Book), SEEK_SET); //буфер структуры
                        fread(&bk1, sizeof(struct Book), 1, f);
                        for (int n = i; n > w; n--) { // читаю с конца, заменяю до нужного элемента

                            fseek(f, (n - 1) * sizeof(struct Book), SEEK_SET);
                            fread(&bk2, sizeof(struct Book), 1, f);

                            fseek(f, (n - 1) * sizeof(struct Book), SEEK_SET);
                            fwrite(&bk1, sizeof(struct Book), 1, f);

                            bk1 = bk2;
                        }
                    }
                }
            }
            fseek(f, (i - 1) * sizeof(struct Book), SEEK_SET); // указатель на последнюю строку
            ftruncate(fileno(f), sizeof(struct Book) * i - sizeof(struct Book)); // удаляю одну структуру
            //_chsize(_fileno(f), sizeof(struct Book)) * i - sizeof(struct Book));
            printf("\x1b[31mYour books deleted\x1b[0m \n");
        }
        else{
            printf("\x1b[31mdon't founded\x1b[0m\n");
        }
    }
    fclose(f);
}
