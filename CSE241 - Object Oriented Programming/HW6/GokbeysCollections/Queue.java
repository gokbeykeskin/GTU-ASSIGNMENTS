package GokbeysCollections;
public interface Queue<E> extends Collection<E>{
	/** Adds given element to the queue throws exception if the capacity is full*/
    public void add(E e);
    /** Removes the first element of the queue, if there is nothing to remove, throws an error.*/
    public void remove();
    /** Adds given element to the queue and returns true, returns false if the capacity is full*/
    public boolean offer(E e);
    /**Retrieves and removes the head of this queue, or throws if this
queue is empty.*/
    public E poll();
    /**Retrieves, but does not remove, the head of this queue.*/
    public E element();
}