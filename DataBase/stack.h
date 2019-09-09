#ifndef STACK_H
#define STACK_H

#include "list.h"

template <class T>

/**
 *  Stack class
 *  ----------
 *  This class will contain a list and use the list functions in order
 *  to perform different tasks such as pushing, popping, whether the
 *  queue is empty and displaying the front of the queue. The Stack
 *  is a linked list in which the first node in is the last node out
 *  when popped.
 *
 */

class Stack{
public:

    void push(T item);
    T pop();
    T top();
    bool empty();
    Iterator<T> begin();
    Iterator<T> end();
    friend ostream& operator << (ostream& outs, const Stack& s){
        return outs<<s._list;
    }
private:
    List<T> _list;
};

/**
 * push(T item)
 * input: item - the item to set for the new node
 * return: none
 * task: adds a new node to the front of the Stack with the item inside
 */
template <class T>
void Stack<T>::push(T item)
{
    _list.pushTop(item);
}

/**
 * pop()
 * input: none
 * return: the item from the last node
 * task: removes the last node of the Stack
 */
template <class T>
T Stack<T>::pop()
{
    return _list.popTop();
}

/**
 * top()
 * input: none
 * return: item in first node of list
 * task: returns the first node of the list(node that _top is pointint to
 * in the case of a stack)
 */
template <class T>
T Stack<T>::top()
{
    return _list.displayHead();
}

/**
 * empty()
 * input: none
 * return: true of false based on if the stack is empty
 * task: returns true or false depending if the current Stack is empty
 */
template <class T>
bool Stack<T>::empty()
{
    return _list.isEmpty();
}

/**
 * begin()
 * input: none
 * return: an iterator that has a copy of the list
 * task: creates a iterator provided the head node
 */
template <class T>
Iterator<T> Stack<T>::begin()
{
    return _list.begin();
}

/**
 * end()
 * input: none
 * return: an iterator that has a copy of the list
 * task: creates a iterator provided from null (end of the list)
 */
template <class T>
Iterator<T> Stack<T>::end()
{
    return _list.end();
}
#endif // STACK_H
