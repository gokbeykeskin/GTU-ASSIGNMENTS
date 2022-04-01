package HW7;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Random;

public class MySkipList<E extends Comparable<E>> {

    private final int MAX_LEVEL =10;
    private final Random seed = new Random();

    private final SLNode<E> head;

    private int currentMaxLevel;
    private int size;

    public MySkipList() {
        this.head = new SLNode<>(null, MAX_LEVEL);
        this.currentMaxLevel = 1;
        this.size = 0;
    }

    @SuppressWarnings("unchecked")
    public void insert(E value) {

        // With a probability of 1/2, make the node a part of the lowest-level list only. With 1/4 probability, the
        // node will be a part of the lowest two lists. With 1/8 probability, the node will be a part of three lists.
        // And so forth. Insert the node at the appropriate position in the lists that it is a part of.
        // Max level is set to 10 now
        int level = flipAndIncrementLevel();

        SLNode<E> newNode = new SLNode<>(value,level);

        SLNode cur_walker = head;

        for (int i = currentMaxLevel - 1; i >= 0; i--) {
            // walk down the level until it find a range
            while (null != cur_walker.next[i]) {
                // when at bottom level, i is always 0, needs to find the right node to stop
                if (greaterThan((E) cur_walker.next[i].getValue(), value) ) {
                    break;
                }
                cur_walker = cur_walker.next[i];
            }

            if (i <= level) {
                newNode.next[i] = cur_walker.next[i];
                cur_walker.next[i] = newNode;
            }
        }

        size++;
    }

    @SuppressWarnings("unchecked")
    public boolean contains(E value) {
        SLNode cur_walker = head;
        for (int i = currentMaxLevel - 1; i >= 0; i--) {
            // walk down the level until it find a range
            while (null != cur_walker.next[i]) {
                // when at bottom level, i is always 0, needs to find the right node to stop
                if (greaterThan((E) cur_walker.next[i].getValue(), value)) {
                    break;
                }
                if (equalTo((E) cur_walker.next[i].getValue(), value)) {
                    return true;
                }
                cur_walker = cur_walker.next[i];
            }
        }

        return false;
    }

    @SuppressWarnings("unchecked")
    public E delete(E value) {
        SLNode cur_walker = head;
        E result = null;
        for (int i = currentMaxLevel - 1; i >= 0; i--) {
            // walk down the level until it find a range
            while (null != cur_walker.next[i]) {
                // when at bottom level, i is always 0, needs to find the right node to stop
                if (greaterThan((E) cur_walker.next[i].getValue(), value)) {
                    break;
                }
                if (equalTo((E) cur_walker.next[i].getValue(), value)) {
                    cur_walker.next[i] = cur_walker.next[i].next[i];
                    result = value;
                    size--;
                    // just like insert, delete one level is not enough, all levels needs to be removed;
                    break;
                }
                cur_walker = cur_walker.next[i];
            }
        }

        return result;
    }

    public int getSize(){
        return this.size;
    }


    public Iterator<E> SLIterator(){
        return new SkipListIterator();
    }


    private class SkipListIterator implements Iterator<E> {
        SLNode<E> current;
        SkipListIterator(){
            current = head;
        }
        public boolean hasNext(){
            return current.next[0] != null;
        }
        public E next(){
            current = current.next[0];
            return current.value;
        }
    }


    private boolean lessThan(E a, E b) {
        return a.compareTo(b) < 0;
    }

    private boolean equalTo(E a, E b) {
        return a.compareTo(b) == 0;
    }

    private boolean greaterThan(E a, E b) {
        return a.compareTo(b) > 0;
    }

    private int flipAndIncrementLevel() {
        boolean head = true;
        int level = 0;

        for (int i = 0; i < MAX_LEVEL; i++) {
            head = seed.nextBoolean();

            if (head) {
                level++;
                if (level == this.currentMaxLevel) {
                    currentMaxLevel++;
                    break;
                }
            } else {
                break;
            }
        }

        return level;
    }





    private static class SLNode<E>{
        public SLNode[] next;

        private final E value;
        private final int level;

        public SLNode(E value, int level)
        {
            this.value = value;
            this.level = level;
            // 0 is the base level
            this.next = new SLNode[level+1];
        }

        public E getValue() {
            return this.value;
        }

        @Override
        public String toString() {
            return "[ level " + level + " | value "+value + " ]";
        }
    }

    @Override
    public String toString(){
        StringBuilder sb = new StringBuilder();
        // find the start level
        SLNode cur_walker = head;
        int start = MAX_LEVEL - 1;
        while (null == cur_walker.next[start]) {
            start--;
        }

        // collect all node
        cur_walker = head;
        List<SLNode> ref = new ArrayList<>();
        while (null != cur_walker) {
            ref.add(cur_walker);
            cur_walker = cur_walker.next[0];
        }

        for (int i = 0; i <= start; i++) {

            cur_walker = head;
            cur_walker = cur_walker.next[i];
            sb.append("Layer ").append(i).append(" | level ").append(MAX_LEVEL).append(" | head |");

            int levelIndex = 1;
            while (null != cur_walker) {


                if (i > 0) {
                    while (ref.get(levelIndex).getValue() != cur_walker.getValue()) {
                        levelIndex++;
                        sb.append( "--------------------------");
                    }
                    levelIndex++;
                }

                sb.append("---> ").append(cur_walker);
                cur_walker = cur_walker.next[i];
            }

            sb.append("\n");
        }
        return sb.toString();
    }
}
