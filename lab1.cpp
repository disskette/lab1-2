/*
1) Реализовать на языке C++ программирования сортировки для массива объектов в  соответствии с вариантом. 
2) Перегрузить операторы сравнения (>, <, >=, <=) для сравнения  объектов. Правила сравнения указаны в варианте.
3) Входные данные для сортировки массива обязательно считывать из  внешних источников: текстовый файл, файлы csv, 
xlsx, данные из СУБД (любое на выбор). Выходные данные (отсортированный массив) записывать в файл.
4) Выбрать 10-20 наборов данных для сортировки размерности от 100 и  более (но не менее, чем до 100000). 
Засечь (программно) время сортировки  каждым алгоритмом и std::sort. По полученным точкам построить графики  
зависимости времени сортировки от размерности массива для каждого из алгоритмов сортировки на одной оси координат. 
Сделать выводы о  том, в каком случае, какой из методов лучше применять.
5) Сделать отчет, состоящий из:
документации к коду работы, сгенерированную с помощью case средства (doxygen, sphinx, etc);
ссылку на исходный код программы в репозитории; 
графики времени сортировок. 


10) 10, г, е, ж 

г) Шейкер-сортировка 
е) Быстрая сортировка 
ж) Сортировка слиянием


Массив данных - электронный справочник деловых  партнеров: ФИО, название организации, 
номер рабочего  телефона, адрес электронной почты (сравнение по полям – название организации, ФИО, номер телефона)

Логика сравнения: 
1. сравниваю по порядку каждое поле класса: органищация, ФИО, телефон
2. Останавливаюсь на первом несовпадающем поле
3. Если значение поля первого объекта сравнительно больше, чем у второго, то первый объект больше. Иначе меньше.

Пример 1:
1: аааа, 1234, ббб
2. аааа, 1234, абв

Второй объект меньше

Пример 2:
1. аааа, 8888, ббб
2. бббб, 7777, ааа

Первый объект меньше несмотря на сравнение оставшихся элементов. Логика позаимствована у лексикографического сравнения C-строк
*/
#include "partner.h"
#include <unistd.h> // для read()
#include <stdlib.h> // Для exit()
#include <fcntl.h> // для open()
#include <cstring> // memset()
#include <iostream>

template<class T>
T* shakerSort(const T* a, long size)   // входной массив теперь const
{
    if (size <= 0) return nullptr;

    // Выделяем память под копию
    T* sorted = new T[size];
    // Копируем элементы
    for (long i = 0; i < size; ++i)
        sorted[i] = a[i];

    // Алгоритм шейкерной сортировки над sorted
    long lb = 0;
    long ub = size - 1;
    bool swapped = true;

    while (lb < ub && swapped)
    {
        swapped = false;

        // проход слева направо
        for (long j = lb; j < ub; ++j)
        {
            if (sorted[j] > sorted[j + 1])
            {
                T temp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = temp;
                swapped = true;
            }
        }
        --ub;

        if (!swapped) break;

        swapped = false;

        // проход справа налево
        for (long j = ub; j > lb; --j)
        {
            if (sorted[j - 1] > sorted[j])
            {
                T temp = sorted[j - 1];
                sorted[j - 1] = sorted[j];
                sorted[j] = temp;
                swapped = true;
            }
        }
        ++lb;
    }

    return sorted;   // вызывающий должен вызвать delete[]
}


bool fillMas(const char* filename, Partner* p_mas)
{
    char buf[80]; // Для строки, которая будет аргументом setAttr
    memset(buf, 0,80);

    char ch;  //каждый считанный символ
    int index = 0; // индекс для заполнения buf
    int p_index = 0; //индекс для заполнения массива p_mas
    int col =0 ; // номер столбца (0 - ФИО, 1 - организация, 2 - телефон, 3 - почта)
    int fd;

    
    fd = open(filename, 0); // открываю csv на чтение
    if (fd)
    {   
        while (read(fd, &ch, 1) > 0) { //Потихоньку читаю весь вывод из файла
            if (ch == ';') { // Разрыв - текст в поле закончился 
                buf[index] = '\0';
                
                switch (col) // Решаю, какое из полей класса заполнять
                {
                case 0:
                    p_mas[p_index].setAttr(buf);
                    break;
                case 1:
                    p_mas[p_index].setAttr(nullptr, buf);
                    break;
                case 2:
                    p_mas[p_index].setAttr(nullptr, nullptr, buf);
                    break;
                default:
                    break;
                }
                    
                col ++; // Дальше читаю следующее поле
                index=0; // Сбрасываю всё для записи строки в buf
                memset(buf, 0,80);
            }
            else if (ch == '\n') // Строка закончилась. До этого считывала почту
            {
                if (index == 0) break; // Дошли до конца csv
                buf[index] = '\0';
                p_mas[p_index].setAttr(nullptr, nullptr, nullptr, buf);
                
                // Сбрасываю всё для записи новой строки в buf
                memset(buf, 0,80);
                index=0; 
                col = 0;
                p_index+=1;
            }
            else buf[index++]=ch;
        }
        close(fd);
        return true;
    }
    return false; //Массив не записался
}

bool writeCSV(const char* filename, Partner* p_mas, int size)
{
    int fd = 0;
    int fd_terminal = dup(1);

    fd = creat(filename, 0664); // rw rw r

    if (fd)
    {
        dup2(fd,1);
        for (int i=0; i<size; i++)
        {
            std::cout<<p_mas[i].getName()<<";"<<p_mas[i].getOrg()<<";"<<p_mas[i].getPhone()<<";"<<p_mas[i].getEmail()<< std::endl;
        }
        close(fd);
        dup2(fd_terminal,1);
        return true;
    }
    return false;
}

int main()
{
    Partner p1[100000];
    fillMas("username.csv",p1);


    Partner* sortedArr = shakerSort(p1, 5);
    writeCSV("test.csv", sortedArr, 5);
}

