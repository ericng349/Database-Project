#ifndef BPlusTree_H
#define BPlusTree_H

#include <iomanip>
#include <cassert>

#include "array_functions.h"

template <class T>
class BPlusTree
{
    public:

        class Iterator{
            public:
                friend class BPlusTree;
                Iterator(BPlusTree<T>* _it = NULL, int _key_ptr = 0)
                         :it(_it), key_ptr(_key_ptr){}

                T operator *(){
                    assert(key_ptr < it->data_count);

                    return it->data[key_ptr];
                }

                Iterator operator++(int un_used)
                {
                    Iterator temp = *this;
                    if(key_ptr + 1 < it->data_count)
                    {
                        key_ptr++;
                    }
                    else
                    {
                        key_ptr = 0;
                        it = it->next;
                    }
                    return temp;
                }

                Iterator operator++()
                {
                    if(key_ptr + 1< it->data_count)
                    {
                        key_ptr++;
                    }
                    else
                    {
                        key_ptr = 0;
                        it = it->next;
                    }

                    return *this;
                }
                friend bool operator ==(const Iterator& lhs, const Iterator&
                                        rhs)
                {
                    return lhs.it->data[lhs.key_ptr] == rhs.it->data[rhs.
                            key_ptr];
                }

                friend bool operator !=(const Iterator& lhs, const Iterator&
                                        rhs)
                {
                    if(rhs.it == NULL)
                        return lhs.it != NULL;
                    else
                    {
                        return lhs.it != rhs.it || lhs.key_ptr != rhs.key_ptr;
                    }

                }
                void print_Iterator()
                {
                    cout << it->data[key_ptr] << " -> ";
                }
                bool is_null(){return !it;}
            private:
                BPlusTree<T>* it;
                int key_ptr;
        };

        BPlusTree(bool dups = false);
        //big three:
        BPlusTree(const BPlusTree<T>& other);
        ~BPlusTree();
        BPlusTree<T>& operator =(const BPlusTree<T>& RHS);

        void insert(const T& entry);            //insert entry into the tree
        void remove(const T& entry);            //remove entry from the tree

        void clear_tree();         //clear this object (delete all nodes etc.)
        void copy_tree(const BPlusTree<T>& other);  //copy other into this
                                                    //object

        bool contains(const T& entry) const; //true if entry can be found in
                                             //the array
        const T& get(const T& entry) const; //return a reference to entry in
                                            //the tree
        T& get(const T &entry);

        int size() const;           //count the number of elements in the tree
        bool empty() const;         //true if the tree is empty

        void create_tree(const int SIZE); //Inserts SIZE random elements into
                                          //the tree

        Iterator begin();
        Iterator end();
        Iterator find(const T& entry);  //return an iterator to this key.
                                        //NULL if not there.

        bool is_valid();

        //void print_tree(BPlusTree<T>* root,int level = 0, ostream &outs=cout)
        //const; //print a readable version of the tree
        void print_tree(int level = 0, ostream &outs=cout) const; //print a
                                                                  //readable
                                                                  //version
                                                                 //of the tree
        friend ostream& operator<<(ostream& outs, const BPlusTree<T>& print_me)
        {
            print_me.print_tree(0, outs);

            return outs;
        }
    private:
        static const int MINIMUM = 1;
        static const int MAXIMUM = 2 * MINIMUM;

        bool dups_ok;                                   //true if duplicate
                                                        //keys may be inserted
        int data_count;                                 //number of data
                                                        //elements
        T data[MAXIMUM + 1];                            //holds the keys
        int child_count = 0;                                //number of children
        BPlusTree* subset[MAXIMUM + 2];                     //suBPlusTrees
        //Array of BPlusTree pointers
        bool is_leaf() const {return child_count==0;}   //true if this is a
                                                        //leaf node

        //insert element functions
        void loose_insert(const T& entry);              //allows MAXIMUM+1
                                                        //data elements in the
                                                        //root
        void fix_excess(int i);                         //fix excess of data
                                                        //elements in child i

        //remove element functions:
        void loose_remove(const T& entry, T* origin = NULL);//allows MINIMUM-1
                                                        //data elements in the
                                                        //root
        BPlusTree<T>* fix_shortage(int i, T* origin = NULL);
                                                        //fix shortage of data
                                                        //elements in child i

        void remove_biggest(T& entry);                  //remove the biggest
                                                        //child of this
                                                        //tree->entry
        void rotate_left(int i);                        //transfer one
                                                        //element LEFT from
                                                        //child i
        void transfer_left(int i);                      //transfer one element
                                                        //LEFT from child i

        void transfer_right(int i);                     //transfer one element
                                                        //RIGHT from child i
        void rotate_right(int i);                       //transfer one element
                                                        //RIGHT from child i
        BPlusTree<T>* merge_with_next_subset(int i);    //merge subset i with
                                                        //subset i+1

        BPlusTree* get_next(const T& entry);            //Gets the tree pointer
                                                        //holding next item

        BPlusTree<T>* get_smallest_node();
        BPlusTree<T>* get_largest_node();

        void copy_next();

        BPlusTree* next;

};

/**
  BPlusTree
  Constructor for the BPlusTree
 * @param dups - whether to allow dups or not
 */
template <typename T>
BPlusTree<T>::BPlusTree(bool dups):dups_ok(dups), data_count(0),
                                   child_count(0), next(NULL)
{
    //left emtpy
    for(int i = 0; i < MAXIMUM + 2; i++)
        subset[i] = NULL;
}


/**
 * BPlusTree(other)
 * The copy constructor that will
 * @param other - copies the other tree
 */
template <typename T>
BPlusTree<T>::BPlusTree(const BPlusTree<T>& other):
                        data_count(0), child_count(0)
{
    //Copy the other tree
    copy_tree(other);

    //Set up the next
    if(child_count != 0)
        copy_next();
}
/**
 * Operator =
 * @param RHS - the right hand side object to copy
 */
template <typename T>
BPlusTree<T>& BPlusTree<T>::operator =(const BPlusTree<T>& RHS)
{
    const bool debug = false;
    if (debug) cout<<"** assignment operator fired!"<<endl;
    //Clear the old tree
    clear_tree();

    //Create a new tree
    copy_tree(RHS);
    //Copy the next's
    if(child_count != 0)
        copy_next();

    return *this;
}

/**
 * ~BPlusTree
 * Destructor method that will clear the tree
 */
template <typename T>
BPlusTree<T>::~BPlusTree(){
    const bool debug = false;
    if (debug) cout<<"** destructor fired!"<<endl;
    clear_tree();
}
//---------------------------------------------------------------
//      P R I N T  E T C.
//---------------------------------------------------------------
template <typename T>
void BPlusTree<T>::print_tree(int level, ostream& outs) const{
    //1. print the last child (if any)
    //2. print all the rest of the data and children

    //Checks if the child count is greater than 1 since it is subtracting 2 to
    //get to the left side
    if(child_count >= 1)
    {
        subset[child_count - 1]->print_tree(level + 1);

        print_array(data,data_count,level);
        //Since this is being printed after we don't want to call
        //child_count - 1 again, so we call everything before it
        //starting from the one to the larger number
        for(int i = child_count - 2; i > -1;i--)
        {
            subset[i]->print_tree(level + 1);

            if(child_count > 2)
                outs << endl;
        }
    }
    //Prints the last child
    else
    {
        print_array(data,data_count,level);
    }

}
//------------------------------------------------
//          I N S E R T
//------------------------------------------------
/**
 * insert(const T& entry)
 *
 * @param entry - the item to insert
 *
 * inserts the entry at it's specified location
 */
template <typename T>
void BPlusTree<T>::insert(const T& entry){
    //in order for this class to be able to keep track of the number of the
    //keys, this function (and the functions
    //  it calls ) must return a success code.
    //If we are to keep track of the number the keys (as opposed to key/values)
    //then the success
    //  code must distinguish between inserting a new key, or adding a new key
    //to the existing key.
    //  (for "dupes_ok")
    //
    //loose_insert this entry into this root.
    //loose_insert(entry) will insert entry into this tree. Once it returns,
    //all the suBPlusTrees are valid
    //  BPlusTree BPlusTrees EXCEPT this root may have one extra data item:
    //    in this case (we have excess in the root)
    //      create a new node, copy all the contents of this root into it,
    //      clear this root node,
    //      make the new node this root's only child (subset[0])
    //
    //Then, call fix_excess on this only subset (subset[0])
    loose_insert(entry);
    //Check if the root needs to be fixed
    if(data_count > MAXIMUM)
    {
        //Creates a temporary BPlusTree pointer
        BPlusTree* temp = new BPlusTree<T>();

        //Copy the tree over
        temp->copy_tree(*this);

        //Set the next for temp
        if(child_count > 0)
            temp->copy_next();

        //Clear the tree
        clear_tree();


        //Set subsets to null
        for(int i = 0; i < MAXIMUM + 2; i++)
            subset[i] = NULL;

        //Set the data count and child count to 1 so we have 1 extra empty
        //spot at the top
        data_count = 0;
        child_count = 1;
        //set the child of new empty array to the old BPlusTree which is temp
        subset[data_count] = temp;
        //Get the first GE
        int i = first_ge(data, data_count, entry);

        //Fix excess on the tree
        fix_excess(i);
    }

}

/**
 * loose_insert(const T& entry)
 *
 * @param entry - the entry to insert
 *
 * this will insert the item in it's location before the the root and fix
 * the excess as it returns from the recursion
 */
template <typename T>
void BPlusTree<T>::loose_insert(const T& entry)
{
    //Gets the first greater than or equal to element

    int i = first_ge(data, data_count, entry);
    //Tests if the element was found
    bool found = (i < data_count && data[i] == entry);

    //If found means that there is a duplicate
    if (found && is_leaf())
    {
        //uses the overloaded + operator
        if(dups_ok)
            data[i] = data[i] + entry;
        else
            data[i] = entry;
    }
    //If found and is not leaf
    if(found && !is_leaf())
    {
        subset[i + 1]->loose_insert(entry);
        if(subset[i + 1]->data_count > MAXIMUM)
        {
            fix_excess(i + 1);
        }
    }
    //Otherwise it will insert it into the leaf
    if (!found && is_leaf())
    {
        ordered_insert(data,data_count,entry);
        i = first_ge(data, data_count, entry);
        found = (i < data_count && data[i] == entry);
        //exit(-1);
    }
    //If its not found
    if (!found && !is_leaf())  //yet...
    {
        subset[i]->loose_insert(entry);
        //Fix excess if the data count is incorrect
        if(subset[i]->data_count > MAXIMUM)
        {
            fix_excess(i);
        }
    }
}

/**
 * fix_excess(int i)
 *
 * @param i - the current index in the tree
 *
 * if the subtree has too many data then it will swap and fix the excess
 * data until the tree is balanced
 */
template <typename T>
void BPlusTree<T>::fix_excess(int i)   //fix excess of data elements in child i
{
    //Create a temporary BPlusTree pointer
    BPlusTree* temp = new BPlusTree<T>(dups_ok);

    //Insert a new blank tree in front of the data
    insert_item(subset,i + 1,child_count,temp);

    //Split the data
    split(subset[i]->data,subset[i]->data_count,subset[i + 1]->data
          ,subset[i + 1]->data_count);

    //Split the child and move it up
    split(subset[i]->subset,subset[i]->child_count,subset[i + 1]->subset
          ,subset[i + 1]->child_count);

    //detatch and insert the number into the parent
    T detatched;
    detach_item(subset[i]->data,subset[i]->data_count,detatched);

    if(subset[i]->is_leaf())
    {
        //Make a copy of the deatatched
        T* copy = new T();
        *copy = detatched;
        //Insert that copy into data[i]
        ordered_insert(data,data_count,*copy);
        //Move the detatched item to left of subset[i + 1]
        ordered_insert(subset[i+1]->data,subset[i+1]->data_count,detatched);

        //Since a new subset is created when it splits the subset will be i + 1
        //cout << temp->next;
        temp->next = subset[i]->next;
        subset[i]->next = temp;
    }
    else
    {
        //Insert the data back in
        ordered_insert(data,data_count,detatched);
    }


}

/**
 * remove(const T& entry)
 *
 * @param entry - the entry to remove
 *
 * removes the item and fixes it as it leaves the recursion, afterwards it
 * fixes the root if needed
 */
template <typename T>
void BPlusTree<T>::remove(const T& entry){
    //Loose_remove the entry from this tree.
    //once you return from loose_remove, the root (this object) may have no
    //data and only a single subset
    //now, the tree must shrink:
    //  point a temporary pointer (shrink_ptr) and point it to this root's
    //only subset
    //  copy all the data and subsets of this subset into the root (through
    //shrink_ptr)
    //  now, the root contains all the data and poiners of it's old child.
    //  now, simply delete shrink_ptr (blank out child), and the tree has
    //shrunk by one level.
    //  Note, the root node of the tree will always be the same, it's the
    //child node we delete

    loose_remove(entry);
    //If the root is empty and has children
    if(data_count < MINIMUM && child_count > 0)
    {
        BPlusTree<T>* shrink_ptr = subset[0];
        copy_tree(*shrink_ptr);
        if(child_count > 0)
            copy_next();
        shrink_ptr->clear_tree();
        shrink_ptr = NULL;
        delete shrink_ptr;
    }

}

/**
 *  loose_remove(const T& entry, T *origin)
 *
 *  @param entry - the entry to remove
 *  @param origin - the pointer to the data that needs to be changed
 *
 *  removes the entry and fixes the tree if needed and changes the
 *  data that is not on the leaf
 */
template <typename T>
void BPlusTree<T>::loose_remove(const T& entry, T *origin){
     /* four cases:
          a. leaf && not found target: there is nothing to do
          b. leaf && found target: just remove the target
          c. not leaf and not found target: recursive call to loose_remove
          d. not leaf and found: replace target with largest child of subset[i]

             |   !found    |   found       |
       ------|-------------|---------------|-------
       leaf  |  a: nothing | b: delete     |
             |     to do   |    target     |
       ------|-------------|---------------|-------
       !leaf | c: loose_   | d: replace    |
             |    remove   |    w/ biggest |
       ------|-------------|---------------|-------


     */
    //==================== BPlusTree ==========================
    //Gets the first greater than or equal to
    int i = first_ge(data, data_count, entry);

    //Check if its found
    bool found = (i < data_count && data[i] == entry);

    if (is_leaf()){

        if(found){
            /*
             * sometimes there are cases where you need to swap when
             * the left child is being removed swap out data for right
             * ex.
             *               23
             *             /    \
             *            5     23,24
             * removing 23 causes it to change with 24
             */
            //we do data_count - 2 because we need to account for 0 index
            //and we want to do a swap if we remove anything before the last
            //data
            //ex. data count is 2 we want to swap if we're removing anything
            //    less than 1 which would be 0
            //    data count is 3 (in the even our tree can hold 4
            //    instead of 3 data) we want to swap if its anything less than
            //    index 2
            //Origin won't = to null if it was found before.
            //Wont run if theres one element since data_count - 1 would be 0
            //and i can never be -1
            if(origin != NULL && i < data_count - 1)
            {
                //Sets the original spot it was found to the element to it's
                //right
                //find whether we replace the left or right side
                //Since we have the whole btree we don't know which data to
                //replace
                //by comparing keys we can find which

                *origin = data[i + 1];
            }

           //[b.] just delete the item
           //   my parent will take care of my shortage
            T deleted_entry;
            delete_item(data,i,data_count,deleted_entry);
            //sets child to null when at leaf
            for(int i = child_count; i < MAXIMUM + 2; i++)
            {
                subset[i] = NULL;
            }
        }

    }
    else{
       //not leaf:
       // ---- 000 B_PLUS_TREE: no more remove_biggest
       if (!found){
           //[c.] Not here: subset[i]->loose_remove
           //not found yet. search in the next level:

           subset[i]->loose_remove(entry,origin);


           if(subset[i]->data_count < MINIMUM)
           {
               fix_shortage(i,origin);
           }

       }
       else{

           //[d.] found key in an inner node:subset[i+1]->loose_remove
           assert(i < child_count-1);

           //If its found it will be somewhere within subset[i + 1] and then
           //get next will grab the BPluesTree* containing the key and then get
           //the next BPlusTree* and the data[0]

           subset[i+1]->loose_remove(entry,&this->data[i]);

           if(subset[i + 1]->data_count < MINIMUM)
           {
               fix_shortage(i + 1,&this->data[i]);
           }

       }
       // --------------------------------------------------------
    }
}

/**
 * fix_shortage(int i, T* origin)
 *
 * @param origin - a pointer to the original location that is not a leaf to
 *                 change if needed to
 *
 * fixes the data if the data_count is 0 and then changes the original
 * location if it needs to
 */
template <typename T>
BPlusTree<T>* BPlusTree<T>::fix_shortage(int i, T* origin){

    /*
     * fix shortage in suBPlusTree i:
     * if child i+1 has more than MINIMUM, rotate left
     * elif child i-1 has more than MINIMUM, rotate right
     * elif there is a right child, merge child i with next child
     * else merge child i with left child
     */
     //1. if its greater than 2 subset[i + 1] is non-existent
     //2. sometimes subset needs to merge at i = 0
    //Check for subset[3] since it is null and can't get data count of null


    //================== BPlusTree =========================================
    //Checks for if we transfer right
    if(i != 0 && subset[i - 1]->data_count > MINIMUM)
    {
        //Change the location in the tree to the left subchild's largest data
        if(origin != NULL && subset[i - 1]->is_leaf())
        {
            *origin = subset[i - 1]->data[subset[i - 1]->data_count - 1];
        }

        transfer_right(i - 1);


    }
    //If its not rotate right then rotate left
    else if(subset[i + 1] != NULL && subset[i + 1]->data_count > MINIMUM && i+1 < child_count)
    {
        //Change the location in the tree to the right subchild's smallest data
        if(origin != NULL && subset[i + 1]->is_leaf())
        {
            *origin = subset[i + 1]->data[0];
        }

        transfer_left(i + 1);

        assert(!(i == 2));
    }
    //do a right merge
    else if(i  != 0)
    {
        //No need to fix the original data since there is none
        merge_with_next_subset(i);
    }
    //do a left merge
    else
    {
        if(origin != NULL && subset[i]->is_leaf())
        {
            //Change origin's data
            *origin = merge_with_next_subset(i)->data[i];
        }
        else
            merge_with_next_subset(i);
    }
}

/**
 * remove_biggest(T& entry)
 *
 * @param entry - the entry that got removed
 *
 * removes the largest item from the tree, which would be the farthest
 * right data
 */
template <typename T>
void BPlusTree<T>::remove_biggest(T& entry){
    // Keep looking in the last suBPlusTree (recursive)
    //  until you get to a leaf.
    // Then, detach the last (biggest) data item
    //
    // after the recursive call, fix shortage.
    if(child_count > 1)
    {
       subset[child_count - 1]->remove_biggest(entry);

       for(int i = child_count; i < MAXIMUM + 2; i++)
       {
           subset[i] = NULL;
       }

       if(subset[child_count - 1]->data_count < MINIMUM)
       {
           fix_shortage(child_count - 1);
       }
    }
    else
    {
        detach_item(data,data_count,entry);
    }

}

/**
 *transfer_left(int i)
 *
 * @param i - the index in the tree
 *
 * moves the data to the left and fixes the data, however if it is not a leaf
 * node it will rotate left
 */
template <typename T>
void BPlusTree<T>::transfer_left(int i)
{
    if(subset[i]->is_leaf())
    {
        //i = the next node over from where it was found
        T deleted_entry;
        //Take data[0] from the right side and place it in the data that is
        //missing
        ordered_insert(subset[i - 1]->data,subset[i - 1]->data_count,subset[i]
                                                                    ->data[0]);
        //Remove data[0] from the right side
        delete_item(subset[i]->data,0,subset[i]->data_count,deleted_entry);

        //set the data in position i - 1 to be the number on top
        data[i - 1] = subset[i]->data[subset[i]->data_count - 1];
     }
    else
    {
        rotate_left(i);
    }

}

/**
 *transfer_right(int i)
 *
 * @param i - the current index in the tree
 *
 * borrows a data from the right, and if it is not a leaf rotate the tree right
 */
template <typename T>
void BPlusTree<T>::transfer_right(int i)
{
    if(subset[i]->is_leaf())
    {
        //i = the next node over from where it was found
        T deleted_entry;
        //Take data[0] from the right side and place it in the data that is
        //missing
        ordered_insert(subset[i + 1]->data,subset[i + 1]->data_count,
                       subset[i]->data[subset[i]->data_count - 1]);
        //Remove data[0] from the right side
        delete_item(subset[i]->data,subset[i]->data_count - 1,subset[i]->
                    data_count,deleted_entry);
    }
    else
    {
        rotate_right(i);
    }
}

/**
 * merge_with_next_subset(int i)
 *
 * @param i - the index in the current tree
 *
 * deletes the item either at i/2 and don't move it if it is a leaf
 * then merge the items
 */
template <typename T>
BPlusTree<T>* BPlusTree<T>::merge_with_next_subset(int i){
    /*
     *  Merge subset[i] with subset [i+1] with data[i] in the middle
     *
     *   1. remove data[i] from this object
     *   2. append it to child[i]->data
     *   3. Move all data items from subset[i+1]->data to subset[i]->data
     *   4. Move all subset pointers from subset[i+1]->subset to subset[i]->
     *      subset
     *   5. delete subset[i+1] (store in a temp ptr)
     *   6. delete temp ptr
     */
    //Take the i/2 data and move it down
    //Moving the children over if it is not leaf

    //Moving data down

    //removes the data[i/2] from the object
    T deleted_entry;

    delete_item(data,i/2,data_count,deleted_entry);
    if(!subset[i]->is_leaf())
    {
        //Inserts the data[i/2] into the child [i/2]
        insert_item(subset[i]->data,0,subset[i]->data_count,deleted_entry);
    }


    //Merging
    //For i = 2 and i = 1 they will merge with i - 1
    //For i = 0 it will merge with i + 1
    if(i != 0)
    {
        //Merge child and data for anything greater than 0
        merge(subset[i - 1]->data,subset[i - 1]->data_count,subset[i]->
                data,subset[i]->data_count);
        merge(subset[i - 1]->subset,subset[i - 1]->child_count,subset[i]->
                subset,subset[i]->child_count);

        //Moves the next over
        if(subset[i]->is_leaf())
            subset[i - 1]->next = subset[i]->next;

        //does either a detatch or delete
        BPlusTree* deleted_entry;
        if(child_count == i + 1)
        {
            detach_item(subset,child_count,deleted_entry);
        }
        else
        {
            delete_item(subset,i,child_count,deleted_entry);
        }
        for(int i = child_count; i < MAXIMUM + 2; i++)
        {
            subset[i] = NULL;
        }

        return deleted_entry;
    }
    else
    {

        //Merge the child and data for 0
        merge(subset[i]->data,subset[i]->data_count,subset[i + 1]->data,
                subset[i + 1]->data_count);
        merge(subset[i]->subset,subset[i]->child_count,subset[i + 1]->subset,
                subset[i + 1]->child_count);


        //Moves the next over
        if(subset[i]->is_leaf())
            subset[i]->next = subset[i + 1]->next;

        BPlusTree* deleted_entry;
        if(child_count == i + 1)
        {
            detach_item(subset,child_count,deleted_entry);
        }
        else
        {
            delete_item(subset,i + 1,child_count,deleted_entry);
        }

        for(int i = child_count; i < MAXIMUM + 2; i++)
        {
            subset[i] = NULL;
        }

        return deleted_entry;
    }
}
/**
 * rotate_left(int i)
 *
 * @param i - the current index in the tree
 *
 * rotates the tree to the left by taking the child at index 0 and
 * moving it to the left
 */
template <typename T>
void BPlusTree<T>::rotate_left(int i){
    /*
        * (0 < i < child_count) and (subset[i]->data_count > MINIMUM)
        * subset[i-1] has only MINIMUM - 1 entries.
        *
        * item transfers from child[i] to child [i-1]
        *
        * FIRST item of subset[i]->data moves up to data to replace data[i-1],
        * data[i-1] moves down to the RIGHT of subset[i-1]->data
        *
        *  i = 1:
        *              [50 100]
        *  [  ]        [65 75]       ....
        *            [a]  [b]  [c]
        *
        *  65 move up to replace 50 (data[i])
        *  65's child (its child 0) moves over to be the child of 50
        *  50 moves down to the right of subset[i]->data
        *
        *              [65 100]
        *  [50]         [ 75 ]       ....
        *     [a]      [b]  [c]
        *
        *
        *
        *
        * last item of subset[i-1]->data moves up to data,
        * data[i] moves down to the RIGHT of subset[i]->data
        */
    // If necessary, shift first subset of subset[i] to end of subset[i-1]
    if(!subset[i - 1]->is_leaf())
    {
        //Deletes the left child of the subset
        BPlusTree* deleted_entry;
        delete_item(subset[i]->subset,0,subset[i]->child_count,
                deleted_entry);

        //Sets anything that is unused to null
        for(int i = child_count; i < MAXIMUM + 2; i++)
        {
            subset[i] = NULL;
        }

        //Attatches the left child of the subset to the left branch's subset
        insert_item(subset[i - 1]->subset,subset[i - 1]->child_count,
                    subset[i - 1]->child_count,deleted_entry);
    }

    //Moves the data[i] down
    ordered_insert(subset[i - 1]->data,subset[i - 1]->data_count,data[i - 1]);

    //Deletes data[i]
    T Entry;

    delete_item(data,i - 1,data_count,Entry);

    //moves the first data of subset up
    ordered_insert(data,data_count,subset[i]->data[0]);

    //deletes the first data
    delete_item(subset[i]->data,0,subset[i]->data_count,Entry);
}

/**
 * rotate_right(int i)
 *
 * @param i - the current index in the tree
 *
 * rotates the tree to the right by taking the child at the rightmost index
 * and move it to the right
 */
template <typename T>
void BPlusTree<T>::rotate_right(int i){
    /* (i < child_count - 1) and (subset[i]->data_count > MINIMUM)
     * subset[i+ 1] has only MINIMUM - 1 entries.
     *
     * item transfers from child[i] to child [i+1]
     *
     * LAST item of subset[i]->data moves up to data to replace data[i],
     * data[i] moves down to the LEFT of subset[i+1]->data
     *
     * i = 1
     *                     [50 100]
     *      [20 30]        [65 75]          [ ]
     *  [..] [..] [..]   [a] [b] [c]        [..]
     *
     *  75 moves up to replace 100 (data[i])
     *  75's child (its last child) moves over to be the (child 0) child of 100
     *  100 moves down to subset[i]->data
     *
     *                     [50 75]
     *      [20 30]          [65]          [100]
     *  [..] [..] [..]     [a] [b]        [c] [..]
     *
     *
     *
     *
     * last item of subset[i-1]->data moves up to data,
     * data[i] moves down to the RIGHT of subset[i]->data
     */
    // If necessary, shift last subset of subset[i] to front of subset[i+1]
    if(!subset[i + 1]->is_leaf())
    {
        BPlusTree* deleted_entry;
        delete_item(subset[i]->subset,subset[i]->child_count - 1,
                    subset[i]->child_count,deleted_entry);

        insert_item(subset[i + 1]->subset,0,subset[i + 1]->child_count,
                    deleted_entry);

        for(int i = child_count; i < MAXIMUM + 2; i++)
        {
            subset[i] = NULL;
        }

    }
    //Moves the data[i] down
    ordered_insert(subset[i + 1]->data,subset[i + 1]->data_count,data[i]);

    //Deletes data[i]
    T Entry;

    delete_item(data,i,data_count,Entry);

    //moves the last data of subset up
    ordered_insert(data,data_count,subset[i]->data[1]);

    //deletes the first data
    delete_item(subset[i]->data,1,subset[i]->data_count,Entry);
}


//---------------------------------------------------------------------
//            C O N T A I N S / F I N D / G E T / E T C .
//---------------------------------------------------------------------
/**
 *  contains(const T& entry)
 *
 *  @param entry - true or false if it contains the entry
 *
 *  returns true or false depending if the tree contains the entry
 */
template <typename T>
bool BPlusTree<T>::contains(const T& entry) const
{
    //Gets the first greater than or equal to element
    int i = first_ge(data, data_count, entry);
    //Tests if the element was found
    bool found = (i < data_count && data[i] == entry);

    //If found means that there is a duplicate
    if (found && is_leaf())
    {
        return true;
    }
    //If found and is not leaf
    if(found && !is_leaf())
    {
        return subset[i + 1]->contains(entry);
    }
    //Otherwise it will insert it into the leaf
    if (!found && is_leaf())
    {
        return false;
    }
    //If its not found
    if (!found && !is_leaf())  //yet...
    {
        return subset[i]->contains(entry);
    }
}

/**
 *  get(const T& entry)
 *
 *  @param entry - the entry to find
 *
 *  Gets the reference of the item if it is in the tree
 */
template <typename T>
const T &BPlusTree<T>::get(const T& entry) const{

    //If entry is not in the tree, CRASH
    //assert(contains(entry));

    //Gets the first greater than or equal to element
    int i = first_ge(data, data_count, entry);
    //Tests if the element was found
    bool found = (i < data_count && data[i] == entry);

    //If found means that there is a duplicate
    if (found && is_leaf())
    {
        return data[i];
    }
    //If found and is not leaf
    if(found && !is_leaf())
    {
        return subset[i + 1]->get(entry);
    }
    //Otherwise it will insert it into the leaf
    if (!found && is_leaf())
    {
        cout<<"get called with non-existent entry: "<<entry<<endl;
        insert(entry);
        return get(entry);
    }
    //If its not found
    if (!found && !is_leaf())  //yet...
    {
        return subset[i]->get(entry);
    }

}

/**
 *  get(const T& entry)
 *
 *  @param entry - the entry to find
 *
 *  Gets the reference of the item if it is in the tree
 */
template <typename T>
T &BPlusTree<T>::get(const T& entry){
    //If entry is not in the tree, CRASH
    //assert(contains(entry));

    //Gets the first greater than or equal to element
    int i = first_ge(data, data_count, entry);
    //Tests if the element was found
    bool found = (i < data_count && data[i] == entry);

    //If found means that there is a duplicate
    if (found && is_leaf())
    {
        return data[i];
    }
    //If found and is not leaf
    if(found && !is_leaf())
    {
        return subset[i + 1]->get(entry);
    }
    //Otherwise it will insert it into the leaf
    if (!found && is_leaf())
    {
        cout<<"get called with non-existent entry: "<<entry<<endl;
        insert(entry);
        return get(entry);
    }
    //If its not found
    if (!found && !is_leaf())  //yet...
    {
        return subset[i]->get(entry);
    }

}

/**
 * clear_tree()
 *
 * clears the tree and sets the subsets to null and data counts and child
 * counts to 0
 */
template <typename T>
void BPlusTree<T>::clear_tree()
{
    //TODO: Delete the next pointers

    if(child_count > 1)
    {
        //Goes down all the way till there is no child left
        //Then on the recursive call back it starts deleting the pointers
        for(int i = child_count - 1; i > -1;i--)
        {
            subset[i]->clear_tree();
        }
        //Delete the children and data of the subsets on the way back
        for(int i = 0; i < child_count;i++)
        {
            subset[i]->data_count = 0;
            subset[i]->child_count = 0;

            delete subset[i];
        }

        //When it reaches root we clear the data and child count so that
        //when we insert the elements inside will change
        data_count = 0;
        child_count = 0;

        //Make all the subset null
        for(int i = child_count; i < MAXIMUM + 2; i++)
        {
            subset[i] = NULL;
        }
    }
    else
    {
        data_count = 0;
        child_count = 0;

        //Make all the subset null
        for(int i = child_count; i < MAXIMUM + 2; i++)
        {
            subset[i] = NULL;
        }
    }
}

/**
 *copy_tree(const BPlusTree<T>& other)
 *
 * @param other - the other BPlusTree to copy the information
 *
 * copies the subsets and the tree from the other tree that will not change
 * when one is deleted
 */
template <typename T>
void BPlusTree<T>::copy_tree(const BPlusTree<T>& other)
{

    data_count = 0;
    child_count = other.child_count;
    dups_ok = other.dups_ok;
    //Maybe only thing that copies is from root
    //If it's greater than MAXIMUM + 2 by default nodes are set to point to
    //NULL so we need to account only for everything before subset[3]
    //(subset 3 -> next is always null)

    if(other.child_count > 1)
    {
        //Goes down all the way till there is no child left
        //Then on the recursive call back it starts deleting the pointers
        for(int i = other.child_count - 1; i > -1;i--)
        {
            subset[i] = new BPlusTree<T>(dups_ok);
            subset[i]->copy_tree(*other.subset[i]);
        }
        copy_array(data,other.data,data_count,other.data_count);

    }
    else
    {
        copy_array(data,other.data,data_count,other.data_count);
    }
}

/**
 *copy_next(BPlusTree<T>* dest, BPlusTree<T>* src)
 *
 * @param dest - the next to fix
 * @param src - the next to copy from
 *
 * since the src's next are already created, the destination will look
 * at the src's next and attach it's next to the same key but the one
 * in destination
 */
template <typename T>
void BPlusTree<T>::copy_next()
{
    //gets the subset before the leaf
    if(subset[0]->is_leaf())
    {
        for(int j = 0; j < child_count - 1; j++)
        {
            subset[j]->next = subset[j + 1];
        }

        subset[child_count - 1]->next = NULL;
    }
    else
    {
        for(int i = 0; i < child_count; i++)
        {
            subset[i]->copy_next();
            if(subset[i]->subset[0]->is_leaf())
            {
                //sets the largest of previous branch to smallest of next branch
                if(i + 1 < child_count)
                    subset[i]->get_largest_node()->next = subset[i + 1]->
                            get_smallest_node();
                //null if it is the last branch
                else
                    subset[i]->get_largest_node()->next = NULL;
            }
        }
    }
}

/**
 * get_next(const T& entry)
 *
 * @param entry - the entry to find
 *
 * gets the next of the current entry
 */
template <typename T>
BPlusTree<T>* BPlusTree<T>::get_next(const T& entry)
{
    int i = first_ge(data, data_count, entry);

    bool found = (i < data_count && data[i] == entry);

    //Returns the pointer to the item
    if (found && is_leaf())
        return this;
    else if(found && !is_leaf())
        return subset[i + 1]->get_next(entry);

    //Returns a null pointer if it cant be found
    else
        return subset[i]->get_next(entry);
}

/**
 *size()
 *
 * gets the size of the tree and returns it by counting the data
 * in the leaf
 */
template <typename T>
int BPlusTree<T>::size() const
{
    int size = 0;   //Keeps track of the size of the tree
    BPlusTree<T>* walker;

    walker = subset[0];

    //Counts the amount of leaf nodes
    if(walker != NULL)
    {
        while(!walker->is_leaf())
        {
            walker = walker->subset[0];
        }

        while(walker != NULL)
        {
            size += walker->data_count;
            walker = walker->next;
        }
    }
    else
    {
        size = data_count;
    }
    return size;
}

/**
 * empty()
 *
 * displays whether or not the tree is empty
 */
template <typename T>
bool BPlusTree<T>::empty() const
{
    if(this != NULL)
        return child_count == 0 && data_count == 0;
    else
        return true;
}

/**
 * create_tree(const int SIZE)
 *
 * @param - the size of the tree to create
 *
 * this will create a tree of random datas with the size requested
 */
template <typename T>
void BPlusTree<T>::create_tree(const int SIZE)
{
    int key = 0;
    for(int i = 0; i < SIZE; i++)
    {
        key = Random(1,100);
        cout << "============= Insertion " << i + 1 << " ================="
             << endl;
        cout << "Inserting entry: " << key << endl;
        insert(key);
        cout << *this << endl;

        for(BPlusTree<T>::Iterator i = begin();i != end(); ++i)
        {
            i.print_Iterator();
        }

        cout << " ||| " << endl << endl;

        if(is_valid())
            cout << "Valid!" << endl;
        else
            cout << "Invalid!" << endl;
    }
}
/**
 * get_smallest_node()
 *
 * returns the BPlusTree* with the smallest data
 */
template <typename T>
BPlusTree<T>* BPlusTree<T>::get_smallest_node()
{
    //Finds the greatest from left sub BPlusTree
    BPlusTree<T>* walker;

    if(child_count > 0)
    {
        walker = subset[0];

        while(!walker->is_leaf())
        {
            walker = walker->subset[0];
        }

        return walker;
    }
    else
        return NULL;
}

template <typename T>
BPlusTree<T>* BPlusTree<T>::get_largest_node()
{
    //Finds the greatest from left sub BPlusTree
    BPlusTree<T>* walker;

    if(child_count > 0)
    {
        walker = subset[child_count - 1];

        while(!walker->is_leaf())
        {
            walker = walker->subset[child_count - 1];
        }

        return walker;
    }
    else
        return NULL;
}

/**
 * find(const T& entry)
 *
 * @param entry - the entry to find
 *
 * this will return an iterator of the entry that was requested
 */
template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::find(const T& entry)
{
    int i = first_ge(data, data_count, entry);

    bool found = (i < data_count && data[i] == entry);

    //If found means that there is a duplicate
    if (found && is_leaf())
    {
        return Iterator(this,i);
    }
    //If found and is not leaf
    if(found && !is_leaf())
    {
        return subset[i + 1]->find(entry);
    }
    //Otherwise it will insert it into the leaf
    if (!found && is_leaf())
    {
        return NULL;
    }
    //If its not found
    if (!found && !is_leaf())  //yet...
    {
        return subset[i]->find(entry);
    }
}

/**
    begin()

    gets the smallest node, but as a iterator
*/
//Finds the left most node
template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::begin(){

    return Iterator(get_smallest_node());
}

/**
    end()

    gets the largest node, but as a iterator
*/
//Finds the left most node
template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::end(){

    return Iterator(NULL);
}

/**
 * is_valid()
 *
 * runs through multiple checks to see if the tree is valid and will
 * output messages if it finds any errors
 *
 */
template <typename T>
bool BPlusTree<T>::is_valid()
{
    bool data_check = true; //If the data is never greater than it will remain
                            //true
    bool smaller_data_check = true;
    bool larger_data_check = true;
    bool found_smallest = true;

    if(is_leaf())
        return true;


    //Checks if the data previous is greater than next data
    for(int i = 0; i < data_count - 1; i++)
    {
        if(!(data[i] < data[i + 1]))
            return false;
    }

    //========== check if data is smaller than right most subset =========
    //Checks if the right most data is less than the right most child data
    if(child_count > 0)
    {
        //Goes through the data of the right most child
        for(int i = 0; i < subset[child_count - 1]->data_count; i++)
        {
            if(!(data[data_count - 1] <= subset[child_count - 1]->data[i]))
                return false;
        }

    }

    //========= checks if data is bigger than anything before right most =
    if(child_count > 0)
    {
        //run through the current data's
        for(int k = 0; k < data_count; k++)
        {
            if(!(data[k] >= subset[k]->data[0]))
                return false;
        }
    }

    //Check if data[i] is equal to subset[i + 1] smallest
    if(child_count > 0)
    {
        //Runs through the current data's
        for(int i = 0; i < data_count;i++)
        {
            //Incase the subset[i + 1] is already the smallest
            //ex.
            /*
             *      10
             *     /  \
             *    5   10 15
            */
            if(subset[i + 1]->is_leaf())
            {
                if(!(data[i] == subset[i + 1]->data[0]))
                    return false;
            }
            //goes to subset i + 1 and finds the smallest node
            else
            {
                if(!(data[i] == subset[i + 1]->get_smallest_node()->data[0]))
                    return false;
            }
       }
    }

    //Goes down all the way till there is no child left
    //Then on the recursive call back it starts deleting the pointers
    for(int i = child_count - 1; i > -1;i--)
    {
        if(!subset[i]->is_valid())
            return false;
    }
    if(!data_check)
        cout << "Not all of data[i] is greater than data[i + 1]!" << endl;
    if(!smaller_data_check)
        cout << "not all the right subset data is greater than the "
                "current data" << endl;
    if(!larger_data_check)
        cout << "Not every data is greater than the subset of the same "
                "index"  <<endl;
    if(!found_smallest)
        cout << "Not every data could be found in the leaf!" << endl;

    return true;

}

#endif // BPlusTree_H
