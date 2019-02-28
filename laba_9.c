//Программа позволяет составить словарь слов 9.1 или вывод слов до указанной точки 9.2

#include <stdio.h>
#include <string.h>

void laba91(void);
void laba92(void);

//  Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of          Lorem Ipsum.

int main(void){
//    laba91();
    laba92();
}


void laba92(void)
{
    printf(" Input your string: \n");
    char *string[5000];
    char c[5000];
    gets(c);
    string[0] = strdup(c);
    
    char *dict[10000];
    char *token, *last;
    int i =0;
    token = strtok_r(string[0], " ", &last);
    long min_count = 0;
    while (token) {     // Разбиение на слова //
        dict[i] = token;
        if ( min_count < strlen(dict[i])){
            min_count = strlen(dict[i]);
        }
//        printf("%s %d \n", dict[i], strlen(dict[i]));
        i++;
        token = strtok_r(NULL, " ", &last);
    }
    printf("\n\n  Input your size string >= %li: \n\n", min_count);
    int n;
    scanf("%d", &n);
    
//    int count = i;
//    printf("%d ", count);
    i = 0;
    long k = 0;
    if (n >= min_count){
        while(dict[i]){
    //        printf("%s %i \n", dict[i], strlen(dict[i]));
            if ( (k + strlen(dict[i])) <= n ){
                printf("%s ", dict[i]);
                k = k + strlen(dict[i]);
                i++;
            }
            else{
                for (long ddd = k; ddd < n; ddd++){
                    printf(" ");
                }
                printf("\n");
                k = 0;
            }
            
//            printf("%s ", dict[i]);
            
        }
    }
    printf("\n");
    printf("\n");
    printf("\n");
    
}

void laba91(void)
{

    
    printf("Input your string: \n");
    char *string[5000];
    char c[5000];
    gets(c);
    string[0] = strdup(c);
    string[1] = strdup(c);
    printf("\n\nOut your string: \n\n");
    printf("%s \n", string[0]);
    printf("Dict:\n");
    int k = 0;
    char *Fdict[10000];
    int count = 0;
    char *token, *last;
    int i =0;
    token = strtok_r(string[0], ". ,'", &last);
    while (token) {     // Занесение в словарь уникальных слов
        int d = 0;
        for (int k = 0; k < i-count; k++){
            if (Fdict[k] != NULL)
                if (strcmp(Fdict[k], token) == 0){
                    d = 1;
                    count++;
                    break;
                }
        }
        if(d == 0){
            Fdict[i] = token;
//            printf("%s %d\n", Fdict[i], i);
            k++;
        }
        i++;
        token = strtok_r(NULL, " ,.'-", &last);
    }
    int f = 0;
    char *dict[k];
    for (int i = 0; i < k; i++){  // Словарь неповторяющихся слов
        if (Fdict[i] != NULL){
            dict[f] = Fdict[i];
//            printf("%s ", dict[f]);
            f++;
        }
    }
    int Alfa[f];
    for (int i = 0; i < f; i++){
        Alfa[i] = 0;
    }
    char  *la;
    token = strtok_r(string[1], ". ,'", &la);  // Подсчет слов для множественного словаря
    while (token) {
        for (int k = 0; k < f; k++){
//            printf("%s %s \n",token, dict[k]);
            if (strcmp(dict[k], token) == 0){
                Alfa[k] += 1;
            }
        }
        
        token = strtok_r(NULL, " ,.'-", &la);
    }
    
    printf("\n\n\n");
    
    
    for (int i = f; i>=0; i--){  // Вывод словаря по убыванию частоты встречаемости
        for(int j = 0; j<f; j++){
            if(Alfa[j] == i ){
                printf("%s %d \n",dict[j], Alfa[j]);
            }
        }
    }
    
}
