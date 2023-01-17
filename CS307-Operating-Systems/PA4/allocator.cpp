// Mert Türe

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <pthread.h>
#include <unistd.h>

using namespace std;


pthread_mutex_t lock;

struct Node{
    int id;
    int size;
    int index;

    Node * next;
    Node * prev;

    Node():next(NULL), prev(NULL){}

    Node(int id_, int size_, int index_)
        :id(id_), size(size_), index(index_), next(NULL), prev(NULL){}
};


class HeapManager {
    public:
        HeapManager():head(NULL){}

        int initHeap(int size) {
            head = new Node(-1, size, 0);
            print();
            return 1;
        }

        int myMalloc(int ID, int size) {
            pthread_mutex_lock(&lock);

            Node *currNode = head;
            bool flag = false;
            while(currNode) {
                if (currNode->id == -1 && currNode->size >= size) {
                    int left = currNode->size - size;


                    if (left > 0) {
                        currNode->id = ID;
                        currNode->size = size;
                        Node *right_node = new Node(-1, left, currNode->index + size);
                        right_node->next = currNode->next;
                        right_node->prev = currNode;
                        currNode->next = right_node;

                        if (right_node->next) {
                            right_node->next->prev = right_node;
                        }
                    }
                    else {
                        currNode->id = ID;
                        currNode->size = size;
                    }
                    flag = true;
                    break;
                }
                currNode = currNode->next;
            }

            if (flag) {
                cout << "Allocated for thread " << ID << endl;
                print();
                pthread_mutex_unlock(&lock);
                return currNode->index;
            }
            else {
                cout << "Can not allocate, requested size "<< size << " for thread "<< ID << " is bigger than remaining size " << endl;
                print();
                pthread_mutex_unlock(&lock);
                return -1;
            }

        }

        int myFree (int ID, int index) {
            pthread_mutex_lock(&lock);
            Node *currNode = head;

            bool flag = false;

            while (currNode) {
                if (currNode->id == ID && currNode->index == index) {
                    currNode->id = -1;

                    if (currNode->prev && currNode->prev->id == -1) {

                        currNode->size += currNode->prev->size;

                        currNode->index = currNode->prev->index;
                        
                        if (currNode->prev->prev) {
                            currNode->prev = currNode->prev->prev;
                        }
                        else {
                            currNode->prev = NULL;
                            head = currNode;
                        }

                        if (currNode->prev) {
                            currNode->prev->next = currNode;
                        }
                        
                    }

                    if (currNode->next && currNode->next->id == -1) {
                        currNode->size += currNode->next->size;

                        if (currNode->next->next) {
                            currNode->next = currNode->next->next;
                        }
                        else {
                            currNode->next = NULL;
                        }

                        if (currNode->next) {
                            currNode->next->prev = currNode;
                        }
                    }
                    flag = true;
                    break;
                }

                currNode = currNode->next;

            }

            if (flag) {
                cout << "Freed for thread " << ID << endl;
                print();
                pthread_mutex_unlock(&lock);
                return 1;
            }
            else {
                cout << "No allocated memory found for thread "<< ID << " at index "<< index  << endl;
                print();
                pthread_mutex_unlock(&lock);
                return -1;
            }
        }

        void print() {
            Node *currNode = head;

            bool first = false;
            while (currNode) {
                if (first) {
                    cout << "---";
                }
                first = true;

                cout << "[" << currNode->id << "]" << "[" << currNode->size << "]" << "[" << currNode->index << "]";
                currNode = currNode->next;
            }
            cout << endl;

        }


    private:
        Node *head;

};