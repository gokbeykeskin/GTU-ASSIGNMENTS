package GokbeysfurnitureCompany;

/**
 * Order class for the Furniture Company.
 * @author gokbey_gazi_keskin_19010402631
 */
public class Order {
    private Product product;
    private int amount;

    /**
     * Constructor for Order
     * @param product Product of the order.
     * @param amount Amount of the product.
     */
    public Order(Product product, int amount) {
        this.product = product;
        this.amount = amount;
    }

    /**
     * Getter for product.
     * @return product
     */
    public Product getProduct() {
        return product;
    }

    /**
     * Setter for product
     * @param product Product of the order.
     */
    public void setProduct(Product product) {
        this.product = product;
    }
    /**
     * Getter for amount.
     * @return amount
     */
    public int getAmount() {
        return amount;
    }
    /**
     * Setter for amount
     * @param amount Amount of the product.
     */
    public void setAmount(int amount) {
        this.amount = amount;
    }

    /**
     * toString implementation for Order class.
     * @return String representation of the order class.
     */
    @Override
    public String toString(){
        return "Product Name: " + product.getName() + "\nProduct Color: "+product.getColor() +"\nOrder Amount: " + amount + "\n";
        //didn't use product.toString() because product amount and order amount are different.
    }

}
