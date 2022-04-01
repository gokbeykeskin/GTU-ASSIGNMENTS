package GokbeysCollections;
import java.util.Arrays;
public class ArrayList<E> implements List<E> {
    /**Default capacity: 30 (Capacity is dynamic It increases itself when it hits the limit.)
    */
    public ArrayList(){
        used=0;
        cap=30;
        ListArr= (E[]) new Object[cap];
        ListIter = new myIterator<E>(ListArr);
    }

    public myIterator<E> iterator(){
        ListIter = new myIterator<E>(ListArr);
        return ListIter;
    }
    private void increaseCap(){
        ListArr = Arrays.copyOf(ListArr, ListArr.length *2);
        ListIter = new myIterator<E>(ListArr);
    }
    public void add(E e){
        if(cap==used){
            increaseCap();
        }
        ListArr[used]=e;
        used++;
    }
    public void insert(E e,int index){
        if(cap==used){
            increaseCap();
        }
        for(int i=used;i>=index;i--){
            ListArr[i+1]=ListArr[i];
        }
        ListArr[index]=e;
        used++;
    }

    public void clear(){
        for(int i=0;i<used;i++){
            ListArr[i]=null;
        }
        used=0;
    }
    public boolean contains(E e){
        for(int i=0;i<used;i++){
            if(ListArr[i]==null)
                return false;
            else if(ListArr[i].equals(e))
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
        for(int i=0;i<ListArr.length;i++){
            if(e.equals(ListArr[i])){
                for(int j=i;j<ListArr.length-1;j++){
                    ListArr[j]=ListArr[j+1];
                }
                used--;
            }
        }
        //ListIter = new myIterator(ListArr);
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
                if(it.next().equals(ListArr[i]))
                    remove(ListArr[i]);
            }
        }
    }
    public void retainAll(Collection<E> c){
        boolean removeFlag;
        myIterator<E> it= c.iterator();
        for(int i=0;i<used;i++){
            removeFlag=true;
            for(int j=0;j<c.size();j++){
                if(ListArr[i]==it.next())
                    removeFlag=false;
            }
            it=c.iterator();
            if(removeFlag==true)
                remove(ListArr[i--]);
        }
    }

    public int size(){
        return used;
    }

    private myIterator<E> ListIter;
    private E[] ListArr;
    private int used;
    private int cap;
}
