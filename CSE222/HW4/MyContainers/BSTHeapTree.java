package MyContainers;

import java.util.*;

/**
 * A Binary search tree which holds Heaps of Nodes as elements.
 * @param <E> elements of Tree
 * @author gokbey_gazi_keskin
 */
public class BSTHeapTree<E extends Comparable<E>> {
    private final TreeSet< MyHeap< Node<E> > > tree;
    private Node<E> mode;

    /**
     * No-parameter constructor.
     */
    public BSTHeapTree(){
        mode = new Node<>(null);
        tree = new TreeSet<>(MyHeap::compareTo);
    }

    /**
     * Private inner class Node
     * @param <E> data type of the nodes.
     */
    private static class Node<E extends Comparable<E>> implements Comparable<Node<E>> {
        private final E data;
        private int occurrence;

        /**
         *
         * @param data data
         */
        public Node(E data){
            this(data,0);
        }

        /**
         *
         * @param data Data
         * @param amount amount (occurrence) of the amount
         */
        public Node(E data,int amount){
            this.data = data;
            occurrence = amount;

        }

        /**
         * Increases occurrence of the data by amount.
         * @param amount amount to be increased
         */
        public void insert(int amount){
            occurrence +=amount;
        }
        /**
         * Decreases occurrence of the data by 1.
         */
        public void remove(){
            remove(1);
        }
        /**
         * Decreases occurrence of the data by amount.
         * @param amount amount to be decreased
         */
        public void remove(int amount){
            occurrence -=amount;
        }

        /**
         * @return Data of the node
         */
        public E getData(){
            return data;
        }

        /**
         * @return Occurrence of data
         */
        public int getOccurrence(){
            return occurrence;
        }

        /**
         * compareTo implementation for Node
         * @param o other node to be compared
         * @return 0 if equal.
         */
        @Override
        public int compareTo(Node<E> o) {
            return getData().compareTo(o.getData());
        }

        /**
         * toString implementation for Node class.
         * @return String representation of the node.
         */
        @Override
        public String toString(){
            return "Data: " + data + " Occurrences: " + occurrence;
        }

    }

    /**
     * Adds one of given data to the tree.
     * @param data data to be added
     * @return new occurence of the data
     */
    public int add(E data){
        return add(data,1);
    }
    /**
     * Adds given amount of given data to the tree.
     * @param data data to be added
     * @param amount amount to be added
     * @return new occurrence of the data
     */
    public int add(E data,int amount){

        if(tree.isEmpty()){ //if BST is empty create a new heap and append to BST
            tree.add(new MyHeap<>());
        }

        MyHeap<Node<E> > availableHeap = null;

        for(MyHeap<Node<E>> i : tree){
            MyIterator<Node<E>> iter = i.heapIter();
            while(iter.hasNext()){
                Node<E> temp = iter.next();
                if(temp.getData().equals(data)) { //if the node already exists
                    temp.insert(amount);
                    if(temp.getOccurrence() > mode.getOccurrence())
                        mode = temp;
                    return temp.getOccurrence();
                }
            }
            if(i.size()<7){
                availableHeap = i;
            }
        }
        Node<E> newNode = new Node<>(data, amount);
        if(availableHeap==null) {
            availableHeap = new MyHeap<>();
            tree.add(availableHeap);
        }
            availableHeap.add(newNode);
            if(newNode.getOccurrence()>mode.getOccurrence())
                mode = newNode;

            return newNode.getOccurrence();
    }

    /**
     * Removes one of the given data from the tree
     * @param data data to be removed
     * @return new occurrence of the data
     * @throws NoSuchElementException When given data doesn't exist in tree.
     * @throws IllegalStateException When Tree is empty.
     */
    public int remove(E data) throws NoSuchElementException,IllegalStateException{
        if(tree.isEmpty()){ //if BST is empty create a new heap and append to BST
            throw new IllegalStateException("BSTHeapTree is empty.");
        }
        MyIterator<Node<E>> iter;
        for(MyHeap<Node<E>> i : tree){
            iter = i.heapIter();
            while(iter.hasNext()){
                Node<E> temp = iter.next();
                if(temp.getData().equals(data)) { //if the node exists
                    temp.remove();
                    if(temp.getOccurrence()==0){
                        i.remove(temp);

                        if(i.size()==0){
                            tree.remove(i);

                        }
                    }
                    if(mode.equals(temp))
                        update_mode();
                    return temp.getOccurrence();
                }
            }

        }

        throw new NoSuchElementException("There is no such element in MyContainers.BSTHeapTree.");
    }

    /**
     * Finds the given item in tree
     * @param item item to be found
     * @return occurrence of the item.
     * @throws NoSuchElementException If the item doesn't exists.
     */
    public int find(E item) throws NoSuchElementException{
        for(MyHeap<Node<E>> i : tree) {
            MyIterator<Node<E>> iter = i.heapIter();
            while (iter.hasNext()) {
                Node<E> temp = iter.next();
                if (temp.getData().equals(item)) { //if the node already exists
                    return temp.getOccurrence();
                }
            }
        }
        throw new NoSuchElementException("There is no such element in MyContainers.BSTHeapTree");
    }

    /**
     * Finds the new mode.
     */
    private void update_mode() {
        MyIterator<Node<E>> iter;
        Node<E> temp;
        for (MyHeap<Node<E>> i : tree) {
            iter = i.heapIter();
            while (iter.hasNext()) {
                 temp = iter.next();

                if (temp.getOccurrence() > mode.getOccurrence()) {
                    mode = temp;
                }
            }
        }
    }

    /**
     * Getter for mode of the BSTHeapTree
     * @return Mode of the tree
     */
    public E mode(){
        return mode.getData();
    }

    /**
     * toString implementation for MyContainers.BSTHeapTree
     * @return the string representation of the MyContainers.BSTHeapTree
     */
    public String toString(){
        StringBuilder returnString = new StringBuilder();
        int amount=0;
        for(MyHeap<Node<E>> x : tree){
            returnString.append(x.toString());
            amount++;
        }
        returnString.append("Mode:").append(mode()).append(" Occurrence:").append(mode.occurrence).append("\n").append("Amonut of heaps:").append(amount);
        return returnString.toString();
    }

}
