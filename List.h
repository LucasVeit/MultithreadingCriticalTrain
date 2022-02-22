#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED
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
        T temp = node->getData();
        //std::cout << "Removing: " << node->getData().getValue() << std::endl;
        if(size == 1){
            first = nullptr;
            last = nullptr;
        }else if(node == first){
            node->getNext()->setPrevious(nullptr);
            first = node->getNext();
        }else if(node == last){
            node->getPrevious()->setNext(nullptr);
            last = node->getPrevious();
        }else{
            node->getPrevious()->setNext(node->getNext());
            node->getNext()->setPrevious(node->getPrevious());
        }
        size--;
        delete node;
        return temp;
    };

    ~List(){

    };
};

#endif