package GokbeysCollections;
import java.util.Arrays;

public class myIterator<E> implements Iterator<E> {
    
    public myIterator(E[] x){
        myContainer=x;
        index=0;
    }
    public E next(){
        return myContainer[index++];
    }
    public boolean hasNext(){
        if(myContainer[index]==null) return false;
        return true;
    }
    public void remove(){
        E[] temp = Arrays.copyOf(myContainer, myContainer.length + 1); //create new array from old array and allocate one more element
        int j=0;
        for(int i=0;i<myContainer.length;i++){
            if(index!=i){
                temp[j]=myContainer[i];
                j++;
            }
        }
        myContainer=temp;
    }
    private int index;
    private E[] myContainer;
}