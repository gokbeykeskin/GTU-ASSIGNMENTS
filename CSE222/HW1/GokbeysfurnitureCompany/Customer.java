package GokbeysfurnitureCompany;

import java.util.NoSuchElementException;

/**
 * Interface for BrandCustomer.
 * @author gokbey_gazi_keskin
 */
public interface Customer {

    /**
     * Getter for phone number of the customer.
     * @return Phone number of the customer.
     */
    public String getPhoneNumber();

    /**
     *  Setter for phone number of the customer.
     * @param phoneNumber Phon number of the customer.
     */
    public void setPhoneNumber(String phoneNumber);

    /**
     * Getter for E-mail of the customer.
     * @return E-mail of the customer.
     */
    public String geteMail();

    /**
     * Setter for E-mail of the customer.
     * @param eMail E-mail of the customer.
     */
    public void seteMail(String eMail);

    /**
     * Getter for password of the customer.
     * @return Password of the customer.
     */
    public String getPassword();

    /**
     * Setter for password of the customer.
     * @param password Password of the customer.
     */
    public void setPassword(String password);

    /**
     * Login to the system.
     * @param mail E-mail of the customer.
     * @param pw Password of the customer.
     */
    void login(String mail,String pw);

    /**
     *  List all the products in specified branch.
     * @param branchID ID of the branch. This branches products will be listed.
     * @throws IllegalStateException if the customer is not logged in.
     * @throws NoSuchElementException if the branch doesn't exist.
     */
    void listProducts(int branchID) throws IllegalStateException,NoSuchElementException;
    /**
     *  Search a product in all branches.
     * @param productID Product id to search.
     * @throws IllegalStateException when the user is not logged in.
     */
    void search(int productID) throws IllegalStateException;
    /**
     *  Search a product in all branches.
     * @param name Product id to search.
     * @throws IllegalStateException when the user is not logged in.
     */
    void search(String name) throws IllegalStateException;
    /**
     *  Buy the specified product from specified branch.
     * @param branchID ID of the branch
     * @param productID ID of the product
     * @param amount Amount of the product
     * @throws NoSuchElementException If the branch or the product doesn't exist.
     * @throws IllegalStateException If the user is not logged in.
     */
    void buyOnline(int branchID,int productID,int amount);
    /**
     * Add previous order to customer data.
     * @param order Order of the customer.
     */
    void addToPreviousOrders(Order order);
    /**
     * Getter for Previous Orders.
     * @return Previous orders.
     */
    CompanyContainer<Order> getPreviousOrders();
}
