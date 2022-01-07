package ShoppingSystem;

import java.util.List;
import java.util.Random;

public class Product implements Comparable<Product>{
    private final List<String> category;
    private final String name;
    private final String description;
    private final Integer price;
    private final Integer discount;
    private final String ID;
    private String traderName;

    /**
     * Constructor for ShoppingSystem.Product which already exists in products.txt
     * @param name name of the product
     * @param category category of the product
     * @param price price of the product
     * @param discount discount of the product
     * @param description description of the product
     */
    public Product(String name,List<String> category,Integer price, Integer discount,String description) {
        this.category = category;
        this.name = name;
        this.description = description;
        this.price = price;
        this.discount = discount;
        ID = generateID();
    }

    /**
     * Constructor for constructing a product which already exists in products.txt
     * @param name name of the product
     * @param category category of the product
     * @param price price of the product
     * @param discount discount of the product
     * @param description description of the product
     * @param ID existing id of the product.
     */
    public Product(String name,List<String> category,Integer price, Integer discount,String description,String ID) {
        this.category = category;
        this.name = name;
        this.description = description;
        this.price = price;
        this.discount = discount;
        this.ID = ID;
    }

    public String getID() {
        return ID;
    }

    public void setTraderName(String trader) {
        this.traderName = trader;
    }

    public String getTrader() {
        return traderName;
    }

    public List<String> getCategory() {
        return category;
    }


    public String getName() {
        return name;
    }


    public String getDescription() {
        return description;
    }


    public int getPrice() {
        return price;
    }


    public int getDiscount() {
        return discount;
    }

    /**
     * Generates a random 16 digits upper case ID
     * @return
     */
    private String generateID(){
        int leftLimit = 'A'; // letter 'A'
        int rightLimit = 'Z'; // letter 'Z'
        int targetStringLength = 16;
        Random random = new Random();

        return random.ints(leftLimit, rightLimit + 1)
                .limit(targetStringLength)
                .collect(StringBuilder::new, StringBuilder::appendCodePoint, StringBuilder::append)
                .toString();

    }

    /**
     * compareTo implementation of the ShoppingSystem.Product (compares names)
     * @param o Other product
     * @return negative if this products name is less then the other, 0 if they are the same, positive otherwise.
     */
    @Override
    public int compareTo(Product o) {
        return name.compareTo(o.getName());
    }

    /**
     * toString implementation for the ShoppingSystem.Product class.
     * @return String representation of the product class.
     */
    @Override
    public String toString(){
        return "ShoppingSystem.Product Details\n" + "ShoppingSystem.Product ID:" + ID + "\nName: " + name + "\nCategory: " + category
                + "\nPrice: " + price + "\nDiscount: " + discount + "\nDescription:" + description + "\n\n";
    }
}
