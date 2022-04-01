package GokbeysCollections;
public interface Iterator<E>{
	/**Returns the next element of the container*/
    public E next();
    /**Checks if there is a next element on the container*/
    public boolean hasNext();
    /**Deletes the current element of the container*/
    public void remove();    
}