#include <iostream>
#include <chrono>
#include "List.h"
#include "Item.h"

#include <thread>
#include <mutex>
std::mutex illegalWagon;
std::mutex countingValue;
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
        }       
        train->insert(wagon);
    }
}

void Print(List<List<Item>>* train){
    Node<List<Item>>* wagon = train->getFirst();
    std::cout << "Tamanho Trem "<< train->getSize() << std::endl;
        
    while(wagon != nullptr){
        std::cout << "Tamanho Vagao "<< wagon->getData().getSize() << std::endl;
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

void IndividualThread(Node<List<Item>>* wagon, List<Item>* illegals, int* totalValue){
    unsigned int localValue = 0;
    Item item;
    Node<Item>* items = wagon->getData().getFirst();
    while(items != nullptr){
        if(!items->getData().isLegal()){
            item = wagon->getData().remove(items);

            illegalWagon.lock();
            illegals->insert(item);
            illegalWagon.unlock();
        }else{
            localValue += items->getData().getValue();
        }

        items = items->getNext(); 
    }

    countingValue.lock();
    totalValue += localValue;
    countingValue.unlock();
}

void Parallel(List<List<Item>>* train, List<Item>* illegals, int* totalValue){
    //Controla a criação e acesso de threads
    Node<List<Item>>* wagon = train->getFirst();
    std::thread threads[N_WAGONS];
    for(int i = 0; wagon != nullptr; ++i, wagon = wagon->getNext()){
        threads[i]= std::thread(IndividualThread, wagon, illegals, totalValue);
    }

    for(int i = 0; i < N_WAGONS; ++i){
        threads[i].join();
    }
}

int main(){
    List<List<Item>>* train = new List<List<Item>>();
    List<Item>* illegals = new List<Item>();
    int* totalValue;
    totalValue = 0;
    
    Populate(train, N_WAGONS, MIN_ITEMS, MAX_ITEMS);

    Print(train);
/*
    auto start = std::chrono::high_resolution_clock::now();
   
    std::cout << "Deu Ruim" << std::endl;
    Serial(train, illegals, totalValue);
   
    std::cout << "Deu Ruim" << std::endl;
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Tempo para executar serial: " << duration.count() << " microseconds" << std::endl;
 
      
    std::cout << "Deu Ruim" << std::endl;
    delete train;
    delete illegals;
    train = new List<List<Item>>();
    illegals = new List<Item>();
    *totalValue = 0;
   
    std::cout << "Deu Ruim" << std::endl;
    Populate(train, N_WAGONS, MIN_ITEMS, MAX_ITEMS);

    start = std::chrono::high_resolution_clock::now();

    Parallel(train, illegals, totalValue);

    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Tempo para executar paralelo: " << duration.count() << " microseconds" << std::endl;
 */

    //Print(train);
    //PrintIllegal(illegals);
    return 0;
}