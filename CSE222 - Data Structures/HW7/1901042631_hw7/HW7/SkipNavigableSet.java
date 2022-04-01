package HW7;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.Set;

/**
 * Navigable Set implementation
 * @param <E> The generic type which will the set hold.
 */
public class SkipNavigableSet<E extends Comparable<E>> implements MyNavigableSet<E> {
    private final MySkipList<E> sl;

    /**
     * Constructor
     */
    public SkipNavigableSet(){
        sl = new MySkipList<>();
    }

    /**
     * Inserts the given element to the set.
     * @param element element which will be inserted.
     */
    @Override
    public void insert(E element) {
        sl.insert(element);
    }

    /**
     * Deletes the given element from the set.
     * @param element element which will be deleted.
     * @return The deleted element
     */
    @Override
    public E delete(E element) {
        return sl.delete(element);
    }


    /**
     * Creates a descending iterator over the set.
     * @return the descending iterator.
     */
    @Override
    public Iterator<E> descendingIterator() {
        return new DescendingIterator();
    }


    /**
     * Descending iterator
     */
    private class DescendingIterator implements Iterator<E>{
        ArrayList<E> arr;

        /**
         * Constructor
         */
        DescendingIterator(){
            arr = new ArrayList<>();
            Iterator<E> iter = sl.SLIterator();
            while(iter.hasNext()){
                arr.add(iter.next());
            }
        }

        /**
         * @return true if there are more elements on the list to iterate.
         */
        @Override
        public boolean hasNext(){
            return arr.size()>0;
        }

        /**
         * @return The next element of the set.
         */
        @Override
        public E next(){
            return arr.remove(arr.size()-1);
        }
    }

    /**
     * @return an iterator over the set
     */
    @Override
    public Iterator<E> iterator() {
        return sl.SLIterator();
    }

    /**
     * Don't use
     * @throws UnsupportedOperationException when called.
     */
    @Override
    public Set<E> headSet(E upperLimit) throws UnsupportedOperationException{
        throw new UnsupportedOperationException();
    }

    /**
     * Don't use
     * @throws UnsupportedOperationException when called.
     */
    @Override
    public Set<E> tailSet(E lowerLimit) throws UnsupportedOperationException {
        throw new UnsupportedOperationException();
    }
}
