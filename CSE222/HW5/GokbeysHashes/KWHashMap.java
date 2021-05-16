package GokbeysHashes;

interface KWHashMap<K, V>{
    V get(Object key);
    boolean isEmpty();
    V put(K key, V value);
    V remove(Object key);
    int size();
}
