package GokbeysfurnitureCompany;

import java.util.NoSuchElementException;

/**
 * Interface for BranchEmployee.
 * @author gokbey_gazi_keskin
 */
public interface Employee {
    /**
     * Inquire Products of the branch. Usually used with informAdmin method.
     * @param productID ID of the product which will bi inquired
     * @throws NullPointerException if there is no such product.
     * @return true if the product is needed(less then 10 on amount). false otherwise.
     */
    public boolean inquireProduct(int productID);
    /**
     * Inform the Admin if a product is needed. Usually used with inquireProduct method.
     * @param productID ID of the product which will bi inquired
     */
    public void informAdmin(int productID);
    /**
     * Add some stock to a already existing product.
     * @param productID ID of the product.
     * @param amount Amount of the product which will be added to stock.
     * @throws IllegalArgumentException if the given amount is less then 1.
     * @throws NoSuchElementException if the given product ID doesn't belong to eny product in the branch.
     */

    public void makeSale(int productID, int customerNum,int amount)throws NoSuchElementException,IllegalArgumentException;

    /**
     * Create subscription to a customer. Only used for face-to-face sells. Online sells handles this automatically.
     * @param newCus New Customer.
     * @throws IllegalArgumentException If the customer already has a subscription.
     */
    public void createSubscription(BrandCustomer newCus) throws IllegalArgumentException;

    /**
     * Getter for previous orders of a customer.
     * @param customerNum ID of the customer
     * @return Previous orders of the customer. Null if there isn't any.
     */
    CompanyContainer<Order> getPreviousOrders(int customerNum);
}
