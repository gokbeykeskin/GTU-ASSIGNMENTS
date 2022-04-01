package HW7;

import java.util.Iterator;
import java.util.Set;

/**
 * Navigable Set interface
 */
public interface MyNavigableSet<E extends Comparable<E>> {
    void insert(E element);
    E delete(E element);
    Iterator<E> iterator();
    Iterator<E> descendingIterator();
    Set<E> headSet(E upperLimit);
    Set<E> tailSet(E lowerLimit);
}
