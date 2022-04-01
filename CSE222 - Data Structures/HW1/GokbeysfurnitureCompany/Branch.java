package GokbeysfurnitureCompany;

/**
 *Branch class for the furniture company.
 * @author gokbey_gazi_keskin_19010402631
 */
public class Branch {
    private int BranchID;
    private final CompanyContainer<BranchEmployee> branchEmployees;
    private final CompanyContainer<Product> products;
    private final CompanyContainer<Product> neededProducts;
    private final CompanyContainer<BrandCustomer> customers; //customers who bought something from this branch

    /**
     * Constructor for Branch
     * @param branchID ID of the branch.
     */
    public Branch(int branchID) {
        branchEmployees = new CompanyContainer<>();
        products = new CompanyContainer<>();
        neededProducts = new CompanyContainer<>();
        customers = new CompanyContainer<>();
        BranchID = branchID;
    }

    /**
     * Getter for name of the branch.
     * @return name of the branch.
     */
    public int getBranchID() {
        return BranchID;
    }

    /**
     *  Setter for branch ID.
     * @param branchID Name of the Branch.
     * @throws IllegalStateException if the given ID already is taken by another branch.
     */
    public void setBranchID(int branchID) {
        for(int i=0;i<Administrator.branches.size();i++) {
            if (Administrator.branches.at(i).getBranchID() ==branchID)
                throw new IllegalArgumentException("This brand ID already exists.");
        }
        BranchID = branchID;
    }

    /**
     * Adds a new customer to the branch.
     * @param newCustomer Customer which will be added to the branch.
     * @throws IllegalStateException if the customer visited the branch or the branch page at the company's website.
     */
    public void addNewCustomer(BrandCustomer newCustomer){
        if(customers.contains(newCustomer)==-1)
            customers.insert(newCustomer);
        else throw new IllegalArgumentException("This customer already has an account in this Branch.");
    }

    /**
     * Getter for customers.
     * @return Customers of this branch.
     */
    public CompanyContainer<BrandCustomer> getCustomers(){
        return customers;
    }

    /**
     * Getter for Employees of the branch.
     * @return Employees of this branch.
     *
     */
    public CompanyContainer<BranchEmployee> getBranchEmployees() {
        return branchEmployees;
    }

    /**
     * Adds the given Employee to this branch.
     * @throws IllegalArgumentException if the given employee already works in this branch.
     * @param branchEmployee Employee which will be added to branch.
     */
    public void addBranchEmployee(BranchEmployee branchEmployee) throws IllegalArgumentException{
        if(branchEmployees.contains(branchEmployee)>=0)
            throw new IllegalArgumentException("This employee already works in this branch!");
        branchEmployees.insert(branchEmployee);
        branchEmployees.at(branchEmployees.size()-1).setEmployeesBranch(this);
    }



    /**
     * Add a product to the needed products to inform Administrator about the need.
     * @param productID ID for the needed product.
     */
    public void addNeededProduct(int productID){
        for(int i=0;i<products.size();i++){
            if(productID==products.at(i).getId()){
                 neededProducts.insert(products.at(i));
            }
        }
    }

    /**
     * Getter for products.
     * @return products of the branch.
     */
    public CompanyContainer<Product> getProducts(){
        try{
            return products;
        }
        catch(NullPointerException e){
            System.out.println("There is no product in this branch.\n");
        }
        return null;
    }

    /**
     * Getter for needed products(To inform administrator.)
     * @return needed products.
     */
    public CompanyContainer<Product> getNeededProducts() {
        return neededProducts;
    }

    /**
     * Getter for an employee of the branch by the name.
     * @param id ID of the employee.
     * @return Employee if the employee works in this branch. null if he/she doesn't.
     */
    public BranchEmployee getBranchEmployee(int id){
        for(int i=0;i<branchEmployees.size();i++){
            if(id==branchEmployees.at(i).getId())
                return branchEmployees.at(i);
        }
        return null;
    }

    /**
     * Getter for a customer of the branch by the name.
     * @param id ID of the customer.
     * @return Customer if the customer has subscription in the branch. null if he/she doesn't.
     */
    public BrandCustomer getCustomer(int id){
        for(int i=0;i<customers.size();i++){
            if(id == customers.at(i).getId())
                return customers.at(i);
        }
        return null;
    }

    /**
     * Getter for a customer of the branch by the name.
     * @param productID ID of the product.
     * @return Product if the product exists in the branch. null if it doesn't.
     */
    public Product getProduct(int productID) {
        for(int i=0;i<products.size();i++){
            if(productID==products.at(i).getId()){
                return products.at(i);
            }
        }
        return null;
    }


    /**
     * toString implementation for Branch.
     * @return String representation of the Branch.
     */
    @Override
    public String toString(){
        return "Branch Code: " + BranchID + "\n";
    }
}
