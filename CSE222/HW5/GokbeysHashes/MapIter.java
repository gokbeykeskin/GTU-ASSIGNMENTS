package GokbeysHashes;

public interface MapIter<K> {
    boolean hasNext();
    boolean hasPrev();
    K next();
    K prev();


}
