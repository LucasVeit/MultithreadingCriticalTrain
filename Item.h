#ifndef ITEM_H_INCLUDED
#define ITEM_H_INCLUDED
#include <iostream>

class Item{
    int value;
    bool legal;
    public:
        Item(){};
        
        int getValue(){
            return value;
        };

        void setValue(int _value){
            value = _value;
        };

        bool isLegal(){
            return legal;
        };

        void setIsLegal(bool _legal){
            legal = _legal;
        };

        void print(){
            std::cout << "Value: " << value << std::endl;
            std::cout << "Legal: " << legal << std::endl;
        }
};

#endif