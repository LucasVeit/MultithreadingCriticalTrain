#include <iostream>
#include <chrono>
#include "List.h"
#include "Item.h"
//using namespace std;
#define N_WAGONS 100000
#define MIN_ITEMS 1000
#define MAX_ITEMS 2000

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

    auto start = std::chrono::high_resolution_clock::now();

    Serial(train, illegals, totalValue);

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Time taken by function: " << duration.count() << " microseconds" << std::endl;
        
    //Print(train);
    //PrintIllegal(illegals);
    return 0;
}