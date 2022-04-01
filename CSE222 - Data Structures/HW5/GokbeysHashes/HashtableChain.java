package GokbeysHashes;

import java.util.LinkedList;

/**
 * Hash table implementation using chaining.
 *  @author gokbey_gazi_keskin
 **/
public class HashtableChain<K, V> implements KWHashMap<K, V> {

    private LinkedList<Entry<K, V>>[] table;
    private int numKeys;
    private static final int CAPACITY = 101;
    private static final double LOAD_THRESHOLD = 3.0;

    /**
     * No-Parameter constructor for GokbeysHashes.HashtableChain
     */
    @SuppressWarnings("unchecked")
    public HashtableChain(){
        table = new LinkedList[CAPACITY];
    }

    /**
     * Get the value of the entry with the given key.
     * @param key Wanted Entry's key.
     * @return Wanted Entry's value.
     */
    @Override
    public V get(Object key) {
        int index = key.hashCode() % table.length;
        if(index<0)
            index+=table.length;
        if(table[index]==null){
            return null;
        }
        else{
            for (Entry<K, V> i : table[index]) {
                if (i.getKey().equals(key)) {
                    return i.getVal();
                }
            }
        }
        return null;
    }

    /**
     * isEmpty implementation for GokbeysHashes.HashtableChain
     * @return true if there is no element in table
     */
    @Override
    public boolean isEmpty() {
        return numKeys==0;
    }

    /**
     * Creates a entry with given key and vale and puts it in Hash Table
     * @param key New entry's key
     * @param value New entry's value
     * @return the previous value of the given key. Null if it is a new entry.
     */
    @Override
    public V put(K key, V value) {
        int index = key.hashCode() % table.length;
        if(index<0)
            index+=table.length;

        if(table[index]==null){
            table[index] = new LinkedList<>();
        }

        for (Entry<K, V> i : table[index]) {
            if (i.key.equals(key)) {
                V oldVal = i.getVal();
                i.setVal(value);
                return oldVal;
            }
        }
        table[index].addFirst(new Entry<>(key,value));
        numKeys++;
        if(numKeys > LOAD_THRESHOLD * table.length)
            rehash();
        return null; //there was no initial value.
    }

    /**
     * Hashes the table again.
     */
    @SuppressWarnings("unchecked")
    private void rehash() {
        LinkedList<Entry<K,V>>[] oldTable= table;
        table = new LinkedList[oldTable.length*2+1];
        numKeys = 0;
        for(LinkedList<Entry<K,V>> i : oldTable){
            if(i!=null)
                for(Entry<K, V> j : i){
                    put(j.key,j.value);
                }
        }
    }

    /**
     * Removes an entry from table.
     * @param key key of the entry.
     * @return The value of the removed entry.
     */
    @Override
    public V remove(Object key) {
        int index = key.hashCode() % table.length;
        if(index<0)
            index+=table.length;

        if(table[index]==null){
            table[index] = new LinkedList<>();
        }

        for (Entry<K, V> i : table[index]) {
            if (i.key.equals(key)) {
                V oldVal = i.value;
                table[index].remove(i);
                numKeys--;
                if(table[index].size()==0){
                    table[index]=null;
                }
                return oldVal;
            }
        }
        return null;
    }

    /**
     * Getter for size
     * @return size of the table.
     */
    @Override
    public int size() {
        return numKeys;
    }

    /**
     * Private inner class Entry
     * @param <K> Key
     * @param <V> Value
     */
    private static class Entry<K,V>{
        private final K key;
        private V value;

        public Entry(K key,V val){
            this.key = key;
            value = val;
        }
        public K getKey(){return key;}
        public V getVal(){return value;}
        public void setVal(V newVal){value = newVal;}

    }

    /**
     * toString implementation for GokbeysHashes.HashtableChain.
     * @return String representation of GokbeysHashes.HashtableChain.
     */
    @Override
    public String toString(){
        StringBuilder returnString = new StringBuilder();
        for(LinkedList<Entry<K,V>> x : table){
            if(x!=null) {
                for (Entry<K, V> y : x)
                    if(y!=null) {
                        returnString.append(y.key).append("-").append(y.value);
                        returnString.append("\n");
                    }
            }

        }
        return returnString.toString();
    }

}
