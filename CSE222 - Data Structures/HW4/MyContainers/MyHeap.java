package MyContainers;

import java.util.*;

/**
 * Heap implementation using Priority Queue as Data field and delegate methods.
 * @param <E> elements of heap
 * @author gokbey_gazi_keskin
 */
public class MyHeap<E extends Comparable<E> > implements Comparable<MyHeap<E>> {
    private PriorityQueue<E> data;

    /**
     * No parameter constructor (max heap).
     */
    public MyHeap(){
        data = new PriorityQueue<>(new Comparator<>() { // transform min heap to max heap.
            public int compare(E lhs, E rhs) {
                if (lhs.compareTo(rhs) < 0) return +1;
                if (lhs.equals(rhs)) return 0;
                return -1;
            }
        });

    }

    /**
     * Constructor with initial capacity (max heap).
     * @param initialCap Initial capacity.
     */
    public MyHeap(int initialCap){
        data = new PriorityQueue<>(initialCap, new Comparator<>() { // transform min heap to max heap.
            public int compare(E lhs, E rhs) {
                if (lhs.compareTo(rhs) < 0) return +1;
                if (lhs.equals(rhs)) return 0;
                return -1;
            }
        });
    }

    /**
     * Returns a new heap iterator.
     * @return HeapIterator
     */
    public HeapIterator heapIter(){
        return new HeapIterator();
    }

    /**
     * Compares the Heap with another heap.
     * @param o Other Heap to compare
     * @return 1 if they don't have the same size, -1 if not equal, 0 otherwise.
     */
    @Override
    public int compareTo(MyHeap<E> o) {
        if(data.size() == o.size()){
            Iterator<E> iter = data.iterator();
            Iterator<E> iter2 = o.data.iterator();
            while(iter.hasNext()){
                if(iter.next()!=iter2.next())
                    return -1;
            }
            return 0;
        }
        return 1;

    }

    /**
     * Private inner class HeapIterator.
     */
    private class HeapIterator implements MyIterator<E> {

        private final Iterator<E> iter;
        private final ArrayList<E> toBeAdded;

        /**
         * No-parameter constructor.
         */
        public  HeapIterator(){
            iter = data.iterator();
            toBeAdded = new ArrayList<>();
        }

        /**
         * hasNext implementation for MyContainers.MyHeap class.
         * @return true if iterator has next.
         */
        @Override
        public boolean hasNext() {

            if(!(iter.hasNext()) && toBeAdded.size()>0) { //if iteration has ended, modify the data.
                data.addAll(toBeAdded);
                return false;
            }

            return iter.hasNext();
        }

        /**
         * next implementation for MyContainers.MyHeap class.
         * @return Next element of the heap.
         */
        @Override
        public E next() {
            return iter.next();
        }

        /**
         * Removes the lastly returned element.
         */
        @Override
        public void remove() {
            iter.remove();
        }

        /**
         * Sets the lastly returned element
         * @param element new value.
         */
        public void set(E element){
            iter.remove();
            toBeAdded.add(element); //I didn't added directly to data because it is dangerous to modify the
                                    // data of a tree while still iterating.

        }
    }

    /**
     * Add a new element to the heap.
     * @param obj New data to be added.
     */
    public void add(E obj){
        data.add(obj);
    }

    /**
     * Removes the given object.
     * @param obj data to be removed.
     * @return true if succesfully removed.
     */
    public boolean remove(E obj){
        return data.remove(obj);
    }

    /**
     * Gets the largest element(root) of the heap.
     * @return the largest element of the heap.
     */
    public E peek(){
        return data.peek();
    }

    /**
     * Gets the largest element(root) of the heap and removes it.
     * @return the largest element of the heap.
     */
    public E poll(){
        return data.poll();
    }

    /**
     * Getter for size of the heap.
     * @return size of the heap.
     */
    public int size(){
        return data.size();
    }

    /**
     * Returns true if there is no elements in the heap
     * @return true if the heap is empty.
     */
    public boolean isEmpty(){
        return data.isEmpty();
    }

    /**
     * Merges two heaps.
     * @param other other heap to be merged.
     */
    public void merge(MyHeap<E> other){
        PriorityQueue<E> temp;

        //for efficiency, smaller heap's elements should be appended to larger one.
        if(other.size() > size()) { //swap
            temp = this.data;
            this.data = other.data;
            other.data = temp;
        }
        while(!other.data.isEmpty()){
            data.add(other.data.poll());
        }
    }

    /**
     * Searches an element of the heap.
     * @param element element to be searched.
     * @return the element if it exists, null otherwise.
     */
    public E search(E element){

        for(E x : data){
            if(x.equals(element))
                return x;
        }
        return null;
    }

    /**
     * Removes the i'th largest element of the heap.
     * @param i index
     * @return true if successfully removed.
     */
    public boolean removeIthLargest(int i){
        PriorityQueue<E> temp = new PriorityQueue<>( new Comparator<>() { // transform min heap to max heap.
            public int compare(E lhs, E rhs) {
                if (lhs.compareTo(rhs) < 0) return +1;
                if (lhs.equals(rhs)) return 0;
                return -1;
            }
        });

        int j;

        if(i<0 || i>= data.size())
            return false;
        for(j = 0;j<i;j++){
            temp.add(data.poll());
        }

        data.poll();

        for(j=0;j<data.size();j++){
            temp.add(data.poll());
        }

        data = temp;
        return true;
    }

    /**
     * toString implementation for MyContainers.MyHeap class.
     * @return String representation of the class.
     */
    public String toString(){
        StringBuilder returnString = new StringBuilder();

        for(E i : data){
            returnString.append(i).append("\n");
        }
        returnString.append("\n");
        return returnString.toString();

    }

}
