#ifndef PARTNER_H
#define PARTNER_H
#include "partner.h"
#endif
#include <cstring>

// в качестве метода разрешения коллизий выберу метод цепочек
struct HashNode {
    Partner data;
    HashNode* next;

    HashNode(const Partner& p) : data(p), next(nullptr) {}
};

class HashTable {
private:
    HashNode** table; // массив указателей на начала цепочек
    int capacity; // размер таблицы (по умолчанию будет простое число, чтобы меньше коллизий было)
    int collisions;   // счётчик коллизий
    

    // Полиномиальный хэш из презентации:
    // h(S) = S[0] + S[1]*P + S[2]*P^2 + ...
    // P = 31
    // Результат по модулю capacity, чтобы получить индекс
    int hash(const char* str) const {
        const int P = 31;
        long long h = 0;
        long long p_pow = 1;
        for (int i = 0; str[i] != '\0'; i++) {
            // str[i] - 'a' + 1 чтобы у строки "aaa" хэш был ненулевой
            // но у нас могут быть любые символы, поэтому просто берём код
            h += (unsigned char)str[i] * p_pow;
            p_pow *= P;
        }
        // модуль
        int index = (int)(h % capacity);
        if (index < 0) index += capacity;
        return index;
    }

public:
    HashTable(int cap = 100003) : capacity(cap), collisions(0) {
        table = new HashNode*[capacity];
        for (int i = 0; i < capacity; i++)
            table[i] = nullptr;
    }

    ~HashTable() {
        for (int i = 0; i < capacity; i++) {
            // удаляю всю цепочку
            HashNode* cur = table[i];
            while (cur != nullptr) {
                HashNode* next = cur->next;
                delete cur;
                cur = next;
            }
        }
        delete[] table;
    }


    void insert(Partner& p) {
        int index = hash(p.getOrg());
        HashNode* node = new HashNode(p);

        if (table[index] != nullptr &&
            strcmp(table[index]->data.getOrg(), p.getOrg()) != 0)
        {
            collisions++;
        }
        // вставка в начало цепочки
        node->next = table[index];
        table[index] = node;
    }

    int searchAll(const char* key, Partner* result, int maxSize) const {
        int count = 0;
        int index = hash(key);

        // по всей цепочке проход
        HashNode* cur = table[index];
        while (cur != nullptr) {
            if (strcmp(cur->data.getOrg(), key) == 0) {
                if (count < maxSize)
                    result[count++] = Partner(cur->data);
            }
            cur = cur->next;
        }

        return count;
    }

    int getCollisions() const { return collisions; }
};

