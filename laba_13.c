//Лабораторная База книг на бинарных файлах с индексами по рангу(другой файл) работа с базой в меню


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <fcntl.h>
#include <unistd.h>

const int len = 255;                        // фиксированная длина строк
char const filename[] = "myfile.bin";       // мой файл, там лежат структуры книг
char const filecount[] = "filecount.bin";   // мой файл, там лежат индексы книг
char const filecountAuthor[] = "author.bin";
char const filecountRank[] = "rank.bin";

struct Book {
    char Name[len];     // название
    char Author[len];   // автор
    int PageCount;      // кол-во страниц
    int Rank;           // ранг книги
};                          // наша структура книги

void setBook(void);                         // добавить книгу
void setCount(void);                        // добавить индекс

void getBook(void);                         // вывести все книги
void getCount(void);                        // вывести все индексы

void sortBook(void);                        // сортировка книг с индексами методом половинного деления на индексе
void SwapIndex(int c1, int c2, int i);
int a_Lower_than_b(int a, int b);           // узнаю правду или ложь по отношению индексов друг к другу

void delBook(void);                         // удалить кингу с индексом

void Search(void);                          // запуск поиска
void poisk(int a, int b, char *name);       // рекрсивный поиск методом половинного деления на индексе

// второстепенные функции помощники
int forBookgetCount(struct Book bk);        // для индекса получить книгу
struct Book forCountgetBook(int a);         // для книги получить индекс

void outBook(struct Book b);                // просто вывод книги как переданной структуры



int main()
{
   FILE *file = fopen(filecount, "ab");


    int n;
    while(1){
        printf("\n       \x1b[31mMenu:\x1b[0m      \n");
        printf("1 - Add book      \n");
        printf("2 - Print books   \n");
        printf("3 - Sort books    \n");
        printf("4 - Delete book   \n");
        printf("5 - Search        \n");
        printf("6 - Exit          \n");
        printf("Enter your choice:\n");
        scanf("%d", &n);
        getchar();
        if (n == 1){       // занесение книги
            setBook();
        } else if(n == 2){ // вывод книг
            getBook();
        } else if(n == 3){ // сортировка индексов смотря на книги
            sortBook();
        } else if(n == 4){ // удаление книги с индексом
            delBook();
        } else if(n == 5){ // удаление книги с индексом
            Search();
        } else if(n == 6){ // выход
            printf("       \x1b[31mEnd\x1b[0m  \n");
            break;
        } else if( n == 7){ // очистка(фича разраба)
            FILE *f = fopen(filename, "wb");
            f = fopen(filecount, "wb");
            fclose(f);
        }
    }
}

void setBook(void){                            // занесение в бинарный файл новой книги + вызов функции заносящей индекс
    // записываю в структуру параметры книги и вношу в бинарный файл
    struct Book bk;
    FILE* filebin = NULL;
    filebin = fopen(filename, "ab");
    if (filebin == NULL){
        filebin = fopen(filename, "wb");
    }
    printf("Enter name of the book:");
    fgets(bk.Name, len, stdin);
    printf("Enter Author of the book:");
    fgets(bk.Author, len, stdin);
    printf("Enter Page count:");
    scanf("%d", &bk.PageCount);
    printf("Enter Rank book:");
    scanf("%d", &bk.Rank);

    fwrite(&bk, sizeof(struct Book), 1, filebin);

    fclose(filebin);
    setCount();
}                       // две смежные(getCount внутри этой) функции записывают книгу и индекс
void setCount(void){                           // занесение в базу индексов новый путем дозаписи
    // нахожу максимальный индекс книги
    FILE* file = fopen(filecount, "rb");
    int max = 0;
    int r;
    while(fread(&r, sizeof(int), 1, file)){
        if (max < r ){
            max = r;
        }
    }
    fclose(file);
    // записываю максимальный индекс + 1
    file = fopen(filecount, "ab");
    int c = max+1;
    fwrite(&c, sizeof(int), 1, file);
    fclose(file);
    //-----------------------
    // нахожу максимальный индекс книги
    file = fopen(filecountAuthor, "rb");
    max = 0;
    while(fread(&r, sizeof(int), 1, file)){
        if (max < r ){
            max = r;
        }
    }
    fclose(file);
    // записываю максимальный индекс + 1
    file = fopen(filecountAuthor, "ab");
    c = max+1;
    fwrite(&c, sizeof(int), 1, file);
    fclose(file);
    //----------------------
    file = fopen(filecountRank, "rb");
    max = 0;
    while(fread(&r, sizeof(int), 1, file)){
        if (max < r ){
            max = r;
        }
    }
    fclose(file);
    // записываю максимальный индекс + 1
    file = fopen(filecountRank, "ab");
    c = max+1;
    fwrite(&c, sizeof(int), 1, file);
    fclose(file);
}                      // ставлю индекс в файл max от существующих

void getBook(void){                            // вывод всех книг
    FILE *f = fopen(filename, "rb");
    int i = 1;
    struct Book bk;
    while(fread(&bk, sizeof(struct Book), 1, f))
    {
        printf("   \x1b[34mBook %d\n\x1b[0m", i);
        outBook(bk);
        i++;
    }
    fclose(f);
    getCount();
}                       // вывод всех индексов книг
void getCount(void){
    printf("Индексы по имени книги \n");
    FILE* file = fopen(filecount, "rb");
    if(file == NULL){
        file = fopen(filecount, "wb");
        printf("Файл только что создался");
        fclose(file);
    }else {
        int c;
        while (fread(&c, sizeof(int), 1, file)) {
            printf("%d\n", c);
        }
        fclose(file);
    }
    //-------------------------
    printf("Индексы по Автору книги \n");
    file = fopen(filecountAuthor, "rb");
    if(file == NULL){
        file = fopen(filecountAuthor, "wb");
        printf("Файл только что создался");
        fclose(file);
    }else {
        int c;
        while (fread(&c, sizeof(int), 1, file)) {
            printf("%d\n", c);
        }
        fclose(file);
    }
    //---------------------------
    printf("Индексы по ранку книги \n");
    file = fopen(filecountRank, "rb");
    if(file == NULL){
        file = fopen(filecountRank, "wb");
        printf("Файл только что создался");
        fclose(file);
    }else {
        int c;
        while (fread(&c, sizeof(int), 1, file)) {
            printf("%d\n", c);
        }
        fclose(file);
    }
}                      // вывод всех индексов книг

void sortBook(void){                           // сортировка индексов по книгам
    struct Book bk1;
    struct Book bk2;
    FILE *f = fopen(filename, "rb+");
    int i = 0;
    while(fread(&bk1, sizeof(struct Book), 1, f))//определяю сколько книг
    {
        i++;
    }

    if (i > 1) { // если книг больше одной(иначе сортировать нечего)
        for(int r = 0; r < i*i; r++) { // пузырьком прохожусь
            for (int w = 0; w < (i-1); w++) {     // построчно читаю по две структуры и сравниваю
                fseek(f, (w) * sizeof(struct Book), SEEK_SET);
                fread(&bk1, sizeof(struct Book), 1, f);
                fseek(f, (w + 1) * sizeof(struct Book), SEEK_SET);
                fread(&bk2, sizeof(struct Book), 1, f);
                if (strcmp(bk1.Name, bk2.Name) > 0 && a_Lower_than_b(forBookgetCount(bk1), forBookgetCount(bk2))) { // если рейтинг ниже, то поменять
                    SwapIndex(forBookgetCount(bk1), forBookgetCount(bk2), i);
                }
            }
        }
    }
    fclose(f);
}                      // сортирока индексов по книгам(по именам)
void SwapIndex(int c1, int c2, int i){         // перестановка местами индексов
    // с1 и с2 - то что нужгно свапнуть, а i - кол-во индексов в бинрнике с индексами
    // буфер индексов из файла
    int *A = (int*) malloc((i) * sizeof(int));
    // считываю все индексы
    FILE* file = fopen(filecount, "rb");
    int w = 0;
    int s;
    //читаем все индексы в массив и узнаем сколько их

    while(fread(&s, sizeof(int), 1, file)){
        A[w] = s;
        w++;
    }

    fclose(file);//закрываю файл
    // переставляю местами индекса в массиве
    for(int j = 0; j < w; j++){
        if(A[j] == c1){
            A[j] = c2;
        }else {
            if (A[j] == c2) {
                A[j] = c1;
            }
        }
    }
    // записываю все индексы без удаленного в перезаписанный(чистый) файл
    file = fopen(filecount, "wb");
    for(int j = 0; j < w; j++){
        fwrite(&A[j], sizeof(int), 1, file);
    }
    fclose(file);
}    // смена индексов, побочная функция
int a_Lower_than_b(int a, int b){              // сравнивает относительное положэение индексов в файле с индесами
    // a и b - то что нужно сравнить
    // считываю все индексы
    FILE* file = fopen(filecount, "rb");
    int s;
    //читаем все индексы в массив и узнаем сколько их

    while(fread(&s, sizeof(int), 1, file)){
        if(s == a){ // если  a -ый индекс выше b то возвращаем  true (1)
            return 1;
        }
        if(s == b){ // если и-ый индекс выше а, то возвращаем false (0)
            return 0;
        }
    }
}         // побочная функция для сравнения, ниже ли один индекс книги индекса другой книги

void delBook(void){                            // удаление книги и индекса
    struct Book bk1;
    struct Book bk2;
    char name[len];
    FILE *f = fopen(filename, "rb+");
    printf("Введите имя Автора, книгу которогу вы хотите удалить:");
    fgets(name, len + 1, stdin);
    int i = 0;
    while(fread(&bk1, sizeof(struct Book), 1, f))//определяю сколько книг
    {
        i++;
    }
    if (i > 0) { // проверяю, есть ли книги
        int X = i;
        int *A = (int*) malloc(i * sizeof(int));
        int k = 0;
        int need = -1;
        for (int w = 0; w < i; w++) {     // построчно читаю структуры чтобы узнать, есть ли нужная книга(для удаления)
            fseek(f, (w) * sizeof(struct Book), SEEK_SET);
            fread(&bk1, sizeof(struct Book), 1, f);
            if (strcmp(bk1.Author, name) == 0) { // если совподает нужные имена, то k++
                k++;
                need = w+1;
            }
        }
        if(k>0) { // если есть нужная книга на удаление
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
            // это для винды


            // с файлом со структурами закончили, начинаем менять индексы
            // тк это все лежит в одном if, то книга точно есть и ошибки не будет
            // открываем файл с индексами для работы с чтением
            FILE* file = fopen(filecount, "rb");
            int w = 0;
            //читаем все индексы в массив
            while(fread(&w, sizeof(int), 1, file)){
                A[w] = w;
                w++;
            }
            fclose(file);//закрываю файл
            // перезаписываю массив со сдвигом индексов
            for(w = 0; w <= X; w++){
                if(A[w] == need){
                    A[w] = - 100;
                }
                if(A[w] > need){
                    A[w] = A[w] - 1;
                }
            }
            // записываю все индексы без удаленного в перезаписанный(чистый) файл
            file = fopen(filecount, "wb");
            for(i = 0; i <= X; i++){
                if(A[i] > 0){
                    fwrite(&A[i], sizeof(int), 1, file);
                }
            }
            fclose(file);

            printf("\x1b[31mКнига удалена\x1b[0m \n");
        }
        else{
            printf("\x1b[31mКнига не найдена в списке с таким автором\x1b[0m\n");
        }
    }
    sortBook();
    fclose(f);
}                       // удаление книги с индексом

void Search(void){                             // запуск рекурсивного поиска по имени книги
    struct Book bk1;
    char name[len];
    FILE *f = fopen(filename, "rb");
    printf("Введите имя Книги, которую вы хотите найти:");
    fgets(name, len + 1, stdin);
    int i = 0;
    while(fread(&bk1, sizeof(struct Book), 1, f))//определяю сколько книг
    {
        i++;
    }
    if (i > 1){ // если книг большще одной
        poisk(0, i, name);
    }else{ // если нет, то просто получаю по индексу первой книги
        if(i >0) {
            outBook(forCountgetBook(1));
        }
    }
    fclose(f);
}                        // запуск поиска с помощью побочной функции
void poisk(int a, int b, char *name){          // рекурсивная функция для поиска книги по имени
    // а и b - границы массива индексов, name - имя которое мы каждый раз сравниваем
    if (a == b){ // если совпали границы, то выводим книгу
        return outBook(forCountgetBook(a));
    }else{
        if(a < b){ // если границы разные, то делим
            // буфер индексов из файла
            int *A = (int*) malloc((b+2) * sizeof(int));
            // считываю все индексы
            FILE* file = fopen(filecount, "rb");
            int s;
            int y = 0;
            //читаем все нужные в промежутке индексы в массив
            for (int w = a; w < b; w++) {     // построчно читаю массив индексов
                fseek(file, (w) * sizeof(int), SEEK_SET);
                fread(&s, sizeof(int), 1, file);
                A[y] = s;
                y++;
            }
            fclose(file);//закрываю файл

            // вычисляю границу массива, которую мне надо
            int c = b/2;
            int index = A[b/2]; // получаю сам индекс книги из массива

            struct Book bk;
            bk = forCountgetBook(index); // получаю книгу, которая посередине

            if (strcmp(bk.Name, name) == 0){ // если это та книга, то выводим
                outBook(bk);
                printf("      \x1b[31mКнига найдена!\x1b[0m  \n");
            }else if ( strcmp(bk.Name, name) < 0){ // если она выше, то устанавливаем границу от середины и до верхней границы
                poisk(c, b, name);
            }
            else if ( strcmp(bk.Name, name) > 0){ // иначе от начала и до середины
                poisk(a, c, name);
            }
        }
    }
}     // рекурсивная функция поиска для поиска книги по имени


int forBookgetCount(struct Book bk){           // функция нахождения индекса по книге
    // bk  - е индекс мы ищем в файле с индексами
    FILE *f = fopen(filename, "rb");
    int i = 1;
    struct Book B;
    int need = -1;
    while(fread(&B, sizeof(struct Book), 1, f))
    {
        if(strcmp(B.Name,bk.Name) ==0  && strcmp(B.Author, bk.Author) == 0 && B.PageCount == bk.PageCount && B.Rank == bk.Rank){
            need  = i;
            return need;
        }
        i++;
    }
    fclose(f);
    return need;
}      //  нахождение индекса по книге в файле книг
struct Book forCountgetBook(int a){            // функция для нахождения книги по индексу
    // a - индекс книги, которую мы вернем
    FILE *f = fopen(filename, "rb");
    int i = 1;
    struct Book bk;
    while(fread(&bk, sizeof(struct Book), 1, f))
    {
        if(i == a){
            return bk;
        }
        i++;
    }
    fclose(f);
    return bk;
}       //  для нахождения книги по индексу в файле индексов
void outBook(struct Book a){                   // функция для вывода книги
    // книгу "а" мы принтанем
    printf("\x1b[32mName:\x1b[0m %s", a.Name);
    printf("\x1b[32mAuthor:\x1b[0m %s", a.Author);
    printf("\x1b[32mPage Count:\x1b[0m %d\n", a.PageCount);
    printf("\x1b[32mRank:\x1b[0m %d\n", a.Rank);
}              // функция для вывода книги
