#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED
#include <iostream>
#include "Node.h"

template <typename T> class List{
    Node<T> *first, *last;
    int size;

    public:
    int getSize(){
        return size;
    }
    Node<T>* getFirst(){
        return first;
    }

    List(){
        first = nullptr;
        last = nullptr;
        size = 0;
    };

    // void insert(Node<T> *node){
    //     std::cout << "entrou insert" << std::endl;
    //     if(size == 0){
    //         first = node;
    //         //std::cout << size << std::endl;
    //     }else{
    //         //std::cout << size << std::endl;
    //         last->setNext(node);
    //         node->setPrevious(last);
    //     }      
    //     last = node;
    //     size++;
    //     std::cout << "size: " << size << std::endl;
    // };

    void insert(const T &_data){
        Node<T> *p = new Node<T>();

        p->setData(_data);

        if (size == 0) first = p;
        else{
            last->setNext(p);
            p->setPrevious(last);
        } 
        last = p;
        ++size;
    }


    T remove(Node<T> *node){
        if(node == first){
            //std::cout << "First ";
            node->getNext()->setPrevious(nullptr);
            first = node->getNext();
            //std::cout << "Set first to: " << first << " ";
        }else if(node == last){
            //std::cout << "Last ";
            node->getPrevious()->setNext(nullptr);
            last = node->getPrevious();
        }else{
            //std::cout << "Meio ";
            node->getPrevious()->setNext(node->getNext());
            node->getNext()->setPrevious(node->getPrevious());
        }
        std::cout << "first: " << first->getData().getValue() << std::endl;
        size--;
        return node->getData();
    };

    ~List(){

    };
};

#endif