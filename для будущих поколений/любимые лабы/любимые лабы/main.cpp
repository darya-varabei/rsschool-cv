//
//  main.cpp
//  любимые лабы
//
//  Created by Дарья Воробей on 1/10/20.
//  Copyright © 2020 Дарья Воробей. All rights reserved.
//



//////////////////////////////////////////////////////////////////////////ЛАБА  3/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include <iomanip>
#include <iostream>

using namespace std;


double checkChoice(double* x){
    int t = 0;
    while (true)
    {
        cin >> *x;
        if ( cin.fail() )
        {
            cin.clear();
            cin.ignore(32767,'\n');
            cout << "Упс, некорректный ввод.Попробуйте ещё раз\n";
            t++;
            if( t == 3 )
            {
                cout<<"Татьяна Михайловна,не ломайте код, пожалуйста :)"<<endl;;
                t = 0;
                }
        }
        else
        {
            cin.ignore(32767,'\n');
  return *x;
        }
    }
}
 int checkChoiceInt(int* x){
     int t = 0;
     while (true)
     {
         cin>>*x;
         if ( cin.fail() )
         {
             cin.clear();
             cin.ignore(32767,'\n');
             cout << "Упс, некорректный ввод.Попробуйте ещё раз\n";
             t++;
             if( t == 3 )
             {
                 cout<<"Татьяна Михайловна,не ломайте код, пожалуйста :)"<<endl;;
                 t = 0;
                 }
         }
         else
         {
             cin.ignore(32767,'\n');
   return *x;
         }
     }
 }


int main()
{
    double a,b,h;
    int n;
    double rez, y, x, r, sum;
    cout << "Введите значения переменных" << endl;
   
    cout << "введите значение:a= ";
    checkChoice(&a);
 
    cout << "введите значение:b= ";
    checkChoice(&b);
    
    cout << "введите значение:h= ";
   checkChoice(&h);
    
    cout << "введите значение:n= ";
    checkChoiceInt(&n);
    for (x = a; x <= b; x += h) {
        sum = 0;
        for (int k = 1; k <= n; k++) {
            r = 0;
            r =pow(-1,k+1)*pow(x,2*k)/((2*k)*(2*k-1)) ;
            sum += r;
        }
        y = x * atan(x)-log(sqrt(1+pow(x,2)));
        rez = fabs(y - sum);
        cout << "x=" << x << setw(15) << "S(x)=" << sum << setw(15) << "Y(x)=" << y << setw(15) << "Y(x)-S(x)=" << rez << endl;
    }
    
    return 0;
}







//////////////////////////////////////////////////////////////////////////ЛАБА  4/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




#include <math.h>
#include <iomanip>
#include <iostream>

using namespace std;

void func(double, double );
void summ(int, double*, double);
void sub(double, double, double* );
void Function(double, double, double, int);
double checkChoice(double*,double, int);
int checkChoiceInt(int*, int, int);

int main()
{
    double a,b,h;
    int n;
    cout << "Введите значения переменных" << endl;
    cout << "введите значение:a= ";
    checkChoice(&a, 0, 32767);
    cout << "введите значение:b= ";
    checkChoice(&b, a, 32767);
    cout << "введите значение:h= ";
    checkChoice(&h,0, b);
    cout << "введите значение:n= ";
    checkChoiceInt(&n, 0, 32767);
    Function(a,b,h,n);
    return 0;
}

void summ( int n, double* sum, double x){
    double r = x*x/2;
    *sum = r;
    for (int k = 2; k <= n; k++) {
        r = r * (-1) * x * x / (2*k + 3) ;
        *sum += r;
    }
}

void func(double* y,double x){
     *y = x*atan(x)-log(sqrt(1+x*x));
}

void sub(double sum, double y, double* s){
    *s = y - sum;
}

void Function(double a, double b, double h, int n){
    double sum, y, x = 0 , s ;
    int f;
    while(1){
        cout<<"Выберите, что вы хотите расчитать:\n1.S(x)\n2.Y(x)\n3.|Y(x)-S(x)|\n4.Выход из программы\n";
        checkChoiceInt(&f, 1, 4);
        switch(f){
            case 1:
                for (x = a; x <= b; x += h) {
                    summ( n, &sum, x);
                    cout << setprecision(1) << fixed << "x=" << x << setw(25) <<setprecision(5) << fixed << "S(x)=" <<sum << "\t\n\n";
                }
                break;
            case 2:
                for (x = a; x <= b; x += h) {
                    func(&y, x);
                    cout << setprecision(1) << fixed << "x=" << x <<  setw(25) << "Y(x)="<< setprecision(5) << fixed <<  y << "\t\n\n" ;
                }
                break;
            case 3:
                for (x = a; x <= b; x += h) {
                    summ( n, &sum, x);
                    func(&y, x);
                    sub(sum, y ,&s);
                    cout  << setprecision(1) << fixed << "x=" << x << setw(25) << "|Y(x)-S(x)|=" << setprecision(5) << fabs(s) << "\t\n\n";
                }
                break;
            case 4:
                exit(0);
                break;
        }
    }
}
double checkChoice(double* x, double a, int b){
    int t = 0;
    while (true)
    {
        cin>>*x;
        if (( *x < a )||( *x > b )|| cin.fail())
        {
            cin.clear();
            cin.ignore(32767,'\n');
            cout << "Упс, некорректный ввод.Попробуйте ещё раз\n";
            t++;
            if( t == 3 )
            {
                cout<<"Уважаемый пользователь,не ломайте код, пожалуйста :)"<<endl;;
                t = 0;
            }
        }
        else
        {
            cin.ignore(32767,'\n');
            return *x;
        }
    }
}
int checkChoiceInt(int* x, int a, int b ){
    int t = 0;
    while (true)
    {
        cin>>*x;
        if ( ( *x < a )||( *x > b )|| cin.fail() )
        {
            cin.clear();
            cin.ignore(32767,'\n');
            cout << "Упс, некорректный ввод.Попробуйте ещё раз\n";
            t++;
            if( t == 3 )
            {
                cout<<"Уважаемый пользователь,не ломайте код, пожалуйста :)"<<endl;;
                t = 0;
            }
        }
        else
        {
            cin.ignore(32767,'\n');
            return *x;
        }
    }
}








//////////////////////////////////////////////////////////////////////////ЛАБА  5/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////







/*#include <iostream>
using namespace std;

int checkChoice(int*, int, int);
int main()
{
    int i = 0, sum = 0, pos1 = -1, pos2 = -2, num, rez;
    while(1){
    //int* arr = new int[num];
    srand(time(0));
    cout << "Введите размер массива: ";
    checkChoice(&num, 0, 32767);
   int* arr = new int[num];
     
        cout << "Массив создан" << endl;
    cout << "Заполнить\n 1.рандомом\n2.вручную\n3.Выход из программы\n ";
    checkChoice(&rez, 1, 3);
    switch (rez)
    {
        case 1:
            for (int i = 0; i < num; i++)
            {
                arr[i] = rand() % 100;
            }
            cout << "Массив заполнен" << endl;
            break;
            
        case 2:
            for (int i = 0; i < num; i++)
            {
                cout << "arr[" << i << "] = ";
                checkChoice(&arr[i], 0, 32767);// (arr+i);
            }
            cout << "Массив заполнен" << endl;
            break;
        case 3:
            exit(0);
            break;
    }
    for (int i = 0; i < num; i++)
    {
        cout << "arr[" << i << "] = " << arr[i] << endl;
    }
    cout << "Массив выведен" << endl;
    while (i < num)
    {
        if (arr[i] == 0){
            pos1 = i;
            break;
        }
        i++;
    }
    i=num-1;
    while ((i < num) && ( i >= 0 ))
    {
        if (arr[i] == 0){
            pos2 = i;
            break;
        }
        i--;
    }
    
    if((pos1 == num) || (pos2 < 0))
        cout << "Не найдено нулей" << endl;
    else
        if (pos1 == pos2)
            cout << "Одно нулевое значение" << endl;
        else
        {
            for (i = pos1; i <= pos2; i++)
                sum+= arr[i];
            cout << "Сумма между нулевыми значениями равна " << sum << "\n" << endl;
            
        }
        pos1 = -1; pos2 = -2;
        delete[] arr;
    }
    return 0;
}


int checkChoice(int* x, int a, int b)
{
    int t=0;
    while (true)
    {
        cin>>*x;
        if ((*x<a)||(*x>b)||cin.fail())
        {
            cin.clear();
            cin.ignore(32767,'\n');
            cout << "Упс, некорректный ввод.Попробуйте ещё раз\n";
            t++;
            if( t == 3 )
            {
                cout<<"Уважаемый пользователь,не ломайте код, пожалуйста :)"<<endl;;
                t = 0;
            }
        }
        else
        {
            cin.ignore(32767,'\n');
            return *x;
        }
    }
}*/







//////////////////////////////////////////////////////////////////////////ЛАБА  6/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////







/*#include <iostream>

using namespace std;

int checkChoice(int*, int, int);

int main(){
    while(1){
    int rez;
    int n , m , k=0 , t=0;
    cout<<"введите количество столбцов : "<<endl;
    checkChoice(&n, 1, 32767);
    cout<<"введите количество строк : "<<endl;
    checkChoice(&m, 1, 32767);
    cout << "Массив создан" << endl;
    int **a;
    a=new int *[n];
    for (int k=0; k<n; k++)
        a[k] = new int [m];
    cout << "Заполнить\n 1.рандомом\n2.вручную\n3.Выход из программы\n ";
    checkChoice(&rez, 1, 2);
    switch(rez) {
        case 1:
            cout<<"введенный массив"<<endl;
            srand((int)time(0));
            for (k= 0; k < n; k++) {
                for (t = 0; t < m; t++) {
                    a[k][t] = rand()%10;
                    cout << a[k][t] <<"\t";
                }
                cout<<endl;
            }
            break;
        case 2:
            cout << "введите массив:"<<endl;
            for (k = 0; k < n; k++){
                for (t = 0; t < m; t++){
                    cout << "a[" << k << "][" << t << "] =";
                    checkChoice(&a[k][t], 0, 32767);
                }
            }
            cout<<"введенный массив:"<<endl;
            for (k = 0; k < n; k++){
                for (t = 0; t < m; t++){
                    cout << a[k][t] << "\t";
                }
                cout<<endl;
            }
            break;
        case 3:
            exit(0);
            break;
    }
    int *b = new int[n];
    for (int i = 0; i < n; i++)
        b[i]=0;
    cout<<"Полученный массив: "<<endl;
    
    for (int i = 0; i < n; i++)
    {
        bool flag = true;
        for(int j = 1; j < m; j++)
        {
            if (a[i][j] >= a[i][j-1])
                flag = false;
            if (flag == false)
                break;
        }
        if (flag == true)
            b[i] = 1;
    }
    
    for (int i = 0; i < n; i++)
        cout << b[i] << " ";
    cout<<endl;
    delete[] a;
    delete[] b;
    }
    return 0;
}

int checkChoice(int* x, int a, int b)
{
    int t=0;
    while (true)
    {
        cin>>*x;
        if ((*x<a)||(*x>b)||cin.fail())
        {
            cin.clear();
            cin.ignore(32767,'\n');
            cout << "Упс, некорректный ввод.Попробуйте ещё раз\n";
            t++;
            if( t == 3 )
            {
                cout<<"Уважаемый пользователь,не ломайте код, пожалуйста :)"<<endl;;
                t = 0;
            }
        }
        else
        {
            cin.ignore(32767,'\n');
            return *x;
        }
    }
}*/








//////////////////////////////////////////////////////////////////////////ЛАБА  7/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////







/*#include <iostream>
#include <stdlib.h>

using namespace std;

int checkChoice(int*, int, int);
void Task(char* );
int my_strlen(char* );


int main(){
    while(1){
    int choice, t = 0, num = 0;
    char ctr[81]="";
    bool isValid = false;
    cout << "Выберите способ введения массива :\n1.Вручную\n 2.Тестовая строка" << endl;
    checkChoice(&choice, 1, 2);
    if(choice==1)
    {
        do{
            cout << "Введите строку, состоящую из нулей и единиц(без пробелов)" << endl;
            cin.getline(ctr, 80);
            char* str = &ctr[0];
            int len = my_strlen(str);
            for (int i = 0; i < len; i++)
            {
                if (str[i] == 48 || str[i] == 49)
                    num++;
            }
            if (num == len)
            {
                isValid = true;
            }
            else
            {
                isValid = false;
                num = 0;
                t++;
                if(t == 3)
                {
                    cout<<"Татьяна Михайловна, не надо ломать лабу)))"<<endl;
                    t = 0;
                }
            }
        } while (isValid == false);
        Task(&ctr[0]);
    }
    else if(choice==2)
    {
        char* str = (char*)"11111110000011110000000111110000000111110000000011111100000000";
        cout << "Тестовая строка: " << endl;
        cout << str << endl;
        Task(str);
    }
            char exi;
            cout<<"Нажмите 1 для выхода , для продолжения работы- любую другую клавишу"<<endl;
            cin>>exi;
            if(exi=='1'){
                cout<<"Выход выполнен успешно"<<endl;
                exit(0);
            }
        }
    return 0;
}

int checkChoice(int* x, int a, int b)
{
    int t=0;
    while (true)
    {
        cin>>*x;
        if ((*x<a)||(*x>b)||cin.fail())
        {
            cin.clear();
            cin.ignore(32767,'\n');
            cout << "Упс, некорректный ввод.Попробуйте ещё раз\n";
            t++;
            if( t == 3 )
            {
                cout<<"Татьяна Михайловна,не надо ломать лабу)))"<<endl;;
                t = 0;
            }
        }
        else
        {
            cin.ignore(32767,'\n');
            return *x;
        }
    }
}

void Task(char* str){
    int str_len = my_strlen (str);
    char* res = str;
    int num;
    while (*str) {
        num = 0;
        do {
            num++; str++;
        }
        while (*str == *(str- 1));
        
        if (num < str_len)
            res = str-(str_len = num);
    }
    cout<<"Самая короткая группа в строке: "<<endl;
    for (int i = 0; i< str_len; i++)
        cout << res[i];
    cout << endl;
}

int my_strlen(char* str)
{
    int length=0;
    while(*str++)
        ++length;
    return length;
}*/









//////////////////////////////////////////////////////////////////////////ЛАБА  8/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






/*#include <iostream>
#include <cctype>
#include <cstring>
#include <cstdlib>
#include <fstream>

using namespace std;

typedef struct
{
    char name[30];
    int year;
    long int group;
    int phys, chem, math, inf;
    float ave;
}Tstudent;

void create();
void add(int*, Tstudent*);
void ind(int*,Tstudent*);
void clean(int*,Tstudent*);
void correct(int*,Tstudent*);
void display(int*, Tstudent*);
int  checkChoice(int*, int, int);
void Out();
void ChooseDel(int*,Tstudent* );
void addCorrect(int*, Tstudent*);
long int checkGroup(long int* );
void Swap(char*);
void inFile(int*,Tstudent*);
int my_strlen(char* );

int main()
{
    ifstream fin("/Users/dariavarabei/Desktop/Data.txt", ios_base::in);
    int count=0 ,choice, t;
    Tstudent *stud;
    stud = new Tstudent[count];
    if(fin.is_open()){
        cout<<"Считать начальную информацию из файла Data.txt\n1.Да\n2.Нет"<<endl;
        checkChoice( &t, 1, 2 );
        if(t==1)
        {
    for (int i = 0;fin>>stud[i].name >> stud[i].year>> stud[i].group>> stud[i].math>> stud[i].phys>>stud[i].inf >>stud[i].chem;i++)
    {
         stud[i].ave = (double)(stud[i].phys + stud[i].math + stud[i].inf + stud[i].chem) / 4 ;
        count++;
    }
        if(count==0)
            cout<<"Информации в файле Data.txt не обнаружено"<<endl;
        }
    }
    else
        cout<<"Не удалось открыть файл"<<endl;
      while (1)
           {
               cout << "Выберите действие" << endl;
                                   cout << "1.Создать новый файл" << endl;
                                   cout << "2.Добавить информацию о студенте" << endl;
                                   cout << "3.Вывести данные в консоль" << endl;
                                   cout << "4.Редактировать данные" << endl;
                                   cout << "5.Удалить данные" << endl;
                                   cout << "6.Вывести результат индивидуального задания" << endl;
                                   cout << "7.Выход из программы" << endl;
               checkChoice( &choice, 1, 7 );
               switch ( choice )
                      {
                          case 1:
                              create();
                              break;
                          case 2:
                              add(&count, stud);
                              count++;
                              inFile(&count, stud);
                              break;
                          case 3:
                              display(&count, stud);
                              break;
                          case 4:
                              display(&count, stud);
                              correct(&count, stud);
                               inFile(&count, stud);
                              break;
                          case 5:
                              display(&count, stud);
                             ChooseDel(&count, stud);
                               inFile(&count, stud);
                              break;
                          case 6:
                              ind(&count, stud);
                              break;
                          case 7:
                              Out();
                              break;
                      }
           }
    delete []stud;
    fin.close();
}

void create(){
    ifstream fin("/Users/dariavarabei/Desktop/FileIn.txt", ios_base::in);
    if(fin.is_open())
    {
         cout<<"Файл успешно создан!"<<endl;
        fin.close();
    }
    else
         cout<<"Ошибка в создании файла!"<<endl;
}

void add(int *number, Tstudent *stud){
    cout << "\n\tВведите Фамилию и инициалы: ";
    cin.getline(stud[ *number ].name, 30);
    Swap(&stud[ *number ].name[0]);
        
    cout << "\n\tВведите год рождения: ";
    checkChoice(&stud[ *number ].year, 1990, 2002);
        
    cout << "\n\tВведите номер группы: ";
    checkGroup(&stud[ *number ].group);
    
    cout << "\n\tОценка по физике: ";
    checkChoice(&stud[ *number ].phys, 1, 10);
        
    cout << "\n\tОценка по математике: ";
     checkChoice(&stud[ *number ].math, 1, 10);
        
    cout << "\n\tОценка по информатике: ";
    checkChoice(&stud[ *number ].inf, 1, 10);
        
    cout << "\n\tОценка по химии: ";
    checkChoice(&stud[ *number ].chem, 1, 10);
    
    stud[ *number ].ave = (double)(stud[ *number ].phys + stud[ *number ].math + stud[ *number ].inf + stud[ *number ].chem) / 4;
    }
 
 long int checkGroup(long int* x){
     int t = 0;
     while (true)
     {
          cin>>*x;
         if ((*x < 011111 )||(*x > 999999 ))
         {
             cin.clear();
             cin.ignore(32767,'\n');
             cout << "Oops, that input is invalid.  Please try again.\n";
             t++;
             if( t == 3 )
             {
                 cout<<"Татьяна Михайловна,не ломайте код, пожалуйста :)"<<endl;;
                 t = 0;
                 }
         }
         else
         {
             cin.ignore(32767,'\n');
             return *x;
         }
     }
 }

 int checkChoice(int* x, int a, int b){
     int t = 0;
     while (true)
     {
         cin>>*x;
         if (( *x < a )||( *x > b )|| cin.fail() )
         {
             cin.clear();
             cin.ignore(32767,'\n');
             cout << "Упс, некорректный ввод.Попробуйте ещё раз\n";
             t++;
             if( t == 3 )
             {
                 cout<<"Татьяна Михайловна,не ломайте код, пожалуйста :)\n";;
                 t = 0;
                 }
         }
         else
         {
             cin.ignore(32767,'\n');
   return *x;
         }
     }
 }

void Swap(char* first){
    if( *first > 90 )
    {
       *first = *first - 32;
    }
    else
    *first = *first;
}

void display(int* number, Tstudent *stud){
    int i = 0;
    cout<< "\n     Name               Date     Group     Physics   Maths  Informatics  Chemistry  Average"<<endl;
    for( i = 0 ; i < *number ; i++)
     {
          int name_len=my_strlen(stud[i].name);
         cout << (i + 1) <<". "<< stud[i].name << setw(25-name_len) << stud[i].year << setw(10) << stud[i].group <<setw(10) << stud[i].phys << setw(10)<< stud[i].math << setw(10) << stud[i].inf << setw(10) << stud[i].chem << setw(10) << stud[i].ave<< "\n";
       }
}


void correct(int* number, Tstudent *stud){
        int num_del = 0, rez, i;
        cout<<"Введите номер студента, данные которого вы хотите откорректировать:";
         checkChoice(&num_del, 1, 32767);
    cout<<"Вы уверены,что хотите откорректировать данные студента № "<<num_del<<"?\n1.Да\n2.Нет"<<endl;
    num_del--;
    checkChoice(&rez, 1, 2);
    switch(rez){
        case 1:
        for( i = 0 ; i < *number ; i++)
        {
        if( num_del == i )
        {
            addCorrect(&num_del,stud);
            num_del++;
            cout<<"Данные студента № "<<num_del<<" откорректированы"<<endl;
            break;
        }
    }
            break;
        case 2:
            cout<<"Данные остались в неизменном состоянии"<<endl;
            break;
    }
}

void clean(int* number,Tstudent *stud){
     int del;
     cout<<"Вы уверены,что хотите удалить информацию из файла?\n1.Нет\n2.Да\n"<<endl;
      checkChoice(&del, 1, 2);
     switch(del)
        {
          case 1:
                cout<<"Файл остался в неизменном состоянии"<<endl;
            break;
          case 2:
                delete []stud;
                 ofstream fout("/Users/dariavarabei/Desktop/FileIn.txt", ios::out);
                *number=0;
               // fstream fileStrm("FileIn.txt", ios_base::trunc);
                 cout<<"Информация успешно удалена из файла и структуры"<<endl;
                fout.close();
        break;
    }
}

void ind(int* number, Tstudent *stud){
    ofstream fout("/Users/dariavarabei/Desktop/Task.txt", ios::out);
    char letter;
    int rez = 0;
    if(fout.is_open())
    {
    cout << "Введите нужную вам букву:" << endl;
    cin >> letter;
        Swap(&letter);
         cout<< "\n     Name            Date     Group     Physics   Maths  Informatics  Chemistry  Average"<<endl;
         fout<< "\n     Name            Date     Group     Physics   Maths  Informatics  Chemistry  Average"<<endl;
    for(int i = 0 ; i < *number ; i++)
    {
        int name_len=my_strlen(stud[i].name);
        if((stud[i].name[0] == letter)&&(stud[i].phys >= 9)&&(stud[i].math >= 9)&&(stud[i].inf >= 9)&&(stud[i].chem >= 9))
        {
            rez=1;
                       cout << stud[i].name << setw(25-name_len) << stud[i].year << setw(10) << stud[i].group << setw(10) << stud[i].phys << setw(10) << stud[i].math << setw(10) << stud[i].inf << setw(10) << stud[i].chem << setw(10) << stud[i].ave<< "\n";
             fout << stud[i].name <<setw(25-name_len) << stud[i].year <<setw(10) << stud[i].group << setw(10) <<stud[i].phys <<setw(10) << stud[i].math <<setw(10) << stud[i].inf<<setw(10) << stud[i].chem <<setw(10) <<stud[i].ave<<"\n";
        }
        if((i==*number-1)&&( rez == 0 ))
        {
               cout<<"Не найдено отличников с фамилиями на данную букву"<<endl;
            break;
        }
    }
    }
    fout.close();
}

void Out(){
    cout<<"Вы уверены, что хотите выйти из программы?\n1.Да\n 2.Нет"<<endl;
        int ex;
     checkChoice(&ex, 1, 2);
        switch(ex)
        {
            case 1:
                cout<<"Выход выполнен успешно"<<endl;
                exit(0);
            case 2:
                break;
            }
}

void addCorrect(int *number, Tstudent *stud){
    
    cout << "\n\tВведите год рождения: ";
    checkChoice(&stud[*number].year, 1990, 2002);
        
    cout << "\n\tВведите номер группы: ";
    checkGroup(&stud[*number].group);
        
    cout << "\n\tОценка по физике: ";
    checkChoice(&stud[*number].phys, 1, 10);
    
    cout << "\n\tОценка по математике: ";
    checkChoice(&stud[*number].math, 1, 10);
    
    cout << "\n\tОценка по информатике: ";
    checkChoice(&stud[*number].inf, 1, 10);
    
    cout << "\n\tОценка по химии: ";
    checkChoice(&stud[*number].chem, 1, 10);
    
    stud[*number].ave = (double)(stud[*number].phys + stud[*number].math + stud[*number].inf + stud[*number].chem) / 4;
}

void  ChooseDel(int* number, Tstudent *stud){
      
    cout<<"Выберите, что именно вы хотите удалить:\n1.Одного студента\n2.Всю информацию"<<endl;
    int choice, num, rez;
    checkChoice(&choice, 1, 2);
    switch(choice)
    {
        case 1:
            cout<<"Введите номер студента , данные которого вы хотите удалить"<<endl;
            checkChoice(&num, 1, 32767);
                   num--;
            cout<<"Вы уверены , что хотите удалить студента из файла?\n1.Да\n2.Нет"<<endl;
            checkChoice(&rez, 1, 2);
            switch(rez)
            {
                case 1:
                   for( int i = 0 ; i < *number ; i++)
                   {
                       for( i = num ; i < *number ; i++)
                       {
                           stud[i] = stud[i+1];
                       }
                       *number = *number-1;
                   }
                    num++;
                    cout<<"Студент № "<<num<<" удален из файла"<<endl;
                    break;
                case 2:
                    cout<<"Данные остались в неизменном состоянии"<<endl;
                    break;
            }
            break;
        case 2:
            clean(&*number, stud);
            break;
        }
}
   
void inFile(int *number,Tstudent *stud){
    //fstream clear_file("/Users/dariavarabei/Desktop/FileIn.txt", ios::out);
    //clear_file.close();

    ofstream fout("/Users/dariavarabei/Desktop/FileIn.txt", ios::out);
    int rez;
    if(fout.is_open())
    {
    cout<<"Желаете вывести данные в файл?\n1.Да\n2.Нет"<<endl;
    checkChoice(&rez, 1, 2);
    switch(rez){
        case 1:
            fout<< "\n     Name             Date    Group     Physics  Maths  Informatics  Chemistry   Average"<<endl;
            for(int i=0; i<*number; i++)
            {
                int name_len=my_strlen(stud[i].name);
                fout << i + 1 <<". "<< stud[i].name<< setw(25-name_len)<< stud[i].year << setw(10) << stud[i].group << setw(10) << stud[i].phys << setw(10) << stud[i].math << setw(10) << stud[i].inf << setw(10) << stud[i].chem << setw(10) << stud[i].ave<< "\n";
            }
            cout<<"Действие выполнено успешно!Данные сохранены в файл с именем FileIn.txt"<<endl;
            fout.close();
            break;
        case 2:
            cout<<"Данные не сохранены в файле"<<endl;
            break;
    }
        }
    else
    cout<<"Не удалось открыть файл"<<endl;
}

int my_strlen(char* str)
{
    int length=0;
    while(*str++)
        ++length;
    return length;
}*/


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/*#include <iostream>
#include <cctype>
#include <cstring>
#include <cstdlib>
#include <fstream>

using namespace std;

typedef struct
{
    char name[30];
    int year;
    long int group;
    int phys, chem, math, inf;
    float ave;
}Tstudent;
Tstudent *stud;

void create();
void add();
void ind();
void clean(int*,Tstudent*);
void correct();
void display();
int checkChoice(int*, int, int);
void Out();
void ChooseDel();
void addCorrect(int*,Tstudent*);
long int checkGroup(long int* );
void Swap(char*);
int my_strlen(char* );

int main()
{
    int choice;
    while (1)
    {
        cout << "Выберите действие" << endl;
        cout << "1.Создать новый файл" << endl;
        cout << "2.Добавить информацию о студенте" << endl;
        cout << "3.Вывести данные в консоль" << endl;
        cout << "4.Редактировать данные" << endl;
        cout << "5.Удалить данные" << endl;
        cout << "6.Вывести результат индивидуального задания" << endl;
        cout << "7.Выход из программы" << endl;
        checkChoice( &choice, 1, 7 );
        switch ( choice )
        {
            case 1:
                create();
                break;
            case 2:
                add();
                break;
            case 3:
                display();
                break;
            case 4:
                display();
                correct();
                break;
            case 5:
                display();
                ChooseDel();
                break;
            case 6:
                ind();
                break;
            case 7:
                Out();
                break;
        }
    }
}

void create(){
    ifstream fin("/Users/dariavarabei/Desktop/MyFile.txt", ios_base::in);
    if(fin.is_open())
    {
        cout<<"Файл успешно создан!"<<endl;
        fin.close();
    }
    else
        cout<<"Ошибка в создании файла!"<<endl;
}

void add(){
    ofstream fadd("/Users/dariavarabei/Desktop/FileIn.txt", ios::app);
    
    if(fadd.is_open()){
        cout << "\n\tВведите Фамилию и инициалы через пробелы без точек: ";
        cin.getline(stud->name, 29, '\n');
        Swap(&stud->name[0]);
        int name_len=my_strlen(stud->name);
        for(int i=0; i<name_len; i++)
        {
            if(stud->name[i]==32)
            {
                stud->name[i]=46;
                Swap(&stud->name[i+1]);
            }
        }
        cout << "\n\tВведите год рождения: ";
        checkChoice(&stud->year, 1990, 2002);
        
        cout << "\n\tВведите номер группы: ";
        checkGroup(&stud->group);
        
        cout << "\n\tОценка по физике: ";
        checkChoice(&stud->phys, 1, 10);
        
        cout << "\n\tОценка по математике: ";
        checkChoice(&stud->math, 1, 10);
        
        cout << "\n\tОценка по информатике: ";
        checkChoice(&stud->inf, 1, 10);
        
        cout << "\n\tОценка по химии: ";
        checkChoice(&stud->chem, 1, 10);
        
        stud->ave = (double)(stud->phys + stud->math + stud->inf + stud->chem) / 4;
        fadd <<stud->name<< setw(25-name_len)<< stud->year << setw(10) << stud->group << setw(10) << stud->phys << setw(10) << stud->math << setw(10) << stud->inf << setw(10) << stud->chem << setw(10) << stud->ave<< "\n";
    }
    fadd.close();
}

long int checkGroup(long int* x){
    int t = 0;
    while (true)
    {
        cin>>*x;
        if ((*x < 10000 )||(*x > 999999 ))
        {
            cin.clear();
            cin.ignore(32767,'\n');
            cout << "Oops, that input is invalid.  Please try again.\n";
            t++;
            if( t == 3 )
            {
                cout<<"Татьяна Михайловна,не ломайте код, пожалуйста :)"<<endl;;
                t = 0;
            }
        }
        else
        {
            cin.ignore(32767,'\n');
            return *x;
        }
    }
}

int checkChoice(int* x, int a, int b){
    int t = 0;
    while (true)
    {
        cin>>*x;
        if (( *x < a )||( *x > b )|| cin.fail() )
        {
            cin.clear();
            cin.ignore(32767,'\n');
            cout << "Упс, некорректный ввод.Попробуйте ещё раз\n";
            t++;
            if( t == 3 )
            {
                cout<<"Татьяна Михайловна,не ломайте код, пожалуйста :)\n";;
                t = 0;
            }
        }
        else
        {
            cin.ignore(32767,'\n');
            return *x;
        }
    }
}

void Swap(char* first){
    if( *first > 90 )
    {
        *first = *first - 32;
    }
    else
        *first = *first;
}

void display(){
    ifstream fin("/Users/dariavarabei/Desktop/FileIn.txt", ios_base::in);
    int count=0;
    stud = new Tstudent[count];
    if(fin.is_open())
    {
        for (int i = 0;fin>>stud[i].name >> stud[i].year>> stud[i].group>> stud[i].math>> stud[i].phys>>stud[i].inf >>stud[i].chem>>stud[i].ave ;i++)
        {
            count++;
        }
        if(count==0)
            cout<<"Информации в файле FileIn.txt не обнаружено"<<endl;
    }
    else
        cout<<"Не удалось открыть файл"<<endl;
    cout<< "\n     Name               Date     Group     Physics   Maths  Informatics  Chemistry  Average"<<endl;
    for( int i = 0 ; i < count ; i++)
    {
        int name_len=my_strlen(stud[i].name);
        cout << (i + 1) <<". "<< stud[i].name << setw(25-name_len) << stud[i].year << setw(10) << stud[i].group <<setw(10) << stud[i].phys << setw(10)<< stud[i].math << setw(10) << stud[i].inf << setw(10) << stud[i].chem << setw(10) << stud[i].ave<< "\n";
    }
    fin.close();
    delete[] stud;
}



void correct(){
    int num_del = 0, rez,count=0 ;
    stud = new Tstudent[count];
    ifstream fin("/Users/dariavarabei/Desktop/FileIn.txt", ios_base::in);
    if(fin.is_open()){
        for (int i = 0;fin>>stud[i].name >> stud[i].year>> stud[i].group>> stud[i].math>> stud[i].phys>>stud[i].inf >>stud[i].chem>>stud[i].ave ;i++)
        {
            count++;
        }
        if(count==0)
            cout<<"Информации в файле FileIn.txt не обнаружено"<<endl;
    }
    else
        cout<<"Не удалось открыть файл"<<endl;
    fin.close();
    ofstream fout("/Users/dariavarabei/Desktop/FileIn.txt", ios::out);
    cout<<"Введите номер студента, данные которого вы хотите откорректировать:";
    checkChoice(&num_del, 1, 32767);
    cout<<"Вы уверены,что хотите откорректировать данные студента № "<<num_del<<"?\n1.Да\n2.Нет"<<endl;
    num_del--;
    checkChoice(&rez, 1, 2);
    switch(rez){
        case 1:
            for(int i = 0 ; i < count ; i++)
            {
                if( num_del == i )
                {
                    addCorrect(&num_del,stud);
                    num_del++;
                    cout<<"Данные студента № "<<num_del<<" откорректированы"<<endl;
                    break;
                }
            }
            for(int i = 0 ; i < count ; i++)
            {
                int name_len=my_strlen(stud[i].name);
                fout << stud[i].name <<setw(25-name_len) << stud[i].year <<setw(10) << stud[i].group << setw(10) <<stud[i].phys <<setw(10) << stud[i].math <<setw(10) << stud[i].inf<<setw(10) << stud[i].chem <<setw(10) <<stud[i].ave<<"\n";
            }
            break;
        case 2:
            for(int i = 0 ; i < count ; i++)
            {
                int name_len=my_strlen(stud[i].name);
                fout << stud[i].name <<setw(25-name_len) << stud[i].year <<setw(10) << stud[i].group << setw(10) <<stud[i].phys <<setw(10) << stud[i].math <<setw(10) << stud[i].inf<<setw(10) << stud[i].chem <<setw(10) <<stud[i].ave<<"\n";
            }
            cout<<"Данные остались в неизменном состоянии"<<endl;
            break;
    }
    fout.close();
    delete[] stud;
}


void clean(int* number,Tstudent *stud){
    int del;
    cout<<"Вы уверены,что хотите удалить информацию из файла?\n1.Нет\n2.Да\n"<<endl;
    checkChoice(&del, 1, 2);
    switch(del)
    {
        case 1:
            cout<<"Файл остался в неизменном состоянии"<<endl;
            break;
        case 2:
            delete []stud;
            ofstream fout("/Users/dariavarabei/Desktop/FileIn.txt", ios::out);
            cout<<"Информация успешно удалена из файла и структуры"<<endl;
            fout.close();
            break;
    }
}

void ind(){
    ofstream fout("/Users/dariavarabei/Desktop/Task.txt", ios::out);
    char letter;
    int rez = 0,count=0;
    ifstream fin("/Users/dariavarabei/Desktop/FileIn.txt", ios_base::in);
    stud = new Tstudent[count];
    if(fin.is_open())
    {
        for (int i = 0;fin>>stud[i].name >> stud[i].year>> stud[i].group>> stud[i].math>> stud[i].phys>>stud[i].inf >>stud[i].chem>>stud[i].ave ;i++)
        {
            count++;
        }
        if(count==0)
            cout<<"Информации в файле FileIn.txt не обнаружено"<<endl;
    }
    else
        cout<<"Не удалось открыть файл"<<endl;
    fin.close();
    if(fout.is_open())
    {
        cout << "Введите нужную вам букву:" << endl;
        cin >> letter;
        Swap(&letter);
        cout<< "\n     Name            Date     Group     Physics   Maths  Informatics  Chemistry  Average"<<endl;
        fout<< "\n     Name            Date     Group     Physics   Maths  Informatics  Chemistry  Average"<<endl;
        for(int i = 0 ; i < count ; i++)
        {
            int name_len=my_strlen(stud[i].name);
            if((stud[i].name[0] == letter)&&(stud[i].phys >= 9)&&(stud[i].math >= 9)&&(stud[i].inf >= 9)&&(stud[i].chem >= 9))
            {
                rez=1;
                cout << stud[i].name << setw(25-name_len) << stud[i].year << setw(10) << stud[i].group << setw(10) << stud[i].phys << setw(10) << stud[i].math << setw(10) << stud[i].inf << setw(10) << stud[i].chem << setw(10) << stud[i].ave<< "\n";
                fout << stud[i].name <<setw(25-name_len) << stud[i].year <<setw(10) << stud[i].group << setw(10) <<stud[i].phys <<setw(10) << stud[i].math <<setw(10) << stud[i].inf<<setw(10) << stud[i].chem <<setw(10) <<stud[i].ave<<"\n";
            }
            if((i==count-1)&&( rez == 0 ))
            {
                cout<<"Не найдено отличников с фамилиями на данную букву"<<endl;
                break;
            }
        }
    }
    fout.close();
    delete[] stud;
}

void Out(){
    cout<<"Вы уверены, что хотите выйти из программы?\n1.Да\n 2.Нет"<<endl;
    int ex;
    checkChoice(&ex, 1, 2);
    switch(ex)
    {
        case 1:
            cout<<"Выход выполнен успешно"<<endl;
            exit(0);
        case 2:
            break;
    }
}

void addCorrect(int *number, Tstudent *stud){
    
    cout << "\n\tВведите год рождения: ";
    checkChoice(&stud[*number].year, 1990, 2002);
    
    cout << "\n\tВведите номер группы: ";
    checkGroup(&stud[*number].group);
    
    cout << "\n\tОценка по физике: ";
    checkChoice(&stud[*number].phys, 1, 10);
    
    cout << "\n\tОценка по математике: ";
    checkChoice(&stud[*number].math, 1, 10);
    
    cout << "\n\tОценка по информатике: ";
    checkChoice(&stud[*number].inf, 1, 10);
    
    cout << "\n\tОценка по химии: ";
    checkChoice(&stud[*number].chem, 1, 10);
    
    stud[*number].ave = (double)(stud[*number].phys + stud[*number].math + stud[*number].inf + stud[*number].chem) / 4;
}

void  ChooseDel(){
    
    int count=0,choice, num, rez;
    stud = new Tstudent[count];
    ifstream fin("/Users/dariavarabei/Desktop/FileIn.txt", ios_base::in);
    if(fin.is_open()){
        for (int i = 0;fin>>stud[i].name >> stud[i].year>> stud[i].group>> stud[i].math>> stud[i].phys>>stud[i].inf >>stud[i].chem>>stud[i].ave ;i++)
        {
            count++;
        }
        if(count==0)
            cout<<"Информации в файле FileIn.txt не обнаружено"<<endl;
    }
    else
        cout<<"Не удалось открыть файл"<<endl;
    fin.close();
    ofstream fout("/Users/dariavarabei/Desktop/FileIn.txt", ios::out);
    cout<<"Выберите, что именно вы хотите удалить:\n1.Одного студента\n2.Всю информацию"<<endl;
    checkChoice(&choice, 1, 2);
    switch(choice)
    {
        case 1:
            cout<<"Введите номер студента , данные которого вы хотите удалить"<<endl;
            checkChoice(&num, 1, 32767);
            num--;
            if(num>count){
               cout<<"Данного студента нет в файле"<<endl;
                for(int i = 0 ; i < count ; i++)
                {
                    int name_len=my_strlen(stud[i].name);
                    fout << stud[i].name <<setw(25-name_len) << stud[i].year <<setw(10) << stud[i].group << setw(10) <<stud[i].phys <<setw(10) << stud[i].math <<setw(10) << stud[i].inf<<setw(10) << stud[i].chem <<setw(10) <<stud[i].ave<<"\n";
                }
            break;
            }
            else{
            cout<<"Вы уверены , что хотите удалить студента из файла?\n1.Да\n2.Нет"<<endl;
            checkChoice(&rez, 1, 2);
            switch(rez)
            {
                case 1:
                    for( int i = 0 ; i < count ; i++)
                    {
                        for( i = num ; i < count ; i++)
                        {
                            stud[i] = stud[i+1];
                        }
                        count = count-1;
                    }
                    num++;
                    cout<<"Студент № "<<num<<" удален из файла"<<endl;
                    for(int i = 0 ; i < count ; i++)
                    {
                        int name_len=my_strlen(stud[i].name);
                        fout << stud[i].name <<setw(25-name_len) << stud[i].year <<setw(10) << stud[i].group << setw(10) <<stud[i].phys <<setw(10) << stud[i].math <<setw(10) << stud[i].inf<<setw(10) << stud[i].chem <<setw(10) <<stud[i].ave<<"\n";
                    }
                    break;
                case 2:
                    for(int i = 0 ; i < count ; i++)
                    {
                        int name_len=my_strlen(stud[i].name);
                        fout << stud[i].name <<setw(25-name_len) << stud[i].year <<setw(10) << stud[i].group << setw(10) <<stud[i].phys <<setw(10) << stud[i].math <<setw(10) << stud[i].inf<<setw(10) << stud[i].chem <<setw(10) <<stud[i].ave<<"\n";
                    }
                    cout<<"Данные остались в неизменном состоянии"<<endl;
                    break;
            }
            delete[] stud;
            fout.close();
            break;
            }
        case 2:
            clean(&count, stud);
            break;
    }
}

int my_strlen(char* str)
{
    int length=0;
    while(*str++)
        ++length;
    return length;
}*/


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





/*#include <iostream>
#include <cctype>
#include <cstring>
#include <cstdlib>
#include <fstream>


using namespace std;

typedef struct
{
    char name[30];
    int year;
    char group[9];
    int phys, chem, math, inf;
    float ave;
}Tstudent;
Tstudent *stud;

void create();
void add();
void ind();
void correct();
void display();
int checkChoice(int*, int, int);
void Out();
void ChooseDel();
void addCorrect(int*,Tstudent*);
void Swap(char*);
int my_strlen(char* );

int main()
{
    int choice;
    while (1)
    {
        cout << "Выберите действие" << endl;
        cout << "1.Создать новый файл" << endl;
        cout << "2.Добавить информацию о студенте" << endl;
        cout << "3.Вывести данные в консоль" << endl;
        cout << "4.Редактировать данные" << endl;
        cout << "5.Удалить данные" << endl;
        cout << "6.Вывести результат индивидуального задания" << endl;
        cout << "7.Выход из программы" << endl;
        checkChoice( &choice, 1, 7 );
        switch ( choice )
        {
            case 1:
                create();
                break;
            case 2:
                add();
                break;
            case 3:
                display();
                break;
            case 4:
                display();
                correct();
                break;
            case 5:
                display();
                ChooseDel();
                break;
            case 6:
                ind();
                break;
            case 7:
                Out();
                break;
        }
    }
}

void create(){
    ifstream fin("/Users/dariavarabei/Desktop/FileIn.txt", ios_base::in);
    if(fin.is_open())
    {
        cout<<"Файл успешно создан!"<<endl;
        fin.close();
    }
    else
        cout<<"Ошибка в создании файла!"<<endl;
}

void add(){
    
    ofstream fadd("/Users/dariavarabei/Desktop/FileIn.txt", ios::app);
    if(fadd.is_open()){
        cout << "\n\tВведите Фамилию и инициалы через пробелы без точек: ";
        cin.getline(stud->name,30);
        Swap(&stud->name[0]);
        int name_len=my_strlen(stud->name);
        for(int i=0; i<name_len; i++)
        {
            if(stud->name[i]==32)
            {
                stud->name[i]=46;
                Swap(&stud->name[i+1]);
            }
        }
        cout << "\n\tВведите год рождения: ";
        checkChoice(&stud->year, 0, 2002);
        
        cout << "\n\tВведите номер группы: ";
        cin.getline(stud->group,9);
        
        cout << "\n\tОценка по физике: ";
        checkChoice(&stud->phys, 1, 10);
        
        cout << "\n\tОценка по математике: ";
        checkChoice(&stud->math, 1, 10);
        
        cout << "\n\tОценка по информатике: ";
        checkChoice(&stud->inf, 1, 10);
        
        cout << "\n\tОценка по химии: ";
        checkChoice(&stud->chem, 1, 10);
        
        stud->ave = (double)(stud->phys + stud->math + stud->inf + stud->chem) / 4;
        fadd <<stud->name<< setw(25-name_len)<< stud->year << setw(10) << stud->group << setw(10) << stud->phys << setw(10) << stud->math << setw(10) << stud->inf << setw(10) << stud->chem << setw(10) << stud->ave<< "\n";
    }
    fadd.close();
}


int checkChoice(int* x, int a, int b){
    int t = 0;
    while (true)
    {
        cin>>*x;
        if (( *x < a )||( *x > b )|| cin.fail() )
        {
            cin.clear();
            cin.ignore(32767,'\n');
            cout << "Упс, некорректный ввод.Попробуйте ещё раз\n";
            t++;
            if( t == 3 )
            {
                cout<<"Уважаемый пользователь,не ломайте код, пожалуйста :)\n";;
                t = 0;
            }
        }
        else
        {
            cin.ignore(32767,'\n');
            return *x;
        }
    }
}

void Swap(char* first){
    if( *first > 90 )
    {
        *first = *first - 32;
    }
    else
        *first = *first;
}

void display(){
    ifstream fin("/Users/dariavarabei/Desktop/FileIn.txt", ios_base::in);
    int count=0;
    char elem[100];
    cout<< "\n     Name               Date     Group     Physics   Maths  Informatics  Chemistry  Average"<<endl;
    if(fin.is_open())
    {
    while(fin.getline(elem, 100)){
        cout<<count+1<<". "<<elem<<endl;
        count++;
    }
        if(count==0)
        cout<<"Информации в файле FileIn.txt не обнаружено"<<endl;
    }
    else
           cout<<"Не удалось открыть файл"<<endl;
    fin.close();
}

void correct(){
    int num_del = 0, rez,count=0 ;
    stud = new Tstudent[count];
    ifstream fin("/Users/dariavarabei/Desktop/FileIn.txt", ios_base::in);
    if(fin.is_open()){
        for (int i = 0;fin>>stud[i].name >> stud[i].year>> stud[i].group>> stud[i].math>> stud[i].phys>>stud[i].inf >>stud[i].chem>>stud[i].ave ;i++)
        {
            count++;
        }
        if(count==0)
            cout<<"Информации в файле FileIn.txt не обнаружено"<<endl;
    }
    else
        cout<<"Не удалось открыть файл"<<endl;
    fin.close();
    ofstream fout("/Users/dariavarabei/Desktop/FileIn.txt", ios::out);
    cout<<"Введите номер студента, данные которого вы хотите откорректировать:";
    checkChoice(&num_del, 1, count);
    cout<<"Вы уверены,что хотите откорректировать данные студента № "<<num_del<<"?\n1.Да\n2.Нет"<<endl;
    checkChoice(&rez, 1, 2);
    switch(rez){
        case 1:
            for(int i = 0 ; i < count ; i++)
            {
                if( num_del-1 == i )
                {
                    num_del--;
                    addCorrect(&num_del,stud);
                    num_del++;
                    cout<<"Данные студента № "<<num_del<<" откорректированы"<<endl;
                    break;
                }
            }
            for(int i = 0 ; i < count ; i++)
            {
                int name_len=my_strlen(stud[i].name);
                fout << stud[i].name <<setw(25-name_len) << stud[i].year <<setw(10) << stud[i].group << setw(10) <<stud[i].phys <<setw(10) << stud[i].math <<setw(10) << stud[i].inf<<setw(10) << stud[i].chem <<setw(10) <<stud[i].ave<<"\n";
            }
            break;
        case 2:
            for(int i = 0 ; i < count ; i++)
            {
                int name_len=my_strlen(stud[i].name);
                fout << stud[i].name <<setw(25-name_len) << stud[i].year <<setw(10) << stud[i].group << setw(10) <<stud[i].phys <<setw(10) << stud[i].math <<setw(10) << stud[i].inf<<setw(10) << stud[i].chem <<setw(10) <<stud[i].ave<<"\n";
            }
            cout<<"Данные остались в неизменном состоянии"<<endl;
            break;
    }
    fout.close();
    delete[] stud;
}

void ind(){
    ofstream fout("/Users/dariavarabei/Desktop/Task.txt", ios::out);
    ifstream fin("/Users/dariavarabei/Desktop/FileIn.txt", ios_base::in);
    char letter;
    int rez = 0,count=0;
    char task[100];
    if(fin.is_open())
    {
        if(fout.is_open())
        {
            cout << "Введите нужную вам букву:" << endl;
            cin >> letter;
            Swap(&letter);
            cout<< "\n     Name            Date     Group     Physics   Maths  Informatics  Chemistry  Average"<<endl;
            fout<< "\n     Name            Date     Group     Physics   Maths  Informatics  Chemistry  Average"<<endl;
            while(fin.getline(task, 100)){
                if(task[0]==letter&&(task[44]==57||task[43]==49)&&(task[54]==57||task[43]==49)&&(task[64]==57||task[43]==49)&&(task[74]==57||task[43]==49)){
                    cout<<task<<endl;
                    fout<<task<<endl;
                    rez=1;
                }
                count++;
            }
            if(rez==0){
                cout<<"Не найдено отличников с фамилиями на данную букву"<<endl;
            }
            if(count==0)
                cout<<"Информации в файле FileIn.txt не обнаружено"<<endl;
        }
        else
            cout<<"Не удалось открыть файл"<<endl;
    }
    fout.close();
    fin.close();
}

void Out(){
    cout<<"Вы уверены, что хотите выйти из программы?\n1.Да\n 2.Нет"<<endl;
    int ex;
    checkChoice(&ex, 1, 2);
    switch(ex)
    {
        case 1:
            cout<<"Выход выполнен успешно"<<endl;
            exit(0);
        case 2:
            break;
    }
}

void addCorrect(int *number, Tstudent *stud){
    
    cout << "\n\tВведите год рождения: ";
    checkChoice(&stud[*number].year, 0, 2002);
    
    cout << "\n\tВведите номер группы: ";
    cin.getline(stud[*number].group,9);
    
    cout << "\n\tОценка по физике: ";
    checkChoice(&stud[*number].phys, 1, 10);
    
    cout << "\n\tОценка по математике: ";
    checkChoice(&stud[*number].math, 1, 10);
    
    cout << "\n\tОценка по информатике: ";
    checkChoice(&stud[*number].inf, 1, 10);
    
    cout << "\n\tОценка по химии: ";
    checkChoice(&stud[*number].chem, 1, 10);
    
    stud[*number].ave = (double)(stud[*number].phys + stud[*number].math + stud[*number].inf + stud[*number].chem) / 4;
}

void  ChooseDel(){
    int count=0,choice, num, rez,del;
    stud = new Tstudent[count];
    
    ifstream fin("/Users/dariavarabei/Desktop/FileIn.txt", ios_base::in);
    if(fin.is_open()){
        for (int i = 0;fin>>stud[i].name >> stud[i].year>> stud[i].group>> stud[i].math>> stud[i].phys>>stud[i].inf >>stud[i].chem>>stud[i].ave ;i++)
        {
            count++;
        }
        if(count==0)
            cout<<"Информации в файле FileIn.txt не обнаружено"<<endl;
    }
    else
        cout<<"Не удалось открыть файл"<<endl;
    fin.close();
    ofstream fout("/Users/dariavarabei/Desktop/FileIn.txt", ios::out);
    cout<<"Выберите, что именно вы хотите удалить:\n1.Одного студента\n2.Всю информацию"<<endl;
    checkChoice(&choice, 1, 2);
    switch(choice)
    {
        case 1:
            cout<<"Введите номер студента , данные которого вы хотите удалить"<<endl;
            checkChoice(&num, 1, count);
            cout<<"Вы уверены , что хотите удалить студента из файла?\n1.Да\n2.Нет"<<endl;
            checkChoice(&rez, 1, 2);
            switch(rez)
            {
                case 1:
                    for( int i = 0 ; i < count ; i++)
                    {
                        for( i = num-1 ; i < count ; i++)
                        {
                            stud[i] = stud[i+1];
                        }
                        count = count-1;
                    }
                    cout<<"Студент № "<<num++<<" удален из файла"<<endl;
                    break;
                case 2:
                    cout<<"Данные остались в неизменном состоянии"<<endl;
                    break;
            }
            for(int i = 0 ; i < count ; i++)
            {
                int name_len=my_strlen(stud[i].name);
                fout << stud[i].name <<setw(25-name_len) << stud[i].year <<setw(10) << stud[i].group << setw(10) <<stud[i].phys <<setw(10) << stud[i].math <<setw(10) << stud[i].inf<<setw(10) << stud[i].chem <<setw(10) <<stud[i].ave<<"\n";
            }
            delete[] stud;
            fout.close();
            break;
        case 2:
               cout<<"Вы уверены,что хотите удалить информацию из файла?\n1.Нет\n2.Да\n"<<endl;
               checkChoice(&del, 1, 2);
               switch(del)
               {
                   case 1:
                       for(int i = 0 ; i < count ; i++)
                       {
                           int name_len=my_strlen(stud[i].name);
                           fout << stud[i].name <<setw(25-name_len) << stud[i].year <<setw(10) << stud[i].group << setw(10) <<stud[i].phys <<setw(10) << stud[i].math <<setw(10) << stud[i].inf<<setw(10) << stud[i].chem <<setw(10) <<stud[i].ave<<"\n";
                       }
                       cout<<"Файл остался в неизменном состоянии"<<endl;
                       break;
                   case 2:
                       delete []stud;
                       ofstream fout("/Users/dariavarabei/Desktop/FileIn.txt", ios::out);
                       cout<<"Информация успешно удалена из файла "<<endl;
                       fout.close();
                       break;
               }
            break;
    }
}

int my_strlen(char* str)
{
    int length=0;
    while(*str++)
        ++length;
    return length;
}
*/


/////////////////////4 лаба 2 сема
#include <iostream>
#include <clocale>
#include <string>
using namespace std;
 
struct stack {
char s;
stack *next;
};
 
void in(stack **, char);
void out(stack **, char &);
int Prioritet(char);
double calculate(stack **, string, double, double, double, double, double);
 
int main()
{
    setlocale(LC_ALL, "rus");
    double a, b, c, d, e;
    int length;
    char ss, ss_buff;
    stack *top = NULL;
    string final, begin;
    cout << "Введите формулу которую нужно преобразовать: ";
    cin >> begin;
    length = begin.length();
    for (int i = 0; i < length; i++)
    {
        ss = begin[i];
        if (ss == '(')
            in(&top, ss);
        if (ss == ')')
        {
            while (top->s != '(')
            {
                out(&top, ss_buff);
                final += ss_buff;
            }
            out(&top, ss_buff);
        }
        if (ss >= 'a' && ss <= 'z')
            final += ss;
        if (ss == '-' || ss == '+' || ss == '*' || ss == '/')
        {
            while (top != NULL && Prioritet(top->s) >= Prioritet(ss))
            {
                out(&top, ss_buff);
                final += ss_buff;
            }
            in(&top, ss);
        }
    }
    while (top != NULL)
    {
        out(&top, ss_buff);
        final += ss_buff;
    }
    cout<<final;
    cout << "Ваша формула в польской записи: " << final << endl;
    cout << "Введите значения переменных!\na = ";
    cin >> a;
    cout << "b = ";
    cin >> b;
    cout << "c = ";
    cin >> c;
    cout << "d = ";
    cin >> d;
    cout << "e = ";
    cin >> e;
    cout << "Результат вашего выражения = " << calculate(&top, final, a, b, c, d, e) << endl;
    return 0;
}

void in(stack **top, char s)
{
    stack *t = new stack;
    t->s = s;
    if (t == NULL)
    {
        *top = t;
    }
    else
    {
        t->next = *top;
        *top = t;
    }
}

void out(stack **top, char &buff)
{
    stack *q = *top;
    buff = q->s;
    if (q == *top)
    {
        *top = q->next;
        free(q);
    }
    else
        free(q);
    q = q->next;
}

int Prioritet(char s)
{
    if (s == '*' || s == '/')
        return 2;
    if (s == '+' || s == '-')
        return 1;
    if (s == '(')
        return 0;
    else
        return -1;
}

double calculate(stack **top, string final, double a, double b, double c, double d, double e)
{
    char ss, comp1, comp2;
    double op1, op2, rez = 0.0;
    
    double mas[201];
    
    mas[int('a')] = a;
    mas[int('b')] = b;
    mas[int('c')] = c;
    mas[int('d')] = d;
    mas[int('e')] = e;
    
    
    char chr = 'z' + 1;
    
    for (int i = 0; i < final.length(); i++)
    {
        ss = final[i];
        if (ss >= 'a' && ss <= 'z')   in(top, ss);        // ЕСЛИ НЕ ОПЕРАТОР ТО ЗАНОСИТСЯ В СТЕК
        else
        {
            out(top, comp1);  // ВЫНОСЯТСЯ ДВЕ ПЕРЕМЕННЫЕ ИЗ СТЕКА
            out(top, comp2);
            
            op1 = mas[int(comp1)];
            op2 = mas[int(comp2)];
            
            switch (ss)
            {
                case '*':   rez = op2 * op1;   break;
                case '/':   rez = op2 / op1;   break;
                case '+':   rez = op2 + op1;   break;
                case '-':   rez = op2 - op1;   break;
            }
            
            mas[int(chr)] = rez;
            in(top, chr);
            chr++;
        }
    }
    
    return rez;
}

/*Введите формулу которую нужно преобразовать: (a+b)*(c-d)/e
abcde/-(*+(Ваша формула в польской записи: abcde/-(*+(
Введите значения переменных!
a = 7.4
b = 3.6
c = 2.8
d = 9.5
e = 0.9
*/
