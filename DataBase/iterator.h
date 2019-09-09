#ifndef ITERATOR_H
#define ITERATOR_H

#include "linked_list_functions.h"

template <class T>
class Iterator{
    public:
        Iterator() : current(NULL){}
        Iterator(node<T>* other);
        bool operator ==(const Iterator<T>& rhs) const;
        bool operator !=(const Iterator<T>& rhs) const;
        node<T> operator *();
        Iterator<T> operator++();
    private:
        node<T>* current;
};

/**
 * Iterator(node<T>* other)
 * input: other - the first node of the list
 * return: none
 * task: makes current point to the first node of the other list
 */
template <class T>
Iterator<T>::Iterator(node<T>* other)
{
    //Making current point to the first node of the other list
    current = other;
}

/**
 * operator ==
 * input: rhs - the Iterator object to compare to
 * return: true or false depending if the lists are the same
 * task: returns true or false based on if the two iterators
 * current point to the same node
 */
template <class T>
bool Iterator<T>::operator ==(const Iterator<T>& rhs) const
{
    return current == rhs.current;
}

/**
 * operator !=
 * input: rhs - the Iterator object to compare to
 * return: true or false depending if the lists are not the same
 * task: returns true or false based on if the two iterators
 * current point to different node
 */
template <class T>
bool Iterator<T>::operator!=(const Iterator<T>& rhs) const
{
    return current != rhs.current;
}

/**
 * operator *
 * input: none
 * return: the node that it is pointing to
 * task: returns the node that it is pointing to so that it can be
 * displayed
 */
template <class T>
node<T> Iterator<T>::operator *()
{
    return *current;
}

/**
 * operator++()
 * input: none
 * return: the iterator object
 * task: returns the iterator object after it has moved current
 * to the next node in the list
 */
template <class T>
Iterator<T> Iterator<T>::operator++()
{
    current = current->_next;
    return *this;
}

#endif // ITERATOR_H
