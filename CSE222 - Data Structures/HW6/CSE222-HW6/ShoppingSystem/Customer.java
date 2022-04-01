package ShoppingSystem;

import java.io.*;
import java.util.*;

/**
 * ShoppingSystem.Customer class for Shopping system. Extends ShoppingSystem.User.
 * @see Sort Private inner class Sort to sort Products.
 * @author gokbey_gazi_keskin
 */
public class Customer extends User{
    private static final String filePath = "products.txt";
    private static final String ordersFilePath = "orders.txt";
    private static final String customersFilePath = "customers.txt";

    private ArrayList<Product> searchedProducts;

    /**
     * Constructor for ShoppingSystem.Customer
     * @param ID ID of the ShoppingSystem.Customer
     * @param name Name of the ShoppingSystem.Customer
     * @param pw Password of the customer
     */
    public Customer(String ID, String name, String pw) throws IOException {
        super(ID, name, pw);
        searchedProducts = new ArrayList<>();
        saveCustomer();
    }


    @Override
    public boolean login(String id,String pw) throws IOException {
        BufferedReader br = new BufferedReader(new FileReader(customersFilePath));
        String idInFile;
        while( (idInFile = br.readLine()) != null){
            if(idInFile.equals(getID()) && idInFile.equals(id)){
                if(br.readLine().equals(loginInfo.get(id).pw) && loginInfo.get(id).pw.equals(pw))
                    return true;
                else{
                    System.out.println("Wrong password!");
                    return false;
                }
            }
        }
        System.out.println("Wrong id!");
        return false;
    }

    private void saveCustomer() throws IOException {
        FileWriter fw = new FileWriter(customersFilePath,true);
        fw.append(this.toString());
        fw.close();
    }


    /**
     * Finds the product which has the search text in it's name or description.
     * @param searchText Text which you want to search for.
     * @throws IOException when the file is not able to open
     */
    public void searchProduct(String searchText) throws IOException {
        searchedProducts = new ArrayList<>();
        BufferedReader br = new BufferedReader(new FileReader(filePath));
        String line;
        String name, description,ID;
        int price, discount;
        List<String> category;
        while ((line = br.readLine()) != null) {
            if(isID(line)){
                ID = line;
                name = br.readLine();
                category = new LinkedList<>(Arrays.asList(br.readLine().split(" >> ")));
                price = Integer.parseInt(br.readLine());
                discount = Integer.parseInt(br.readLine());
                description = br.readLine();
                if(name.contains(searchText) || description.contains(searchText)){
                    searchedProducts.add(new Product(name, category, price, discount, description,ID));
                }
            }
        }
        System.out.println("Search results:\n");

        int opt;
        Scanner input = new Scanner(System.in);
        if(!displaySearchedProducts()){
            System.out.println("There is no product with this criteria.");
            return;
        }
        while(true) {
            System.out.println("1)Buy\n2)Sort\n3)Filter\n4)Exit");
            opt = input.nextInt();
            input.nextLine(); //consume \n
            if (opt == 1) buy();
            else if (opt == 2) {
                sort();
            } else if (opt == 3) {
                filter();
            }
            else if(opt == 4){
                break;
            }
        }
        System.out.println("xx");

    }

    /**
     * Lists all the traders
     * @throws IOException if the file couldn't open.
     */
    public void listTraders() throws IOException{
        Queue<String> traders = new ArrayDeque<>();
        BufferedReader br = new BufferedReader(new FileReader(filePath));
        String line,traderName;
        while ((line = br.readLine()) != null) {
            if (isID(line)) {
                br.readLine();
                br.readLine();
                br.readLine();
                br.readLine();
                br.readLine();
                traderName = br.readLine();
                if(!traders.contains(traderName))
                    traders.add(traderName);
            }
        }
        for(int i=0;i<traders.size();i++)
            System.out.println(traders.poll());
        br.close();
    }

    /**
     * Lists the products of given trader.
     * @param traderName Name of the trader
     * @throws IOException if the file doesn't exist.
     */
    public void listProductsOfTrader(String traderName) throws IOException {
        searchedProducts = new ArrayList<>();
        BufferedReader br = new BufferedReader(new FileReader(filePath));
        String id;
        String name, description,traderNameinFile;
        int price, discount;
        List<String> category;
        while ((id = br.readLine()) != null) {
            if(isID(id)){
                name = br.readLine();
                category = new LinkedList<>(Arrays.asList(br.readLine().split(" >>")));
                price = Integer.parseInt(br.readLine());
                discount = Integer.parseInt(br.readLine());
                description = br.readLine();
                traderNameinFile = br.readLine();
                if(traderName.equals(traderNameinFile)){
                    searchedProducts.add(new Product(name, category, price, discount, description,id));
                }
            }
        }
        System.out.println("Search results:\n");

        int opt;
        Scanner input = new Scanner(System.in);
        if(!displaySearchedProducts()){
            System.out.println("There is no product with this criteria.");
            return;
        }
        while(true) {
            System.out.println("1)Buy\n2)Sort\n3)Filter\n4)Exit");
            opt = input.nextInt();
            input.nextLine(); //consume \n
            if (opt == 1) buy();
            else if (opt == 2) {
                sort();
            } else if (opt == 3) {
                filter();
            }
            else if(opt == 4){
                break;
            }
        }
        br.close();
    }

    /**
     * Sort the searched products by:
     * Name with merge sort
     * Price by heap sort
     * Discount percentage by shell sort
     */
    private void sort(){
        Scanner input = new Scanner(System.in);
        System.out.println("Sort by:\n1)Name\n2)Price\n3)Discount Percentage");
        int opt = input.nextInt();
        switch(opt){
            case 1:
                Sort.mergeSort(searchedProducts);
                displaySearchedProducts();
                System.out.println("Sorted by name.(A to Z) (listed above.)");
                break;
            case 2:
                Sort.heapSort(searchedProducts);
                displaySearchedProducts();
                System.out.println("Sorted by price (lowest to highest). (listed above.)");
                break;
            case 3:
                Sort.shellSort(searchedProducts);
                displaySearchedProducts();
                System.out.println("Sorted by discount percentage (highest to lowest). (listed above)");
                break;
            default:
                System.out.println("Wrong input.");
                break;
        }
    }

    /**
     * Filters the searched products by category or price.
     * @throws IOException when the file is not able to open.
     */
    private void filter() throws IOException {
        Scanner input = new Scanner(System.in);
        System.out.println("Filter by:\n1)Category\n2)Price");
        int opt = input.nextInt(); input.nextLine();

        switch(opt){
            case 1:
                String category;
                System.out.println("Type the category (you can type a higher or lower level category):");
                category = input.nextLine();
                boolean productFlag = false;
                int i = 1;
                for(Product j : searchedProducts){
                    if(j.getCategory().contains(category)) {
                        System.out.println(i + ") " + j);
                        productFlag = true;
                    }
                    i++;
                }
                if(productFlag) {
                    System.out.println("Which one would you like to buy?(type number.)");
                    i = input.nextInt();
                    buy(searchedProducts.get(i));
                }
                else {
                    System.out.println("There is no product with this category.");
                }
                break;

            case 2:
                System.out.println("Type the lower price bound");
                double lower = input.nextDouble();
                System.out.println("Type the upper price bound:");
                double upper = input.nextDouble();

                i = 1;
                productFlag = false;
                for(Product j : searchedProducts){
                    if(j.getPrice() <= upper && j.getPrice() >= lower) {
                        System.out.println(i + ") " + j);
                        productFlag = true;
                    }
                    i++;
                }
                if(productFlag) {
                    System.out.println("Which one would you like to buy?(type number.)");
                    i = input.nextInt();
                    buy(searchedProducts.get(i-1));
                }
                else
                    System.out.println("There is no product in this category.");
                break;
            default:
                System.out.println("Wrong input.");
                break;
        }

    }

    /**
     * used to validate given if the given id is a product id.
     * @param id Id of the product
     * @return true if it is a product id.
     */
    private boolean isID(String id){
        boolean isAllUpperorDigit = true; //id's contains only upper case characters and digits.

        for(int i=0;i<id.length();i++) {
            if (!(Character.isUpperCase(id.charAt(i)) || Character.isDigit(id.charAt(i))) ) {
                isAllUpperorDigit = false;
                break;
            }
        }
        return id.length() == 16 && !id.contains(" ") && isAllUpperorDigit;
    }

    /**
     * Buy a product (from the displayed menu)
     * @throws IOException when the file is not able to open.
     */
    private void buy() throws IOException {

        displaySearchedProducts();
        System.out.println("Which one would you like to buy (type index)?");
        Scanner input = new Scanner(System.in);
        int opt = input.nextInt();
        if(opt<1 || opt > searchedProducts.size()){
            System.out.println("There is no such ShoppingSystem.Product.");
        }
        else{
            System.out.println("How much would you like to buy?");
            int amount = input.nextInt();
            saveOrder(new Order(searchedProducts.get(opt-1),this,amount));
            System.out.println("We got your order!");
        }
    }

    /**
     * Buy the specified product.
     * @param product Buy the given product.
     * @throws IOException when the file is not able to open.
     */
    private void buy(Product product) throws IOException {

        Scanner input = new Scanner(System.in);

        System.out.println("How much would you like to buy?");
        int amount = input.nextInt();
        saveOrder(new Order(product,this,amount));
        System.out.println("We got your order!");

    }

    /**
     * Saves the order to the orders.txt file after buy operation.
     * @param order ShoppingSystem.Order of the customer
     * @throws IOException when the file is not able to open.
     */
    private void saveOrder(Order order) throws IOException {
        FileWriter fw = new FileWriter(ordersFilePath, true);
        fw.append(order.toString());
        fw.close();
    }

    /**
     * Displays searched products
     * @return true if there are searched products to list.
     */
    private boolean displaySearchedProducts(){
        int i=1;
        if(searchedProducts.size()==0)
            return false;


        for(Product j: searchedProducts){
            System.out.println(i + ")" +j);
            i++;
        }
        System.out.println(searchedProducts.size() + " products found (listed above)");
        return true;
    }


    /**
     * Private inner class Sort
     */
    private static class Sort {

        /**
         * Merge sort algorith
         * @param inputArray Arraylist to sort.
         */
        public static void mergeSort(ArrayList<Product> inputArray){ //sorts according to names of the products.
            divide(inputArray,0, inputArray.size()-1);
        }
        private static void divide(ArrayList<Product> inputArray,int startIndex,int endIndex){

            //Divide till you breakdown your list to single element
            if(startIndex<endIndex && (endIndex-startIndex)>=1){
                int mid = (endIndex + startIndex)/2;
                divide(inputArray,startIndex, mid);
                divide(inputArray,mid+1, endIndex);

                //merging Sorted array produce above into one sorted array
                merger(inputArray, startIndex,mid,endIndex);
            }
        }

        private static void merger(ArrayList<Product> inputArray,int startIndex,int midIndex,int endIndex){

            //Below is the mergedarray that will be sorted array Array[i-midIndex] , Array[(midIndex+1)-endIndex]
            ArrayList<Product> mergedSortedArray = new ArrayList<>();

            int leftIndex = startIndex;
            int rightIndex = midIndex+1;

            while(leftIndex<=midIndex && rightIndex<=endIndex){
                if(inputArray.get(leftIndex).getName().compareTo(inputArray.get(rightIndex).getName())<=0){
                    mergedSortedArray.add(inputArray.get(leftIndex));
                    leftIndex++;
                }else{
                    mergedSortedArray.add(inputArray.get(rightIndex));
                    rightIndex++;
                }
            }

            //Either of below while loop will execute
            while(leftIndex<=midIndex){
                mergedSortedArray.add(inputArray.get(leftIndex));
                leftIndex++;
            }

            while(rightIndex<=endIndex){
                mergedSortedArray.add(inputArray.get(rightIndex));
                rightIndex++;
            }

            int i = 0;
            int j = startIndex;
            //Setting sorted array to original one
            while(i<mergedSortedArray.size()){
                inputArray.set(j, mergedSortedArray.get(i++));
                j++;
            }
        }

        /**
         * Heap sort algorithm
         * @param inputArray Arraylist to be sorted.
         */
        public static void heapSort(ArrayList<Product> inputArray)
        {
            int n = inputArray.size();

            // Build heap (rearrange array)
            for (int i = n / 2 - 1; i >= 0; i--)
                heapify(inputArray, n, i);

            // One by one extract an element from heap
            for (int i = n - 1; i > 0; i--) {
                // Move current root to end
                Product temp = inputArray.get(0);
                inputArray.set(0,inputArray.get(i));
                inputArray.set(i,temp);

                // call max heapify on the reduced heap
                heapify(inputArray, i, 0);
            }
        }


        private static void heapify(ArrayList<Product> inputArray, int n, int i)
        {
            int largest = i; // Initialize largest as root
            int l = 2 * i + 1; // left = 2*i + 1
            int r = 2 * i + 2; // right = 2*i + 2

            // If left child is larger than root
            if (l < n && inputArray.get(l).getPrice() > inputArray.get(largest).getPrice())
                largest = l;

            // If right child is larger than largest so far
            if (r < n && inputArray.get(r).getPrice() > inputArray.get(largest).getPrice())
                largest = r;

            // If largest is not root
            if (largest != i) {
                Product swap = inputArray.get(i);
                inputArray.set(i,inputArray.get(largest));
                inputArray.set(largest,swap);

                heapify(inputArray, n, largest);
            }
        }

        /**
         * Shell sort algorithm
         * @param inputArray ArrayList to be sorted.
         */
        private static void shellSort(ArrayList<Product> inputArray)
        {
            int n = inputArray.size();

            for (int gap = n/2; gap > 0; gap /= 2)
            {

                for (int i = gap; i < n; i += 1)
                {

                    Product temp = inputArray.get(i);


                    int j;
                    for (j = i; j >= gap && inputArray.get(j-gap).getDiscount() / inputArray.get(j-gap).getPrice() <
                                                                temp.getDiscount() / temp.getPrice(); j -= gap)
                        inputArray.set(j,inputArray.get(j - gap));  //sorting wrt discount / overall price


                    inputArray.set(j,temp);
                }
            }
        }
    }

}
