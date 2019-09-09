#ifndef MPAIR_H
#define MPAIR_H

#include "array_functions.h"

template <typename K, typename V>
struct MPair{
    K key;
    vector<V> value_list;

    //------------------------------------------------------------------------
    /*      MPair CTORs:
     *  must have these CTORs:
     *  - default CTOR / CTOR with a key and no value: this will create an
     *    empty vector
     *  - CTOR with a key AND a value: pushes the value into the value_list
     *  _ CTOR with a key and a vector of values (to replace value_list)
     */
    //------------------------------------------------------------------------
    MPair(const K& k=K());
    MPair(const K& k, const V& v);
    MPair(const K& k, const vector<V>& vlist);
    //------------------------------------------------------------------------

    //You'll need to overlod << for your vector:
    friend ostream& operator <<(ostream& outs, const MPair<K, V>& print_me)
    {
        outs << "{ " << print_me.key << " : " << print_me.value_list << " }";
        return outs;
    }

    friend bool operator ==(const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        return lhs.key == rhs.key;
    }

    friend bool operator !=(const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        return lhs.key != rhs.key;
    }

    friend bool operator < (const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        return lhs.key < rhs.key;
    }

    friend bool operator <= (const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        return lhs.key <= rhs.key;
    }

    friend bool operator >= (const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        return lhs.key >= rhs.key;
    }

    friend bool operator > (const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        return lhs.key > rhs.key;
    }

    friend MPair<K, V> operator + (const MPair<K, V>& lhs, const MPair<K, V>&
                                   rhs)
    {
        MPair<K,V> temp;
        temp.key = lhs.key;
        for(int i = 0; i < lhs.value_list.size();i++)
        {
            temp.value_list += lhs.value_list[i];
        }
        for(int i = 0; i < rhs.value_list.size();i++)
        {
            temp.value_list += rhs.value_list[i];
        }

        return temp;

    }
};


template <typename K, typename V>
MPair<K,V>::MPair(const K& k)
{
    key = k;
}

template <typename K, typename V>
MPair<K,V>::MPair(const K& k, const V& v)
{
    key = k;
    value_list.push_back(v);
}

template <typename K, typename V>
MPair<K,V>::MPair(const K& k, const vector<V>& vlist)
{
    key = k;

    //replaces current value list
    value_list.clear();
    for(int i = 0; i < vlist.size();i++)
    {
        value_list.push_back(vlist[i]);
    }
}
#endif // MPAIR_H
