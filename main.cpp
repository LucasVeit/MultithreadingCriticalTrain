#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "List.h"
#include "Item.h"
//using namespace std;
#define N_WAGONS 10
#define MIN_ITEMS 4
#define MAX_ITEMS 6

void Populate(List<List<Item>>* train, int n_wagon, int min_items, int max_items){
    srand(42);
    int n_items;
    Item item;
    for (int i = 0; i < n_wagon; i++){
        n_items = rand() % (max_items - min_items) + min_items;
        List<Item> wagon;
        for (int j = 0; j < n_items; j++){
            item.setValue(rand() % 1000);
            item.setIsLegal(rand() % 2);
            wagon.insert(item);
            //std::cout << wagon->getData().getFirst()->getData().getValue() << std::endl;
            //std::cout << node->getData().getValue() << std::endl;
            //std::cout << wagon->getData().getSize() << std::endl;
        }       
        train->insert(wagon);
        //std::cout << wagon.getSize() << std::endl;
    }
    std::cout << train->getSize() << std::endl;
}

void Print(List<List<Item>>* train){
    Node<List<Item>>* wagon = train->getFirst();
    while(wagon != nullptr){
        Node<Item>* items = wagon->getData().getFirst();
        while(items != nullptr){
            std::cout << items->getData().getValue() << "-" << items->getData().isLegal() << "  ";
            items = items->getNext();
        }
        std::cout << std::endl;
        wagon = wagon->getNext();
    }
}

void PrintIllegal(List<Item>* illegals){
    Node<Item>* item = illegals->getFirst();
    while(item != nullptr){
        std::cout << item->getData().getValue() << "  " << item->getData().isLegal() << "   ";
        item = item->getNext();
    }
}

void Serial(List<List<Item>>* train, List<Item>* illegals, int* totalValue){
    Node<List<Item>>* wagon = train->getFirst();
    Item item;
    while(wagon != nullptr){
        Node<Item>* items = wagon->getData().getFirst();
        while(items != nullptr){
            if(!items->getData().isLegal()){
                //std::cout << "Removido: " << items->getData().getValue() << std::endl;
                item = wagon->getData().remove(items);
                illegals->insert(item);
            }else{
                totalValue += items->getData().getValue();
            }
            items = items->getNext();
        }
        wagon = wagon->getNext();
    }

}

int main(){
    List<List<Item>>* train = new List<List<Item>>();
    List<Item>* illegals = new List<Item>();
    int* totalValue = 0;
    Populate(train, N_WAGONS, MIN_ITEMS, MAX_ITEMS);
    
    Print(train);

    Serial(train, illegals, totalValue);

        
    Print(train);
    PrintIllegal(illegals);
    return 0;
}