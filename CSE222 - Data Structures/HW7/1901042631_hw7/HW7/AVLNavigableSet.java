package HW7;

import java.util.HashSet;
import java.util.Iterator;
import java.util.Set;

/**
 * Navigable Set implementation with AVL Tree as data field.
 * @param <E> The generic value which will the set hold
 */
public class AVLNavigableSet<E extends Comparable<E>> implements MyNavigableSet<E>  {
    AVLTree<E> avltree;

    /**
     * Constructor
     */
    AVLNavigableSet(){
        avltree = new AVLTree<>();
    }

    /**
     * Insert a new element.
     * @param element element to be inserted
     */
    @Override
    public void insert(E element) {
        avltree.add(element);
    }

    /**
     * Delete the given element
     * @param element Element which will be removed.
     * @return
     */
    @Override
    public E delete(E element) {
        return avltree.delete(element);
    }

    /**
     * Returns a pre-order iterator over the Set
     * @return Iterator
     */
    @Override
    public Iterator<E> iterator() {
        return avltree.preOrderIterator();
    }

    /**
     * Don't use
     * @throws UnsupportedOperationException when called.
     */
    @Override
    public Iterator<E> descendingIterator() throws UnsupportedOperationException {
        throw new UnsupportedOperationException();
    }

    /**
     * Creates a set with elements less then the upper limit.
     * @param upperLimit Upper limit of the set
     * @return the set view
     */
    @Override
    public Set<E> headSet(E upperLimit) {
        Iterator<E> iter = avltree.preOrderIterator();
        Set<E> setView = new HashSet<>();
        while(iter.hasNext()){
            E next = iter.next();
            if(next.compareTo(upperLimit) < 0)
            setView.add(next);
        }
        return setView;
    }

    /**
     * Creates a set with elements more then the lower limit.
     * @param lowerLimit Lower limit of the set
     * @return
     */
    @Override
    public Set<E> tailSet(E lowerLimit) {
        Iterator<E> iter = avltree.preOrderIterator();
        Set<E> setView = new HashSet<>();
        while(iter.hasNext()){
            E next = iter.next();
            if(next.compareTo(lowerLimit) > 0)
                setView.add(next);
        }
        return setView;
    }
}
