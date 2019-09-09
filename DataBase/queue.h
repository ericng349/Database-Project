#ifndef QUEUE_H
#define QUEUE_H

#include "linked_list_functions.h"
#include "iterator.h"

/**
 *  Queue class
 *  ----------
 *  This class will use the linked list functions in order to perform
 *  different tasks such as pushing, popping, whether the queue is empty
 *  and displaying the front of the queue. The Queue is a linked list
 *  in which the first node in is also the first node out when popped.
 *
 */

template <class T>
class Queue{
public:
    Queue():_head(NULL), _tail(NULL){}

    ~Queue();
    Queue(const Queue<T>& other);
    Queue<T> &operator =(const Queue& rhs);

    void push(T item);
    T pop();
    bool empty();
    T front();
    Iterator<T> begin();
    Iterator<T> end();

    friend ostream& operator <<(ostream& outs, const Queue& q){
        return print_list(q._head, outs);

    }

private:
    node<T>* _head;
    node<T>* _tail;
};

/**
 * ~Queue()
 * input: none
 * return: none
 * task: deallocates the dynamic memory used, when out of scope
 */
template <class T>
Queue<T>::~Queue()
{
    delete_all(_head);
    _tail = NULL;
}

/**
 * Queue(const Queue<T>& other)
 * input: other - the Queue to be copied
 * return: none
 * task: creates a dynamic copy of the other Queue
 */
template <class T>
Queue<T>::Queue(const Queue<T>& other)
{
    //deallocate memory
    init_head(_head);
    _tail = NULL;

    //allocate new memory
    _tail = copy_list(other._head,_head);
}

/**
 * operator =(const Queue& rhs)
 * input: rhs - other queue object to copy
 * return: the same queue object after it has copied the items
 * task: creates a dynamic copy of the other Queue
 */
template <class T>
Queue<T>& Queue<T>::operator =(const Queue& rhs)
{
    //Check for self assignment
    if(this == &rhs)
        return *this;

    //deallocate memory
    init_head(_head);

    //allocate new memory
    _tail = copy_list(rhs._head,_head);

    return *this;
}

/**
 * push(T item)
 * input: item - the item to set for the new node
 * return: none
 * task: adds a new node to the end of the queue with the item inside
 */
template <class T>
void Queue<T>::push(T item)
{
    _tail = insert_after(_head,_tail,item);
}

/**
 * pop()
 * input: none
 * return: the item in the first node of the queue
 * task: removes the first node of the queue
 */
template <class T>
T Queue<T>::pop()
{
    return delete_head(_head);
}

/**
 * empty()
 * input: none
 * return: true or false based on whether the queue is empty
 * task: returns true or false depending if the current queue is empty
 */
template <class T>
bool Queue<T>::empty()
{
    return ::empty(_head);
}


/**
 * front()
 * input: none
 * return: the item of the first node in the queue
 * task: displays the item at the head of the queue (also considered front
 * of the queue)
 */
template <class T>
T Queue<T>::front()
{
    assert(!(_head == NULL));
    return _head->_item;
}

/**
 * begin()
 * input: none
 * return: an iterator that has a copy of the list
 * task: creates a iterator provided the head node
 */
template <class T>
Iterator<T> Queue<T>::begin()
{
    return Iterator<T>(_head);
}

/**
 * end()
 * input: none
 * return: an iterator that has a copy of the list
 * task: creates a iterator provided from null (end of the list)
 */
template <class T>
Iterator<T> Queue<T>::end()
{
    return Iterator<T>();
}
#endif // QUEUE_H
