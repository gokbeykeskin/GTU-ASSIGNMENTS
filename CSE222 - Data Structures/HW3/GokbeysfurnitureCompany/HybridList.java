package GokbeysfurnitureCompany;

import java.util.ListIterator;

/**
 * HybridList class for containing products.
 * @author gokbey_gazi_keskin
 */
public class HybridList<E> {
    private static final int MAX_NUMBER = 10;
    private KWLinkedList<KWArrayList<E>> hybridlist;
    private boolean firstprev = true;

    /**
     * No parameter constructor for hybridlist
     */
    public HybridList(){
        hybridlist = new KWLinkedList<KWArrayList<E>>();
        hybridlist.addFirst(new KWArrayList<>());
        //current = hybridlist.getFirst();
    }

    /**
     * Add an element to the end of the list.
     * @param obj The entry which will be added.
     */
    public void add(E obj){
        hybridIterator(size()-1).add(obj);
    }

    /**
     * Remove the element in the given index
     * @param index Target index
     * @return removed object.
     */
    public void remove(int index){
        hybridIterator(index).remove();
    }

    /**
     * Size getter
     * @return size of the ArrayList
     */
    public int size(){
        int arrListsSize=0;
        for(KWArrayList<E> i : hybridlist)
            for(E j : i){
                if(j!=null)
                    arrListsSize++;
            }

        return arrListsSize*hybridlist.size();
    }

    /**
     * Get the element in the given index.
     * @param index Target index.
     * @return Object in the given index.
     */
    public E get(int index) {
        if (index < 0 || index >= size()) {
            throw new ArrayIndexOutOfBoundsException(index);
        }
        ListIterator<KWArrayList<E>> iter = hybridlist.listIterator();
        while(index>MAX_NUMBER && iter.hasNext()){
            iter.next();
            index-=MAX_NUMBER;
        }
        return iter.next().get(index);
    }

    /**
     * Set the element in the given index to given value
     * @param index Target index
     * @param newValue New value
     * @return Old value
     */
    public E set(int index, E newValue) {
        if(hybridlist.size()==0)
            return null;
        ListIterator<KWArrayList<E>> iter = hybridlist.listIterator();
        while(index>MAX_NUMBER && iter.hasNext()){
            iter.next();
            index-=MAX_NUMBER;
        }
        E oldValue = iter.next().get(index);
        iter.next().set(index,newValue);
        return oldValue;
    }
    /**
     *
     * @return List iterator for KWLinkedList class which starts from the given index
     */
    public ListIterator<E> hybridIterator(int i){
        return new HybridListIter(i);
    }
    /**
     *
     * @return List iterator for KWLinkedList class which starts from the given index
     */
    public ListIterator<E> hybridIterator(){
        return new HybridListIter(0);
    }

    /**
     * Private inner Iterator class.
     */
    private class HybridListIter implements ListIterator<E> {

        ListIterator<KWArrayList<E>> iter;
        private KWArrayList<E> current;
        int currentArrListIndex;

        /**
         * Constructs a Hybrid List Iterator starting from the given index.
         * @param i Starting index
         */
        public HybridListIter(int i) {
            iter = hybridlist.listIterator();
            while(i>MAX_NUMBER && iter.hasNext()){
                current = iter.next();
                i-=MAX_NUMBER;
            }
            currentArrListIndex = i;

            current = iter.next();
        }

        /**
         *
         * @return True if hybridList has a next element.
         */
        public boolean hasNext() {
            if(currentArrListIndex != MAX_NUMBER){
                return currentArrListIndex != current.size();
            }
            else return iter.hasNext();
        }

        /**
         * Gives the next element and procceeds.
         * @return next element of the hybridlist.
         */
        public E next() {
            if(currentArrListIndex == MAX_NUMBER && iter.hasNext()){
                currentArrListIndex=0;
                current  = iter.next();
                return current.get(currentArrListIndex++);
            }
            else if(currentArrListIndex == current.size())
                return null;
            return current.get(currentArrListIndex++);
        }
        /**
         *
         * @return True if hybridList has a previous element.
         */
        public boolean hasPrevious() {
            if(currentArrListIndex != -1) return true;
            else return iter.hasPrevious();
        }

        /**
         * Gives the previous element and procceeds backwards.
         * @return previous element of the hybridlist.
         */
        public E previous() {
            if(firstprev){
                firstprev = false;
                current = iter.previous();
            }
            if(currentArrListIndex == current.size())
                currentArrListIndex--;
            if(currentArrListIndex == -1 && iter.hasPrevious()){
                current = iter.previous();
                currentArrListIndex=current.size()-1;
                return current.get(currentArrListIndex--);
            }
            return current.get(currentArrListIndex--);
        }

        /**
         *
         * @return next element's index.
         */
        @Override
        public int nextIndex() {
            return currentArrListIndex;
        }

        /**
         *
         * @return previous element's index.
         */
        @Override
        public int previousIndex() {
            return currentArrListIndex-1;
        }


        /**
         * Removes the current element.
         */
        @Override
        public void remove()
        {
            current.set(currentArrListIndex,null);
            current.remove(currentArrListIndex--);
            if(current.size()==0 && hybridlist.size()>1)
                hybridlist.remove(current);
        }


        /**
         * Sets the current element to the given object.
         * @param dataItem given object
         */
        public void set(E dataItem)
        {
            current.set(currentArrListIndex,dataItem);
        }


        /**
         * Adds the given object to current index.
         * @param obj given object.
         */
        public void add(E obj) {
            if(currentArrListIndex==MAX_NUMBER) {
                if (!iter.hasNext()) {
                    iter.add(new KWArrayList<>());
                }
                current = iter.next();
                currentArrListIndex = 0;
            }

            current.add(obj);
        }

    }

    /**
     * toString implementation for HybridList class.
     * @return String representation of the HybridList class.
     */
    @Override
    public String toString(){
        StringBuilder returnString = new StringBuilder();
        ListIterator<E> iter = hybridIterator();
        while(iter.hasNext()){
            returnString.append(iter.next().toString()).append("\n");
        }
        return  returnString.toString();
    }
}
