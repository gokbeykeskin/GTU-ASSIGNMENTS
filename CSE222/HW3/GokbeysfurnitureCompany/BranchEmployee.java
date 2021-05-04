package GokbeysfurnitureCompany;

import java.util.NoSuchElementException;

/**
 * BranchEmployee class for the Furniture Company. Extends abstract class User. Implements Employee interface.
 * @author gokbey_gazi_keskin_19010402631
 * */
public class BranchEmployee extends User implements Employee{

    private Branch employeesBranch; //reference to employee's branch.

    /**
     * Constructor for Branch Employee
     * @param name Name of the employee.
     * @param surname Surname of the employee.
     * @param id ID of the employee.
     */
    public BranchEmployee(String name, String surname, int id) {
        super(name, surname, id);
    }

    /**
     * Setter for Employee's Branch.
     * @param employeesBranch Branch which employee will be working at.
     */
    public void setEmployeesBranch(Branch employeesBranch) {
        this.employeesBranch = employeesBranch;
    }

    /**
     * Getter for Employee's Branch.
     * @return Branch which the employee is working at.
     * @throws IllegalArgumentException when employee doesn't have a branch.
     */
    public Branch getEmployeesBranch() throws IllegalArgumentException{
        if(employeesBranch == null) throw new IllegalArgumentException("This employee doesn't have a branch yet.");
        return employeesBranch;
    }

    /**
     * Inquire Products of the branch. Usually used with informAdmin method.
     * @param productID ID of the product which will bi inquired
     * @throws NoSuchElementException if there is no such product.
     * @return true if the product is needed(less then 10 on amount). false otherwise.
     */
    @Override
    public boolean inquireProduct(int productID) throws NoSuchElementException{
        if (employeesBranch.getProduct(productID) == null)
                throw new NoSuchElementException("Product doesn't exist.");

        return employeesBranch.getProduct(productID).getAmount() < 10; //true if new products needed

    }

    /**
     * Inform the Admin if a product is needed. Usually used with inquireProduct method.
     * @param productID ID of the product which will bi inquired
     */
    @Override
    public void informAdmin(int productID) {
        employeesBranch.addNeededProduct(productID);
    }


    /**
     * Make a sale to a customer face-to-face (in store.)
     * @param productID ID of the product which will be selled.
     * @param customerNum ID of the customer.
     * @param amount Amount of the product.
     * @throws NoSuchElementException when there is no customer with this ID.
     * @throws IllegalArgumentException you can't sell zero or less products.
     * @throws IllegalStateException when there is not enough product  to sell
     */
    @Override
    public void makeSale(int productID, int customerNum,int amount)
            throws NoSuchElementException, IllegalArgumentException,IllegalStateException {
        if(employeesBranch.getCustomer(customerNum) == null)
            throw new NoSuchElementException("There is no account for this customer. Create one.");
        else if(employeesBranch.getProduct(productID) == null)
            throw new NoSuchElementException("There is no product with this ID");
        else if(amount<=0)
            throw new IllegalArgumentException("You can't sell zero or less product!");
        else if(amount <= employeesBranch.getProduct(productID).getAmount()) {
            employeesBranch.getProduct(productID).decreaseAmount(amount);
            employeesBranch.getCustomer(customerNum).addToPreviousOrders(
                    new Order(employeesBranch.getProduct(productID),amount));
        }
        else throw new IllegalStateException("There is not enough products.");
    }

    /**
     * Create subscription for a new customer. Only used for face-to-face sells. Online sells handles this automatically.
     * @param newCus New Customer.
     * @throws IllegalArgumentException If the customer already has a subscription.
     */
    public void createSubscription(BrandCustomer newCus) throws IllegalArgumentException{
        if(employeesBranch.getCustomers().contains(newCus))
            throw new IllegalArgumentException("This customer already has a subscription.");
        employeesBranch.addNewCustomer(newCus);
    }

    /**
     * Getter for previous orders of a customer.
     * @param customerNum ID of the customer
     * @return Previous orders of the customer. Null if there isn't any.
     * @throws NoSuchElementException if the given customer doesn't exist."
     */
    @Override
    public KWArrayList<Order> getPreviousOrders(int customerNum) throws NoSuchElementException{
        if(employeesBranch.getCustomer(customerNum)==null)
            throw new NoSuchElementException("This customer doesn't exist.");
        return employeesBranch.getCustomer(customerNum).getPreviousOrders();
    }

    @Override
    public String toString() {
        return super.toString() + " \nPosition: Branch Employee\nBranch:" + employeesBranch;
    }
}
