package GokbeysfurnitureCompany;

import java.util.*;
/** This class implements some of the methods of the Java ArrayList class.
 */

public class KWArrayList<E> extends AbstractList<E> implements List<E>{
    // Data Fields
    /**
     * The default initial capacity
     */
    private static final int INITIAL_CAPACITY = 10;
    /**
     * The underlying data array
     */
    private E[] theData;
    /**
     * The current size
     */
    private int size = 0;
    /**
     * The current capacity
     */
    private int capacity;

    /**
     * No parameter constructor for KWArrayList
     */
    @SuppressWarnings("unchecked")
    public KWArrayList() {
        capacity = INITIAL_CAPACITY;
        theData = (E[]) new Object[capacity];
    }

    /**
     * Size getter
     * @return size of the ArrayList
     */
    @Override
    public int size() {
        return size;
    }

    /**
     * Add an element to the end of the list.
     * @param anEntry The entry which will be added.
     * @return true
     */
    @Override
    public boolean add(E anEntry) {
        if (size == capacity) {
            reallocate();
        }
        theData[size] = anEntry;
        size++;
        return true;
    }

    /**
     * Insert an element to the given index
     * @param index Target index
     * @param anEntry The entry which will be added.
     */
    @Override
    public void add(int index, E anEntry) {
        if (index < 0 || index > size) {
            throw new ArrayIndexOutOfBoundsException(index);
        }
        if (size == capacity) {
            reallocate();
        }
        // Shift data in elements from index to size ‐ 1
        for(int i = size; i > index; i--) {
            theData[i] = theData[i - 1];
        }
        // Insert the new item.
        theData[index] = anEntry;
        size++;
    }

    /**
     * Remove the element in the given index
     * @param index Target index
     * @return removed object.
     */
    @Override
    public E remove(int index) {
        if (index < 0 || index >= size) {
            throw new ArrayIndexOutOfBoundsException(index);
        }
        E returnValue = theData[index];
        for(int i = index + 1; i < size; i++) {
            theData[i - 1] = theData[i];
        }
        size--;
        return returnValue;
    }

    /**
     * Private reallocate method to use in add method.
     */
    private void reallocate() {
        capacity = 2 * capacity;
        theData = Arrays.copyOf(theData, capacity);
    }

    /**
     * Get the element in the given index.
     * @param index Target index.
     * @return Object in the given index.
     */
    @Override
    public E get(int index) {
        if (index < 0 || index >= size) {
            throw new ArrayIndexOutOfBoundsException(index + " " + size);
        }
        return theData[index];
    }

    /**
     * Set the element in the given index to given value
     * @param index Target index
     * @param newValue New value
     * @return Old value
     */
    @Override
    public E set(int index, E newValue) {
        if (index < 0 || index >= size) {
            throw new ArrayIndexOutOfBoundsException(index);
        }
        E oldValue = theData[index];
        theData[index] = newValue;
        return oldValue;
    }

    /**
     * toString implementation for KWArrayList class.
     *
     * @return String representation of the KWArrayList class.
     */
    @Override
    public String toString(){
        StringBuilder returnString = new StringBuilder();
        for(int i=0 ; i < size() ; ++i)
            returnString.append(get(i).toString()).append("\n");

        return returnString.toString();
    }
}
