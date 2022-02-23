#include <iostream>
#include <chrono>
#include <string.h>
#include "List.h"
#include "Item.h"
//using namespace std;
#define N_WAGONS 8
#define MIN_ITEMS 20000000
#define MAX_ITEMS 30000000
#define SEED 42
//#define N_THREAD 8

#include <thread>
#include <mutex>
std::mutex illegalWagon;
std::mutex countingValue;

void Populate(List<List<Item>>* train, int n_wagon, int min_items, int max_items){
    srand(SEED);
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
        while(wagon->getData().getSize() == 0) wagon = wagon->getNext();

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
        //std::cout << "Times " << std::endl;
        Node<Item> *tempNext, *items = wagon->getData().getFirst();
        while(items != nullptr){
            tempNext = items->getNext();
            if(!items->getData().isLegal()){
                item = wagon->getData().remove(items);
                illegals->insert(item);
            }else{
                totalValue += items->getData().getValue();
            }
            items = tempNext;
        }
        wagon = wagon->getNext();
        //std::cout << "Wagon" << wagon << std::endl;
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

    //Controla a criação e   acesso de threads
    Node<List<Item>>* wagon = train->getFirst();
    std::thread threads[N_WAGONS];
    for(int i = 0; wagon != nullptr; ++i, wagon = wagon->getNext()){
        threads[i]= std::thread(IndividualThread, wagon, illegals, totalValue);
    }

    for(int i = 0; i < N_WAGONS; ++i){
        threads[i].join();
    }
}

int main(int argc, char *argv[]){
    List<List<Item>>* train = new List<List<Item>>();
    List<Item>* illegals = new List<Item>();
    int* totalValue = 0;

    std::cout << "Populating with pseudo-random values..." << std::endl;
    Populate(train, N_WAGONS, MIN_ITEMS, MAX_ITEMS);
    
    if(!strcmp(argv[1], "-Serial")){
        std::cout << "Executing Serial algorithm..." << std::endl;
        auto start = std::chrono::high_resolution_clock::now();
        Serial(train, illegals, totalValue);
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        std::cout << "Time taken by Serial function: " << duration.count() << " microseconds" << std::endl;
    }else if(!strcmp(argv[1], "-Parallel")){
        std::cout << "Executing Parallel algorithm..." << std::endl;
        auto start = std::chrono::high_resolution_clock::now();
        Parallel(train, illegals, totalValue);
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        std::cout << "Time taken by Parallel function: " << duration.count() << " microseconds" << std::endl;
    }else{
        std::cout << "Enter a valid parameter" << std::endl << "-Serial || -Parallel" << std::endl;
    }
   
    delete train;
    delete illegals;
    
    return 0;
}