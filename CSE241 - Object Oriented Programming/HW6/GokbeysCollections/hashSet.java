package GokbeysCollections;
import java.util.Arrays;

public class hashSet<E> implements Set<E> {
    /** Capacity is 30 as default and is dynamic so it increases itself when it is full.*/
    public hashSet(){
        used=0;
        cap=30;
        setArr= (E[]) new Object[cap];
        setIter = new myIterator<E>(setArr);
    }

    public myIterator<E> iterator(){
        setIter = new myIterator<E>(setArr);
        return setIter;
    }
    private void increaseCap(){
        setArr = Arrays.copyOf(setArr, setArr.length *2);
        setIter = new myIterator<E>(setArr);
    }
    public void add(E e){
        boolean addFlag=true;
        for(int i=0;i<setArr.length;i++){
            if(e.equals(setArr[i]))
                addFlag=false;
        }
        if(addFlag==true){
            if(cap==used){
                increaseCap();
            }
            setArr[used]=e;
            used++;
        }
    }

    public void clear(){
        for(int i=0;i<used;i++){
            setArr[i]=null;
        }
        used=0;
    }
    public boolean contains(E e){
        for(int i=0;i<used;i++){
            if(setArr[i]==null)
                return false;
            else if(setArr[i].equals(e))
                return true;
        }
        return false;
    }
    public boolean isEmpty(){
        if(size()==0)
            return true;
        return false;
    }
    public void remove(E e){
        for(int i=0;i<setArr.length;i++){
            if(e.equals(setArr[i])){
                for(int j=i;j<setArr.length-1;j++){
                    setArr[j]=setArr[j+1];
                }
                used--;
            }
        }
        //setIter = new myIterator(setArr);
    }
    public void addAll(Collection<E> c){
        myIterator<E> it= c.iterator();
        while(it.hasNext()){
            add(it.next());
        }
    }

    public void removeAll(Collection<E> c){
        for(int i=0;i<used;i++){
            myIterator<E> it= c.iterator();
            while(it.hasNext()){
                if(it.next().equals(setArr[i]))
                    remove(setArr[i]);
            }
        }
    }
    public void retainAll(Collection<E> c){
        boolean removeFlag;
        myIterator<E> it= c.iterator();
        for(int i=0;i<used;i++){
            removeFlag=true;
            for(int j=0;j<c.size();j++){
                if(setArr[i]==it.next())
                    removeFlag=false;
            }
            it=c.iterator();
            if(removeFlag==true)
                remove(setArr[i--]);
        }
    }

    public int size(){
        return used;
    }

    private myIterator<E> setIter;
    private E[] setArr;
    private int used;
    private int cap;
}