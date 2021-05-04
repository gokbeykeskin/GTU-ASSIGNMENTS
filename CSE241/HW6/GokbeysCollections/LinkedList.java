package GokbeysCollections;
import java.lang.IllegalStateException;
public class LinkedList<E> implements List<E>,Queue<E>{
    /** Linked list has 30 elements as default.*/
    public LinkedList(){
        this(30); //LL has 30 elements as default capacity
    }
    /** x is capacity.*/
    public LinkedList(int x){
        used=0;
        cap=x;
        LLArr= (E[]) new Object[cap];
        LLIter = new myIterator<E>(LLArr);
    }

    public myIterator<E> iterator(){
        LLIter = new myIterator<E>(LLArr);
        return LLIter;
    }
    /**Tries to add an element and throws exception if the capacity is full. */
    public void add(E e) throws IllegalStateException{
        
        if(used<cap){
            LLArr[used]=e;
            used++;
        }
        else{
            throw new IllegalStateException("Capacity of the linked list is full!");
        } 
    }
    public boolean offer(E e){
        
        if(used<cap){
            LLArr[used]=e;
            used++;
            return true;
        }
        else{
            return false;
        } 
    }
    /**Directly throws because queue is not a random access container, don't use this.*/
    public void clear()throws IllegalAccessException{
        throw new IllegalAccessException("LINKED LIST IS NOT A RANDOM ACCESS CONTAINER.");
    }
    /**Directly throws because queue is not a random access container, don't use this.*/
    public boolean contains(E e)throws IllegalAccessException{
        throw new IllegalAccessException("LINKED LIST IS NOT A RANDOM ACCESS CONTAINER.");
    }
    public boolean isEmpty(){
        if(size()==0)
            return true;
        return false;
    }
    /**Directly throws because queue is not a random access container, don't use this.*/
    public void remove(E e)throws IllegalAccessException{
        throw new IllegalAccessException("LINKED LIST IS NOT A RANDOM ACCESS CONTAINER.");
    }

    public void remove()throws NullPointerException{
        if(used==0){
            throw new NullPointerException("There is no element in Linked List");
        }
        for(int j=0;j<LLArr.length-1;j++){
            LLArr[j]=LLArr[j+1];
        }
        used--;
        //setIter = new myIterator(setArr);
    }

    @Override
    public E poll() {
        E returnVal=LLArr[0];
        remove();
        return returnVal;
    }

    @Override
    public E element() {
        return LLArr[0];
    }

    public void addAll(Collection<E> c){
        myIterator<E> it= c.iterator();
        while(it.hasNext()){
            add(it.next());
        }
    }
    /**Directly throws because queue is not a random access container, don't use this.*/
    public void removeAll(Collection<E> c)throws IllegalAccessException{
        throw new IllegalAccessException("LINKED LIST IS NOT A RANDOM ACCESS CONTAINER.");
    }
    /**Directly throws because queue is not a random access container, don't use this.*/
    public void retainAll(Collection<E> c)throws IllegalAccessException{
        throw new IllegalAccessException("LINKED LIST IS NOT A RANDOM ACCESS CONTAINER.");
    }

    public int size(){
        return used;
    }
    /**Directly throws because queue is not a random access container, don't use this.*/
    public void insert(E e,int index) throws IllegalAccessException{
        throw new IllegalAccessException("LINKED LIST IS NOT A RANDOM ACCESS CONTAINER.");
    }

    private myIterator<E> LLIter;
    private E[] LLArr;
    private int used;
    private int cap;
}