package GokbeysfurnitureCompany;

/**
 * Interface for CompanyContainer
 * @param <T> Generic Generic data type which will be contained
 * @author gokbey_gazi_keskin_1901042631
 */
public interface Container<T> {
    /**
     * Check if the container is empty.
     * @return true if container is empty, false otherwise.
     */
    boolean empty();
    /**Getter for size of the Container.
     * @return Size of the container
     */
    int size();
    /**
     * Getter for Container elements.
     * @param index specifies the element.
     * @return The element in the given index
     * @throws ArrayIndexOutOfBoundsException when there is no element int the given index
     */
    T at(int index) throws ArrayIndexOutOfBoundsException;
    /**
     * Add a new element at the end of the container.
     * @param element Element which will be insterted.
     * @return false if the element is null or element already exists in container.
     */
    boolean insert(T element);
    /**
     * Deletes an element of the container
     * @param element Element which will be deleted
     * @return false if element is null or doesn't exists in container.
     */
    boolean erase(T element);
    /**
     * Deletes all the elements in the container.
     */
    void clear();
    /**
     * Check if the given element exists in container.
     * @param o Object which will be searched in the container.
     * @return -1 if the given element doesn't exist in container. Index of the element if it exists.
     */
    int contains(Object o);
}
