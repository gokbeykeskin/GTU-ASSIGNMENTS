package GokbeysfurnitureCompany;

import java.util.ListIterator;
import java.util.NoSuchElementException;
/**
 * Administrator class for the Furniture Company. Extends abstract class User. Implements Admin interface.
 * All methods except the ones inherited from Person superclass because all admins can access all branches,employees etc.
 * @author gokbey_gazi_keskin_19010402631
 * */
public class Administrator extends User {
    /**
     * All admins have access to all branches via the static variable branches.
     * */
    static KWLinkedList<Branch> branches = new KWLinkedList<>();

    /**
     * Constructor for the Administrator class.
     * @param name Name of the admin.
     * @param surname Surname of the admin.
     * @param id ID of the admin.
     */
    public Administrator(String name,String surname,int id) {
        super(name,surname,id);
        branches = new KWLinkedList<>();
    }
    /**
     * Adds a new branch to company.
     * @param branchID ID of the Branch.
     * @throws IllegalStateException When the given ID is another branch's ID
     * */
    public static void addBranch(int branchID) throws IllegalStateException {
        for (Branch branch : branches) {
            if (branch.getBranchID() == branchID)
                throw new IllegalStateException("This branch ID is taken by another branch.");
        }

        branches.add(new Branch(branchID));
    }

    /**
     * Getter for Branch.
     * @param branchID ID of the brand which will be returned
     * @return Specified branch.
     */
    public static Branch getBranch(int branchID){
        for(Branch i : branches){
            if(i.getBranchID() == branchID)
                return i;
        }
        return null;
    }

    /**
     * Removes the specified branch.
     * @param branchID ID of the Branch.
     * @throws NoSuchElementException When the given branch doesn't exist.
     */
    public static void removeBranch(int branchID)throws NoSuchElementException {
        boolean removeFlag=false;
        ListIterator<Branch> iter = branches.listIterator();
        while (iter.hasNext()){
            if(iter.next().getBranchID() == branchID) {
                iter.remove();
                removeFlag=true;
                break;
            }
        }
        if(!removeFlag)
            throw(new NoSuchElementException("This branch doesn't exist."));
    }

    /**
     * Adds a new Employee to specified branch.
     * @param branchID ID of the Branch.
     * @param emp Employee which will be added to specified Branch.
     * @throws IllegalArgumentException When the given employee is already working in given branch.
     * @throws NoSuchElementException When the given branch doesn't exist.
     */
    public static void addEmployee(int branchID,BranchEmployee emp)throws IllegalArgumentException,NoSuchElementException {
        boolean branchFlag = false;
        for (Branch i : branches) {
            if (i.getBranchID() == branchID) {
                branchFlag = true;
                try {
                    i.addBranchEmployee(emp);
                    break;
                } catch (IllegalArgumentException e) {
                    System.out.println("This employee already works in this branch!");
                }
            }
        }
        if(!branchFlag)
            throw(new NoSuchElementException("This branch doesn't exist."));
    }

    /**
     * Removes the specified Employee from specified Branch.
     * @param branchID ID of the Branch.
     * @param id ID of the Employee which will be removed.
     * @throws NoSuchElementException When the given employee doesn't exist.
     * @throws NoSuchElementException When the given branch doesn't exist.
     */
    public static void removeEmployee(int branchID,int id) throws NoSuchElementException {
        boolean branchFlag = false;
        for (Branch branch : branches) {
            if (branch.getBranchID() == branchID) {
                branchFlag = true;
                    if(!branch.getBranchEmployees().remove(branch.getBranchEmployee(id)))
                        throw new NoSuchElementException("This employee doesn't exist.");
                    break;
            }
        }
        if(!branchFlag)
            throw(new NoSuchElementException("This branch doesn't exist."));
    }


    /**
     * Queries a branch for needs.
     * @param b Branch which will be queried.
     * @return Needed (low on amount) products. null if there is no need to any Product in the specified branch.
     */
    public static HybridList<Product> query(Branch b) {
        return b.getNeededProducts();
    }

    /**
     * Create a new Product on specified branch.
     * @param branchID ID of the Branch.
     * @param productId ID of the new Product.
     * @param name Name of the product.
     * @param color Color of the product.
     * @throws IllegalArgumentException when the given product already exists.
     * @throws NoSuchElementException when there is no branch with the given ID.
     */
    public static void createProduct(int branchID,int productId,String name,String color)
                                                    throws IllegalArgumentException,NoSuchElementException{

        boolean branchFlag=false;

        int i;
        for(i=0;i<branches.size();i++){
            if(branchID == branches.get(i).getBranchID()){
                branchFlag=true;
                for(int j=0;j<branches.get(i).getProducts().size();j++){

                    if(branches.get(i).getProducts().get(j).getId()==productId)

                        throw new IllegalArgumentException("This product already exists.");
                }
                branches.get(i).getProducts().add(new Product(name,productId,color));
            }
        }
        if(!branchFlag)
            throw new NoSuchElementException("There is no branch with this ID.");
    }

    /**
     * Removes a product from the specified branch.
     * @param branchID Branch's ID
     * @param productId Product's ID
     */
    public static void removeProduct(int branchID,int productId){
        boolean productFlag = false,branchProduct = false;
        for (Branch branch : branches) {
            if (branchID == branch.getBranchID()) {
                branchProduct = true;
                for (int j = 0; j < branch.getProducts().size(); j++) {
                    if (branch.getProducts().get(j).getId() == productId) {
                        branch.getProducts().remove(j);
                        productFlag = true;
                    }
                }
            }
        }
        if(!productFlag)
            throw new NoSuchElementException("There is no such product!");
        else if(!branchProduct)
            throw new NoSuchElementException("There is no such branch!");
    }

    /**
     * Add a product (usually used with query function to supply needed products).
     * @param branchID ID of the Branch.
     * @param productId ID of the Product.
     * @param amount Amount of the Product to be shipped to the specified Branch.
     * @throws NoSuchElementException When there is no branch or product with the given ID's.
     * @throws IllegalArgumentException If the given amount is less then 1.
     */
    public static void addProduct(int branchID,int productId,int amount) throws NoSuchElementException,IllegalArgumentException{
        if(amount<=0)
            throw new IllegalArgumentException("You can't add zero or less products!");
        else if(getBranch(branchID) == null || getBranch(branchID).getProduct(productId)==null)
            throw new NoSuchElementException("Product or branch doesn't exist!");
        else
            getBranch(branchID).getProduct(productId).addProduct(amount);
    }

    /**
     * toString implementation for Administration.
     * @return String representation of the Administration.
     */
    @Override
    public String toString() {
        return super.toString() + " \nPosition: Administrator\n";
    }

}
