#ifndef PARTNER_H
#define PARTNER_H
#include "partner.h"
#endif

#include "general_funcs.h"
#include "rbtree.h"
#include "hash.h"

#include <iostream>
#include <cstring> // memset()
#include <chrono> // Для секундомера
#include <algorithm> // std::sort
#include <map> // std::multimap

long linear_search(Partner a[], long size, const char* name, long last_index) // O(n) в худшем и среднем и в лучшем. Память - О(1)
{
    for (int i = 0; i < size; i++)
    {
    if (strcmp(a[i].getOrg(), name)==0) return i+last_index; // Возвращается индекс первого вхождения
    }
    return -1;
}

long binary_search(Partner a[], long start, long end, const char* name) // O(log n + k), где k - количество найденных элементов. Худший случай (все элементы равны) - O(n). Память - О(1)
{
    if(start > end) return -1;
 
    const int middle = start + ((end - start) / 2);

    if(strcmp(a[middle].getOrg(), name)==0) return middle;
    else if(strcmp(a[middle].getOrg(), name)>0) return binary_search(a, start, middle - 1, name);

    return binary_search(a, middle + 1, end, name);
}

int main(int argc, char* argv[]) //Все имена файлов csv с входными данными передаются при запуске программы
{
    Partner* p1 = new Partner[1000000];
    long size = 0; //Длина массива
    long found_size = 0; // Длина массива найденных элементов
    long found_index = 0; // Индекс элемента, который вернула функция поиска
    long next_index; // Относительный индекс подмассива при линейном поиске

    // Массивы найденных элементов
    Partner* linearArr;
    Partner* binaryArr;
    Partner* redblackArr;
    Partner* hashArr;
    Partner* multimapArr;

    // Засеченное время для всех (в наносекундах)
    int linearSec=0, binarySec=0, redblackSec=0, hashSec=0, multimapSec=0;

    // Массив для хранения имени файлов отсортированных массивов и входных csv
    char foundName[80];
    memset(foundName, 0, 80);

    auto start = std::chrono::steady_clock::now();
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    
    for (int i=1; i<argc; i++) // Пробегаюсь по всем именам CSV
    {
        sprintf(foundName, "./input2/%s", argv[i]);
        size = fillMas(foundName, p1); // Заполняю массив
        memset(foundName, 0, 80);

        char* searchKey;

        if (size != -1) // массив успешно записался
        {
            int len = strlen(p1[size / 2].getOrg()); // Делаю копию строки, иначе будет косяк по адресу после сортировки
            searchKey = new char[len + 1]; // Ключ, по которому буду искать
            strcpy(searchKey, p1[size / 2].getOrg());
            
            
           /*
           int len = strlen("Summers PLC"); // Делаю копию строки, иначе будет косяк по адресу после сортировки
           searchKey = new char[len + 1]; // Ключ, по которому буду искать
           strcpy(searchKey, "Summers PLC");
           */
            
            //Делаю копии массива для каждого поиска
            linearArr = new Partner[size];
            binaryArr = new Partner[size];
            redblackArr = new Partner[size];
            hashArr = new Partner[size];
            multimapArr = new Partner[size];

            // Линейный
            found_size = 0;
            start = std::chrono::steady_clock::now();
            found_index = linear_search(p1, size-1, searchKey, 0);
            while (found_index != -1) // Пока удается найти
            {
                linearArr[found_size++] = Partner(p1[found_index]); // Заполняю массив найденных элементов через конструктор копирования
                found_index = linear_search(p1 + found_index + 1, size - found_index - 1, searchKey, found_index+1); // Возвращается индекс ПЕРЕДАННОГО ПОДМАССИВА
            }
            end = std::chrono::steady_clock::now();
            duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
            linearSec = duration.count();
            std::sort(linearArr,linearArr+found_size);
            

            // Красно-черное дерево. Время: O(log n + k) в среднем и худшем (k - количество значений с данным ключом). Память: O(n) на хранение дерева + O(log n) на стек рекурсии при поиске.
            found_size = 0;
            //start = std::chrono::steady_clock::now(); 
 
            RBTree tree;
            for (long j = 0; j < size; j++)
                tree.insert(p1[j]);

            start = std::chrono::steady_clock::now(); 
            found_size = tree.searchAll(searchKey, redblackArr, size);
    
            end = std::chrono::steady_clock::now();
            duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
            redblackSec = duration.count();
            std::sort(redblackArr,redblackArr+found_size);


            // Хэш-таблица. O(n) для хранения + размер таблицы.
            found_size = 0;
            HashTable table;
            for (long j = 0; j < size; j++)
                table.insert(p1[j]);

            start = std::chrono::steady_clock::now(); 
            found_size = table.searchAll(searchKey, hashArr, size);
    
            end = std::chrono::steady_clock::now();
            duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
            hashSec = duration.count();
            std::sort(hashArr,hashArr+found_size);

            // Мултимап
            found_size = 0;
            std::multimap<std::string, Partner> mmap;
            for (int j = 0; j < size; j++)
                mmap.insert({std::string(p1[j].getOrg()), p1[j]});

            // equal_range возвращает пару итераторов: начало и конец всех вхождений
            start = std::chrono::steady_clock::now();
            auto range = mmap.equal_range(std::string(searchKey));
            

            for (auto it = range.first; it != range.second; ++it)
                multimapArr[found_size++] = Partner(it->second);
            end = std::chrono::steady_clock::now();

            
            duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
            multimapSec = duration.count();
            std::sort(multimapArr,multimapArr+found_size);


            // Бинарный (ставлю в самый конец, потому что сортирую исходный массив)
            found_size = 0;
            found_index = 0;
            std::sort(p1,p1+size);
            long found_index_copy; // Когда пойду отсматривать в левую сторону, нужно будет запомнить, откуда начинать
            start = std::chrono::steady_clock::now();

            found_index = binary_search(p1, 0, size-1, searchKey); //Первый элемент откуда-то из середины. По бокам от него могут быть "дубликаты"
            found_index_copy = found_index-1; //На один меньше, потому что в первом цикле его уже добавлю в массив

            if (found_index != -1) // только если нашли
            {
                found_index_copy = found_index - 1;
                while (found_index < size && strcmp(p1[found_index].getOrg(), searchKey) == 0)
                    binaryArr[found_size++] = Partner(p1[found_index++]);
                while (found_index_copy >= 0 && strcmp(p1[found_index_copy].getOrg(), searchKey) == 0)
                    binaryArr[found_size++] = Partner(p1[found_index_copy--]);
            }
            

            end = std::chrono::steady_clock::now();
            duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
            binarySec = duration.count();
            std::sort(binaryArr,binaryArr+found_size);


            if (searchCheck(multimapArr, linearArr, binaryArr,redblackArr, hashArr, found_size)==true) // Поиски сошлись
            {
                sprintf(foundName, "./output2/foundElems_size_%ld.csv", size); //Получаю имя файла
                writeCSV(foundName, redblackArr, found_size);

                // Вывод всех измерений:
                std::cout << "Массив размера: "<< size <<std::endl;
                std::cout << "Линейный: " << linearSec<< "нс;   " << "Бинарный: " << binarySec<< "нс" << std::endl; 
                std::cout << "RedBlack: " << redblackSec << "нс;  " << "HashTable: " << hashSec<< "нс" << std::endl;
                std::cout << "Мультимап: " << multimapSec<< "нс"<< std::endl;
                std::cout << "Коллизий HashTable: " << table.getCollisions()<< "\n" << std::endl;
            }

            // Очистка массива
            memset(p1, 0, 1000000 * sizeof(Partner));
            delete[] linearArr;
            delete[] binaryArr;
            delete[] redblackArr;
            delete[] hashArr;

            delete[] searchKey;
        }
    }
    delete[] p1;
    return 0;
}

/* TODO
1. Сделать обработку ключа из внешних аргументов вызываемой программы (meh...)
2. Сделать redblack DONE
3. Сделать хэш DONE
*/

// Summers PLC