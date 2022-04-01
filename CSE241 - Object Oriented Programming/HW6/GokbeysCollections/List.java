package GokbeysCollections;
public interface List<E> extends Collection<E>{
	/**Inserts the given element to the given index.*/
    public void insert(E e,int index) throws IllegalAccessException;
}