package HW7;

import java.util.*;

public class Main {
    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);

        System.out.println("Part 1 Driver Code:");
        part1();

        System.out.println("Press Anything to proceed");
        input.nextLine();

        System.out.println("Part 2 Driver Code:");
        part2();

        System.out.println("Press Anything to proceed");
        input.nextLine();

        System.out.println("Part 3 Driver Code:");
        part3();

    }


    public static void part1(){
        MyNavigableSet<Integer> sns = new SkipNavigableSet<>();
        Random rand = new Random();
        for(int i=0;i<20;i++)
            sns.insert(rand.nextInt(100));
        sns.insert(300); //to delete
        System.out.println("Inserted 20 random values and 300 to HW7.SkipNavigableSet");
        Iterator<Integer> iter1 = sns.descendingIterator();
        System.out.println("Descending iterator:");
        while(iter1.hasNext())
            System.out.println(iter1.next());

        System.out.println("------------------------------------------");
        sns.delete(300);
        iter1 = sns.descendingIterator();
        System.out.println("Descending iterator after deleting 300:");
        while(iter1.hasNext())
            System.out.println(iter1.next());
        System.out.println("------------------------------------------");
        MyNavigableSet<Integer> ans = new AVLNavigableSet<>();
        for(int i=0;i<20;i++)
            ans.insert(rand.nextInt(100));
        System.out.println("Inserted 20 random values and 50 to HW7.AVLNavigableSet");
        Iterator<Integer> iter2 = ans.iterator();
        System.out.println("Iterator:");
        while(iter2.hasNext())
            System.out.println(iter2.next());
        System.out.println("Head Set of the HW7.AVLNavigableSet before 50");
        System.out.println(ans.headSet(50));
        System.out.println("Tail Set of the HW7.AVLNavigableSet after 50");
        System.out.println(ans.tailSet(50));
    }

    public static void part2(){
        BinarySearchTree<Integer> bst1 = new BinarySearchTree<>();
        BinarySearchTree<Integer> bst2= new RedBlackTree<>();
        BinarySearchTree<Integer> bst3 = new AVLTree<>();
        Random rand = new Random();
        for(int i=0;i<1000;i++){
            bst1.add(rand.nextInt());
            bst2.add(rand.nextInt());
            bst3.add(rand.nextInt());
        }
        System.out.println("First tree (regular BST) is Red Black Tree: " + BinarySearchTree.isRBTree(bst1));
        System.out.println("Second tree (Red Black Tree) is Red Black Tree: "+BinarySearchTree.isRBTree(bst2));
        System.out.println("Third tree (AVL Tree) is Red Black Tree: " + BinarySearchTree.isRBTree(bst3));
        System.out.println("---------------------------------------------------------------------------");
        System.out.println("First tree (regular BST) is AVL Tree: " + BinarySearchTree.isAvl(bst1));
        System.out.println("Second tree (Red Black Tree) is AVL Tree: "+BinarySearchTree.isAvl(bst2));
        System.out.println("Third tree (AVL Tree) is AVL Tree: " + BinarySearchTree.isAvl(bst3));

    }

    public static void part3(){
        Random rand = new Random();
        BinarySearchTree<Integer> bst;
        BinarySearchTree<Integer> rbt = new RedBlackTree<>();
        TwoThreeTree<Integer> ttt = new TwoThreeTree<>();
        BTree<Integer> bt = new BTree<>();
        MySkipList<Integer> sl = new MySkipList<>();
        long start, finish;
        int bstTime10K=0,bstTime20K=0,bstTime40K=0,bstTime80K=0;
        int rbtTime10K=0,rbtTime20K=0,rbtTime40K=0,rbtTime80K=0;
        int tttTime10K=0,tttTime20K=0,tttTime40K=0,tttTime80K=0;
        int btTime10K=0,btTime20K=0,btTime40K=0,btTime80K=0;
        int slTime10K=0,slTime20K=0,slTime40K=0,slTime80K=0;

        for(int j=0;j<10;j++) {

            bst = new BinarySearchTree<>();
            for (int i = 0; i < 10000; i++) {
                bst.add(rand.nextInt());
            }
            //Inserting extra 100
            start = System.nanoTime();
            for(int i=0;i<100;i++){
                bst.add(rand.nextInt());
            }
            finish = System.nanoTime();
            bstTime10K+=finish-start;


            bst = new BinarySearchTree<>();
            for (int i = 0; i < 20000; i++) {
                bst.add(rand.nextInt());
            }
            //Inserting extra 100
            start = System.nanoTime();
            for(int i=0;i<100;i++){
                bst.add(rand.nextInt());
            }
            finish = System.nanoTime();
            bstTime20K+=finish-start;



            bst = new BinarySearchTree<>();
            for (int i = 0; i < 40000; i++) {
                bst.add(rand.nextInt());
            }
            //Inserting extra 100
            start = System.nanoTime();
            for(int i=0;i<100;i++){
                bst.add(rand.nextInt());
            }
            finish = System.nanoTime();
            bstTime40K+=finish-start;

            bst = new BinarySearchTree<>();
            for (int i = 0; i < 80000; i++) {
                bst.add(rand.nextInt());
            }
            //Inserting extra 100
            start = System.nanoTime();
            for(int i=0;i<100;i++){
                bst.add(rand.nextInt());
            }
            finish = System.nanoTime();
            bstTime80K+=finish-start;


        }

        for(int j=0;j<10;j++) {

            rbt = new RedBlackTree<>();
            for (int i = 0; i < 10000; i++) {
                rbt.add(rand.nextInt());
            }
            //Inserting extra 100
            start = System.nanoTime();
            for(int i=0;i<100;i++){
                rbt.add(rand.nextInt());
            }
            finish = System.nanoTime();
            rbtTime10K+=finish-start;


            rbt = new RedBlackTree<>();
            for (int i = 0; i < 20000; i++) {
                rbt.add(rand.nextInt());
            }
            //Inserting extra 100
            start = System.nanoTime();
            for(int i=0;i<100;i++){
                rbt.add(rand.nextInt());
            }
            finish = System.nanoTime();
            rbtTime20K+=finish-start;



            rbt = new RedBlackTree<>();
            for (int i = 0; i < 40000; i++) {
                rbt.add(rand.nextInt());
            }
            //Inserting extra 100
            start = System.nanoTime();
            for(int i=0;i<100;i++){
                rbt.add(rand.nextInt());
            }
            finish = System.nanoTime();
            rbtTime40K+=finish-start;

            rbt = new RedBlackTree<>();
            for (int i = 0; i < 80000; i++) {
                rbt.add(rand.nextInt());
            }
            //Inserting extra 100
            start = System.nanoTime();
            for(int i=0;i<100;i++){
                rbt.add(rand.nextInt());
            }
            finish = System.nanoTime();
            rbtTime80K+=finish-start;


        }

        for(int j=0;j<10;j++) {

            ttt = new TwoThreeTree<>();
            for (int i = 0; i < 10000; i++) {
                ttt.insert(rand.nextInt());
            }
            //Inserting extra 100
            start = System.nanoTime();
            for(int i=0;i<100;i++){
                ttt.insert(rand.nextInt());
            }
            finish = System.nanoTime();
            tttTime10K+=finish-start;


            ttt = new TwoThreeTree<>();
            for (int i = 0; i < 20000; i++) {
                ttt.insert(rand.nextInt());
            }
            //Inserting extra 100
            start = System.nanoTime();
            for(int i=0;i<100;i++){
                ttt.insert(rand.nextInt());
            }
            finish = System.nanoTime();
            tttTime20K+=finish-start;



            ttt = new TwoThreeTree<>();
            for (int i = 0; i < 40000; i++) {
                ttt.insert(rand.nextInt());
            }
            //Inserting extra 100
            start = System.nanoTime();
            for(int i=0;i<100;i++){
                ttt.insert(rand.nextInt());
            }
            finish = System.nanoTime();
            tttTime40K+=finish-start;

            ttt = new TwoThreeTree<>();
            for (int i = 0; i < 80000; i++) {
                ttt.insert(rand.nextInt());
            }
            //Inserting extra 100
            start = System.nanoTime();
            for(int i=0;i<100;i++){
                ttt.insert(rand.nextInt());
            }
            finish = System.nanoTime();
            tttTime80K+=finish-start;


        }

        for(int j=0;j<10;j++) {

            bt = new BTree<>();
            for (int i = 0; i < 10000; i++) {
                bt.add(rand.nextInt());
            }
            //Inserting extra 100
            start = System.nanoTime();
            for(int i=0;i<100;i++){
                bt.add(rand.nextInt());
            }
            finish = System.nanoTime();
            btTime10K+=finish-start;


            bt = new BTree<>();
            for (int i = 0; i < 20000; i++) {
                bt.add(rand.nextInt());
            }
            //Inserting extra 100
            start = System.nanoTime();
            for(int i=0;i<100;i++){
                bt.add(rand.nextInt());
            }
            finish = System.nanoTime();
            btTime20K+=finish-start;



            bt = new BTree<>();
            for (int i = 0; i < 40000; i++) {
                bt.add(rand.nextInt());
            }
            //Inserting extra 100
            start = System.nanoTime();
            for(int i=0;i<100;i++){
                bt.add(rand.nextInt());
            }
            finish = System.nanoTime();
            btTime40K+=finish-start;

            bt = new BTree<>();
            for (int i = 0; i < 80000; i++) {
                bt.add(rand.nextInt());
            }
            //Inserting extra 100
            start = System.nanoTime();
            for(int i=0;i<100;i++){
                bt.add(rand.nextInt());
            }
            finish = System.nanoTime();
            btTime80K+=finish-start;


        }

        for(int j=0;j<10;j++) {

            sl = new MySkipList<>();
            for (int i = 0; i < 10000; i++) {
                sl.insert(rand.nextInt());
            }
            //Inserting extra 100
            start = System.nanoTime();
            for(int i=0;i<100;i++){
                sl.insert(rand.nextInt());
            }
            finish = System.nanoTime();
            slTime10K+=finish-start;


            sl = new MySkipList<>();
            for (int i = 0; i < 20000; i++) {
                sl.insert(rand.nextInt());
            }
            //Inserting extra 100
            start = System.nanoTime();
            for(int i=0;i<100;i++){
                sl.insert(rand.nextInt());
            }
            finish = System.nanoTime();
            slTime20K+=finish-start;



            sl = new MySkipList<>();
            for (int i = 0; i < 40000; i++) {
                sl.insert(rand.nextInt());
            }
            //Inserting extra 100
            start = System.nanoTime();
            for(int i=0;i<100;i++){
                sl.insert(rand.nextInt());
            }
            finish = System.nanoTime();
            slTime40K+=finish-start;

            sl = new MySkipList<>();
            for (int i = 0; i < 80000; i++) {
                sl.insert(rand.nextInt());
            }
            //Inserting extra 100
            start = System.nanoTime();
            for(int i=0;i<100;i++){
                sl.insert(rand.nextInt());
            }
            finish = System.nanoTime();
            slTime80K+=finish-start;


        }
        System.out.println("Average time in nanoSeconds elapsed for:");
        System.out.println("\tAdding 100 extra elements to Binary Search Tree with 10k elements:  " + bstTime10K/10);
        System.out.println("\tAdding 100 extra elements to Binary Search Tree with 20k elements:  " + bstTime20K/10);
        System.out.println("\tAdding 100 extra elements to Binary Search Tree with 40k elements:  " + bstTime40K/10);
        System.out.println("\tAdding 100 extra elements to Binary Search Tree with 80k elements:  " + bstTime80K/10);
        System.out.println("----------------------------------------------------------------------");
        System.out.println("\tAdding 100 extra elements to Red-Black Tree with 10k elements:  " + rbtTime10K/10);
        System.out.println("\tAdding 100 extra elements to Red-Black Tree with 20k elements:  " + rbtTime20K/10);
        System.out.println("\tAdding 100 extra elements to Red-Black Tree with 40k elements:  " + rbtTime40K/10);
        System.out.println("\tAdding 100 extra elements to Red-Black Tree with 80k elements:  " + rbtTime80K/10);
        System.out.println("----------------------------------------------------------------------");
        System.out.println("\tAdding 100 extra elements to Two-Three Tree Tree with 10k elements:  " + tttTime10K/10);
        System.out.println("\tAdding 100 extra elements to Two-Three Tree Tree with 20k elements:  " + tttTime20K/10);
        System.out.println("\tAdding 100 extra elements to Two-Three Tree Tree with 40k elements:  " + tttTime40K/10);
        System.out.println("\tAdding 100 extra elements to Two-Three Tree Tree with 80k elements:  " + tttTime80K/10);
        System.out.println("----------------------------------------------------------------------");
        System.out.println("\tAdding 100 extra elements to B-Tree Tree with 10k elements:  " + btTime10K/10);
        System.out.println("\tAdding 100 extra elements to B-Tree Tree with 20k elements:  " + btTime20K/10);
        System.out.println("\tAdding 100 extra elements to B-Tree Tree with 40k elements:  " + btTime40K/10);
        System.out.println("\tAdding 100 extra elements to B-Tree Tree with 80k elements:  " + btTime80K/10);
        System.out.println("----------------------------------------------------------------------");
        System.out.println("\tAdding 100 extra elements to Skip-List Tree with 10k elements:  " + slTime10K/10);
        System.out.println("\tAdding 100 extra elements to Skip-List Tree with 20k elements:  " + slTime20K/10);
        System.out.println("\tAdding 100 extra elements to Skip-List Tree with 40k elements:  " + slTime40K/10);
        System.out.println("\tAdding 100 extra elements to Skip-List Tree with 80k elements:  " + slTime80K/10);





    }

}
