#include <iostream>
#include <chrono>
#include "List.h"
#include "Item.h"
//using namespace std;
#define N_WAGONS 8
#define MIN_ITEMS 10000000
#define MAX_ITEMS 20000000
#define SEED 42
//#define N_THREAD 8

#include <semaphore.h>
#include <thread>
#include <mutex>
std::mutex illegalWagon;
std::mutex countingValue;
sem_t threadCounter;

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
    sem_post(&threadCounter);
}

void Parallel(List<List<Item>>* train, List<Item>* illegals, int* totalValue){

    sem_init(&threadCounter, 0, N_WAGONS);
    //Controla a criação e   acesso de threads
    Node<List<Item>>* wagon = train->getFirst();
    std::thread threads[N_WAGONS];
    for(int i = 0; wagon != nullptr; ++i, wagon = wagon->getNext()){
        sem_wait(&threadCounter);
        threads[i]= std::thread(IndividualThread, wagon, illegals, totalValue);
    }

    for(int i = 0; i < N_WAGONS; ++i){
        threads[i].join();
    }
    sem_destroy(&threadCounter);
}

int main(){
    List<List<Item>>* train = new List<List<Item>>();
    List<Item>* illegals = new List<Item>();
    int* totalValue = 0;
    
    Populate(train, N_WAGONS, MIN_ITEMS, MAX_ITEMS);

    //std::cout << sizeof(*(train->getFirst())) << std::endl << sizeof(*(train->getFirst()->getData().getFirst())) << std::endl;
    //Print(train);

    auto start = std::chrono::high_resolution_clock::now();

    Serial(train, illegals, totalValue);

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Time taken by Serial function: " << duration.count() << " microseconds" << std::endl;

    //Print(train);
    //PrintIllegal(illegals);
    delete train;
    delete illegals;

    train = new List<List<Item>>();
    illegals = new List<Item>();
    totalValue = 0;
    
    Populate(train, N_WAGONS, MIN_ITEMS, MAX_ITEMS);
    

    start = std::chrono::high_resolution_clock::now();

    Parallel(train, illegals, totalValue);

    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Time taken by Parallel function: " << duration.count() << " microseconds" << std::endl;
    return 0;
}