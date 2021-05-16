package GokbeysHashes;

public class HashtableCoalesced<K,V>  implements KWHashMap<K, V>{
    private Entry<K, V>[] table;
    private static final int START_CAPACITY = 101;
    private final double LOAD_THRESHOLD = 0.75;
    private int numKeys;
    private int numDeletes;
    private final Entry<K, V> DELETED =
            new Entry<>(null, null);

    /**
     * No-Parameter constructor for GokbeysHashes.HashtableCoalesced
     */
    @SuppressWarnings("unchecked")
    public HashtableCoalesced() {
        table = new Entry[START_CAPACITY];
    }

    /**
     * Private inner class Entry
     * @param <K> Key
     * @param <V> Value
     */
    public static class Entry<K, V>{

        private final K key;
        private V value;
        private Integer nextIndex;

        public Entry(K key, V value) {
            this.key = key;
            this.setValue(value);
            nextIndex = null;
        }

        public K getKey() {
            return key;
        }

        public V getValue() {
            return value;
        }

        public V setValue(V val) {
            V oldVal = value;
            value = val;
            return oldVal;
        }


    }

    /**
     * isEmpty implementation for GokbeysHashes.HashtableCoalesced
     * @return true if there is no element in table
     */
    @Override
    public boolean isEmpty() {
        return numKeys == 0;
    }

    /**
     * Finds the entry with given key, or the first empty index.
     * @param key Key of the entry
     * @return index of the entry
     */
    private int find(Object key) {


        int index = key.hashCode() % table.length;

        if (index < 0) {
            index += table.length;

        }

        while(table[index] != null && table[index] != DELETED &&!table[index].getKey().equals(key) && table[index].nextIndex!=null){
            index=table[index].nextIndex;
        }

        int initialIndex=index,i=1;
        while(table[index] != null && !table[index].getKey().equals(key)){
            index = initialIndex+i*i % table.length;
            i++;
            if (index >= table.length) {
                index = 0; // Wrap around.
            }
        }
        return index;
    }

    /**
     * Get the value of the entry with the given key.
     * @param key Wanted Entry's key.
     * @return Wanted Entry's value.
     */
    @Override
    public V get(Object key) {
        int index;
        try {
            index = find(key);
        }
        catch(NullPointerException ex){
            return null;
        }
        if (table[index] != null) {
            return table[index].getValue();
        } else {
            return null; // key not found.
        }
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

        if (index < 0) {
            index += table.length;

        }
        while(table[index] != null && !table[index].equals(DELETED) && !table[index].getKey().equals(key) && table[index].nextIndex!=null){
            index=table[index].nextIndex;
        }

        int initialIndex=index,i=1;
        while(table[index] != null && !table[index].key.equals(key)){
            index = initialIndex+i*i % table.length;
            i++;
            if (index >= table.length) {
                index = 0; // Wrap around.
            }
        }

        if(table[initialIndex]!=null && initialIndex != index) {
            table[initialIndex].nextIndex = index;
        }

        if (table[index] == null) {
            table[index] = new Entry<>(key, value);
            numKeys++;
            double loadFactor =
                    (double) (numKeys + numDeletes) / table.length;
            if (loadFactor > LOAD_THRESHOLD) {
                rehash();
            }
            return null;
        }

        V oldVal = table[index].value;
        table[index].value = value;
        return oldVal;
    }
    /**
     * Removes an entry from table.
     * @param key key of the entry.
     * @return The value of the removed entry.
     */
    @Override
    public V remove(Object key) {
        int index = find(key);
        V oldVal = null;
        if(table[index]==null || table[index] == DELETED)
            return null;


        if(table[index].key.equals(key)) {
            oldVal = table[index].value;

            Entry<K,V> temp;
            if(table[index].nextIndex!= null) {
                temp = table[table[index].nextIndex];
                table[table[index].nextIndex] = DELETED;
                table[index] = temp;
            }
            else
                table[index] = DELETED;

            numKeys--;
            numDeletes++;
        }

        return oldVal;
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
     * Hashes the table again.
     */
    @SuppressWarnings("unchecked")
    private void rehash() {
        // Save a reference to oldTable.
        Entry<K, V>[] oldTable = table;
        // Double capacity of this table.
        table = new Entry[2 * oldTable.length + 1];

        // Reinsert all items in oldTable into expanded table.
        numKeys = 0;
        numDeletes = 0;
        for (Entry<K, V> kvEntry : oldTable) {
            if ((kvEntry != null) && (kvEntry != DELETED)) {
                // Insert entry in expanded table
                put(kvEntry.key, kvEntry.value);
            }
        }
    }
    /**
     * toString implementation for GokbeysHashes.HashtableCoalesced.
     * @return String representation of GokbeysHashes.HashtableCoalesced.
     */
    @Override
    public String toString(){
        StringBuilder returnString = new StringBuilder();
        for(Entry<K,V> x : table){
            if(x!=null && x!=DELETED) {
                returnString.append(x.key).append("-").append(x.value);
                    returnString.append("\n");
            }

        }
        return returnString.toString();
    }


}
