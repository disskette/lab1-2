#include "partner.h"
#include <cstring> //strlen, strcpy
#include <iostream>


const char* copyString(const char* to_copy)
{
    if (to_copy == nullptr)
    {
        char* empty = new char[1];
        empty[0] = '\0';
        return empty;
    }
    size_t len = strlen(to_copy);
    char* string_copy = new char[len + 1];
    strcpy(string_copy, to_copy);
    return string_copy; // неявное преобразование char* -> const char* допустимо
}

unsigned long copyPhone(const char* to_copy)
{
    if (to_copy == nullptr)
    {
        return 0;
    }
    short counter = 0;
    unsigned long phone = 0;

    char index = 0;
    char ch='0';
    unsigned long digit = 10000000000; //умножение на разряд для прибавления
    while (counter < 11 || index < strlen(to_copy))
    {
        ch = to_copy[index];
        if (ch==' '||ch=='-' || ch == '+' || ch == '('||ch == ')'||ch == 'x'||ch == '.') //Пропускаю лишние символы
        {
            index++;
            continue;
        }
        if (ch == '+') // +7 - пропускаю два первых символа 
        {
            index++;
            ch = '8';
        }

        phone += (ch-'0')*digit;
        counter+=1; // Всего должно быть 10 цифр
        digit/=10;
        index++;
    }
    return phone; 
}

Partner::Partner(const char* name_new, const char* org, const char* phone_new, const char* email_new)
{
    name = copyString(name_new);
    organisation = copyString(org);
    phone = copyPhone(phone_new);
    email = copyString(email_new);
}

Partner::Partner(const Partner& other)
    : name(copyString(other.name)),
      organisation(copyString(other.organisation)),
      phone(other.phone),
      email(copyString(other.email))
{
}

Partner& Partner::operator=(const Partner& other)
{
    if (this == &other) return *this; // защита от самоприсваивания

    delete[] name;
    delete[] organisation;
    delete[] email;

    name = copyString(other.name);
    organisation = copyString(other.organisation);
    phone = other.phone;
    email = copyString(other.email);

    return *this;
}

Partner::Partner()
    : name(copyString(nullptr)), organisation(copyString(nullptr)), phone(0), email(copyString(nullptr))
{
}

Partner::~Partner()
{
    if (name!=nullptr) delete[] name;
    if (organisation!=nullptr) delete[] organisation;
    phone=0;
    if (email!=nullptr) delete[] email;
};

const char* Partner::getName() {return  this->name;};
const char* Partner::getOrg() {return  this->organisation;};
const char* Partner::getEmail() {return  this->email;};
const unsigned long Partner::getPhone() {return  this->phone;};

bool Partner::operator == (const Partner& other) const
{
    if (strcmp(name, other.name) != 0) return false;
    if (strcmp(organisation, other.organisation)!=0) return false;
    if (phone!= other.phone) return false;
    return true;
}

bool Partner::operator != (const Partner& other) const
{
    if (*this == other) return false;
    return true;
}

bool Partner::operator > (const Partner& other) const
{
    if (*this == other) return false; // Дальше сравнивать незачем

    // Как и описывала, сравниваю первое несовпадающее поле
    if (strcmp(organisation, other.organisation)!=0) return strcmp(organisation, other.organisation)>0; // Зависимость оставшихся полей неважна
    if (strcmp(name, other.name) != 0) return strcmp(name, other.name)>0;
    if (phone >  other.phone) return true;

    // Если все проверки не пройдены, значит, существует хотя бы одно поле первого объекта, меньшее, чем у второго
    return false;
}

bool Partner::operator < (const Partner& other) const
{
    if (*this == other) return false; // Дальше сравнивать незачем
    if (*this > other) return false;
    return true;
}

bool Partner::operator >= (const Partner& other) const
{
    if (*this == other || *this > other) return true; 

    return false;
}

bool Partner::operator <= (const Partner& other) const
{
    if (*this == other || *this < other) return true; 

    return false;
}

bool Partner::setAttr(const char* name_new, const char* org, const char* phone_new, const char* email_new)
{
    if (name_new != nullptr) name = copyString(name_new);
    if (org != nullptr) organisation = copyString(org);
    if (phone_new != nullptr) phone = copyPhone(phone_new);
    if (email_new != nullptr) email = copyString(email_new);
    return true;
}

