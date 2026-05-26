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
#ifndef PARTNER_H
#define PARTNER_H
#include "partner.h"
#endif

#include "general_funcs.h"

#include <iostream>
#include <cstring> // memset()
#include <chrono> // Для секундомера
#include <algorithm> // std::sort

template<class T>
void shakerSort(T* arr, long size) // O(n^2) в худщем и среднем // O(n) в лучшем. Память O(1)
{
    if (size <= 0) return;
 
    long lastSwapPos = size - 1; // позиция последнего обмена
    long leftBound = 1; // левая граница несортированной части
    long rightBound  = size - 1; // правая граница несортированной части
    T temp;
 
    do
    {
        // проход справа налево - минимальный элемент всплывает влево
        for (long i = rightBound; i > 0; i--)
        {
            if (arr[i - 1] > arr[i])
            {
                temp = arr[i - 1];
                arr[i - 1] = arr[i];
                arr[i]= temp;
                lastSwapPos = i;
            }
        }
        // сдвигаем левую границу - всё левее уже отсортировано
        leftBound = lastSwapPos + 1;
 
        // проход слева направо - максимальный элемент тонет вправо
        for (long i = 1; i <= rightBound; i++)
        {
            if (arr[i - 1] > arr[i])
            {
                temp = arr[i - 1];
                arr[i - 1] = arr[i];
                arr[i] = temp;
                lastSwapPos = i;
            }
        }
        // сдвигаем правую границу — всё правее уже отсортировано
        rightBound = lastSwapPos - 1;
 
    } while (leftBound < rightBound); // пока есть несортированная часть
}
 
template<class T>
void quickSortR(T* arr, long size) // O(nlogn) в   среднем // O(n^2) в худшем. Память O(log n)
{
    long left  = 0; // левый указатель
    long right = size - 1; // правый указатель
    T pivot = arr[size >> 1]; // опорный элемент - (пиво) середина массива 
    T temp;
 
    // переношу элементы меньше опорного влево, больше опорного - вправо
    do
    {
        while (arr[left] < pivot) left++;  // элемент >= pivot слева надо загнать влево
        while (arr[right] > pivot) right--; // элемент <= pivot справа надо зашнать вправо
 
        if (left <= right)
        {
            // меняю местами найденные элементы
            temp = arr[left];
            arr[left] = arr[right];
            arr[right] = temp;
            left++;
            right--;
        }
    } while (left <= right);
 
    // рекурсивная сортировка обеих частей
    if (right > 0) quickSortR(arr, right + 1);
    if (size  > left) quickSortR(arr + left, size - left);
}
 
template<class T>
void merge(T arr[], long low, long mid, long high) // O(nlogn) в всегда // Память O(n) для буфера
{
    T* temp = new T[high - low + 1]; // временный массив для слияния
    long leftPtr = low; // указатель на текущий элемент левой части
    long rightPtr = mid + 1; // указатель на текущий элемент правой части
    long tempIndex = 0; // индекс для заполнения временного массива
 
    // Сравниваем элементы обеих частей и записываем меньший во временный массив
    while (leftPtr <= mid && rightPtr <= high)
    {
        if (arr[leftPtr] <= arr[rightPtr])
            temp[tempIndex++] = arr[leftPtr++];
        else
            temp[tempIndex++] = arr[rightPtr++];
    }
 
    // если в левой (првой) части остались элементы, их надо дописать
    while (leftPtr <= mid)
        temp[tempIndex++] = arr[leftPtr++];
 
    while (rightPtr <= high)
        temp[tempIndex++] = arr[rightPtr++];
 
    // результат обратно в исходный массив копируется
    for (long i = 0; i <= high - low; i++)
        arr[low + i] = temp[i];
 
    delete[] temp;
}
 
template<class T>
void merge_sort(T arr[], long low, long high)
{
    if (low >= high) return; // базовый случай: подмассив из одного элемента
 
    long mid = (low + high) / 2; // середина текущего подмассива
 
    merge_sort(arr, low, mid); // сортируем левую половину
    merge_sort(arr, mid + 1, high); // сортируем правую половину
    merge(arr, low, mid, high); // сливаем отсортированные половины
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

    // Массив для хранения имени файлов отсортированных массивов и входных данных csv
    char sortedName[80];
    memset(sortedName, 0, 80);

    auto start = std::chrono::steady_clock::now();
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    for (int i=1; i<argc; i++) // Пробегаюсь по всем именам CSV
    {
        sprintf(sortedName, "./input1/%s", argv[i]);
        size = fillMas(sortedName, p1); // Заполняю массив
        memset(sortedName, 0, 80);

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
            if (sortCheck(stdArr, shakerArr, quickArr, mergeArr, size) == true) // Сортировки совпали
            {
                sprintf(sortedName, "./output/sortedArray_size_%d.csv", size); //Получаю имя файла
                writeCSV(sortedName, stdArr, size);

                // Вывод всех измерений:
                std::cout << "Массив размера: "<< size <<std::endl;
                std::cout << "Шейкер: " << shakerSec<< "мс;   " << "Быстрая: " << quickSec<< "мс" << std::endl; 
                std::cout << "Слияние: " << mergeSec << "мс;  " << "std::sort: " << stdSec<< "мс" << "\n" << std::endl;
            }

            // Очистка массива
            memset(p1, 0, 1000000 * sizeof(Partner));
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
