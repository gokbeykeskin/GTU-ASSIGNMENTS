package GokbeysfurnitureCompany;

/**
 *Branch class for the furniture company.
 * @author gokbey_gazi_keskin_19010402631
 */
public class Branch {
    private int BranchID;
    private final KWArrayList<BranchEmployee> branchEmployees;
    private final HybridList<Product> products;
    private final HybridList<Product> neededProducts;
    private final KWArrayList<BrandCustomer> customers; //customers of the branch

    /**
     * Constructor for Branch
     * @param branchID ID of the branch.
     */
    public Branch(int branchID) {
        branchEmployees = new KWArrayList<>();
        products = new HybridList<>();
        neededProducts = new HybridList<>();
        customers = new KWArrayList<>();
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
            if (Administrator.branches.get(i).getBranchID() ==branchID)
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
        if(!customers.contains(newCustomer))
            customers.add(newCustomer);
        else throw new IllegalArgumentException("This customer already has an account in this Branch.");
    }

    /**
     * Getter for customers.
     * @return Customers of this branch.
     */
    public KWArrayList<BrandCustomer> getCustomers(){
        return customers;
    }

    /**
     * Getter for Employees of the branch.
     * @return Employees of this branch.
     *
     */
    public KWArrayList<BranchEmployee> getBranchEmployees() {
        return branchEmployees;
    }

    /**
     * Adds the given Employee to this branch.
     * @throws IllegalArgumentException if the given employee already works in this branch.
     * @param branchEmployee Employee which will be added to branch.
     */
    public void addBranchEmployee(BranchEmployee branchEmployee) throws IllegalArgumentException{
        if(branchEmployees.contains(branchEmployee))
            throw new IllegalArgumentException("This employee already works in this branch!");
        branchEmployees.add(branchEmployee);
        branchEmployees.get(branchEmployees.size()-1).setEmployeesBranch(this);
    }



    /**
     * Add a product to the needed products to inform Administrator about the need.
     * @param productID ID for the needed product.
     */
    public void addNeededProduct(int productID){
        for(int i=0;i<products.size();i++){
            if(productID==products.get(i).getId()){
                 neededProducts.add(products.get(i));
            }
        }
    }

    /**
     * Getter for products.
     * @return products of the branch.
     */
    public HybridList<Product> getProducts(){
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
    public HybridList<Product> getNeededProducts() {
        return neededProducts;
    }

    /**
     * Getter for an employee of the branch by the name.
     * @param id ID of the employee.
     * @return Employee if the employee works in this branch. null if he/she doesn't.
     */
    public BranchEmployee getBranchEmployee(int id){
        for (BranchEmployee branchEmployee : branchEmployees) {
            if (id == branchEmployee.getId())
                return branchEmployee;
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
            if(id == customers.get(i).getId())
                return customers.get(i);
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
            if(productID==products.get(i).getId()){
                return products.get(i);
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
