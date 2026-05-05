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
#include <chrono> // Для секундомера
#include <algorithm>

template<class T>
void shakerSort(T* a, long size)  
{
    if (size <= 0) return;

    // Алгоритм шейкерной сортировки над 
    long j, k = size-1;
    long lb=1, ub = size-1; // bounds of unsorted part of the array
    T x;

    do
    {
    // from bottom to top passage 
    for( j=ub; j>0; j-- )
    {
        if ( a[j-1] > a[j] )
        {
        x=a[j-1]; a[j-1]=a[j]; a[j]=x;
        k=j;
        }
    }

    lb = k+1;

    // passage from top to bottom 
    for (j=1; j<=ub; j++)
    {
        if ( a[j-1] > a[j] )
        {
        x=a[j-1]; a[j-1]=a[j]; a[j]=x;
        k=j;
        }
    }

    ub = k-1;
    }
    while ( lb < ub );
}

template<class T> 
void quickSortR(T* a, long N)
{
    long i = 0, j = N-1; 		// initializing the bounds i and j
    T p = a[ N>>1 ];		// base element p is a central element

    T temp; //Для swap

    // division procedure
    do
    {
    while ( a[i] < p ) i++;
    while ( a[j] > p ) j--;

    if (i <= j)
    {
        temp = a[i];
        a[i] = a[j];
        a[j] = temp;
        i++; j--;
    }
    } while ( i<=j );

    // recursive division 
    if ( j > 0 ) quickSortR(a, j+1);
    if ( N > i ) quickSortR(a+i, N-i);
}

template<class T> 
void merge(T a[], long low, long mid, long high)
{
	// Variables declaration. 
	T *b = new T[high+1-low];
	long h,i,j,k;
	h=low;
	i=0;
	j=mid+1;
	// Merges the two array's into b[] until the first one is finish
	while((h<=mid)&&(j<=high))
	{
		if(a[h]<=a[j])
		{
			b[i]=a[h];
			h++;
		}
		else
		{
			b[i]=a[j];
			j++;
		}
		i++;
	}
	// Completes the array filling in it the missing values
	if(h>mid)
	{
		for(k=j;k<=high;k++)
		{
			b[i]=a[k];
			i++;
		}
	}
	else
	{
		for(k=h;k<=mid;k++)
		{
			b[i]=a[k];
			i++;
		}
	}
	// Prints into the original array
	for(k=0;k<=high-low;k++) 
	{
		a[k+low]=b[k];
	}
	delete[] b;
}

template<class T> 
void merge_sort(T a[], long low, long high)		
{// Recursive sort ...
	long mid;
	if(low<high)
	{
		mid=(low+high)/2;
		merge_sort(a, low, mid);
		merge_sort(a, mid+1, high);
		merge(a, low, mid, high);
	}
}

int fillMas(const char* filename, Partner* p_mas)
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
        return p_index; //Возвращаю длину массива
    }
    return -1; //Массив не записался
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

int main(int argc, char* argv[]) //Все имена файлов csv с входными данными передаются при запуске программы
{
    Partner p1[100000];
    int size = 0; //Длина массива

    // Отсортированные массивы
    Partner* stdArr;
    Partner* shakerArr;
    Partner* quickArr;
    Partner* mergeArr;

    // Засеченное время для всех (в милисекундах)
    int shakerSec=0, quickSec=0, mergeSec=0, stdSec=0;

    // Массив для хранения имени файлов отсортированных массивов
    char sortedName[80];
    memset(sortedName, 0, 80);

    auto start = std::chrono::steady_clock::now();
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    for (int i=1; i<argc; i++) // Пробегаюсь по всем именам CSV
    {
        size = fillMas(argv[i],p1); // Заполняю массив
        if (size != -1) // массив успешно записался
        {
            //Делаю копии массива для каждой сортировки
            shakerArr = new Partner[size];
            stdArr = new Partner[size];
            quickArr = new Partner[size];
            mergeArr = new Partner[size];

            // Копирую элементы
            for (long i = 0; i < size; ++i)
            {
                shakerArr[i]=p1[i];
                stdArr[i]=p1[i];
                quickArr[i]=p1[i];
                mergeArr[i]=p1[i];
            }

            // Шейкер
            start = std::chrono::steady_clock::now();
            shakerSort(shakerArr, size);
            end = std::chrono::steady_clock::now();
            duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            shakerSec = duration.count();

            // Быстрая
            start = std::chrono::steady_clock::now();
            quickSortR(quickArr, size);
            end = std::chrono::steady_clock::now();
            duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            quickSec = duration.count();

            // Слиянием
            start = std::chrono::steady_clock::now();
            merge_sort(mergeArr,0 ,size-1);
            end = std::chrono::steady_clock::now();
            duration = std::chrono::duration_cast<std::chrono::milliseconds>(end- start);
            mergeSec = duration.count();

            // std::sort (TODO: что за сортировка под std::sort?)
            start = std::chrono::steady_clock::now();
            std::sort(stdArr,stdArr+size);
            end = std::chrono::steady_clock::now();
            duration = std::chrono::duration_cast<std::chrono::milliseconds>(end- start);
            stdSec = duration.count();

            // Вывод отсортированного массива в файл
            sprintf(sortedName, "/output/sortedArray_num_%d.csv", i); //Получаю имя файла
            writeCSV(sortedName, stdArr, size);

            // Вывод всех измерений:
            std::cout << "Массив размера: "<< size <<std::endl;
            std::cout << "Шейкер: " << shakerSec<< "мс;   " << "Быстрая: " << quickSec<< "мс" << std::endl; 
            std::cout << "Слияние: " << mergeSec << "мс;  " << "std::sort: " << stdSec<< "мс" << "\n" << std::endl;

            // Очистка массива
            memset(p1, 0, 100000);
            delete[] shakerArr;
            delete[] quickArr;
            delete[] stdArr;
            delete[] mergeArr;
        }
    }
    return 0;
}


/*
TODO:
1. Починить шейкер DONE
2. Сделать слияние, быструю сортировку DONE
4. Нагенерить 10 входных файлов DONE
5. Починить быструю сортировку DONE
6. Починить сортировку слиянием DONE
*/
