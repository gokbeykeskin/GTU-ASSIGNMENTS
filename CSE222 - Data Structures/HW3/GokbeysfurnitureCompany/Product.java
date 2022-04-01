package GokbeysfurnitureCompany;

/**
 * Product class for Furniture Company.
 * @author gokbey_gazi_keskin_1901042631
 */
public class Product {
    private String name;
    private int id;
    private String color;
    private int amount;


    /**
     * Constructor for Product. Inıtial amount is 0.
     * @param name Name of the product.
     * @param id ID of the product.
     * @param color Color of the product.
     */
    public Product(String name, int id, String color) {
        setName(name);
        setId(id);
        setColor(color);
        setAmount(0);
    }

    /**
     * Increases the amount of the product.
     * @param amount Increment amount.
     */
    public void addProduct(int amount){
        this.amount+=amount;
    }

    /**
     * Getter for name of the product.
     * @return Name of the product.
     */
    public String getName() {
        return name;
    }

    /**
     * Setter for name of the product.
     * @param name Name of the product.
     */
    public void setName(String name) {
        this.name = name;
    }
    /**
     * Getter for ID of the product.
     * @return ID of the product.
     */
    public int getId() {
        return id;
    }

    /**
     * Setter for ID of the product.
     * @param id Name of the product.
     */
    public void setId(int id) {
        this.id = id;
    }
    /**
     * Getter for color of the product.
     * @return color of the product.
     */
    public String getColor() {
        return color;
    }

    /**
     * Setter for color of the product.
     * @param color Name of the product.
     */
    public void setColor(String color) {
        this.color = color;
    }
    /**
     * Getter for amount of the product.
     * @return amount of the product.
     */
    public int getAmount() {
        return amount;
    }

    /**
     * Setter for amount of the product.
     * @param amount amount of the product.
     */
    public void setAmount(int amount) {
        this.amount = amount;
    }

    /**
     * Decrease the amount of the product
     * @param amount Amount to be decreased.
     */
    public void decreaseAmount(int amount){
        if(!(amount<=this.amount))
            throw new IllegalArgumentException("There is not enough Product in this branch!.");
            this.amount-=amount;
    }
    /**
     * toString implementation for Product.
     * @return String representation of the Product.
     */
    @Override
    public String toString(){
        return "Name:" + name + "\nColor:" + getColor() + "\nAmount:" +
                getAmount() + "\nProduct Code(Model):" + getId() + "\n";
    }
}
