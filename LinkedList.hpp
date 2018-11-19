#ifndef LINKEDLIST_HPP
#define	LINKEDLIST_HPP

#include <iostream>
#include "Node.hpp"

using namespace std;

/*....DO NOT EDIT BELOW....*/
template <class T>
class LinkedList {
private:
    Node<T>* head;
    int length;
public:

    LinkedList();
    LinkedList(const LinkedList<T>& ll);
    LinkedList<T>& operator=(const LinkedList<T>& ll);
    ~LinkedList();


    Node<T>* getHead() const;
    Node<T>* first() const;
    Node<T>* findPrev(const T& data) const;
    Node<T>* findNode(const T& data) const;
    void insertNode(Node<T>* prev, const T& data);
    void deleteNode(Node<T>* prevNode);
    void clear();
    size_t getLength() const;
    void print() const;
    void swap(int index1, int index2);
};

template <class T>
void LinkedList<T>::print() const {
    const Node<T>* node = first();
    while (node) {
        std::cout << node->getData();
        node = node->getNext();
    }
    cout << std::endl;
}

/*....DO NOT EDIT ABOVE....*/

/* TO-DO: method implementations below */

template <class T>
LinkedList<T>::LinkedList () {
    head = new Node<T>();
    length = 0;
}

template<class T>
LinkedList<T>::LinkedList(const LinkedList<T> &ll) {
    head = new Node<T>();
    length = ll.getLength();
    if (ll.first()) {
        Node<T> *newNode = new Node<T>(ll.first()->getData());
        head->setNext(newNode);

        for (Node<T> *temp = ll.first()->getNext(); temp; temp = temp->getNext()) {
            newNode->setNext(new Node<T>(temp->getData()));
            newNode = newNode->getNext();
        }
    }
}

template<class T>
LinkedList<T> &LinkedList<T>::operator=(const LinkedList<T> &ll) {
    if (this != &ll) {
        clear();
        const Node<T> *r = ll.first(); // Temp node to iterate through rhs
        Node<T> *p = head;
        while(r) {
            insertNode(p, r->getData());
            p = p->getNext();
            r = r->getNext();
        }
        length = ll.getLength();
    }
    return *this;
}

template<class T>
LinkedList<T>::~LinkedList() {
    clear();
    delete head;
}

template<class T>
Node<T> *LinkedList<T>::getHead() const {
    return head;
}

template<class T>
Node<T> *LinkedList<T>::first() const {
    return head->getNext();
}

template<class T>
Node<T> *LinkedList<T>::findPrev(const T &data) const {
    Node<T>* prev = head;

    while (prev->getNext()) {
        if (prev->getNext()->getData() == data) {
            return prev;
        }
        prev = prev->getNext();
    }
    return NULL;
}

template<class T>
Node<T> *LinkedList<T>::findNode(const T &data) const {
    Node<T> *temp = first();
    while (temp) {
        if (temp->getData() == data) return temp;
        temp = temp->getNext();
    }
    return NULL;
}

template<class T>
void LinkedList<T>::insertNode(Node<T> *prev, const T &data) {
    if (prev) {
        Node<T>* newNode = new Node<T>(data); // Create the new node
        if (prev->getNext()) { // Insert in the middle
            Node<T> *temp = prev->getNext();
            prev->setNext(newNode);
            newNode->setNext(temp);
        }
        else { // Insert at the end
            prev->setNext(newNode);
        }

        length++;
    }
}

template<class T>
void LinkedList<T>::deleteNode(Node<T> *prevNode) {
    if(prevNode) {
        Node<T>* temp = prevNode->getNext();
        if (temp) {
            Node<T>* a = temp->getNext();
            prevNode->setNext(a);
            delete temp;
            length--;
        }
    }
}

template<class T>
void LinkedList<T>::clear() {
    for (Node<T> *next, *temp = first(); temp; temp = next) {
        next = temp->getNext();
        delete temp;
    }
    length = 0;
}

template<class T>
size_t LinkedList<T>::getLength() const {
    return (size_t) length;
}

template<class T>
void LinkedList<T>::swap(int index1, int index2) { // Pointer swap

    Node<T>* node1, *node1prev, *node2, *node2prev;
    // Find the node with index1 and its previous
    node1prev = head;
    node1 = node1prev->getNext();
    for (int i = 0; i < index1; i++) {
        node1prev = node1prev->getNext();
        node1 = node1->getNext();
    }

    // Find the node with index2 and its previous
    node2prev = node1prev;
    node2 = node2prev->getNext();
    for (int i = index1; i < index2; i++) {
        node2prev = node2prev->getNext();
        node2 = node2->getNext();
    }

    if (node1->getNext() == node2) { // Exceptional case
        node1prev->setNext(node2);
        node1->setNext(node2->getNext());
        node2->setNext(node1);
    }
    else if (node2->getNext() == node1){ // Exceptional case
        node2prev->setNext(node1);
        node2->setNext(node1->getNext());
        node1->setNext(node2);
    }
    else { // Regular case where swap nodes are not neighbors
        Node<T>* temp = node2->getNext();
        node2->setNext(node1->getNext());
        node2prev->setNext(node1);
        node1->setNext(temp);
        node1prev->setNext(node2);
    }
}

/* end of your implementations*/
#endif
