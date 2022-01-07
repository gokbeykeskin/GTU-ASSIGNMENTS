package ShoppingSystem;

/**
 * ShoppingSystem.Order class for Shopping System.
 * @author gokbey_gazi_keskin
 */
public class Order {
    private final Product product;
    private final Customer customer;
    private final int amount;
    private final Status status;


    /**
     * Constructor for ShoppingSystem.Order
     * @param product ShoppingSystem.Product of the order
     * @param customer ShoppingSystem.Customer of the order
     * @param amount Amount of the order
     */
    public Order(Product product,Customer customer,int amount){
        this.product = product;
        this.customer = customer;
        this.amount = amount;
        this.status = Status.WAITING;
    }

    @Override
    public String toString(){
        return status + "\n" + product.getID() + "\n" + product.getName()
                                                +"\n" + customer.getID() + "\n" + amount +"\n";
    }
}
