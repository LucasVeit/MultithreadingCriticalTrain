#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

template <typename T> class Node{
    T data;
    Node<T> *previous, *next;
    public:
        Node(){
            previous = nullptr;
            next = nullptr;
        };

        void setData(const T &_data){
            data = _data;
        }

        T& getData(){
            return data;
        };

        void setPrevious(Node<T>* _previous){
            previous = _previous;
        };

        void setNext(Node<T>* _next){
            next = _next;
        };

        Node<T>* getPrevious(){
            return previous;
        }

        Node<T>* getNext(){
            return next;
        }

        ~Node(){
            previous = nullptr;
            next = nullptr;
            
        };



};

#endif