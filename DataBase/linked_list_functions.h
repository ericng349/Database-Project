#ifndef LINKED_LIST_FUNCTIONS_H
#define LINKED_LIST_FUNCTIONS_H

#include <iostream>
#include <cassert>

using namespace std;

template <class T>
struct node{
    T _item;
    node<T>* _next;
    node(const T& item = T(), node<T>* next = NULL):_item(item), _next(next){}
    friend ostream& operator << (ostream& outs, const node<T>& print_me){
        outs<<"["<<print_me._item<<"]->";
        return outs;
    }
};

//initializes head to NULL
template <class T>
node<T>* init_head(node<T>* &head)
{
    head = NULL;

    return head;
}


//deletes all the nodes in the list
template<class T>
void delete_all(node<T>*&head)
{
    node<T>* temp;
    while(head != NULL)
    {
        temp = head;
        head = head->_next;
        delete temp;
    }
}

//true if head is NULL, false otherwise.
template <class T>
bool empty(const node<T>* head)
{
    return head == NULL;
}

//makes a copy of the list, returns a pointer to the last node:
template <class T>
node<T>* copy_list(const node<T>* head, node<T>* & cpy)
{
    if(head != NULL)
    {
        node<T>* walker = new node<T>(head->_item);
        cpy = walker;
        while(head->_next != NULL)
        {
            head = head->_next;
            node<T>* temp = new node<T>(head->_item);
            walker->_next = temp;
            walker = temp;
        }
        return walker;
    }
    else
    {
        cpy = NULL;
        return cpy;
    }
}

//insert at the beginning of the list:
template <class T>
node<T>* insert_head(node<T>* &head, T item)
{
    if(head != NULL)
    {
        node<T>* temp = new node<T>(item);
        temp->_next = head;
        head = temp;
        return head;
    }
    else
    {
        head = new node<T>(item);
        return head;
    }
}

//insert_after: if after is NULL, inserts at head
template <class T>
node<T>* insert_after(node<T>* &head, node<T>* after, const T& item)
{
    if(head != NULL)
    {
        node<T>* temp = new node<T>(item);
        after->_next = temp;
        temp->_next = NULL;
        after = temp;

        return after;
    }
    else
    {
        head = new node<T>(item);
        after = head;
        return after;
    }
}

//delete the node at the head of the list, return the item:
template <class T>
T delete_head(node<T>* &head)
{
    if(head != NULL)
    {
        T item = head->_item;

        node<T>* temp = head->_next;
        delete head;
        head = temp;

        return item;
    }
    else
    {
        assert(!(head == NULL));
        T item;
        return item;
    }
}

//print the list and return outs
template<class T>
ostream& print_list(const node<T>* head, ostream& outs=cout)
{
    while(head != NULL)
    {
        outs<<*head;
        head = head->_next;
    }
    return outs;
}

#endif // LINKED_LIST_FUNCTIONS_H
