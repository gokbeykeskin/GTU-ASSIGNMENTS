package ShoppingSystem;

import java.io.*;
import java.util.*;

/**
 * ShoppingSystem.Trader class for Shopping system. Extends user
 * @author gokbey_gazi_keskin
 */
public class Trader extends User {
    private static final String filePath = "products.txt";
    private static final String ordersFilePath = "orders.txt";
    private static final String tradersFilePath = "traders.txt";

    /**
     * Constructor for trader
     * @param ID ID of the trader
     * @param name Name of the trader
     * @param pw Password of the trader
     */
    public Trader(String ID, String name, String pw) throws IOException {
        super(ID, name, pw);
        saveTrader();
    }

    @Override
    public boolean login(String id,String pw) throws IOException {
        BufferedReader br = new BufferedReader(new FileReader(tradersFilePath));
        String idInFile;
        while( (idInFile = br.readLine()) != null){
            if(idInFile.equals(getID()) && idInFile.equals(id)){
                if(br.readLine().equals(loginInfo.get(id).pw) && loginInfo.get(id).pw.equals(pw)) {
                    return true;
                }
                else{
                    System.out.println("Wrong password!");
                    return false;
                }
            }
        }
        System.out.println("Wrong id!");
        return false;
    }

    private void saveTrader() throws IOException {
        FileWriter fw = new FileWriter(tradersFilePath,true);
        fw.append(this.toString());
        fw.close();
    }

    /**
     * Adds a new product to traders shop
     * @param name Name of the product
     * @param category Category of the product
     * @param price Price of the product
     * @param discount Discount of the product
     * @param description Description of the products
     * @throws IOException if the file is not able to be opened.
     */
    public void addProduct(String name, List<String> category, Integer price,
                           Integer discount, String description) throws IOException {
        Product product = new Product(name, category, price, discount, description);
        product.setTraderName(this.getName());
        saveProduct(product);
        System.out.println("ID of the new ShoppingSystem.Product: " + product.getID());
    }

    /**
     * Save the product to products.txt
     * @param product ShoppingSystem.Product which will be added to products.txt
     * @throws IOException if the file is not able to be opened.
     */
    private void saveProduct(Product product) throws IOException {
        FileWriter fw = new FileWriter(filePath, true);
        fw.append(product.getID()).append("\n").append(product.getName()).append("\n");
        StringBuilder cat = new StringBuilder();
        for (String i : product.getCategory()) {
            cat.append(i).append(" >> ");
        }
        cat.delete(cat.lastIndexOf(" >> "), cat.length());
        fw.append(cat.toString());
        fw.append("\n").append(String.valueOf(product.getPrice())).append("\n").append(String.valueOf(product.getDiscount())).
                append("\n").append(product.getDescription()).append("\n").append(this.getName()).append("\n\n");
        fw.close();
    }

    /**
     * Searches the products.txt Returns the product with the given ID
     * @param ID ID of the product
     * @return The product with given ID.
     * @throws IOException if the file is not able to be opened.
     */
    public Product getProduct(String ID) throws IOException {
        BufferedReader br = new BufferedReader(new FileReader(filePath));
        String line;
        String name, description;
        int price, discount;
        List<String> category;

        while ((line = br.readLine()) != null) {
            if (line.equals(ID)) {
                name = br.readLine();
                category = new LinkedList<>(Arrays.asList(br.readLine().split(" >>")));
                price = Integer.parseInt(br.readLine());
                discount = Integer.parseInt(br.readLine());
                description = br.readLine();
                Product product = new Product(name, category, price, discount, description,ID);
                product.setTraderName(br.readLine());
                br.close();
                return product;
            }
        }
        br.close();
        return null;
    }


    /**
     * Removes the product from the products.txt
     * @param ID ID of the product which will be removed
     * @throws IOException if the file is not able to be opened.
     * @return true if the product is found and removed.
     */
    public boolean removeProduct(String ID) throws IOException {
        try {
            if (!getProduct(ID).getTrader().equals(this.getName())) {
                System.out.println("This product doesn't belong to you!");
                return false;
            }
        }
        catch(NullPointerException e){
            System.out.println("There is no such product.");
        }
        final int linePerProduct = 7;
        FileWriter fw = new FileWriter("temp.txt", false);
        BufferedReader br = new BufferedReader(new FileReader(filePath));
        String line;
        while ((line = br.readLine()) != null) {
            if (line.equals(ID)) {
                for (int i = 0; i < linePerProduct - 1; i++)
                    br.readLine();
                br.readLine(); //consume blank line
                line = br.readLine();
            }
            fw.append(line).append("\n");
        }
        br.close();
        fw.close();

        File oldFile = new File(filePath);
        File newFile = new File("temp.txt");
        if (oldFile.delete()) {
            newFile.renameTo(oldFile);
        }
        return true;

    }

    /**
     * Edits the product with given id in products.txt
     * @param ID ID of the product
     * @throws IOException if the file is not able to be opened.
     */
    public void editProduct(String ID) throws IOException {
        try {
            if (!getProduct(ID).getTrader().equals(this.getName())) {
                System.out.println("This product doesn't belong to you!");
                return;
            }
        }catch(NullPointerException e){
            System.out.println("There is no such product.");
        }
        BufferedReader br = new BufferedReader(new FileReader(filePath));
        String line;
        String name, description;
        int price, discount;
        List<String> category;
        int opt;
        Scanner input = new Scanner(System.in);
        while ((line = br.readLine()) != null) {
            if (line.equals(ID)) {
                name = br.readLine();
                category = new LinkedList<>(Arrays.asList(br.readLine().split(" >>")));
                price = Integer.parseInt(br.readLine());
                discount = Integer.parseInt(br.readLine());
                description = br.readLine();

                do {
                    input.reset();
                    System.out.println("What would you like to edit this product with ID: " + ID + "\nName :" + name +
                            "\nCategory: " + category + "\nPrice: " + price + "\nDiscount: " + discount +
                            "\nDescription: " + description);
                    System.out.println("1)Name\n2)Category\n3)Price\n4)Discount\n5)Description\n6)Stop Editing");

                    opt = input.nextInt(); input.nextLine();
                    switch (opt) {
                        case 1:
                            System.out.println("New name: ");
                            name = input.nextLine();
                            break;
                        case 2:
                            System.out.println("New category: (input one by one,type 0 to stop):");
                            String newCategory;
                            category = new LinkedList<>();
                            do {
                                newCategory = input.next();
                                if (!newCategory.equals("0"))
                                    category.add(newCategory);
                            } while (!newCategory.equals("0"));
                            break;
                        case 3:
                            System.out.println("New price:");
                            price = input.nextInt(); input.nextLine();
                            break;
                        case 4:
                            System.out.println("New discount:");
                            discount = input.nextInt(); input.nextLine();
                            break;
                        case 5:
                            System.out.println("New description:");
                            description = input.nextLine();
                            break;
                        case 6:
                            System.out.println("Redirecting to trader menu.");
                            break;
                        default:
                            System.out.println("Wrong input.");
                            break;
                    }
                } while (opt != 6);
                br.close();
                Product newProduct = new Product(name,category,price,discount,description,ID);
                removeProduct(ID);
                saveProduct(newProduct);
                break;
            }
        }

    }

    /**
     * Allows trader to see waiting orders and accept-decline them.
     * @throws IOException if the file is not able to be opened.
     */
    public void seeWaitingOrders() throws IOException {
        BufferedReader br = new BufferedReader(new FileReader(ordersFilePath));
        FileWriter fw = new FileWriter("temp.txt", false);

        String line;
        String productID,productName,customerName,amount;
        boolean listed = false;
        while ((line = br.readLine()) != null) {
            productID = br.readLine();
            productName = br.readLine();
            customerName = br.readLine();
            amount = br.readLine();
            if(line.equals(Status.WAITING.toString())) {
                System.out.println("ShoppingSystem.Product ID: " + productID + "\nShoppingSystem.Product Name: " + productName +
                        "\nShoppingSystem.Customer Name: " + customerName + "\nAmount: " + amount + "\n1)Accept 2)Decline");
                Scanner input = new Scanner(System.in);
                int opt = input.nextInt();
                if (opt == 1)
                    line = Status.ACCEPTED.name();
                else if (opt == 2)
                    line = Status.DECLINED.name();
                else
                    System.out.println("Wrong input.");
                listed = true;
            }
            fw.append(line).append("\n").append(productID).append("\n").append(productName).append("\n")
                    .append(customerName).append("\n").append(amount).append("\n");

        }
        if(!listed)
            System.out.println("There is no active orders to list!");

        fw.close();
        br.close();

        File oldFile = new File(ordersFilePath);
        File newFile = new File("temp.txt");

        if (oldFile.delete()) {
            newFile.renameTo(oldFile);
        }

    }
}
