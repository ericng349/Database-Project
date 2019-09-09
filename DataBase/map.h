#ifndef MAP_H
#define MAP_H

#include "pair.h"
#include "BPlusTree.h"

template <typename K, typename V>
class Map
{
    public:

        typedef BPlusTree<Pair<K, V> > map_base;
        class Iterator{
        public:
            friend class Map;
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

            Pair<K, V> operator *()
            {
                return *_it;
            }

            friend bool operator ==(const Iterator& lhs, const Iterator& rhs)
            {
                return lhs._it == rhs._it;
            }

            friend bool operator !=(const Iterator& lhs, const Iterator& rhs)
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


        Map();

    //  Iterators
        Iterator begin();
        Iterator end();

    //  Capacity
        int size() const;
        bool empty() const;

    //  Element Access
        V& operator[](const K& key);
        V& at(const K& key);
        const V& at(const K& key) const;


    //  Modifiers
        void insert(const K& k, const V& v);
        void erase(const K& key);
        void clear();
        V& get(const K& key);

    //  Operations:
        bool contains(const Pair<K, V>& target) const;
        Iterator find(const K& key);

        // I have not writtent hese yet, but we will need them:
        //    int count(const K& key);
        //    lower_bound
        //    upper_bound
        //    equal_range:

        bool is_valid(){return map.is_valid();}

        friend ostream& operator<<(ostream& outs, const Map<K, V>& print_me){
            outs<<print_me.map<<endl;

            return outs;
        }
    private:
        int key_count;
        BPlusTree<Pair<K, V>> map;
};

/**
 *
 */
template <typename K, typename V>
Map<K,V>::Map()
{

    key_count = 0;
}

template <typename K, typename V>
int Map<K,V>::size() const
{
    return map.size();
}

template <typename K, typename V>
bool Map<K,V>::empty() const
{
    return map.empty();
}

template <typename K, typename V>
V& Map<K,V>::operator[](const K& key)
{
    //get function is going all the way to leaf and inserting at leaf
    //but is not fixing it as it goes back up
    if(map.contains(key))
        return map.get(Pair<K,V>(key,V())).value;
    else
    {
        map.insert(Pair<K,V>(key,V()));
        return map.get(Pair<K,V>(key,V())).value;
    }
}

template <typename K, typename V>
V& Map<K,V>::at(const K& key)
{
    if(!map.empty() && map.contains(key))
        return map.get(Pair<K,V>(key,V())).value;
    else
    {
        map.insert(Pair<K,V>(key,V()));
        return map.get(Pair<K,V>(key,V())).value;
    }
}

template <typename K, typename V>
const V& Map<K,V>::at(const K& key) const
{
    return map.get(Pair<K,V>(key,V())).value;
}

template <typename K, typename V>
void Map<K,V>::insert(const K& k, const V& v)
{
   Pair<K,V> entry(k,v);
   map.insert(entry);
}

template <typename K, typename V>
void Map<K,V>::erase(const K& key)
{
    Pair<K,V> entry(key,V());
    map.remove(entry);
}

template <typename K, typename V>
void Map<K,V>::clear()
{
    map.clear_tree();
}

template <typename K, typename V>
V& Map<K,V>::get(const K& key)
{
    map.get(key);
}

template <typename K, typename V>
bool Map<K,V>::contains(const Pair<K, V>& target) const
{
    return map.contains(target);
}

template <typename K, typename V>
typename Map<K,V>::Iterator Map<K,V>::begin(){

    return Iterator(map.begin());
}

template <typename K, typename V>
typename Map<K,V>::Iterator Map<K,V>::end(){
    return Iterator(map.end());
}

template <typename K, typename V>
typename Map<K,V>::Iterator Map<K,V>::find(const K& key){
    return Iterator(map.find(key));
}

#endif // MAP_H
