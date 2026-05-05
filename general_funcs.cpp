#ifndef PARTNER_H
#define PARTNER_H
#include "partner.h"
#endif

#include <unistd.h> // для read(), close 
#include <fcntl.h> // для open(), creat()
#include <cstring> // memset()
#include <iostream>

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

