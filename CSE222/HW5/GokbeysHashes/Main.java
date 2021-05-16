package GokbeysHashes;

import java.util.Random;
import java.util.Scanner;

public class Main {
    public static void main(String[] args){

        Scanner input = new Scanner(System.in);
        int opt=0;
        while(opt!=5) {
            System.out.println("Which one would you like to test?\n1-Part1 (Iterating through HashMap)\n" +
                    "2-Part2.1 (Hashtable with chain method using linked list)\n" +
                    "3-Part2.2 (Hashtable with chaining method with TreeSet)\n" +
                    "4-Part2.3 (Hashtable with Coalesced method" +"\n5-Exit");
            opt = input.nextInt();
            switch (opt) {
                case 1:
                    testPart1();
                    break;
                case 2:
                    System.out.println("Test with:\n 1) Small data (String-Integer)\n2) Big Data(Integer-Integer)");
                    opt = input.nextInt();
                    if (opt == 1) {
                        long start = System.nanoTime();
                        testPart2_1();
                        long elapsedTime = System.nanoTime() - start;
                        System.out.println("Time of the chain method with linked list(small data): " + (double)elapsedTime/100_000_000);
                    }
                    else if (opt == 2) {
                        long start = System.nanoTime();
                        testPart2_1_bigData();
                        long elapsedTime = System.nanoTime() - start;
                        System.out.println("Time of the chain method with linked list(big data): " + (double)elapsedTime/100_000_000);
                    }
                    else
                        System.out.println("Wrong input.");
                    break;
                case 3:
                    System.out.println("Test with:\n 1) Small data (String-Integer)\n2) Big Data(Integer-Integer)");
                    opt = input.nextInt();
                    if (opt == 1) {
                        long start = System.nanoTime();
                        testPart2_2();
                        long elapsedTime = System.nanoTime() - start;
                        System.out.println("Time of the chain method with binary search tree(small data): " +  (double)elapsedTime/100_000_000);
                    }
                    else if (opt == 2) {
                        long start = System.nanoTime();
                        testPart2_2_bigData();
                        long elapsedTime = System.nanoTime() - start;
                        System.out.println("Time of the chain method with binary search tree(small data) " +  (double)elapsedTime/100_000_000);
                    }
                    else
                        System.out.println("Wrong input.");
                    break;
                case 4:
                    System.out.println("Test with:\n 1) Small data (String-Integer)\n2) Big Data(Integer-Integer)");
                    opt = input.nextInt();
                    if (opt == 1) {

                        long start = System.nanoTime();
                        testPart2_3_bigData();
                        long elapsedTime = System.nanoTime() - start;
                        System.out.println("Time of the coalesced method(small data): " + (double)elapsedTime/100_000_000);
                    }
                    else if (opt == 2) {
                        long start = System.nanoTime();
                        testPart2_3_bigData();
                        long elapsedTime = System.nanoTime() - start;
                        System.out.println("Time of the coalesced method (big data): " +  (double)elapsedTime/100_000_000);
                    }
                    else
                        System.out.println("Wrong input.");
                    break;
                case 5:
                    System.out.println("Goodbye!");
                    break;
                default:
                    System.out.println("Wrong input.");
                    break;
            }
        }
    }
    public static void testPart1(){
        System.out.println("PART 1 (Iterating through a HashMap) Test");
        IterableHashMap<String,Integer> ageOfPeople = new IterableHashMap<>();

        ageOfPeople.put("Gökbey",19);
        ageOfPeople.put("Ahmet",15);
        ageOfPeople.put("Nesrin",30);
        ageOfPeople.put("Kemal",3);
        ageOfPeople.put("Ayşe",90);
        ageOfPeople.put("Mehmet",60);
        ageOfPeople.put("Mahmut",6);
        ageOfPeople.put("Beyza",10);
        ageOfPeople.put("Fatma",50);
        ageOfPeople.put("Ali",9);
        ageOfPeople.put("Veli",29);

        MapIter<String> iter = ageOfPeople.MapIterator();
        System.out.println("Iterating forwards:");
        while(iter.hasNext())
            System.out.println(iter.next());
        System.out.println("------------------");
        System.out.println("Iterating backwards:");

        while(iter.hasPrev())
            System.out.println(iter.prev());
        System.out.println("------------------");
        System.out.println("Iterating from middle to forwards:");

        MapIter<String> iter2 = ageOfPeople.MapIterator("Gökbey");
        while(iter2.hasNext())
            System.out.println(iter2.next());
        System.out.println("------------------");
        System.out.println("Iterating from middle to backwards:");
        while(iter2.hasPrev())
            System.out.println(iter2.prev());
        System.out.println("------------------");

    }
    public static void testPart2_1(){

        KWHashMap<String, Integer> ageOfPeople = new HashtableChain<>();
        System.out.println("PART 2.1 (Hashtable with Chaining method using linked list) with small amount of data Test");
        System.out.println("|Testing isEmpty method|:");
        System.out.println("isEmpty on empty table: " + ageOfPeople.isEmpty());
        System.out.println("-----------------------------------------------------------");
        System.out.println("|Testing put method|:");
        System.out.println("Putting a new element with value 19 (return value): " + ageOfPeople.put("Gökbey",19));
        System.out.println("Putting (changing) an existing element to 20 (return value): " + ageOfPeople.put("Gökbey",20));
        System.out.println("-----------------------------------------------------------");

        ageOfPeople.put("Ahmet",15);
        ageOfPeople.put("Nesrin",30);
        ageOfPeople.put("Kemal",3);
        ageOfPeople.put("Ayşe",90);
        ageOfPeople.put("Mehmet",60);
        ageOfPeople.put("Mahmut",6);
        ageOfPeople.put("Beyza",10);
        ageOfPeople.put("Fatma",50);
        ageOfPeople.put("Ali",9);
        ageOfPeople.put("Veli",29);
        System.out.println("Initial map:\n"+ageOfPeople);
        System.out.println("isEmpty on non-empty table: " + ageOfPeople.isEmpty());
        System.out.println("-----------------------------------------------------------");
        System.out.println("|Testing get method|:");
        System.out.println("Getting an existing element \"Gökbey\": " + ageOfPeople.get("Gökbey"));
        System.out.println("Getting an non-existing element \"Kamuran\": " + ageOfPeople.get("Kamuran"));
        System.out.println("-----------------------------------------------------------");
        System.out.println("|Testing remove method|:");
        System.out.println("Size of the map before removal:" + ageOfPeople.size());
        System.out.println("Return value of removing existing element (Gökbey): " + ageOfPeople.remove("Gökbey"));
        System.out.println("Return value of removing non-existing element (Kamuran): " + ageOfPeople.remove("Kamuran"));
        System.out.println("Map after removal:\n"+ageOfPeople);
        System.out.println("Size of the map after removal:" + ageOfPeople.size());
        System.out.println("-----------------------------------------------------------");
    }
    public static void testPart2_1_bigData(){
        KWHashMap<Integer, Integer> ageOfPeople = new HashtableChain<>();
        System.out.println("PART 2.1 (Hashtable with Chaining method using linked list) with big amount of data Test");
        System.out.println("|Testing isEmpty method|:");
        System.out.println("isEmpty on empty table: " + ageOfPeople.isEmpty());
        System.out.println("-----------------------------------------------------------");
        System.out.println("|Testing put method|:");
        System.out.println("Putting a new element with value 15 (return value): " + ageOfPeople.put(15,15));
        System.out.println("Putting (changing) an existing element to 20 (return value): " + ageOfPeople.put(15,20));
        System.out.println("-----------------------------------------------------------");

        Random rand = new Random();
        for(int i=0;i<1000;i++){
            ageOfPeople.put(Math.abs(rand.nextInt()), Math.abs(rand.nextInt()));
        }
        System.out.println("isEmpty on non-empty table: " + ageOfPeople.isEmpty());
        System.out.println("-----------------------------------------------------------");
        System.out.println("Size of table after adding 1001 elements: " + ageOfPeople.size());
        System.out.println("-----------------------------------------------------------");

        System.out.println("|Testing get method|:");
        System.out.println("Getting an existing element \"Gökbey\": " + ageOfPeople.get(15));
        System.out.println("Getting an non-existing element \"Kamuran\": " + ageOfPeople.get(3));
        System.out.println("-----------------------------------------------------------");
        System.out.println("|Testing remove method|:");
        System.out.println("Size of the map before removal:" + ageOfPeople.size());
        System.out.println("Return value of removing existing element (15): " + ageOfPeople.remove(15));
        System.out.println("Return value of removing non-existing element (3): " + ageOfPeople.remove(3));
        System.out.println("Size of the map after removal:" + ageOfPeople.size());
        System.out.println("Press 1 to see toString method test. (1000 elements), press anything else if you don't");
        Scanner input = new Scanner(System.in);
        int x = input.nextInt();
        if(x==1)
            System.out.println(ageOfPeople);
    }
    public static void testPart2_2(){
        KWHashMap<String, Integer> ageOfPeople = new HashtableChainTS<>();
        System.out.println("PART 2.2 (Hashtable with Chaining method using TreeSet) Test");
        System.out.println("|Testing isEmpty method|:");
        System.out.println("isEmpty on empty table: " + ageOfPeople.isEmpty());
        System.out.println("-----------------------------------------------------------");
        System.out.println("|Testing put method|:");
        System.out.println("Putting a new element with value 19 (return value): " + ageOfPeople.put("Gökbey",19));
        System.out.println("Putting (changing) an existing element to 20 (return value): " + ageOfPeople.put("Gökbey",20));
        System.out.println("-----------------------------------------------------------");

        ageOfPeople.put("Ahmet",15);
        ageOfPeople.put("Nesrin",30);
        ageOfPeople.put("Kemal",3);
        ageOfPeople.put("Ayşe",90);
        ageOfPeople.put("Mehmet",60);
        ageOfPeople.put("Mahmut",6);
        ageOfPeople.put("Beyza",10);
        ageOfPeople.put("Fatma",50);
        ageOfPeople.put("Ali",9);
        ageOfPeople.put("Veli",29);
        System.out.println("Initial map:\n"+ageOfPeople);
        System.out.println("isEmpty on non-empty table: " + ageOfPeople.isEmpty());
        System.out.println("-----------------------------------------------------------");
        System.out.println("|Testing get method|:");
        System.out.println("Getting an existing element \"Gökbey\": " + ageOfPeople.get("Gökbey"));
        System.out.println("Getting an non-existing element \"Kamuran\": " + ageOfPeople.get("Kamuran"));
        System.out.println("-----------------------------------------------------------");
        System.out.println("|Testing remove method|:");
        System.out.println("Size of the map before removal:" + ageOfPeople.size());
        System.out.println("Return value of removing existing element (Gökbey): " + ageOfPeople.remove("Gökbey"));
        System.out.println("Return value of removing non-existing element (Kamuran): " + ageOfPeople.remove("Kamuran"));
        System.out.println("Map after removal:\n"+ageOfPeople);
        System.out.println("Size of the map after removal:" + ageOfPeople.size());
        System.out.println("-----------------------------------------------------------");
    }
    public static void testPart2_2_bigData(){
        KWHashMap<Integer, Integer> ageOfPeople = new HashtableChainTS<>();
        System.out.println("PART 2.1 (Hashtable with Chaining method using linked list) with big amount of data Test");
        System.out.println("|Testing isEmpty method|:");
        System.out.println("isEmpty on empty table: " + ageOfPeople.isEmpty());
        System.out.println("-----------------------------------------------------------");
        System.out.println("|Testing put method|:");
        System.out.println("Putting a new element with value 15 (return value): " + ageOfPeople.put(15,15));
        System.out.println("Putting (changing) an existing element to 20 (return value): " + ageOfPeople.put(15,20));
        System.out.println("-----------------------------------------------------------");

        Random rand = new Random();
        for(int i=0;i<1000;i++){
            ageOfPeople.put(Math.abs(rand.nextInt()), Math.abs(rand.nextInt()));
        }
        System.out.println("isEmpty on non-empty table: " + ageOfPeople.isEmpty());
        System.out.println("-----------------------------------------------------------");
        System.out.println("Size of table after adding 1001 elements: " + ageOfPeople.size());
        System.out.println("-----------------------------------------------------------");

        System.out.println("|Testing get method|:");
        System.out.println("Getting an existing element \"Gökbey\": " + ageOfPeople.get(15));
        System.out.println("Getting an non-existing element \"Kamuran\": " + ageOfPeople.get(3));
        System.out.println("-----------------------------------------------------------");
        System.out.println("|Testing remove method|:");
        System.out.println("Size of the map before removal:" + ageOfPeople.size());
        System.out.println("Return value of removing existing element (15): " + ageOfPeople.remove(15));
        System.out.println("Return value of removing non-existing element (3): " + ageOfPeople.remove(3));
        System.out.println("Size of the map after removal:" + ageOfPeople.size());
        System.out.println("Press 1 to see toString method test. (1000 elements), press anything else if you don't");
        Scanner input = new Scanner(System.in);
        int x = input.nextInt();
        if(x==1)
            System.out.println(ageOfPeople);
    }
    public static void testPart2_3(){
        KWHashMap<String, Integer> ageOfPeople = new HashtableCoalesced<>();
        System.out.println("PART 2.3 (Hashtable with Coalesced hashing method) Test");
        System.out.println("|Testing isEmpty method|:");
        System.out.println("isEmpty on empty table: " + ageOfPeople.isEmpty());
        System.out.println("-----------------------------------------------------------");
        System.out.println("|Testing put method|:");
        System.out.println("Putting a new element with value 19 (return value): " + ageOfPeople.put("Gökbey",19));
        System.out.println("Putting (changing) an existing element to 20 (return value): " + ageOfPeople.put("Gökbey",20));
        System.out.println("-----------------------------------------------------------");

        ageOfPeople.put("Ahmet",15);
        ageOfPeople.put("Nesrin",30);
        ageOfPeople.put("Kemal",3);
        ageOfPeople.put("Ayşe",90);
        ageOfPeople.put("Mehmet",60);
        ageOfPeople.put("Mahmut",6);
        ageOfPeople.put("Beyza",10);
        ageOfPeople.put("Fatma",50);
        ageOfPeople.put("Ali",9);
        ageOfPeople.put("Veli",29);
        System.out.println("Initial map:\n"+ageOfPeople);
        System.out.println("isEmpty on non-empty table: " + ageOfPeople.isEmpty());
        System.out.println("-----------------------------------------------------------");
        System.out.println("|Testing get method|:");
        System.out.println("Getting an existing element \"Gökbey\": " + ageOfPeople.get("Gökbey"));
        System.out.println("Getting an non-existing element \"Kamuran\": " + ageOfPeople.get("Kamuran"));
        System.out.println("-----------------------------------------------------------");
        System.out.println("|Testing remove method|:");
        System.out.println("Size of the map before removal:" + ageOfPeople.size());
        System.out.println("Return value of removing existing element (Gökbey): " + ageOfPeople.remove("Gökbey"));
        System.out.println("Return value of removing non-existing element (Kamuran): " + ageOfPeople.remove("Kamuran"));
        System.out.println("Map after removal:\n"+ageOfPeople);
        System.out.println("Size of the map after removal:" + ageOfPeople.size());
        System.out.println("-----------------------------------------------------------");
    }
    public static void testPart2_3_bigData(){
        KWHashMap<Integer, Integer> ageOfPeople = new HashtableCoalesced<>();
        System.out.println("PART 2.1 (Hashtable with Chaining method using linked list) with big amount of data Test");
        System.out.println("|Testing isEmpty method|:");
        System.out.println("isEmpty on empty table: " + ageOfPeople.isEmpty());
        System.out.println("-----------------------------------------------------------");
        System.out.println("|Testing put method|:");
        System.out.println("Putting a new element with value 15 (return value): " + ageOfPeople.put(15,15));
        System.out.println("Putting (changing) an existing element to 20 (return value): " + ageOfPeople.put(15,20));
        System.out.println("-----------------------------------------------------------");

        Random rand = new Random();
        for(int i=0;i<1000;i++){
            ageOfPeople.put(Math.abs(rand.nextInt()), Math.abs(rand.nextInt()));
        }
        System.out.println("isEmpty on non-empty table: " + ageOfPeople.isEmpty());
        System.out.println("-----------------------------------------------------------");
        System.out.println("Size of table after adding 1001 elements: " + ageOfPeople.size());
        System.out.println("-----------------------------------------------------------");

        System.out.println("|Testing get method|:");
        System.out.println("Getting an existing element \"Gökbey\": " + ageOfPeople.get(15));
        System.out.println("Getting an non-existing element \"Kamuran\": " + ageOfPeople.get(3));
        System.out.println("-----------------------------------------------------------");
        System.out.println("|Testing remove method|:");
        System.out.println("Size of the map before removal:" + ageOfPeople.size());
        System.out.println("Return value of removing existing element (15): " + ageOfPeople.remove(15));
        System.out.println("Return value of removing non-existing element (3): " + ageOfPeople.remove(3));
        System.out.println("Size of the map after removal:" + ageOfPeople.size());
        System.out.println("Press 1 to see toString method test. (1000 elements), press anything else if you don't");
        Scanner input = new Scanner(System.in);
        int x = input.nextInt();
        if(x==1)
            System.out.println(ageOfPeople);
    }

}
