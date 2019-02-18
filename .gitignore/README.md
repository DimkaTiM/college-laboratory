# laba_14

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <fcntl.h>
#include <unistd.h>

База данных книг с рангами,бинарная сортировка по рангу 

const int len = 255;

//Файлы для каждой таблицы
char const filecust[] = "customers.bin";
char const fileprod[] = "products.bin";
char const filepur[] = "pursachs.bin";

void cust(void);
void prod(void);
void pur(void);

//Функции Для Покупателя
void Cust_Set(void);                // добавить
void Cust_del(void);                // удалить
void Cust_getall(void);             // вывести всех
void Cust_getallpurchase(void);     // вывести все покупки покупателя
void Cust_getallprod(void);         // вывести все продукты покупателя

//Функции Для Товара
void Prod_Set(void);                // добавить
void Prod_del(void);                // удалить
void Prod_getall(void);             // вывести всех
void Prod_getallpurchase(void);     // вывести все покупки, где упоминается этот товар
void Prod_getallcust(void);         // вывести всех покупателей этого продукта

//Функции Для Покупки
void Pur_set(void);                 // добавить
void Pur_del(void);                 // удалить
void Pur_getall(void);              // вывести всех

//Доп функции
void Cost_for_all_pur(void);
void Max_cost_for_all_pur_cust(void);

struct customer{
    char Name[len];
    int id;
};               // покупатель
struct product{
    char Name[len];
    int id;
    int cost;
    int count;
};                // продукт
struct purchase{
    int idCust;
    int idProd;
    int count;
    int date; // Формат: 06-07-2019 == 20190607
    int check;
};               // покупка

int main() {
    int n;
    while (1) {
        printf("\n       \x1b[31mMenu I:\x1b[0m      \n");
        printf("Покупатель: 1  \n");
        printf("Товар: 2       \n");
        printf("Покупка: 3     \n");
        printf("    \x1b[31mДополнительное меню:\x1b[0m  \n");
        printf("Вывести для каждой покупки её стоимость:  4 \n");
        printf("Для каждого покупателя вывести максимальный чек:  5 \n");
        printf("\x1b[31mВыход:\x1b[0m 6       \n");
        scanf("%i", &n);
        getchar();
        if (n == 1) { //Покупатель
            cust();
        } else if (n == 2) { //Товар
            prod();
        } else if (n == 3) { // Покупка
            pur();
        } else if (n == 4) { // стоимость кажой покупки
            Cost_for_all_pur();
        }else if (n == 5) {
            Max_cost_for_all_pur_cust(); // максимальный чек покупателя
        } else if (n == 6) { // выход
            printf("Вы вышли из программы");
            break;
        }else {
            printf("\x1b[34mВыберете пункт меню!\x1b[0m");
        }
    }
}

void cust(void){
    int n;
    while (1) {
        printf("\n       \x1b[31mMenu 2:\x1b[0m      \n");
        printf("Добавить покупателя: 1       \n");
        printf("Удалить покупателя : 2       \n");
        printf("Вывести всех: 3              \n");
        printf("Все покупки покупателя: 4    \n");
        printf("Все товары покупателя: 5     \n");
        printf("Вернуться в главное меню: 6  \n");
        scanf("%i", &n);
        getchar();
        if(n == 1){
            Cust_Set();
        }else if(n == 2){
            Cust_del();
        }else if(n == 3){
            Cust_getall();
        }else if(n == 4){
            Cust_getallpurchase();
        }else if(n == 5){
            Cust_getallprod();
        }else if(n == 6){
            break;
        }else {
            printf("Выберете пункт меню!  \n");
        }
    }
}
void Cust_Set(void){
    struct customer cst;
    FILE* f = NULL;
    f = fopen(filecust, "ab");
    if (f == NULL) {
        f = fopen(filecust, "wb");
        if(f == NULL){
            printf("    \x1b[31mНе удалось открыть файл\x1b[0m    \n");
        }
    }
    printf("Имя покупателя:");
    fgets(cst.Name, len, stdin);
    printf("Id Покупателя:");
    scanf("%d", &cst.id);
    fwrite(&cst, sizeof(struct customer), 1, f);
    fclose(f);
}
void Cust_del(void){
    struct customer cust1;
    struct customer cust2;
    char name[len];
    FILE *f = fopen(filecust, "rb+");
    printf("Введите имя Покупателя, которого хотите удалить:");
    fgets(name, len + 1, stdin);
    int i = 0;
    while(fread(&cust1, sizeof(struct customer), 1, f))//определяю сколько структур лежит внутри
    {
        i++;
    }
    if (i > 0) { // проверяю, есть ли структура, которую надо удалить
        int k = 0;
        for (int ent = 0; ent < i; ent++) {
            for (int w = 0; w < i; w++) {     // переставляю местами
                fseek(f, (w) * sizeof(struct customer), SEEK_SET);
                fread(&cust1, sizeof(struct customer), 1, f);
                if (strcmp(cust1.Name, name) == 0) {
                    k++;
                }
            }
        }
        if(k>0) {
            rewind(f);//обнуляю указатель, ставлю в начало
            for (int ent = 0; ent < i; ent++) { // пузырьковая сортировка при удалении
                for (int w = 0; w < i; w++) {     // переставляю местами
                    fseek(f, (w) * sizeof(struct customer), SEEK_SET); // ставлю указатель на нужную строчку
                    fread(&cust1, sizeof(struct customer), 1, f); // читаю структуру
                    if (strcmp(cust1.Name, name) == 0) { // если совпадает нужное имя Покупателя

                        fseek(f, (i - 1) * sizeof(struct customer), SEEK_SET); //буфер структуры
                        fread(&cust1, sizeof(struct customer), 1, f);
                        for (int n = i; n > w; n--) { // читаю с конца, заменяю до нужного элемента

                            fseek(f, (n - 1) * sizeof(struct customer), SEEK_SET);
                            fread(&cust2, sizeof(struct customer), 1, f);

                            fseek(f, (n - 1) * sizeof(struct customer), SEEK_SET);
                            fwrite(&cust1, sizeof(struct customer), 1, f);

                            cust1 = cust2;
                        }
                    }
                }
            }
            fseek(f, (i - 1) * sizeof(struct customer), SEEK_SET); // указатель на последнюю строку
            ftruncate(fileno(f), sizeof(struct customer) * i - sizeof(struct customer)); // удаляю одну структуру
            printf("\x1b[31mПокупатель Удален\x1b[0m \n");
        }
        else{
            printf("\x1b[31mНет такого покупателя в базе, которого вы хотите удалить\x1b[0m\n");
        }
    }
    fclose(f);
}
void Cust_getall(void){
    FILE* f = NULL;
    f = fopen(filecust, "rb");
    if (f == NULL) {
        printf("    \x1b[31mНе удалось открыть файл\x1b[0m    \n");
    }
    int i = 1;
    struct customer cst;
    while(fread(&cst, sizeof(struct customer), 1, f))
    {
        printf("   \x1b[34mCustomer %d\n\x1b[0m", i);
        printf("\x1b[32mName:\x1b[0m %s", cst.Name);
        printf("\x1b[32mId:\x1b[0m %d\n", cst.id);
        i++;
    }
    fclose(f);
}
void Cust_getallpurchase(void){
    printf("Введите имя покупателя:");
    char name[len];
    fgets(name, len, stdin);
    FILE* f = NULL;
    f = fopen(filecust, "rb"); // Залезаю в файл с Покупателями и нахожу ID
    if (f == NULL) {
        printf("    \x1b[31mНе удалось открыть файл\x1b[0m    \n");
    }
    int i = 0;
    int Id = 0;
    struct customer cst;
    while(fread(&cst, sizeof(struct customer), 1, f)) //Ищу нужный ID
    {
        if (strcmp(name, cst.Name) == 0){
            Id = cst.id;
            i++;
        }
    }
    fclose(f);
    if(i == 0){ // Если нет нужно, то ничего не делаем
        printf("\x1b[31mНет такого Покупателя!\x1b[0m\n");
    }else{      // Если есть нужный Покупатель, то залезаем в файл с покупками
        i = 0;
        f = fopen(filepur, "rb");
        if (f == NULL) {
            printf("    \x1b[31mНе удалось открыть файл\x1b[0m    \n");
        }
        struct purchase prch;
        while(fread(&prch, sizeof(struct purchase), 1, f))
        {
            if (prch.idCust == Id){ // находим нужные покупки по id и выводим
                printf("   \x1b[34mPurchase %d\n\x1b[0m", i+1);
                printf("\x1b[32mID Покупателя:\x1b[0m %d\n",         prch.idCust);
                printf("\x1b[32mID Продукта:\x1b[0m %d\n",       prch.idProd);
                printf("\x1b[32mКоличество купленного товара:\x1b[0m %d\n",      prch.count);
                printf("\x1b[32mДата покупки:\x1b[0m %d\n",      prch.date);
                printf("\x1b[32mID чека:\x1b[0m %d\n",      prch.check);
                i++;
            }
        }
        if(i == 0){
            printf("\x1b[31mНет покупок у этого Покупателя\x1b[0m");
        }
        fclose(f);
    }


}
void Cust_getallprod(void){
    printf("Введите имя покупателя:");
    char name[len];
    fgets(name, len, stdin);
    FILE* f = NULL;
    f = fopen(filecust, "rb"); // Залезаю в файл с Покупателями и нахожу ID
    if (f == NULL) {
        printf("    \x1b[31mНе удалось открыть файл\x1b[0m    \n");
    }
    int i = 0;
    int Id = 0; //ID покупателя
    struct customer cst;
    while(fread(&cst, sizeof(struct customer), 1, f)) //Ищу нужный ID
    {
        if (strcmp(name, cst.Name) == 0){
            Id = cst.id;
            i++;
        }
    }
    fclose(f);
    if(i == 0){ // Если нет нужного, то ничего не делаем
        printf("\x1b[31mНет такого Покупателя!\x1b[0m\n");
    }else{      // Если есть нужный Покупатель, то залезаем в файл с покупками
        int ProdID = 0;
        i = 0;
        f = fopen(filepur, "rb");
        if (f == NULL) {
            printf("    \x1b[31mНе удалось открыть файл\x1b[0m    \n");
        }
        struct purchase prch;
        int a[100] = {0};
        while(fread(&prch, sizeof(struct purchase), 1, f))
        {
            if (prch.idCust == Id){ // находим нужные покупки по id и вытаскиваем ID товара
                ProdID = prch.idProd;
                FILE* F = fopen(fileprod, "rb");
                if (F == NULL) {
                    printf("    \x1b[31mНе удалось открыть файл\x1b[0m    \n");
                }
                i = 0;

                struct product pr;
                while(fread(&pr, sizeof(struct product), 1, F))
                {
                    int r = 0;
                    if(ProdID == pr.id){
//                        a[i] = pr.id;
                        for(int e = 0; e < 100; e++){
                            if(a[e] == ProdID){
                                r++;
                            }
                        }
                        if(r == 0){
                            printf("   \x1b[34mProduct %d\n\x1b[0m", i+1);
                            printf("\x1b[32mName:\x1b[0m %s", pr.Name);
                            printf("\x1b[32mID:\x1b[0m %d\n", pr.id);
                            printf("\x1b[32mCost:\x1b[0m %d\n", pr.cost);
                            printf("\x1b[32mCount:\x1b[0m %d\n", pr.count);
                            a[i] = ProdID;
                            i++;
                            break;
                        }
                    }
                }



                if(i == 0){
//                    printf("\x1b[31mНет такого товара\x1b[0m \n");
                }
                fclose(F);
            }
        }
        fclose(f);
        if(i == 0){// Если не нашли товар, то ничего не делаем
//            printf("\x1b[31mНет покупок с нужным товаром\x1b[0m \n");
        }
    }
}

void prod(void){
    int n;
    while (1) {
        printf("\n       \x1b[31mMenu 2:\x1b[0m      \n");
        printf("Добавить товар: 1               \n");
        printf("Удалить товар: 2               \n");
        printf("Вывести все товары: 3           \n");
        printf("Все покупки этого товара: 4     \n");
        printf("Все покупатели этого товара: 5  \n");
        printf("Вернуться в главное меню: 6     \n");
        scanf("%i", &n);
        getchar();
        if(n == 1){
            Prod_Set();
        }else if(n == 2){
            Prod_del();
        }else if(n == 3){
            Prod_getall();
        }else if(n == 4){
            Prod_getallpurchase();
        }else if(n == 5){
            Prod_getallcust();
        }else if(n == 6){
            break;
        }else {
            printf("Выберете пункт меню!\n");
        }
    }
}
void Prod_Set(void){
    struct product pr;
    FILE* f = NULL;
    f = fopen(fileprod, "ab");
    if (f == NULL) {
        f = fopen(filecust, "wb");
        if(f == NULL){
            printf("    \x1b[31mНе удалось открыть файл\x1b[0m    \n");
        }
    }
    printf("Имя товара:");
    fgets(pr.Name, len, stdin);
    printf("ID товара:");
    scanf("%d", &pr.id);
    printf("Стоимость:");
    scanf("%d", &pr.cost);
    printf("Количество на складе:");
    scanf("%d", &pr.count);
    fwrite(&pr, sizeof(struct product), 1, f);
    fclose(f);
}
void Prod_del(void){
    struct product cust1;
    struct product cust2;
    char name[len];
    FILE *f = fopen(fileprod, "rb+");
    printf("Введите имя Товара, который вы хотите удалить:");
    fgets(name, len + 1, stdin);
    int i = 0;
    while(fread(&cust1, sizeof(struct product), 1, f))//определяю сколько структур лежит внутри
    {
        i++;
    }
    if (i > 0) { // проверяю, есть ли структура, которую надо удалить
        int k = 0;
        for (int ent = 0; ent < i; ent++) {
            for (int w = 0; w < i; w++) {
                fseek(f, (w) * sizeof(struct product), SEEK_SET);
                fread(&cust1, sizeof(struct product), 1, f);
                if (strcmp(cust1.Name, name) == 0) {
                    k++;
                }
            }
        }
        if(k>0) {
            rewind(f);//обнуляю указатель, ставлю в начало
            for (int ent = 0; ent < i; ent++) { // пузырьковая сортировка при удалении
                for (int w = 0; w < i; w++) {     // переставляю местами
                    fseek(f, (w) * sizeof(struct product), SEEK_SET); // ставлю указатель на нужную строчку
                    fread(&cust1, sizeof(struct product), 1, f); // читаю структуру
                    if (strcmp(cust1.Name, name) == 0) { // если совпадает нужное имя Покупателя

                        fseek(f, (i - 1) * sizeof(struct product), SEEK_SET); //буфер структуры
                        fread(&cust1, sizeof(struct product), 1, f);
                        for (int n = i; n > w; n--) { // читаю с конца, заменяю до нужного элемента

                            fseek(f, (n - 1) * sizeof(struct product), SEEK_SET);
                            fread(&cust2, sizeof(struct product), 1, f);

                            fseek(f, (n - 1) * sizeof(struct product), SEEK_SET);
                            fwrite(&cust1, sizeof(struct product), 1, f);

                            cust1 = cust2;
                        }
                    }
                }
            }
            fseek(f, (i - 1) * sizeof(struct product), SEEK_SET); // указатель на последнюю строку
            ftruncate(fileno(f), sizeof(struct product) * i - sizeof(struct product)); // удаляю одну структуру
            printf("\x1b[31mТовар Удален\x1b[0m \n");
        }

    }else{
        printf("\x1b[31mНет такого товара в базе, который вы хотите удалить\x1b[0m\n");
    }
    fclose(f);
}
void Prod_getall(void){
    FILE* f = NULL;
    f = fopen(fileprod, "rb");
    if (f == NULL) {
        printf("    \x1b[31mНе удалось открыть файл\x1b[0m    \n");
    }
    int i = 1;
    struct product pr;
    while(fread(&pr, sizeof(struct product), 1, f))
    {
        printf("   \x1b[34mProduct %d\n\x1b[0m", i);
        printf("\x1b[32mName:\x1b[0m %s",      pr.Name);
        printf("\x1b[32mId:\x1b[0m %d\n",    pr.id);
        printf("\x1b[32mCost:\x1b[0m %d\n",    pr.cost);
        printf("\x1b[32mCount:\x1b[0m %d\n",   pr.count);
        i++;
    }
    fclose(f);
}
void Prod_getallpurchase(void){
    printf("Введите имя товара:");
    char name[len];
    int IDprod = 0;
    fgets(name, len, stdin);


    FILE* f = NULL;
    f = fopen(fileprod, "rb"); // залезаю в файл с товарами и ищу нужный
    if (f == NULL) {
        printf("    \x1b[31mНе удалось открыть файл\x1b[0m    \n");
    }
    int i = 0;
    struct product pr;
    while(fread(&pr, sizeof(struct product), 1, f))
    {
        if(strcmp(name,pr.Name) == 0){
            IDprod = pr.id;
            i++;
        }
    }
    fclose(f);
    if(i == 0){
        printf("    \x1b[31mНет такого товара\x1b[0m    \n");
    }else{
        f = fopen(filepur, "rb");
        if (f == NULL) {
            printf("    \x1b[31mНе удалось открыть файл\x1b[0m    \n");
        }
        i = 0;
        struct purchase prch;
        while(fread(&prch, sizeof(struct purchase), 1, f))
        {
            if(IDprod == prch.idProd){
                printf("   \x1b[34mPurchase %d\n\x1b[0m", i+1);
                printf("\x1b[32mID Покупателя:\x1b[0m %d\n",         prch.idCust);
                printf("\x1b[32mID Продукта:\x1b[0m %d\n",       prch.idProd);
                printf("\x1b[32mКоличество купленного товара:\x1b[0m %d\n",      prch.count);
                printf("\x1b[32mДата покупки:\x1b[0m %d\n",      prch.date);
                printf("\x1b[32mID чека:\x1b[0m %d\n",      prch.check);
                i++;
            }
        }
        fclose(f);
        if(i == 0){
            printf("    \x1b[31mНет такого продукта в покупках\x1b[0m    \n");
        }
    }
}
void Prod_getallcust(void){
    printf("Введите имя Товара:");
    char name[len];
    int IDprod = 0;
    int IDCust = 0;
    fgets(name, len, stdin);
    FILE* f = NULL;
    f = fopen(fileprod, "rb"); // залезаю в файл продуктов и нахожу ID
    if (f == NULL) {
        printf("    \x1b[31mНе удалось открыть файл\x1b[0m    \n");
    }
    int i = 1;
    struct product pr;
    while(fread(&pr, sizeof(struct product), 1, f))
    {
        if(strcmp(name,pr.Name) == 0){
            IDprod = pr.id;
            i++;
        }
    }
    fclose(f);
    //--------
    if(i == 1){
        printf("    \x1b[31mНет такого Товара\x1b[0m    \n");
    }else{
        f = fopen(filepur, "rb"); // залезаю в файл с покупками и вывожу каждого покупателя
        if (f == NULL) {
            printf("    \x1b[31mНе удалось открыть файл\x1b[0m    \n");
        }
        i = 1;
        struct purchase prch;
        int a[100] = {0};
        while(fread(&prch, sizeof(struct purchase), 1, f))
        {
            if(IDprod == prch.idProd){ // если нашли покупку, где есть нужный товар, то берем ID покупателя и илезем в файл покупателей

                IDCust = prch.idCust;
                FILE* F = NULL;
                F = fopen(filecust, "rb"); // файл покупателей
                if (F == NULL) {
                    printf("    \x1b[31mНе удалось открыть файл\x1b[0m    \n");
                }
                struct customer cst;
                while(fread(&cst, sizeof(struct customer), 1, F))
                {
                    int r = 0;
                    if(IDCust == cst.id){ // если нужный ID -> принтуем
                        for(int e = 0; e < 100; e++){
                            if(a[e] == IDCust){
                                r++;
                            }
                        }
                        if(r == 0) {
                            printf("   \x1b[34mCustomer %d\n\x1b[0m", i);
                            printf("\x1b[32mName:\x1b[0m %s", cst.Name);
                            printf("\x1b[32mId:\x1b[0m %d\n", cst.id);
                            i++;
                            a[i] = IDCust;
                            break;
                        }
                    }
                }
                fclose(F);
            }
        }
        fclose(f);
        if(i == 1){
            printf("    \x1b[31mУ Товара нет покупателей\x1b[0m    \n");
        }
    }
}

void pur(void){
    int n;
    while (1) {
        printf("\n       \x1b[31mMenu 2:\x1b[0m      \n");
        printf("Добавить покупку: 1                  \n");
        printf("Удалить покупку: 2                  \n");
        printf("Вывести все покупки: 3               \n");
        printf("Вернуться в главное меню: 4          \n");
        scanf("%i", &n);
        getchar();
        if(n == 1){
            Pur_set();
        }else if(n == 2){
            Pur_del();
        }else if(n == 3){
            Pur_getall();
        }else if(n == 4){
            break;
        }else {
            printf("Выберете пункт меню! \n");
        }
    }
}
void Pur_del(void){
    struct purchase pur1;
    struct purchase pur2;
    int n;
    FILE *f = fopen(filepur, "rb+");
    printf("Введите номер чека, который вы хотите удалить:");
    scanf("%d", &n);
    int i = 0;
    while(fread(&pur1, sizeof(struct purchase), 1, f))//определяю сколько структур лежит внутри
    {
        i++;
    }
    if (i > 0) { // проверяю, есть ли структура, которую надо удалить
        int k = 0;
        for (int ent = 0; ent < i; ent++) {
            for (int w = 0; w < i; w++) {
                fseek(f, (w) * sizeof(struct purchase), SEEK_SET);
                fread(&pur1, sizeof(struct purchase), 1, f);
                if (pur1.check == n) {
                    k++;
                }
            }
        }
        if(k>0) {
            rewind(f);//обнуляю указатель, ставлю в начало
            for (int ent = 0; ent < i; ent++) { // пузырьковая сортировка при удалении
                for (int w = 0; w < i; w++) {     // переставляю местами
                    fseek(f, (w) * sizeof(struct purchase), SEEK_SET); // ставлю указатель на нужную строчку
                    fread(&pur1, sizeof(struct purchase), 1, f); // читаю структуру
                    if (pur1.check == n) { // если совпадает нужное имя Покупателя

                        fseek(f, (i - 1) * sizeof(struct purchase), SEEK_SET); //буфер структуры
                        fread(&pur1, sizeof(struct purchase), 1, f);
                        for (int n = i; n > w; n--) { // читаю с конца, заменяю до нужного элемента

                            fseek(f, (n - 1) * sizeof(struct purchase), SEEK_SET);
                            fread(&pur2, sizeof(struct purchase), 1, f);

                            fseek(f, (n - 1) * sizeof(struct purchase), SEEK_SET);
                            fwrite(&pur1, sizeof(struct purchase), 1, f);

                            pur1 = pur2;
                        }
                    }
                }
            }
            fseek(f, (i - 1) * sizeof(struct purchase), SEEK_SET); // указатель на последнюю строку
            ftruncate(fileno(f), sizeof(struct purchase) * i - sizeof(struct purchase)); // удаляю одну структуру
            printf("\x1b[31mТовар Удален\x1b[0m \n");
        }

    }else{
        printf("\x1b[31mНет такого товара в базе, который вы хотите удалить\x1b[0m\n");
    }
    fclose(f);
}
void Pur_set(void){
    struct purchase purс;
    FILE* f = NULL;
    f = fopen(filepur, "ab");
    if (f == NULL) {
        f = fopen(filecust, "wb");
        if(f == NULL){
            printf("    \x1b[31mНе удалось открыть файл\x1b[0m    \n");
        }
    }
    printf("Id Покупателя:");
    scanf("%d", &purс.idCust);
    printf("Id Продукта:");
    scanf("%d", &purс.idProd);
    printf("Количество купленного товара:");
    scanf("%d", &purс.count);
    printf("Дата покупки:");
    scanf("%d", &purс.date);
    printf("Id чека");
    scanf("%d", &purс.check);
    fwrite(&purс, sizeof(struct purchase), 1, f);
    fclose(f);
}
void Pur_getall(void){
    FILE* f = NULL;
    f = fopen(filepur, "rb");
    if (f == NULL) {
        printf("    \x1b[31mНе удалось открыть файл\x1b[0m    \n");
    }
    int i = 1;
    struct purchase prch;
    while(fread(&prch, sizeof(struct purchase), 1, f))
    {
        printf("   \x1b[34mPurchase %d\n\x1b[0m", i);
        printf("\x1b[32mID Покупателя:\x1b[0m %d\n",         prch.idCust);
        printf("\x1b[32mID Продукта:\x1b[0m %d\n",       prch.idProd);
        printf("\x1b[32mКоличество купленного товара:\x1b[0m %d\n",      prch.count);
        printf("\x1b[32mДата покупки:\x1b[0m %d\n",      prch.date);
        printf("\x1b[32mID чека:\x1b[0m %d\n",      prch.check);
        i++;
    }
    fclose(f);
}

void Cost_for_all_pur(void){
    FILE* f = NULL;
    f = fopen(filepur, "rb"); // залезаю в файл с покупками, читаю все
    if (f == NULL) {
        printf("    \x1b[31mНе удалось открыть файл\x1b[0m    \n");
    }
    struct purchase prch;
    while(fread(&prch, sizeof(struct purchase), 1, f)) // в каждой покупке лезу в файл с товарами и ищу его цену
    {
        FILE* File = NULL;
        File = fopen(fileprod, "rb");
        if (File == NULL) {
            printf("    \x1b[31mНе удалось открыть файл\x1b[0m    \n");
        }
        printf("   \x1b[34mPurchase \n\x1b[0m");
        printf("\x1b[32mID Покупателя:\x1b[0m %d\n",         prch.idCust);
        printf("\x1b[32mID Продукта:\x1b[0m %d\n",       prch.idProd);
        printf("\x1b[32mКоличество купленного товара:\x1b[0m %d\n",      prch.count);
        printf("\x1b[32mДата покупки:\x1b[0m %d\n",      prch.date);
        printf("\x1b[32mID чека:\x1b[0m %d\n",      prch.check);
        struct product pr;
        while(fread(&pr, sizeof(struct product), 1, File)) // вывожу всю покупку и конечную стоимость
        {
            if(prch.idProd == pr.id){
                printf("\x1b[30mСтоимость покупки:\x1b[0m %d", pr.cost*prch.count);
            }
        }
        fclose(File);
    }
    fclose(f);
}
void Max_cost_for_all_pur_cust(void){
    int max = 0;
    FILE* f = NULL;
    f = fopen(filecust, "rb");
    if (f == NULL) {
        printf("    \x1b[31mНе удалось открыть файл\x1b[0m    \n");
    }
    int i = 1;
    struct customer cst;
    while(fread(&cst, sizeof(struct customer), 1, f))
    {
        printf("   \x1b[34mCustomer %d\n\x1b[0m", i);
        printf("\x1b[32mName:\x1b[0m %s", cst.Name);
        printf("\x1b[32mId:\x1b[0m %d\n", cst.id);
        FILE* file = NULL;
        file = fopen(filepur, "rb");
        if (f == NULL) {
            printf("    \x1b[31mНе удалось открыть файл\x1b[0m    \n");
            break;
        }
        int a = 1;
        struct purchase prch;
        while(fread(&prch, sizeof(struct purchase), 1, file))
        {
            if(prch.idCust == cst.id) {
                FILE* F = NULL;
                F = fopen(fileprod, "rb");
                if (F == NULL) {
                    printf("    \x1b[31mНе удалось открыть файл\x1b[0m    \n");
                    break;
                }
                int x = 1;
                struct product pr;
                while(fread(&pr, sizeof(struct product), 1, F))
                {
                    if(prch.idProd == pr.id){
                        if (max < prch.count*pr.cost){
                            max = prch.count*pr.cost;
                        }
                    }
                    x++;
                }
                if(x == 1){
                    printf("\x1b[33mОшибка в Чеке - нет такого продукта:\x1b[0m \n");
                }
                fclose(F);
                a++;
            }
        }
        if(a == 1){
            printf("\x1b[33mУ покупателя нет покупок:\x1b[0m \n");
        }
        fclose(file);
        if( max != 0) {
            printf("\x1b[33mМаксимальный чек:\x1b[0m %d\n", max);
        }
        i++;
        max = 0;
    }
    fclose(f);
}

