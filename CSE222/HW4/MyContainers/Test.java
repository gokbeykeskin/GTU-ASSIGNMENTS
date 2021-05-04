package MyContainers;

import java.util.*;


/**
 * Driver methods for MyHeap and BSTHeapTree classes.
 * @author gokbey_gazi_keskin
 */
public class Test {

    public static void main(String[] args){
        Scanner input = new Scanner(System.in);
        System.out.println("Run driver code for:\n\t1)Part1\n\t2)Part2 Test1(with my values - Strin test)\n\t" +
                "3)Part2 Test3(with random generated double values) ");
        int opt = input.nextInt();
        switch(opt){
            case 1:
                System.out.println("Part1 Test:");
                part1Test();
                break;
            case 2:
                System.out.println("Part2 Test:");
                part2Test();
                break;
            case 3:
                System.out.println("Part2 Test2:");
                part2Test2();
                break;
            default:
                System.out.println("Wrong input.");
        }



    }

    /**
     * Driver method for part 1
     */
    private static void part1Test(){
        MyHeap<Integer> heap = new MyHeap<>();
        MyHeap<Integer> heap2 = new MyHeap<>();
        System.out.println("isEmpty for empty heap: " + heap.isEmpty());
        System.out.println("poll for empty heap: " + heap.poll());
        System.out.println("peek for empty heap: " + heap.peek());
        System.out.println("remove for empty heap: " + heap.peek());
        heap.add(10);
        heap.add(20);
        heap.add(50);
        heap.add(23);
        heap.add(23);
        heap.add(105);
        heap.add(66);

        heap2.add(10);
        heap2.add(29);
        heap2.add(59);
        heap2.add(49);

        System.out.println("isEmpty for non-empty heap: " + heap.isEmpty());
        System.out.println("------------------------------------------------------------------------");

        System.out.println("\nInitial heap1:\n" + heap);
        System.out.println("Initial size of heap1:" + heap.size());

        System.out.println("Initial heap2:\n" + heap2);
        System.out.println("Initial size of heap2:" + heap2.size());
        System.out.println("------------------------------------------------------------------------");

        heap.merge(heap2);
        System.out.println("\nheap1 after merging with heap2:\n" + heap);
        System.out.println("Size of heap1 after merging with heap2:" + heap.size());
        System.out.println("------------------------------------------------------------------------");
        System.out.println("Remove ith largest element(non-existing index):" + heap.removeIthLargest(300));
        System.out.println("Remove ith largest element(existing index):" + heap.removeIthLargest(3));
        System.out.println("\nheap1 after removing 3rd largest index:\n" + heap);

        System.out.println("------------------------------------------------------------------------");

        System.out.println("Search for non-existing element: " + heap.search(99));
        System.out.println("Search for existing element (49): " + heap.search(49));
        System.out.println("------------------------------------------------------------------------");

        System.out.println("\nPeek heap1: " +  heap.peek());
        System.out.println("Peek heap1:" +  heap.peek());
        System.out.println("Peek heap1:" +  heap.peek());
        System.out.println("heap1 after peek operations:\n" + heap);
        System.out.println("Size of heap1 after peek operations:" + heap.size());
        System.out.println("------------------------------------------------------------------------");

        System.out.println("Poll heap1:" +  heap.poll());
        System.out.println("Poll heap1:" +  heap.poll());
        System.out.println("Poll heap1:" +  heap.poll());
        System.out.println("heap1 after poll operations:\n" + heap);
        System.out.println("Size of heap1 after poll operations:" + heap.size());
        System.out.println("------------------------------------------------------------------------");
        System.out.println("Remove one of the 23s from heap1: " + heap.remove(23));

        MyIterator<Integer> iter = heap.heapIter();
        Integer temp;
        try {
            iter.set(100);
        }
        catch(IllegalStateException ex){
            System.out.println("Set before next(). IllegalStateException!");
        }
        try{
            iter.remove();
        }
        catch (IllegalStateException ex){
            System.out.println("Remove before next(). IllegalStateException!\n");

        }
        System.out.println("------------------------------------------------------------------------");

        while(iter.hasNext()){
            temp = iter.next();
            if(temp == 59)
                iter.remove();
            else if(temp == 49)
                iter.set(100);
        }
        System.out.println("heap1 after removing 59 and setting 49 to 100 via iterator\n" + heap);

    }

    /**
     * driver method for part 2
     */
    private static void part2Test(){
        /* Some info such as mode, amount of heaps or occurence of an element are in toString methods of the classes.
                                                            You can see their test results on terminal*/
        BSTHeapTree<String> tree = new BSTHeapTree<>();
        // first heap:
        tree.add("Ahmet");
        tree.add("Gökbey",22);
        tree.add("Ayşe",3);
        tree.add("Veli",2);
        tree.add("Beyza");
        tree.add("Gizem",7);

        //second heap
        tree.add("Hüseyin");
        tree.add("Mahmut");
        tree.add("Nesrin");
        tree.add("Kazım");
        tree.add("Nazlı",2);
        tree.add("Muhammed",3);
        tree.add("Fatih",2);

        //third heap
        tree.add("Sena");
        tree.add("Mustafa");
        tree.add("Defne");


        System.out.println("Initial Tree(there is a newline character after each heap.):\n" + tree);
        System.out.println("------------------------------------------------------------------------");
        System.out.println("Occurrence of Mehmet in tree after inserting 3 of it:" + tree.add("Mehmet",3));
        System.out.println("Occurrence of Mehmet in tree after deletion:" + tree.remove("Mehmet"));

        System.out.println("------------------------------------------------------------------------");
        tree.add("Ayşe",20);
        System.out.println("Tree after the mode has changed by adding 20 Ayşe:\n" + tree);
        System.out.println("------------------------------------------------------------------------");

        tree.remove("Ayşe"); //Ayşe : 22
        tree.remove("Ayşe"); //Ayşe : 21
        System.out.println("Tree after mode has changed back to original value by removaing 2 Ayşe:\n" + tree);
        System.out.println("------------------------------------------------------------------------");
        tree.remove("Mustafa");
        tree.remove("Defne");
        tree.remove("Mehmet");tree.remove("Mehmet");
        System.out.println("Tree after last heap is fully removed.\n" + tree);
        System.out.println("------------------------------------------------------------------------");

        tree.add("Mustafa");
        tree.add("Defne");
        tree.add("Mehmet");
        tree.remove("Mahmut");
        tree.remove("Nesrin");
        tree.remove("Kazım");
        tree.remove("Nazlı");tree.remove("Nazlı");
        tree.remove("Muhammed");tree.remove("Muhammed");tree.remove("Muhammed");
        tree.remove("Fatih");tree.remove("Fatih");
        tree.remove("Sena");
        System.out.println("Tree after last heap is readded and middle heap is fully removed:\n" + tree);
        System.out.println("------------------------------------------------------------------------");
        BSTHeapTree<Integer> tree2 = new BSTHeapTree<>();
        System.out.println("Mode:" + tree.mode());
        System.out.println("Mode of the empty tree:" + tree2.mode());
        System.out.println("Ocurrence of Gizem: " + tree.find("Gizem"));
        System.out.println("------------------------------------------------------------------------");
        System.out.println("ERROR CHECKS");
        try {
            tree2.remove(5);
        }
        catch(IllegalStateException ex){
            System.out.println("Trying to remove from empty tree throws IllegalStateException");
        }

        try{
            tree.remove("Kamuran");
        }catch(NoSuchElementException ex){
            System.out.println("Trying to remove non-existing element throws NoSuchElement exception");
        }
        try{
            tree.find("Kamuran");
        }catch(NoSuchElementException ex){
            System.out.println("Trying to find non-existing element throws NoSuchElement exception");
        }
    }

    private static void part2Test2(){
        Random rand = new Random();
        BSTHeapTree<Integer> tree = new BSTHeapTree<>();
        ArrayList<Integer> arr = new ArrayList<>();
        Integer randNum;
        for(int i=0;i<3000;i++){
            randNum = rand.nextInt(5000);
            tree.add(randNum);
            arr.add(randNum);
        }
        Collections.sort(arr);
        System.out.println("Searching the first 100 elements");

        for(int i = 0;i<100;i++){
            System.out.println(i+". Element in array: " + arr.get(i) + " Occurrence in BSTHeapTree: " + tree.find(arr.get(i)));
        }
        System.out.println("------------------------------------------------------------------------");
        int i=0;
        System.out.println("Searching for non-existing elements:");
        while(i<10){
            randNum = rand.nextInt(5000);
            if(!arr.contains(randNum)){
                try {
                    System.out.println("Search" + randNum + ": " + tree.find(randNum));
                }
                catch(NoSuchElementException ex){
                    System.out.println("This element doesn't exists in BSTHeapTree");
                }
                i++;

            }
        }
        System.out.println("------------------------------------------------------------------------");
        System.out.println("Mode of the BSTHeapTree: " + tree.mode() +
                                                    " Max Occurrence in array: " + part2FindMaxOccurrence(arr));
        int treeModeOccurence = tree.find(tree.mode());
        Integer treeMode = tree.mode();
        for(i = 0;i<treeModeOccurence;i++){
            tree.remove(treeMode); //removed all occurrences of the mode
            arr.remove(treeMode);
        }

        for(i=1000;i<1100;i++){ //removed 100 numbers
            tree.remove(arr.get(i));
            arr.remove(i);

        }
        System.out.println("After the Removal:\n" +
                "Mode of the BSTHeapTree: " + tree.mode() + " Max Occurrence in array: " + part2FindMaxOccurrence(arr));
        System.out.println("IMPORTANT: THESE VALUES ARE USUALLY DIFFERENT BECAUSE IN MOST CASES\n" +
                "MORE THEN ONE ELEMENT HAS THE SAME MAX OCCURRENCE AMOUNT SO IT SAYS MODE: 50 MAX\n" +
                "OCCURRENCE IN ARRAY: 100 ETC. BUT BOTH 50 AND 100 HAS THE SAME OCCURRENCE AMOUNT SO IT IS NOT WRONG.\n" +
                "IT IS CLEARLY SEEN THAT AFTER DELETION, MODE IS UPDATED AND CHECKED. YOU CAN UNCOMMENT THE NEXT LINE\n" +
                "OF CODE TO CHECK THE FULL TREE AND CONFIRM THE RESULT IS CORRECT. (LINE 269)");
        //System.out.println(tree);

        System.out.println("------------------------------------------------------------------------");
        System.out.println("Trying to remove non-existing elements:");
        i=0;
        while(i<10){
            randNum = rand.nextInt(5000);
            if(!arr.contains(randNum)){
                try {
                    System.out.println("Remove" + randNum + ": " + tree.remove(randNum));
                }
                catch(NoSuchElementException ex){
                    System.out.println("This element doesn't exists in BSTHeapTree");
                }
                i++;

            }
        }
    }

    private static Integer part2FindMaxOccurrence(ArrayList<Integer> arr){
        int max_count = 1, res = arr.get(0);
        int curr_count = 1;

        for (int i = 1; i < arr.size(); i++)
        {
            if (arr.get(i).equals(arr.get(i - 1)))
                curr_count++;
            else
            {
                if (curr_count > max_count)
                {
                    max_count = curr_count;
                    res = arr.get(i-1);
                }
                curr_count = 1;
            }
        }

        // If last element is most frequent
        if (curr_count > max_count)
        {
            max_count = curr_count;
            res = arr.get(arr.size()-1);
        }

        return res;
    }
}
