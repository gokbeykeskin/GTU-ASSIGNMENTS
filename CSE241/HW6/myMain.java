import GokbeysCollections.*;
import java.lang.IllegalAccessException;
public class myMain {

    public static void main(String args[]) throws IllegalAccessException {
        Set<String> x1 = new hashSet<String>();
        Set<Integer> y1 = new hashSet<Integer>();
        List<String> x2 = new ArrayList<String>();
        List<Integer> y2 = new ArrayList<Integer>();
        LinkedList<String> x3 = new LinkedList<String>(6); //A Linked List with 20 capacity
        LinkedList<Integer> y3 = new LinkedList<Integer>(6); //A Linked List with 20 capacity


        myIterator<String> xit;
        myIterator<Integer> yit;
        x1.add("Hello"); x1.add("World"); x1.add("My Name is");x1.add("World"); x1.add("Gokbey");//"World"is only added once even though add method is called twice.
        y1.add(1);y1.add(8);y1.add(99);y1.add(8);y1.add(-65); //8 is onnly added once even though add method is called twice

        x2.add("Hello"); x2.add("World"); x2.add("My Name is");x2.add("World"); x2.add("Gazi"); //"World"is added twice this time.
        y2.add(1);y2.add(8);y2.add(99);y2.add(8);y2.add(-33); //8 is added twice this time.

        x3.add("Hello"); x3.add("World"); x3.add("I am");x3.add("World"); x3.add("Gokbey");
        y3.add(1);y3.add(8);y3.add(55);y3.add(90);y3.add(-65);
        /*String Hashset Test*/
        System.out.println("String Hashset Test:\nInitial set(\"World\" is added only once even though add method called twice):\n");
        xit=x1.iterator();
        while(xit.hasNext()){
            System.out.println(xit.next());
        }
        x1.remove("World"); System.out.println("\n\"World\" is removed.\n");
        xit=x1.iterator();
        while(xit.hasNext()){
            System.out.println(xit.next());
        }
        x1.retainAll(x2);
        System.out.println("\nEverything that doesn't exists in collection x2 are deleted.\n");
        xit=x1.iterator();
        while(xit.hasNext()){
            System.out.println(xit.next());
        }
        x1.removeAll(x3);
        System.out.println("\nEverything that does exists in collection x3 are deleted.\n");
        xit=x1.iterator();
        while(xit.hasNext()){
            System.out.println(xit.next());
        }
        System.out.println("\n");

        x1.addAll(x2);
        System.out.println("\nEverything that exists in collection x3 are added to collection x2.\n");
        xit=x1.iterator();
        while(xit.hasNext()){
            System.out.println(xit.next());
        }
        System.out.printf("\nx1 contains Hello: %b\n",x1.contains(("Hello")));
        System.out.printf("Container x1 is empty: %b\n",x1.isEmpty());
        System.out.printf("Size of the container x1: %d\n",x1.size());
        x1.clear(); System.out.printf("\nAll content of x1 is deleted.\n");
        System.out.printf("x1 contains Hello: %b\n",x1.contains(("Hello")));
        System.out.printf("Container x1 is empty: %b\n",x1.isEmpty());
        System.out.printf("Size of the container x1: %d\n",x1.size());
        System.out.printf("------------------------------------------\n");
        /*Integer Hashset Test*/
        System.out.println("Integer Hashset Test:\nInitial set(8 is added only once even though add method called twice):\n");
        yit=y1.iterator();
        while(yit.hasNext()){
            System.out.println(yit.next());
        }
        y1.remove(8); System.out.println("\n8 is removed.\n");
        yit=y1.iterator();
        while(yit.hasNext()){
            System.out.println(yit.next());
        }
        y1.retainAll(y2);
        System.out.println("\nEverything that doesn't exists in collection y2 are deleted.\n");
        yit=y1.iterator();
        while(yit.hasNext()){
            System.out.println(yit.next());
        }
        y1.removeAll(y3);
        System.out.println("\nEverything that does exists in collection y3 are deleted.\n");
        yit=y1.iterator();
        while(yit.hasNext()){
            System.out.println(yit.next());
        }
        System.out.println("\n");

        y1.addAll(y2);
        System.out.println("\nEverything that exists in collection x3 are added to collection y2.\n");
        yit=y1.iterator();
        while(yit.hasNext()){
            System.out.println(yit.next());
        }
        System.out.printf("\ny1 contains 1: %b\n",y1.contains(1));
        System.out.printf("Container y1 is empty: %b\n",y1.isEmpty());
        System.out.printf("Size of the container y1: %d\n",y1.size());
        y1.clear(); System.out.printf("\nAll content of y1 is deleted.\n");
        System.out.printf("y1 contains 1: %b\n",y1.contains(1));
        System.out.printf("Container y1 is empty: %b\n",y1.isEmpty());
        System.out.printf("Size of the container y1: %d\n",y1.size());
        System.out.printf("------------------------------------------\n");

        /*----------------------------------------------------------------------------------------------------*/
        x1.add("Hello"); x1.add("World"); x1.add("My Name is");x1.add("World"); x1.add("Gokbey");
        y1.add(1);y1.add(8);y1.add(99);y1.add(8);y1.add(-65); 
        /*Refill the old containers.*/
         /*String Arraylist Test*/
         System.out.println("String Arraylistt Test:\nInitial set:\n");
         xit=x2.iterator();
         while(xit.hasNext()){
             System.out.println(xit.next());
         }
         x2.remove("World"); System.out.println("\n\"World\" is removed.\n");
         xit=x2.iterator();
         while(xit.hasNext()){
             System.out.println(xit.next());
         }
         x2.retainAll(x1);
         System.out.println("\nEverything that doesn't exists in collection x1 are deleted.\n");
         xit=x2.iterator();
         while(xit.hasNext()){
             System.out.println(xit.next());
         }
         x2.removeAll(x3);
         System.out.println("\nEverything that does exists in collection x3 are deleted.\n");
         xit=x2.iterator();
         while(xit.hasNext()){
             System.out.println(xit.next());
         }
         System.out.println("\n");
 
         x2.addAll(x1);
         System.out.println("\nEverything that exists in collection x1 are added to collection x2.\n");
         xit=x2.iterator();
         while(xit.hasNext()){
             System.out.println(xit.next());
         }
         x2.insert("GTU",3);
         System.out.println("\nWORD GTU IS INSERTED AT INDEX 3.\n");
         xit=x2.iterator();
         while(xit.hasNext()){
             System.out.println(xit.next());
         }
         System.out.printf("\nx2 contains Hello: %b\n",x2.contains(("Hello")));
         System.out.printf("Container x2 is empty: %b\n",x2.isEmpty());
         System.out.printf("Size of the container x2: %d\n",x2.size());
         x2.clear(); System.out.printf("\nAll content of x2 is deleted.\n");
         System.out.printf("x2 contains Hello: %b\n",x2.contains(("Hello")));
         System.out.printf("Container x2 is empty: %b\n",x2.isEmpty());
         System.out.printf("Size of the container x2: %d\n",x2.size());
         System.out.printf("------------------------------------------\n");
         /*Integer Arraylist Test*/
         System.out.println("Integer Hashset Test:\nInitial set:\n");
         yit=y2.iterator();
         while(yit.hasNext()){
             System.out.println(yit.next());
         }
         y2.remove(8); System.out.println("\n8 is removed.\n");
         yit=y2.iterator();
         while(yit.hasNext()){
             System.out.println(yit.next());
         }
         y2.retainAll(y1);
         System.out.println("\nEverything that doesn't exists in collection y1 are deleted.\n");
         yit=y2.iterator();
         while(yit.hasNext()){
             System.out.println(yit.next());
         }
         y2.removeAll(y3);
         System.out.println("\nEverything that does exists in collection y3 are deleted.\n");
         yit=y2.iterator();
         while(yit.hasNext()){
             System.out.println(yit.next());
         }
         System.out.println("\n");
 
         y2.addAll(y1);
         System.out.println("\nEverything that exists in collection y1 are added to collection y2.\n");
         yit=y2.iterator();
         while(yit.hasNext()){
             System.out.println(yit.next());
         }
         y2.insert(20,3);
         System.out.println("\nINTEGER 20 IS INSERTED AT INDEX 3.\n");
         yit=y2.iterator();
         while(yit.hasNext()){
             System.out.println(yit.next());
         }
         System.out.printf("\ny2 contains 1: %b\n",y2.contains(1));
         System.out.printf("Container y2 is empty: %b\n",y2.isEmpty());
         System.out.printf("Size of the container y2: %d\n",y2.size());
         y2.clear(); System.out.printf("\nAll content of y2 is deleted.\n");
         System.out.printf("y2 contains 1: %b\n",y2.contains(1));
         System.out.printf("Container y2 is empty: %b\n",y2.isEmpty());
         System.out.printf("Size of the container y2: %d\n",y2.size());
         System.out.printf("------------------------------------------\n");

         /*STRING LINKED LIST TEST */
         System.out.println("STRING LINKED LIST TEST\n");
         System.out.printf("Container x3 is empty: %b\n",x3.isEmpty());
         System.out.println("element method called i(size of container) times:\n");
         for(int i=0;i<x3.size();i++)
             System.out.println(x3.element());
         System.out.printf("Container x3 is empty: %b\n",x3.isEmpty());
         System.out.printf("Adding a new element is successfull: %b\n",x3.offer("WORD"));
         System.out.printf("Adding a new element is successfull: %b\n",x3.offer("WORD"));
         System.out.println("poll method called i(size of container) times:\n");;
         for(int i=0,a=x3.size();i<a;i++)
            System.out.println(x3.poll());
         System.out.printf("Container x3 is empty: %b\n",x3.isEmpty());
         System.out.printf("------------------------------------------\n");
        /*INTEGER LINKED LIST TEST */
        System.out.println("INTEGER LINKED LIST TEST\n");
        System.out.printf("Container y3 is empty: %b\n",y3.isEmpty());
        System.out.println("element method called i(size of container) times:\n");
        for(int i=0;i<y3.size();i++)
            System.out.println(y3.element());
        System.out.printf("Container y3 is empty: %b\n",y3.isEmpty());
        System.out.printf("Adding a new element is successfull: %b\n",y3.offer(66));
        System.out.printf("Adding a new element is successfull: %b\n",y3.offer(66));
        System.out.println("poll method called i(size of container) times:\n");;
        for(int i=0,a=y3.size();i<a;i++)
            System.out.println(y3.poll());
        System.out.printf("Container y3 is empty: %b\n",y3.isEmpty());
        System.out.printf("------------------------------------------\n");
        /*EXCEPTIONS TEST:*/
        System.out.println("EXCEPTIONS TEST\n");


        try{
            for(int i=0;i<=6;i++)
                x3.add("Hello");
        }
        catch(IllegalStateException exception){
            System.out.println("Linked List is full!\n");
        }
        try{
            x3.remove("Hello");
        }
        catch(IllegalAccessException exception){
            System.out.println("Linked List is not a random access container.\n");
        }
        try{
            x3.clear();
        }
        catch(IllegalAccessException exception){
            System.out.println("Linked List is not a random access container.\n");
        }
        try{
            x3.contains("Hello");
        }
        catch(IllegalAccessException exception){
            System.out.println("Linked List is not a random access container.\n");
        }
        try{
            for(int i=0;i<=6;i++)
                x3.remove();
        }
        catch(NullPointerException exception){
            System.out.println("There are no more elements to remove!.\n");
        }
        try{
            x3.removeAll(x1);
        }
        catch(IllegalAccessException exception){
            System.out.println("Linked List is not a random access container.\n");
        }
        try{
            x3.retainAll(x1);
        }
        catch(IllegalAccessException exception){
            System.out.println("Linked List is not a random access container.\n");
        }
        try{
            x3.insert("Hello",3);
        }
        catch(IllegalAccessException exception){
            System.out.println("Linked List is not a random access container.\n");
        }
    }
}
