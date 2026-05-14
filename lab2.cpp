#ifndef PARTNER_H
#define PARTNER_H
#include "partner.h"
#endif

#include "general_funcs.h"
#include "rbtree.h"

#include <iostream>
#include <cstring> // memset()
#include <chrono> // Для секундомера
#include <algorithm> // std::sort

int linear_search(Partner a[], long size, const char* name, int last_index)
{
    for (int i = 0; i < size; i++)
    {
    if (strcmp(a[i].getOrg(), name)==0) return i+last_index; // Возвращается индекс первого вхождения
    }
    return -1;
}

int binary_search(Partner a[], int start, int end, const char* name)
{
    // Termination condition: start index greater than end index
    if(start > end) return -1;
 
    // Find the middle element of the vector and use that for splitting
    // the array into two pieces.
    const int middle = start + ((end - start) / 2);

    if(strcmp(a[middle].getOrg(), name)==0) return middle;
    else if(strcmp(a[middle].getOrg(), name)>0) return binary_search(a, start, middle - 1, name);

    return binary_search(a, middle + 1, end, name);
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
    char foundName[80];
    memset(foundName, 0, 80);

    auto start = std::chrono::steady_clock::now();
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    for (int i=1; i<argc; i++) // Пробегаюсь по всем именам CSV
    {
        size = fillMas(argv[i], p1); // Заполняю массив
        if (size != -1) // массив успешно записался
        {

            const char* searchKey = p1[size / 2].getOrg();

            //Делаю копии массива для каждой сортировки
            linearArr = new Partner[size];
            binaryArr = new Partner[size];
            redblackArr = new Partner[size];
            hashArr = new Partner[size];

            // Линейный
            start = std::chrono::steady_clock::now();
            found_index = linear_search(p1, size-1, searchKey, 0);
            while (found_index != -1) // Пока удается найти
            {
                linearArr[found_size++] = Partner(p1[found_index]); // Заполняю массив найденных элементов через конструктор копирования
                found_index = linear_search(p1 + found_index + 1, size - found_index - 1, searchKey, found_index+1); // Возвращается индекс ПЕРЕДАННОГО ПОДМАССИВА
            }
            end = std::chrono::steady_clock::now();
            duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            linearSec = duration.count();

            // Бинарный
            found_size = 0;
            int found_index_copy; // Когда пойду отсматривать в левую сторону, нужно будет запомнить, откуда начинать
            start = std::chrono::steady_clock::now();
            found_index = binary_search(p1, 0, size-1, searchKey); //Первый элемент откуда-то из середины. По бокам от него могут быть "дубликаты"
            found_index_copy = found_index-1; //На один меньше, потому что в первом цикле его уже добавлю в массив
            
            while (strcmp(p1[found_index].getOrg(), searchKey)==0) // Смотрим справа
            {
                binaryArr[found_size++] = Partner(p1[found_index++]); // Заполняю массив найденных элементов через конструктор копирования
            }
            while (strcmp(p1[found_index_copy].getOrg(), searchKey)==0) // Смотрим слева
            {
                binaryArr[found_size++] = Partner(p1[found_index_copy--]); // Заполняю массив найденных элементов через конструктор копирования
            }
            end = std::chrono::steady_clock::now();
            duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            binarySec = duration.count();

            // Красно-черное дерево
            found_size = 0;
            //start = std::chrono::steady_clock::now(); 
 
            RBTree tree;
            for (int j = 0; j < size; j++)
                tree.insert(p1[j]);

            start = std::chrono::steady_clock::now(); 
            found_size = tree.searchAll(searchKey, redblackArr, size);
    
            end = std::chrono::steady_clock::now();
            duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            redblackSec = duration.count();


            // Вывод массива массива найденныйх элементов в файл
            sprintf(foundName, "./output2/foundElems_num_%d.csv", i); //Получаю имя файла
            writeCSV(foundName, redblackArr, found_size);

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

/* TODO
1. Сделать обработку ключа из внешних аргументов вызываемой программы
2. Сделать redblack DONE
3. Сделать хэш
*/

