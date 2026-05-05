#ifndef PARTNER_H
#define PARTNER_H
#include "partner.h"
#endif

#include "general_funcs.h"

#include <iostream>
#include <cstring> // memset()
#include <chrono> // Для секундомера
#include <algorithm> // std::sort

int linear_search(Partner a[], long size, const char* name)
{
    for (int i = 0; i < size; i++)
    {
    if (strcmp(a[i].getOrg(), name)==0) return i; // Возвращается индекс первого вхождения
    }
    return -1;
}

int main(int argc, char* argv[]) //Все имена файлов csv с входными данными передаются при запуске программы
{
    Partner p1[100000];
    int size = 0; //Длина массива
    int found_size = 0; // Длина массива найденных элементов
    int found_index; // Индекс элемента, который вернула функция поиска
    int next_index; // Относительный индекс подмассива при линейном поиске

    // Массивы найденных элементов
    Partner* linearArr;
    Partner* binaryArr;
    Partner* redblackArr;
    Partner* hashArr;

    // Засеченное время для всех (в милисекундах)
    int linearSec=0, binarySec=0, redblackSec=0, hashSec=0;

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
            linearArr = new Partner[size];
            binaryArr = new Partner[size];
            redblackArr = new Partner[size];
            hashArr = new Partner[size];

            // Линейный
            start = std::chrono::steady_clock::now();
            found_index = linear_search(p1, size-1, p1[size/2].getOrg());
            while (found_index != -1) // Пока удается найти
            {
                linearArr[found_size++] = Partner(p1[found_index]); // Заполняю массив найденных элементов через конструктор копирования
                next_index = linear_search(p1 + found_index + 1, size - found_index - 1, p1[size/2].getOrg()); // Возвращается индекс ПЕРЕДАННОГО ПОДМАССИВА
                if (next_index != -1)
                    found_index = found_index + 1 + next_index; // Перевожу в индекс исходного массива
                else
                    found_index = -1;
            }
            end = std::chrono::steady_clock::now();
            duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            linearSec = duration.count();


            // Вывод отсортированного массива в файл
            sprintf(sortedName, "./output2/foundElems_num_%d.csv", i); //Получаю имя файла
            writeCSV(sortedName, linearArr, found_size);

            // Вывод всех измерений:
            std::cout << "Массив размера: "<< size <<std::endl;
            std::cout << "Линейный: " << linearSec<< "мс;   " << "Бинарный: " << binarySec<< "мс" << std::endl; 
            std::cout << "RedBlack: " << redblackSec << "мс;  " << "HashTable: " << hashSec<< "мс" << "\n" << std::endl;

            // Очистка массива
            memset(p1, 0, 100000);
            delete[] linearArr;
            delete[] binaryArr;
            delete[] redblackArr;
            delete[] hashArr;
        }
    }
    return 0;
}




