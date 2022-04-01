package GokbeysHashes;

/** Hash table implementation using open addressing. */
public class IterableHashMap<K, V> implements KWHashMap<K, V> {
    /*
        I used the HashtableOpen implementation from the textbook and added the needed iterator class to it (PART1).
     */
    private static class Entry<K, V> {
        /**
         * The key
         */
        private final K key;
        /**
         * The value
         */
        private V value;

        /**
         * Creates a new key‐value pair.
         *
         * @param key   The key
         * @param value The value
         */
        public Entry(K key, V value) {
            this.key = key;
            this.value = value;
        }

        /**
         * Retrieves the key.
         *
         * @return The key
         */
        public K getKey() {
            return key;
        }

        /**
         * Retrieves the value.
         *
         * @return The value
         */
        public V getValue() {
            return value;
        }

        /**
         * Sets the value.
         *
         * @param val The new value
         * @return The old value
         */
        public V setValue(V val) {
            V oldVal = value;
            value = val;
            return oldVal;
        }
    }

    public MapIter<K> MapIterator() {
        return new MapIterator();
    }
    public MapIter<K> MapIterator(K key) {
        return new MapIterator(key);
    }

    private class MapIterator implements MapIter<K> {
        int count;
        int tableIndex;
        public MapIterator(){
            count = 0;
            tableIndex = 0;
        }

        public MapIterator(K key){
            count = 0;
            tableIndex = find(key);
        }


        @Override
        public boolean hasNext() {
            return !(count==numKeys);
        }
        @Override
        public boolean hasPrev() {
            return !(count==0);
        }


        @Override
        public K next() {
            if(tableIndex==table.length) {
                tableIndex=0;
                return table[tableIndex].key;
            }
            else {
                while(table[tableIndex]==null) {
                    tableIndex++;
                    if(tableIndex==table.length)
                        tableIndex=0;
                }
                tableIndex++;
                count++;
            }
            return table[tableIndex-1].key;
        }

        @Override
        public K prev() {
            if(tableIndex==-1){
                tableIndex=table.length-1;
                return table[tableIndex].key;

            }
            else {
                while(table[tableIndex]==null){
                    tableIndex--;
                    if(tableIndex==-1)
                        tableIndex=table.length-1;
                }
                tableIndex--;
                count--;
            }
            return table[tableIndex+1].key;
        }

    }

    // Data Fields
    private Entry<K, V>[] table;
    private static final int START_CAPACITY = 101;
    private double LOAD_THRESHOLD = 0.75;
    private int numKeys;
    private int numDeletes;
    private final Entry<K, V> DELETED =
            new Entry<>(null, null);

    // Constructor
    @SuppressWarnings("unchecked")
    public IterableHashMap() {
        table = new Entry[START_CAPACITY];
    }

    /** Finds either the target key or the first empty slot in the
     search chain using linear probing.
     @pre The table is not full.
     @param key The key of the target object
     @return The position of the target or the first empty slot if
     the target is not in the table.
     */
    private int find(Object key) {
        // Calculate the starting index.
        int index = key.hashCode() % table.length;
        if (index < 0)
            index += table.length; // Make it positive.
        // Increment index until an empty slot is reached
        // or the key is found.
        while ((table[index] != null)
                && (!key.equals(table[index].getKey()))) {
            index++;
            // Check for wraparound.
            if (index >= table.length)
                index = 0; // Wrap around.
        }
        return index;
    }

    /** Method get for class HashtableOpen.
     @param key The key being sought
     @return the value associated with this key if found;
     otherwise, null
     */
    @Override
    public V get(Object key) {
        // Find the first table element that is empty
        // or the table element that contains the key.
        int index = find(key);
        // If the search is successful, return the value.
        if (table[index] != null)
            return table[index].getValue();
        else
            return null; // key not found.
    }

    @Override
    public boolean isEmpty() {
        return numKeys==0;
    }

    @Override
    public V put(K key, V value) {
        // Find the first table element that is empty
        // or the table element that contains the key.
        int index = find(key);
        // If an empty element was found, insert new entry.
        if (table[index] == null) {
            table[index] = new Entry<>(key, value);
            numKeys++;
            // Check whether rehash is needed.
            double loadFactor =
                    (double) (numKeys + numDeletes) / table.length;
            if (loadFactor > LOAD_THRESHOLD)
                rehash();
            return null;
        }
        // assert: table element that contains the key was found.
        // Replace value for this key.
        V oldVal = table[index].getValue();
        table[index].setValue(value);
        return oldVal;
    }

    @Override
    public V remove(Object key) {

        int index = find(key);
        if(table[index] == null || table[index]==DELETED)
            return null;
        else {
            V val = table[index].value;
            table[index] = DELETED;
            numDeletes++;
            numKeys--;
            return val;
        }
    }

    @Override
    public int size() {
        return numKeys;
    }

    /** Expands table size when loadFactor exceeds LOAD_THRESHOLD
     @post The size of the table is doubled and is an odd integer.
     Each nondeleted entry from the original table is
     reinserted into the expanded table.
     The value of numKeys is reset to the number of items
     actually inserted; numDeletes is reset to 0.
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
                put(kvEntry.getKey(), kvEntry.getValue());
            }
        }
    }


}