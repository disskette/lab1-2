#ifndef PARTNER_H
#define PARTNER_H
#include "partner.h"
#endif
#include <cstring> 

// цвет
enum Color { RED, BLACK };

struct RBNode {
    Partner data;
    Color color;
    RBNode* left;
    RBNode* right;
    RBNode* parent;

    RBNode(const Partner& p)
        : data(p), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

class RBTree {
private:
    RBNode* root;
    RBNode* dummy; // фиктивный узел для пустых листьев и "родителя" корня. У них родители - они сами

    // повороты
    void rotateLeft(RBNode* x) {
        RBNode* y = x->right; // Этот узел станет родителем x
        x->right = y->left; // по картинке переношу узел B от k2 к k1

        if (y->left != dummy) // если B оказался не пустым, то для него указываю нового родителя (k1)
            y->left->parent = x;
        y->parent = x->parent; // Передаю родителя для k2
        if (x->parent == dummy) // Если k1 на самом деле корень
            root = y; // То для всего дерева указываю, что y - корень
        else if (x == x->parent->left) // Если у родителя k1 был левым потомком
            x->parent->left = y; // То левым потомком становится k2
        else
            x->parent->right = y; // Иначе - правым
        y->left = x; // по картинке k1 - левый потомок k2
        x->parent = y; // k2 - родитель k1
    }

    void rotateRight(RBNode* x) {
        RBNode* y = x->left;
        x->left = y->right;
        if (y->right != dummy)
            y->right->parent = x;
        y->parent = x->parent;
        if (x->parent == dummy)
            root = y;
        else if (x == x->parent->right)
            x->parent->right = y;
        else
            x->parent->left = y;
        y->right = x;
        x->parent = y;
    }

    // восстановление свойств
    void fixInsert(RBNode* z) { //Новый узел всегда красный
        while (z->parent->color == RED) { // двух красных подряд быть не должно
            if (z->parent == z->parent->parent->left) { // отец - левый сын деда
                
                RBNode* uncle = z->parent->parent->right; // дядя

                if (uncle->color == RED) {
                    // Случай 1: дядя красный — перекраска
                    z->parent->color = BLACK; // отца в чёрный
                    uncle->color = BLACK; // дядю тоже
                    z->parent->parent->color = RED; // деда в красный
                    z = z->parent->parent; // поднимаюсь наверх, чтобы проверить деда
                } else { // дядя черный
                    if (z == z->parent->right) { // z - правый потомок
                        z = z->parent; // тогда поворот налево относительно отца
                        rotateLeft(z);
                    }
                    // z - левый сын, дядя чёрный
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rotateRight(z->parent->parent);
                }
            } else { // отец - правый сын деда. 
                RBNode* uncle = z->parent->parent->left;

                if (uncle->color == RED) {
                    // красный дядя - перекраска и подъём к деду
                    z->parent->color = BLACK;
                    uncle->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else { // черный дядя
                    if (z == z->parent->left) {
                        // левый потомок
                        z = z->parent;
                        rotateRight(z); // симметрично крутимся относительно отца
                    }
                    // правый сын, чёрный дядя
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rotateLeft(z->parent->parent);
                }
            }
        }
        root->color = BLACK; // корень всегда чёрный
    }

    // очистка
    void clear(RBNode* node) {
        if (node == dummy) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }

public:
    RBTree() {
        // dummy - чёрный, все указатели на себя. самый первый участник дерева
        dummy = new RBNode(Partner());
        dummy->color = BLACK;
        dummy->left = dummy->right = dummy->parent = dummy;
        root = dummy;
    }

    ~RBTree() {
        clear(root);
        delete dummy;
    }

    // вставка нового узла
    void insert(const Partner& p) {
        RBNode* z = new RBNode(p);
        z->left = z->right = z->parent = dummy; // пока заполняю пустышками

        // вставка по канонам bst
        RBNode* y = dummy; // для запоминания родителя
        RBNode* x = root; // Иду от корня вниз
        while (x != dummy) { 
            y = x; 
            // Сортирую по ключу задания
            int cmp = strcmp(z->data.getOrg(), x->data.getOrg());
            if (cmp < 0)
                x = x->left; // меньшие уходят влево
            else
                x = x->right; // большие уходят вправо. равные тоже
        }
        // нашла место вставки
        z->parent = y; // выдаю свитдеельство о рождении
        if (y == dummy) // случай, если дерево было пустым
            root = z;
        else if (strcmp(z->data.getOrg(), y->data.getOrg()) < 0) 
            y->left = z; // меньший уходит влево
        else
            y->right = z; // больший и равный - вправо

        // восстанавливаю свойства rbtree
        fixInsert(z);
    }

    // поиск всех вхождений
    // результаты сразу в массив
    int searchAll(const char* key, Partner* result, int maxSize) const {
        int count = 0;

        // ищу первый узел с ключом через обычный поиск по bst
        RBNode* x = root;
        RBNode* found = dummy; // самый левый узел с этим ключом

        while (x != dummy) {
            int cmp = strcmp(key, x->data.getOrg());
            if (cmp == 0) { // равны
                found = x;
                x = x->left; // нашли, но идём ещё левее — вдруг есть дубликат
            } else if (cmp < 0) {
                x = x->left;
            } else {
                x = x->right;
            }
        }

        if (found == dummy) return 0; // ничего

        // Поскольку дубликаты уходят вправо при вставке, все они образуют
        // непрерывный правый «хвост» от самого левого совпадения.

        x = found;
        while (x != dummy && strcmp(x->data.getOrg(), key) == 0) {
            if (count < maxSize)
                result[count++] = Partner(x->data);
            x = successor(x);
        }

        return count;
    }

    RBNode* successor(RBNode* x) const {
        if (x->right != dummy) { // спускаюсь вправо, собирая хвостик
            x = x->right;
            while (x->left != dummy) x = x->left;
            return x;
        }
        RBNode* y = x->parent; // подни
        while (y != dummy && x == y->right) {
            x = y;
            y = y->parent;
        }
        return y;
    }
};