#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include <functional>
#include <utility>
#include <algorithm>
#include <stdexcept>
#include <unordered_map>

template <
         typename T,
         typename KComparator = std::equal_to<T>,
         typename PComparator = std::less<T>,
         typename Hasher = std::hash<T> >
class Heap
{
public:
    /// Constructs an m-ary heap. M should be >= 2
    Heap(int m = 2,
         const PComparator& c = PComparator(),
         const Hasher& hash = Hasher(),
         const KComparator& kcomp = KComparator()  );

    /// Destructor as needed
    ~Heap();

    /// Adds an item
    void push(const T& item);

    /// returns the element at the top of the heap
    ///  max (if max-heap) or min (if min-heap)
    T const & top() const;

    /// Removes the top element
    void pop();

    /// returns true if the heap is empty
    bool empty() const;

    /// decreaseKey - Finds key matching old object &
    /// updates its location in the heap based on the new value
    void decreaseKey(const T& old, const T& newVal);

    // output
    const std::vector<T> getStor() const;


 private:
    /// Add whatever helper functions and data members you need below
    void trickleUp(int pos);
    void heapify(int pos);
    void swapHelper(int pos, int parent);



 private:
   /// Data members - These should be sufficient
    std::vector< T > store_;
    int m_;
    PComparator c_;
    std::unordered_map<T, size_t, Hasher, KComparator> keyToLocation_;

};

// done
template <typename T, typename KComparator, typename PComparator, typename Hasher >
Heap<T,KComparator,PComparator,Hasher>::Heap(
    int m,
    const PComparator& c,
    const Hasher& hash,
    const KComparator& kcomp ) :

    store_(),
    m_(m),
    c_(c),
    keyToLocation_(100, hash, kcomp)

{

}

// done
template <typename T, typename KComparator, typename PComparator, typename Hasher >
Heap<T,KComparator,PComparator,Hasher>::~Heap()
{

}

// done
template <typename T, typename KComparator, typename PComparator, typename Hasher >
const std::vector<T> Heap<T,KComparator,PComparator,Hasher>::getStor() const
{
    return store_;
}

// done
template <typename T, typename KComparator, typename PComparator, typename Hasher >
void Heap<T,KComparator,PComparator,Hasher>::trickleUp(int pos)
{
    int parent = (pos-1)/m_;

    PComparator comp;

    while(comp(store_[pos], store_[parent]) && parent >= 0)
    {
        T hold = store_[pos];
        store_[pos] = store_[parent];
        store_[parent] = hold;

        //int index = keyToLocation_[hold];    
        keyToLocation_[hold] = pos;

        T parentHold = store_[parent];
        keyToLocation_[parentHold] = parent;


        pos = parent;
        parent = (pos-1)/m_;
    }

}

// done
template <typename T, typename KComparator, typename PComparator, typename Hasher >
void Heap<T,KComparator,PComparator,Hasher>::swapHelper(int pos, int parent)
{

    // this has to stay in this order otherwise it really messes other things up
    // swap the vector values
    T hold = store_[pos];
    store_[pos] = store_[parent];
    store_[parent] = hold;

    //int index = keyToLocation_[hold];    
    // adjust the key values
    keyToLocation_[hold] = pos;

    T parentHold = store_[parent];
    keyToLocation_[parentHold] = parent;

    // keyToLocation_[store_child]

    pos = parent;
    parent = (pos-1)/m_;
}

// done
template <typename T, typename KComparator, typename PComparator, typename Hasher >
void Heap<T,KComparator,PComparator,Hasher>::heapify(int pos)
{
    // redone from last assignment

    PComparator comp;
    int size = store_.size();

    // any child
    int posChild = (m_*pos)+1;
    int hold = pos;

    // bounds
    if(posChild >= size)
    {
        return;
    }

    // for everything
    for(int i = 1; i <= m_; i++)
    {
        bool inBound = false;
        // new child we're on
        int posChildLoop = (m_*pos)+i;

        // bounds check
        if(posChildLoop < size)
        {
            inBound = true;
        }   

        if(inBound == true)
        {
            if(comp(store_[posChildLoop], store_[hold]))
            {
                hold = posChildLoop;
            }
        }
    }

    // if anything happened in the loop
    int new_pos = hold;

    if(comp(store_[new_pos], store_[pos]))
    {
        swapHelper(new_pos, pos);
        heapify(new_pos);
    }

    return;
}

// done
template <typename T, typename KComparator, typename PComparator, typename Hasher >
void Heap<T,KComparator,PComparator,Hasher>::push( const T& item)
{
    store_.push_back(item);

    int pos = store_.size()-1;

    keyToLocation_.insert(std::make_pair(item, pos));

    trickleUp(pos);

}

// done
template <typename T, typename KComparator, typename PComparator, typename Hasher >
void Heap<T,KComparator,PComparator,Hasher>::decreaseKey(const T& old, const T& newVal)
{
    PComparator comp;

    if(keyToLocation_.find(old) == keyToLocation_.end())
    {
        return;
    }

    int oldHold = keyToLocation_[old];
    store_[oldHold] = newVal;
    
    typename std::unordered_map<T, size_t, Hasher, KComparator>::iterator prev = keyToLocation_.find(old);

    keyToLocation_.erase(prev);

    keyToLocation_[newVal] = oldHold;

    if(comp(newVal, old))
    {
        trickleUp(oldHold);
    }

    return;
}

// done
template <typename T, typename KComparator, typename PComparator, typename Hasher >
T const & Heap<T,KComparator,PComparator,Hasher>::top() const
{
    // Here we use exceptions to handle the case of trying
    // to access the top element of an empty heap
    if(empty()) {
        throw std::logic_error("can't top an empty heap");
    }
    // If we get here we know the heap has at least 1 item
    // Add code to return the top element
    return store_[0];
}

// done
template <typename T, typename KComparator, typename PComparator, typename Hasher >
void Heap<T,KComparator,PComparator,Hasher>::pop()
{
    if(empty()) {
        throw std::logic_error("can't pop an empty heap");
    }

    store_[0] = store_.back();
    store_.pop_back();
    heapify(0);
}

// done
template <typename T, typename KComparator, typename PComparator, typename Hasher >
bool Heap<T,KComparator,PComparator,Hasher>::empty() const
{
    return store_.empty();
}


#endif