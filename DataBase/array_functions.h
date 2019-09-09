#ifndef ARRAY_FUNCTIONS_H
#define ARRAY_FUNCTIONS_H

#include <cassert>
#include <vector>
#include <iostream>

using namespace std;


template <class T>
T maximal(const T& a, const T& b)        //return the larger of the two items
{
    return a > b ? a:b;
}

template <class T>
void swap(T& a, T& b)  //swap the two items
{

    T temp = a;
    a = b;
    b = temp;
}

template <class T>
int index_of_maximal(T data[ ], int n) //return index of the largest item in
                                       //data
{
    int largest_index = 0;

    for(int i = 1; i < n; i++)
    {
        largest_index = data[i] > data[largest_index] ? i:largest_index;
    }

    return largest_index;
}

template <class T>
void ordered_insert(T data[ ], int& n, T entry) //insert entry into the sorted
                                                //array data with length n
{

    //Increase the size by 1
    n += 1;

    int index = 0;
    int entry_position = 0;

    //Check until n - 1 since the only entries it can be are n - 1
    while(index < n - 1)
    {
        //Find where the index for entry should be
        if(entry > data[index])
        {
            entry_position++;
        }

        index++;
    }

    //The entry position is the index of where entry should be so everything
    //else that is to the right of it will move over by 1 space

    //from last index move all the way until where it should be inserted
    for(int i = n - 1; i > entry_position; i--)
    {
        data[i] = data[i - 1];
    }

    //sets where the entry should be in the array
    data[entry_position] = entry;

}

template <class T>
int first_ge(const T data[ ], int n, const T& entry)   //return the first
{                                                      //element in data that
                                                       //is not less than entry

    //Finds the first element bigger than entry
    for(int i = 0; i < n; i++)
    {
        if(data[i] >= entry)
            return i;
    }

    //if nothing is found
    return n;
}

template <class T>
void attach_item(T data[ ], int& n, const T& entry)    //append entry to the
                                                       //right of data
{
    //Increase size of array
    n += 1;

    //set the last element before size to entry
    data[n - 1] = entry;
}

template <class T>
void insert_item(T data[ ], int i, int& n, T entry)    //insert entry at
                                                       //index i in data
{

    //Increase size of array
    n += 1;

    //The entry position is the index of where entry should be so everything
    //else that is to the right of it will move over by 1 space
    //from last index move all the way until where it should be inserted
    for(int j = n - 1; j > i; j--)
    {
        data[j] = data[j - 1];
    }
    //sets where the entry should be in the array
    data[i] = entry;
}

template <class T>
void detach_item(T data[ ], int& n, T& entry)      //remove the last
                                                   //element in data and place
                                                   //it in entry
{
    //stores the removed item into entry
    entry = data[n - 1];

    //decrease the size of array
    n -= 1;
}

template <class T>
void delete_item(T data[ ], int i, int& n, T& entry)   //delete item at index
                                                       //i and place it in
                                                       //entry
{
    //Places the deleted item into entry
    entry = data[i];

    //Shift the items after i over by 1;
    for(int j = i; j < n; j++)
    {
        data[j] = data[j + 1];
    }

    //Remove one from the size of data
    n -= 1;
}

template <class T>
void merge(T data1[ ], int& n1, T data2[ ], int& n2)   //append data2 to the
                                                       //right of data1
{
    //Append the elements from data 2 to data 1
    for(int i = 0; i < n2; i++)
    {
        //Begins at the end of data 1
        data1[n1 + i] = data2[i];
    }

    //Change the size of the array
    n1 += n2;
    n2 = 0;
}

template <class T>
void split(T data1[ ], int& n1, T data2[ ], int& n2)   //move n/2 elements from the right of data1
                                                        //and move to data2
{

    int index = 0;

    for(int i = (n1 + 1)/2; i < n1; i++)
    {
        insert_item(data2,index,n2,data1[i]);
        index++;
    }

    n1 = n1 - (n1 / 2);
}

template <class T>
void copy_array(T dest[], const T src[],
                int& dest_size, int src_size)              //copy src[] into dest[]
{
    //Make them the same size
    dest_size = src_size;

    for(int i = 0; i < dest_size; i++)
        dest[i] = src[i];
}

template <class T>
void print_array(const T data[], int n, int level = -1)  //print array data
{
    for(int i = n - 1; i > -1; i--)
    {
        cout<<setw(level * 6) << data[i]<< endl;
    }
}

template <class T>
bool is_gt(const T data[], int n, const T& item)       //item > all data[i]
{
    if(n < 1)
    {
        return item > data[n];
    }
    else
    {
        return is_gt(data, n - 1, item) && item > data[n - 1];
    }
}

template <class T>
bool is_le(const T data[], int n, const T& item)       //item <= all data[i]
{
    if(n < 1)
    {
        return item <= data[n];
    }
    else
    {
        return is_le(data, n - 1, item) && item <= data[n - 1];
    }
}

//-------------- Vector Extra operators: ---------------------

template <typename T>
ostream& operator <<(ostream& outs, const vector<T>& list) //print vector list
{
    for(int i = 0; i < list.size(); i++)
    {
        outs << list[i] << " ";
    }

    return outs;
}

template <typename T>
vector<T>& operator +=(vector<T>& list, const T& addme) //list.push_back addme
{
    list.push_back(addme);

    return list;
}

//Generates random numbers
int Random(int lo, int hi)
{
    int r = rand()%(hi-lo+1)+lo;

    return r;
}
#endif // ARRAY_FUNCTIONS_H
