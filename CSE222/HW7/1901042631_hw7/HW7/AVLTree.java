package HW7;

import java.util.Iterator;
import java.util.Stack;

public class AVLTree<E extends Comparable<E>> extends BinarySearchTree<E> {

    private AVLNode root;

    private class AVLNode {
        private E key;
        private int balance;
        private int height;
        public AVLNode left, right, parent;

        AVLNode(E k, AVLNode p) {
            key = k;
            parent = p;
        }

        public String toString(){
            return key.toString();
        }
    }

    @Override
    public boolean add(E key) {
        if (root == null) root = new AVLNode(key, null);
        else {
            AVLNode n = root;
            AVLNode parent;
            while (true) {
                if (n.key == key) return false;

                parent = n;

                boolean goLeft = n.key.compareTo(key) > 0;
                n = goLeft ? n.left : n.right;

                if (n == null) {
                    if (goLeft) {
                        parent.left = new AVLNode(key, parent);
                    } else {
                        parent.right = new AVLNode(key, parent);
                    }
                    rebalance(parent);
                    break;
                }
            }
        }
        return true;
    }

    private void delete(AVLNode AVLNode) {
        if (AVLNode.left == null && AVLNode.right == null) {
            if (AVLNode.parent == null) root = null;
            else {
                AVLNode parent = AVLNode.parent;
                if (parent.left == AVLNode) {
                    parent.left = null;
                } else parent.right = null;
                rebalance(parent);
            }
            return;
        }
        AVLNode child;
        if (AVLNode.left != null) {
            child = AVLNode.left;
            while (child.right != null) child = child.right;
        } else {
            child = AVLNode.right;
            while (child.left != null) child = child.left;
        }
        AVLNode.key = child.key;
        delete(child);
    }

    public E delete(E delKey) {
        if (root == null) return null;
        AVLNode AVLNode;
        AVLNode child = root;

        while (child != null) {
            AVLNode = child;
            child = delKey.compareTo(AVLNode.key)>=0 ? AVLNode.right : AVLNode.left;
            if (delKey == AVLNode.key) {
                delete(AVLNode);
                return delKey;
            }
        }
        return null;
    }

    private void rebalance(AVLNode n) {
        setBalance(n);

        if (n.balance == -2) {
            if (height(n.left.left) >= height(n.left.right)) n = rotateRight(n);
            else n = rotateLeftThenRight(n);

        } else if (n.balance == 2) {
            if (height(n.right.right) >= height(n.right.left)) n = rotateLeft(n);
            else n = rotateRightThenLeft(n);
        }

        if (n.parent != null) {
            rebalance(n.parent);
        } else {
            root = n;
        }
    }

    private AVLNode rotateLeft(AVLNode a) {

        AVLNode b = a.right;
        b.parent = a.parent;

        a.right = b.left;

        if (a.right != null) a.right.parent = a;

        b.left = a;
        a.parent = b;

        if (b.parent != null) {
            if (b.parent.right == a) {
                b.parent.right = b;
            } else {
                b.parent.left = b;
            }
        }

        setBalance(a, b);

        return b;
    }

    private AVLNode rotateRight(AVLNode a) {

        AVLNode b = a.left;
        b.parent = a.parent;

        a.left = b.right;

        if (a.left != null) a.left.parent = a;

        b.right = a;
        a.parent = b;

        if (b.parent != null) {
            if (b.parent.right == a) {
                b.parent.right = b;
            } else {
                b.parent.left = b;
            }
        }

        setBalance(a, b);

        return b;
    }

    private AVLNode rotateLeftThenRight(AVLNode n) {
        n.left = rotateLeft(n.left);
        return rotateRight(n);
    }

    private AVLNode rotateRightThenLeft(AVLNode n) {
        n.right = rotateRight(n.right);
        return rotateLeft(n);
    }

    private int height(AVLNode n) {
        if (n == null) return -1;
        return n.height;
    }

    @SafeVarargs
    private void setBalance(AVLNode... AVLNodes) {
        for (AVLNode n : AVLNodes) {
            reheight(n);
            n.balance = height(n.right) - height(n.left);
        }
    }

    public void printBalance() {
        printBalance(root);
    }

    private void printBalance(AVLNode n) {
        if (n != null) {
            printBalance(n.left);
            System.out.printf("%s ", n.balance);
            printBalance(n.right);
        }
    }

    private void reheight(AVLNode AVLNode) {
        if (AVLNode != null) {
            AVLNode.height = 1 + Math.max(height(AVLNode.left), height(AVLNode.right));
        }
    }

    public boolean search(E key) {
        AVLNode result = searchHelper(this.root, key);
        return result != null;
    }

    private AVLNode searchHelper(AVLNode root, E key) {
        if (root == null || root.key.equals(key)) return root;

        if (root.key.compareTo(key)>0)
            return searchHelper(root.left, key); // call the function on the AVLNode's left child


        return searchHelper(root.right, key);
    }

    /**
     * Returns a pre order iterator
     * @return Iterator.
     */
    public Iterator<E> preOrderIterator(){
        return new PreOrderIterator();
    }

    /**
     * Pre order iterator class (ascending order)
     */
    private class PreOrderIterator implements Iterator<E> {
        public Stack<AVLNode> stack = new Stack<>();
        public AVLNode current;

        /**
         * Constructor
         */
        public PreOrderIterator() {
            current = root;
        }

        /**
         * Check if there is a next element to iterate
         * @return true if there is a next element.
         */
        public boolean hasNext() {
            return current != null || !stack.isEmpty();
        }

        /**
         * Iterates once
         * @return the next element
         */
        public E next() {
            while (current != null) {
                stack.push(current);
                current = current.left;
            }
            AVLNode rst = stack.pop();
            current = rst.right;
            return rst.key;
        }
    }


    public void preOrder(){
        preOrder(root);
    }

    private void preOrder(AVLNode head) {
        if (head != null) {
            preOrder(head.left);
            System.out.println(head.key);
            preOrder(head.right);
        }
    }
}