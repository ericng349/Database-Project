#ifndef MMAP_H
#define MMAP_H

#include <vector>
#include "BPlusTree.h"
#include "mpair.h"

template <typename K, typename V>
class MMap
{
    public:

        typedef BPlusTree<MPair<K, V> > map_base;
        class Iterator{
            public:
                friend class MMap;
                Iterator(typename map_base::Iterator it)
                {
                    _it = it;
                }

                Iterator operator ++(int unused)
                {
                    return _it++;
                }

                Iterator operator ++()
                {
                    return ++_it;
                }

                MPair<K, V> operator *()
                {
                    return *_it;
                }

                friend bool operator ==(const Iterator& lhs, const Iterator&
                                        rhs)
                {
                    return lhs._it == rhs._it;
                }

                friend bool operator !=(const Iterator& lhs, const Iterator&
                                        rhs)
                {
                    return lhs._it != rhs._it;
                }
                void print_Iterator()
                {
                    _it.print_Iterator();
                }

            private:
                typename map_base::Iterator _it;
        };

        MMap() : mmap(true){}

    //  Iterators
        Iterator begin();
        Iterator lower_bound(const K& from);
        Iterator upper_bound(const K& to);
        Iterator end();


    //  Capacity
        int size() const;
        bool empty() const;

    //  Element Access
        const vector<V>& operator[](const K& key) const;
        vector<V>& operator[](const K& key);

    //  Modifiers
        void insert(const K& k, const V& v);
        void erase(const K& key);
        void clear();

    //  Operations:
        bool contains(const K& key) const;
        vector<V> &get(const K& key);
        Iterator find(const K& key);

        int count(const K& key);
        // I have not writtent hese yet, but we will need them:
        //    int count(const K& key);
        //    lower_bound
        //    upper_bound
        //    equal_range:



        bool is_valid();

        friend ostream& operator<<(ostream& outs, const MMap<K, V>& print_me){
            outs<<print_me.mmap<<endl;
            return outs;
        }

    private:
        BPlusTree<MPair<K, V> > mmap;
};



template <typename K, typename V>
int MMap<K,V>::size() const
{
    return mmap.size();
}

template <typename K, typename V>
bool MMap<K,V>::empty() const
{
    return mmap.empty();
}

template <typename K, typename V>
const vector<V>& MMap<K,V>::operator[](const K& key) const
{
    if(!mmap.empty() && mmap.contains(key))
        return mmap.get(MPair<K,V>(key,V())).value_list;
    else
    {
        mmap.insert(MPair<K,V>(key,V()));
        return mmap.get(MPair<K,V>(key,V())).value_list;
    }
}


template <typename K, typename V>
vector<V>& MMap<K,V>::operator[](const K& key)
{
    if(!mmap.empty() && mmap.contains(key))
        return mmap.get(MPair<K,V>(key)).value_list;
    else
    {
        mmap.insert(MPair<K,V>(key));
        return mmap.get(MPair<K,V>(key)).value_list;
    }
}

template <typename K, typename V>
void MMap<K,V>::insert(const K& k, const V& v)
{
    //MPair has K key and Vector<V> data

    MPair<K,V> entry(k,v);
    mmap.insert(entry);

}

template <typename K, typename V>
void MMap<K,V>::erase(const K& key)
{
    MPair<K,V> entry(key,V());
    mmap.remove(entry);
}

template <typename K, typename V>
void MMap<K,V>::clear()
{
    mmap.clear_tree();
}

template <typename K, typename V>
bool MMap<K,V>::contains(const K& key) const
{
    return mmap.contains(key);
}

template <typename K, typename V>
vector<V>& MMap<K,V>::get(const K& key)
{
    return mmap.get(key);
}

template <typename K, typename V>
typename MMap<K,V>::Iterator MMap<K,V>::begin(){

    return Iterator(mmap.begin());
}

template <typename K, typename V>
typename MMap<K,V>::Iterator MMap<K,V>::end(){
    return Iterator(mmap.end());
}

template <typename K, typename V>
typename MMap<K,V>::Iterator MMap<K,V>::find(const K& key){
    return Iterator(mmap.find(key));
}

template <typename K, typename V>
bool MMap<K,V>::is_valid()
{
    return mmap.is_valid();
}

//gets the item at the location from
template <typename K, typename V>
typename MMap<K,V>::Iterator MMap<K,V>::lower_bound(const K& from)
{
    return Iterator(mmap.find(from));
}

//gets the item at the location to
template <typename K, typename V>
typename MMap<K,V>::Iterator MMap<K,V>::upper_bound(const K& to)
{
     return Iterator(mmap.find(to));
}

#endif // MMAP_H
