#ifndef LIST_H
#define LIST_H

#include <iostream>

using namespace std;

#include "linked_list_functions.h"
#include "iterator.h"

/**
 *  List class
 *  ----------
 *  This class will create a list class that uses the linked list functions
 *  in order to perform different tasks such as pushing, popping, empty, and
 *  displaying the head node. (also defines big 3 so that new lists can be
 *  created)
 *
 */

template <class T>
class List{
    public:
        List():_top(NULL){}

        ~List();
        List(const List<T>& other);
        List<T> &operator =(const List& rhs);

        void pushTop(T item);

        T popTop();
        bool isEmpty();
        T displayHead();

        Iterator<T> begin();
        Iterator<T> end();

        friend ostream& operator <<(ostream& outs, const List& q){
            return print_list(q._top, outs);
        }

    private:
        node<T>* _top;
};

/**
 * List(const List<T>& other)
 * input: other - the list to be copied
 * return: none
 * task: creates a dynamic copy of the other list
 */

template <class T>
List<T>::List(const List<T>& other)
{
    //sets head to null in order to initialize head and make sure
    //it isn't pointing to junk
    init_head(_top);

    //creates a copy of the list and returns the last node (bottom
    //just holds the last node)
    node<T>* _bottom = copy_list(other._top,_top);
}

/**
 * ~List()
 * input: none
 * return: none
 * task: deallocates the dynamic memory used, when out of scope
 */
template <class T>
List<T>::~List()
{
    delete_all(_top);
}

/**
 * operator =(const List& rhs)
 * input: rhs - other list object to copy
 * return: a dynamic copy of the rhs list
 * task: creates a dynamic copy of the other list
 */
template <class T>
List<T>& List<T>::operator =(const List& rhs)
{
    //Check for self assignment
    if(this == &rhs)
        return *this;

    //sets head to null in order to initialize head and make sure
    //it isn't pointing to junk
    init_head(_top);

    //creates a copy of the list and returns the last node (bottom
    //just holds the last node)
    node<T>* _bottom = copy_list(rhs._top,_top);

    return *this;
}

/**
 * pushTop(T item)
 * input: item - the item to set for the new node
 * return: none
 * task: adds a new node to the front of the list with the item inside
 */
template <class T>
void List<T>::pushTop(T item)
{
      _top = insert_head(_top,item);
}

/**
 * popTop()
 * input: none
 * return: the item in the last node of the list
 * task: removes the last node of the list
 */
template <class T>
T List<T>::popTop()
{
    return delete_head(_top);
}

/**
 * isEmpty()
 * input: none
 * return: true or false based on whether the list is empty
 * task: returns true or false depending if the current list is empty
 */
template <class T>
bool List<T>::isEmpty()
{
    return empty(_top);
}

/**
 * displayHead()
 * input: rhs - other list object to copy
 * return: the first node of the list
 * task: returns the first item on the list (since we're using list to
 * help stack, in this case it would be the top of the list)
 */
template <class T>
T List<T>::displayHead()
{
    assert(!(_top == NULL));
    return _top->_item;
}

/**
 * begin()
 * input: none
 * return: an iterator that has a copy of the list
 * task: creates a iterator provided the head node
 */
template <class T>
Iterator<T> List<T>::begin()
{
    return Iterator<T>(_top);
}

/**
 * end()
 * input: none
 * return: an iterator that has a copy of the list
 * task: creates a iterator provided from null (end of the list)
 */
template <class T>
Iterator<T> List<T>::end()
{
    return Iterator<T>(NULL);
}
#endif // LIST_H
