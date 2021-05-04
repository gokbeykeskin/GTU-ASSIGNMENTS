package GokbeysfurnitureCompany;

import java.util.NoSuchElementException;

/**
 * Brand Customer class Furniture Company.
 * @author gokbey_gazi_keskin_1901042631
 */
public class BrandCustomer extends User implements Customer{
    private String phoneNumber;
    private String eMail;
    private String password;
    private static int customerNum=10000;
    private final CompanyContainer<Order> previousOrders;
    private static final CompanyContainer<String> mails = new CompanyContainer<>();
    private boolean loggedIn;

    /**
     * Constructor for Brand Customer
     * @param name Name of the Customer.
     * @param surname Surname of the Customer.
     * @param phoneNumber Phone number of the Customer.
     * @param eMail E-mail of the Customer.
     * @param password Password of the Customer.
     */
    public BrandCustomer(String name, String surname,String phoneNumber,String eMail,String password) {
        super(name, surname, generateID(eMail));
        this.phoneNumber = phoneNumber;
        if(mails.contains(eMail)>=0)
            throw new IllegalArgumentException("This mail is already signed up!");
        mails.insert(eMail);
        this.eMail = eMail;
        this.password = password;
        previousOrders= new CompanyContainer<>();


        loggedIn=false;
    }

    /**
     * Generates a unique ID for the customer.
     * @param eMail mail adress of the new customer
     * @return ID of the customer.
     */
    private static int generateID(String eMail){
        while(User.getIds().contains(customerNum)>=0){
                customerNum++;
        }
        if(mails.contains(eMail)==-1)
        System.out.println("-----------------Your auto-generated ID: "
                +customerNum + "-----------------");
        return customerNum;
    }

    /**
     * Getter for phone number of the customer.
     * @return Phone number of the customer.
     */
    public String getPhoneNumber() {
        return phoneNumber;
    }

    /**
     *  Setter for phone number of the customer.
     * @param phoneNumber Phon number of the customer.
     */
    public void setPhoneNumber(String phoneNumber) {
        this.phoneNumber = phoneNumber;
    }

    /**
     * Getter for E-mail of the customer.
     * @return E-mail of the customer.
     */
    public String geteMail() {
        return eMail;
    }

    /**
     * Setter for E-mail of the customer.
     * @param eMail E-mail of the customer.
     */
    public void seteMail(String eMail) {
        this.eMail = eMail;
    }

    /**
     * Getter for password of the customer.
     * @return Password of the customer.
     */
    public String getPassword() {
        return password;
    }

    /**
     * Setter for password of the customer.
     * @param password Password of the customer.
     */
    public void setPassword(String password) {
        this.password = password;
    }

    /**
     * Login to the system.
     * @param mail E-mail of the customer.
     * @param pw Password of the customer.
     */
    public void login(String mail,String pw){
        if (!loggedIn && mail.equals(eMail) && pw.equals(password)) {
            loggedIn = true;
            System.out.println("You have successfully logged in.");
        }
        else if(loggedIn)
            System.out.println("You are already logged in.");
        else
            System.out.println("Wrong info!");
    }


    /**
     *  List all the products in specified branch.
     * @param branchID ID of the branch. This branches products will be listed.
     * @throws IllegalStateException if the customer is not  logged in.
     * @throws NoSuchElementException if the branch doesn't exist.
     */
    public void listProducts(int branchID) throws  NoSuchElementException,IllegalStateException{
        if(loggedIn) {
            boolean branchFlag=false;
            for (int i = 0; i < Administrator.branches.size(); i++) {
                if (Administrator.branches.at(i).getBranchID() == branchID) {
                    branchFlag=true;
                    for (int j = 0; j < Administrator.branches.at(i).getProducts().size(); j++) {
                        System.out.print(Administrator.branches.at(i).getProducts().at(j) + "\n");
                    }
                }
            }
            if(!branchFlag)
                throw new NoSuchElementException("This branch doesn't exist");
        }
        else throw new IllegalStateException("This user is not logged in!");
    }

    /**
     *  Search a product in all branches.
     * @param productID Product id to search.
     * @throws IllegalStateException when the user is not logged in.
     */
    public void search(int productID) throws IllegalStateException{
        if(loggedIn) {

            for (int i = 0; i < Administrator.branches.size(); i++) {
                for (int j = 0; j < Administrator.branches.at(i).getProducts().size(); j++) {
                    if (Administrator.branches.at(i).getProducts().at(j).getId() == productID)
                        System.out.print("Branch code " + Administrator.branches.at(i).getBranchID() + " has "
                                + Administrator.branches.at(i).getProducts().at(j).getAmount() + " of this product.\n");
                }
            }
        }
        else throw new IllegalStateException("This user is not logged in!");
    }

    /**
     * Search a product in all branches.
     * @param productName Product name to search.
     */
    public void search(String productName){
        if(loggedIn) {
            for (int i = 0; i < Administrator.branches.size(); i++) {
                for (int j = 0; j < Administrator.branches.at(i).getProducts().size(); j++) {
                    if (Administrator.branches.at(i).getProducts().at(j).getName().equals(productName))
                        System.out.print("Branch code " + Administrator.branches.at(i).getBranchID() + " has "
                                + Administrator.branches.at(i).getProducts().at(j).getAmount() + " of this product.\n");
                    else
                        System.out.println("This Product doesn't exist.");
                }
            }
        }
        else throw new IllegalStateException("This user is not logged in!");
    }

    /**
     *  Buy the specified product from specified branch.
     * @param branchID ID of the branch
     * @param productID ID of the product
     * @param amount Amount of the product
     * @throws NoSuchElementException If the branch or the product doesn't exist.
     * @throws IllegalStateException If the user is not logged in.
     */
    public void buyOnline(int branchID,int productID,int amount) throws NoSuchElementException,IllegalStateException{
        if(loggedIn) {
            if(Administrator.getBranch(branchID) == null)
                throw new NoSuchElementException("This branch doesn't exist!");
            else if(Administrator.getBranch(branchID).getProducts() == null)
                throw new NoSuchElementException("This product doesn't exist!");
            else if(amount<=0)
                throw new IllegalArgumentException("You can't buy zero or less products!");

            if(Administrator.getBranch(branchID).getProduct(productID).getAmount() >= amount) {
                Administrator.getBranch(branchID).getProduct(productID).
                        setAmount(Administrator.getBranch(branchID).getProduct(productID).getAmount() - amount);
                System.out.println("Products will be shipped to you. You can pay only at door right now\n");
                if(getPreviousOrders().size()==0 && Administrator.getBranch(branchID).getCustomers().contains(this)==-1)
                    Administrator.getBranch(branchID).addNewCustomer(this);
                addToPreviousOrders(new Order(Administrator.getBranch(branchID).getProduct(productID),amount));
            }
            else
                System.out.println("There is not enough Product in this branch. Try again later.\n");
        }
        else throw new IllegalStateException("This user is not logged in!");
    }

    /**
     * Add previous order to customer data.
     * @param order Order of the customer.
     */
    public void addToPreviousOrders(Order order){
        previousOrders.insert(order);
    }

    /**
     * Getter for Previous Orders.
     * @return Previous orders.
     */
    public CompanyContainer<Order> getPreviousOrders(){
        return previousOrders;
    }

    /**
     * toString implementation for BrandCustomer.
     * @return String representation of BrandCustomer.
     */
    @Override
    public String toString() {
        return "Customer Info:\n" + super.toString() +  "\nPhone number: " + phoneNumber + "\nEmail: " + eMail + "\n";
    }
}
