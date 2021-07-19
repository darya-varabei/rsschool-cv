#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <dirent.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
//#include <time.h>

void list_of_fils();                            //Вывод списка файлов
void work_fils_main();                          //Работа со всеми файлами
void* work_file(void* arg);                     //Поточная функция поиска слова в файле
void open_thread(char*, pthread_t*);            //Открытие потока (если нужно)
void zap();                                     //Начальное заполнение массива занятости идентифекаторов потока
//void time_of_work();                            //Расчёт времени поиска файлов
//void kol_of_ans();                              //Вывод количества файлов, в которых найдено нужное слово

struct str                              //Создание массива структур
{
        char* name_of_file;
        char word[256];
        int loc_num;
} str_glob[10];

bool rez[10];                           //Массив занятости идентифекаторов потока

//unsigned int begin, end;                //Переменные для времени начала программы и её конца
//int kol_ans = 0;                        //Переменная для подсчёта количества файлов в которых найдено нужное слово

int main()
{
        zap();
        list_of_fils();
        work_fils_main();
       // kol_of_ans();
        //time_of_work();
}
void list_of_fils()
{
        printf("\n");
        DIR *dir = opendir(".");                                //Открытие папки
        if (dir)                                                //Проверка на то, чтобы она открылась
        {
                struct dirent *ent;
                while ((ent = readdir(dir)) != NULL)
                {
                        printf("     %s\n", ent->d_name);       //Вывод имён файлов, содержащихся в открытой папке
                }
                printf("\n");
        }
        else
        {
                fprintf(stderr, "Error\n");                     //Вывод ошибки, если файл не может быть открыт
        }

        printf("\n");
}


void work_fils_main()
{
        printf("Enter word for search: ");                                              //Ввод слова для поиска
        scanf("%s", str_glob[1].word);
        printf("\n");

       // begin = clock();                                                                //Засекаем время начала работы с файлами

        DIR *dir = opendir(".");                                                        //Открытие папки
        if (dir)
        {
                struct dirent *ent;
                pthread_t* threads = (pthread_t*) malloc(10 * sizeof(pthread_t));       //Создание массива из 10 идентификаторов потока
                int num_1;
                while ((ent = readdir(dir)) != NULL)
                {
                        char* name_1 = "a.out";
                        char* name_2 = "..";
                        char* name_3 = ".";
                        char* name_4 = "Lab6.c";
                        int k = 0;
                        for (int i = 0; i < strlen(ent->d_name); i++)                   //Запрет на открытие системных и программных файлов
                        {
                                if (ent->d_name[i] == name_1[i] || ent->d_name[i] == name_2[i] || ent->d_name[i] == name_3[i] || ent->d_name[i] == name_4[i])
                                        k++;
                        }
                        if (k == strlen(ent->d_name))

           continue;
                        else
                        {
                                open_thread(ent->d_name, threads);                      //функция для создания потока
                                //printf("%s work\n", ent->d_name);                     //Вывод всех файлов которые проверялись
                        }
                }

                for(int f = 0; f < 10; f++)
                {
                        pthread_join(threads[f], NULL);                                 //Ожидание окончания работы всех потоков
                }

                printf("\n");
        }
        else
        {
                fprintf(stderr, "Error\n");
        }

      //  end = clock();                                                                  //Засекаем время конца работы с файлами
}
    
    
void* work_file(void* arg){
 int loc_num = *(int *) arg;                                     //Преобразование аргумента
        char str[256];
        FILE *f = fopen(str_glob[loc_num].name_of_file, "r");           //Открытие файла

        if (f == NULL){
                printf("\nFile is not open\n");
                fclose(f);
        }
        else{
                int n = 1;
               // int scan;
                for (int i = 0; i < n; i++)                             //Цикл сканирования файла до конца
                {
                        if (fscanf(f, "%s", str) == 1)                  //Проверка на конец файла
                        {
                                for (int q = 0; q < strlen(str); q++)   //Замена знаков препинания на нуль-терминатор
                                {
                                        if (str[q] == '.' || str[q] == ',' || str[q] == '!' || str[q] == '?')
                                                str[q] = '\0';
                                }

                                if (strcmp(str, str_glob[1].word) == 0) //Сравнение со словом, которое нужно найти
                                {
                                        printf("%s\tWord №%i\t\t%i\n", str_glob[loc_num].name_of_file, n, loc_num);
                                        //printf("%i\n", loc_num);      //Вывод имени нужного файла
                                        //printf("%s\n", str);          //Вывод найденного слова
                                        //printf("Word №%d\n", n);      //Вывод номера позиции найденного слова в файле
                                        //kol_ans++;
                                        rez[loc_num] = true;            //Идентификатор потока свободен
                                        pthread_exit(NULL);             //Конец потока
                                }
                                n++;                                    //Продолжить главный цикл
                        }
                        else
                        {
                                fclose(f);                              //Закрытие файла
                                rez[loc_num] = true;                    //Идентификатор свободен
                                pthread_exit(NULL);                     //Конец потока
                        }
                }
        }
}
void open_thread(char* name, pthread_t* threads)
{
        int num_1;
        int j = 0;
        int per = 1;
        while (per != 0)                                                        //Поиск свободных идентификаторов
        {
                if (rez[j])
                {
                        str_glob[j].name_of_file = name;                        //В структуру закидывается имя файла
                        str_glob[j].loc_num = j;                                //И локальный номер потока
                        num_1 = j;
                        rez[str_glob[j].loc_num] = false;                       //Идентификатор занят
                        pthread_create(&(threads[str_glob[num_1].loc_num]), NULL, work_file, (&str_glob[num_1].loc_num));
                                                                                //Создание нового потока
                        per = 0;                                                //Завершение поиска свободного идентификатора
                }
                else
                {
                        j++;                                                    //Рассмотрим следующий идентификатор
                        if (j == 10)                                            //Всего 10 идентификаторов
                                j = 0;
                }
        }
}

void zap()
{
        for (int i = 0; i < 10; i++)
                rez[i] = true;                                          //Все идентифекаторы свободны
}

/*void time_of_work()
{
        double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;             //Расчёт времени работы с файлами
        printf("Time of search %f\n\n", time_spent);                            //Вывод времени

}


void kol_of_ans()
{
        if (kol_ans == 0)                                       //Проверка количества найденных файлов
                printf("Word is not found\n\n");
        else
                printf("Number of fils = %i\n\n", kol_ans);
}*/




