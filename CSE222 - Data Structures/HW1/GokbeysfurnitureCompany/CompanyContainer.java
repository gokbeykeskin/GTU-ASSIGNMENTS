package GokbeysfurnitureCompany;

import java.util.Arrays;

/**
 * Container class implementation for holding,adding,deleting data of the Furniture Company easier.
 * @param <T> Generic data type which will be contained
 * @author gokbey_gazi_keskin_1901042631
 */
public class CompanyContainer<T> implements Container<T>{
    private T[] container;
    private int used;
    private int capacity;

    /**
     * Default constructor for CompanyContainer.
     *
     */
    public CompanyContainer() {
        this(10);
    }

    /**
     * Constructor for for CompanyContainer.
     * @param cap Inıtıal capacity of the container. It multiplies itself with 2 when the container is full.
     */
    public CompanyContainer(int cap) {
        used=0;
        capacity=cap;
        // noinspection unchecked
        container = (T[] )new Object[cap];

    }

    /**
     * Check if the container is empty.
     * @return true if container is empty, false otherwise.
     */
    @Override
    public boolean empty() {
        return used==0;
    }

    /**Getter for size of the Container.
     * @return Size of the container
     */
    @Override
    public int size() {
        return used;
    }

    /**
     * Returns the element of the container in given index.
     * @param index specifies the element.
     * @return The element in the given index
     * @throws ArrayIndexOutOfBoundsException when there is no element int the given index
     */
    @Override
    public T at(int index) throws ArrayIndexOutOfBoundsException {
        if(index < 0 || index >= used)
            throw new ArrayIndexOutOfBoundsException("Invalid index!");

        return container[index];
    }

    /**
     * Add a new element at the end of the container.
     * @param element Element which will be insterted.
     * @return false if the element is null or element already exists in container.
     */
    @Override
    public boolean insert(T element) {
        if(element == null || contains(element) >= 0)
            return false;

        if(capacity==used){
            capacity = capacity*2;
            container = Arrays.copyOf(container,capacity);
        }
        container[used] = element;
        used++;
        return true;
    }

    /**
     * Deletes an element of the container
     * @param element Element which will be deleted
     * @return false if element is null or doesn't exists in container.
     */
    @Override
    public boolean erase(T element) {
        if(element == null || contains(element) == -1)
            return false;

        boolean flag = true;

        for(int i=0 ; i < used && flag; ++i)
            if(at(i).equals(element))
            {
                this.container[i] = at(used-1);
                flag = false;
            }

        used--;
        return true;
    }


    /**
     * Deletes all the elements in the container.
     */
    @Override
    public void clear() {
        container = null;
        used=0;
        capacity=10;
    }

    /**
     * Check if the given element exists in container.
     * @param o Object which will be searched in the container.
     * @return -1 if the given element doesn't exist in container. Index of the element if it exists.
     */
    @Override
    public int contains(Object o) {
        if(o == null)
            return -1;

        for(int i=0 ; i < used ; ++i)
            if(container[i].equals(o))
                return i;
        return -1;
    }

    /**
     * Checks if the 2 given container are the same or not.
     * @param o Other container
     * @return true if they are the same. false otherwise.
     */
    @Override
    public boolean equals(Object o)
    {
        if(o == null)
            return false;
        if(!(o instanceof CompanyContainer))
            return false;
        @SuppressWarnings("unchecked")
        CompanyContainer<T> temp = ((CompanyContainer<T>)o);

        if(used != temp.size())
            return false;

        for(int i=0 ; i < used ; ++i)
            if(container[i] != temp.container[i])
                return false;

        return true;
    }
    /**
     * toString implementation for CompanyContainer.
     * @return String representation of the CompanyContainer.
     */
    @Override
    public String toString()
    {
        String returnString = "";
        for(int i=0 ; i < size() ; ++i)
            returnString += container[i].toString() + "\n";

        return returnString;
    }
}
