package GokbeysCollections;
public interface Collection<E>{
    /** Adds an element to the collection.*/
    public void add(E e); 
    /** Adds every element of the given collection*/
    public void addAll(Collection<E> c);
    /** Deletes all elements of the collection.*/
    public void clear() throws IllegalAccessException;
    /** Checks if the given element exists in given collection. */
    public boolean contains(E e) throws IllegalAccessException;
    /** Checks if the collection has any elements in it or not.*/
    public boolean isEmpty();
    /** Returns collection's iterator.*/
    public myIterator<E> iterator(); 
    /** Removes the given element from collection.*/
    public void remove(E e) throws IllegalAccessException;
    /** Removes every element which exists in the given collection.*/
    public void removeAll(Collection<E> c) throws IllegalAccessException;
    /** Removes every element which doesn't exists in the given collection.*/
    public void retainAll(Collection<E> c) throws IllegalAccessException;
    /** Returns the size of the collection.*/
    public int size();
}