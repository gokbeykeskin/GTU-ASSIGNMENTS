package GokbeysfurnitureCompany;

import java.util.NoSuchElementException;
import java.util.Scanner;

/**
 * Driver class for cargo company.
 * @author gokbey_gazi_keskin_1901042631
 */
public class Driver {
    /**
     * Main method.
     */
    public static void main(String[] args){
        CompanyContainer<User> admins = new CompanyContainer<>();
        CompanyContainer<User> employees = new CompanyContainer<>();
        CompanyContainer<User> customers = new CompanyContainer<>();
        /*
        I ADDED SOME ADMINISTRATORS,EMPLOYEES, CUSTOMERS, BRANCHES AND PRODUCTS FOR CONVENIENCE,
        BUT YOU CAN ADD NEW ONES FROM THE MENU.

        I DIDN'T DO ANY ERROR CHECKING IN THIS SECTION BECAUSE I ADDED EVERYTHING BY HAND BY THE CORRECT PARAMETERS.
        BUT IN THE MENU, EVERYTHING IS ERROR CHECKED.
         */
        admins.insert(new Administrator("Gokbey Gazi","Keskin",10000));
        employees.insert(new BranchEmployee("Ahmet","Şahin",10001));
        employees.insert(new BranchEmployee("Mehmet","Nazlı",10002));
        Administrator.addBranch(1);
        Administrator.addBranch(2);
        Administrator.addEmployee(1, (BranchEmployee) employees.at(0)); //added Ahmet to branch 1
        Administrator.addEmployee(2, (BranchEmployee) employees.at(1)); //added Mehmet to branch 2
        Administrator.createProduct(1,1,
                "Office Chair","Black"); //Branch 1 sells black office chairs (Item code: 1) now.
        Administrator.addProduct(1,1,100); //Branch 1 has 100 black office chairs now.
        Administrator.createProduct(1,2,
                "Office Chair","Blue"); //Branch 1 sells blue office chairs (Item code: 2) now.
        Administrator.addProduct(1,2,150); //Branch 1 has 150 blue office chairs now.
        Administrator.createProduct(2,1,"Office Desk","White");
        Administrator.addProduct(2,1,5);
        /*Item 1 on branch 2 is low on amount so when employee inquires this product,
          he/she will get an option for informing admin about product need. If he/she informs admin
          then admin can see the needs of a branch with Query Branch option.
          Then admin should send new products with Add Stock to a product option.*/
        customers.insert(new BrandCustomer
                        ("Aslı","Demirci","05542895963",
                                    "aslidemirci111@gmail.com","12345"));
        //Login info of customer Aslı eMail = aslidemirci111@gmail.com,Password = 12345
        ((BranchEmployee) employees.at(0)).addCustomer((BrandCustomer) customers.at(0)); //added customer to branch 0
        ((BranchEmployee) employees.at(1)).addCustomer((BrandCustomer) customers.at(0)); //added customer to branch 1
        int option=-1;
        while(option!=7) {
            System.out.println("Welcome to the Furniture Company Automation System\n" +
                    "1) Admin Login\n2) Employee Login\n3) Customer Interface\n" +
                    "4) Add Admin\n5) Add Employee\n6) Create Account (customer)\n7) Exit\n");
            Scanner input = new Scanner(System.in);
            option = input.nextInt();
            input.nextLine(); //consume the \n after int input
            switch (option) {
                case 4:
                    System.out.println("What's new admin's name?\n");
                    String name = input.nextLine();
                    System.out.println("What's new admin's surname?\n");
                    String surname = input.nextLine();
                    System.out.println("What's new admin's 5 digit ID (cant be negative and has to be 5 digits.)?\n");
                    int id = input.nextInt();
                    try {
                        admins.insert(new Administrator(name, surname, id));
                        System.out.println("Admin successfully created.");
                    }
                    catch(IllegalArgumentException e){
                        System.out.println("Invalid ID");
                    }
                    catch(IllegalStateException e){
                        System.out.println("This id already exists.");
                    }
                    catch(Exception e){
                        System.out.println("ID has to be an integer. Try again.");
                    }

                    break;
                case 5:
                    System.out.println("What's new employee's name?\n");
                    String name2 = input.nextLine();
                    System.out.println("What's new employee's surname?\n");
                    String surname2 = input.nextLine();
                    System.out.println("What's new employee's 5 digit ID " +
                            "(cant be negative and has to be 5 digits.) ?\n");
                    int id2 = input.nextInt();
                    try {
                        employees.insert(new BranchEmployee(name2, surname2, id2));
                        System.out.println("Employee succesfully created.");
                    }
                    catch(IllegalArgumentException e){
                        System.out.println("Invalid ID");
                    }
                    catch(IllegalStateException e){
                        System.out.println("This id already exists.");
                    }
                    catch(Exception e){
                        System.out.println("ID has to be an integer. Try again.");
                    }
                    break;
                case 6:
                    System.out.println("What's your name?\n");
                    String name3 = input.nextLine();
                    System.out.println("What's your surname?\n");
                    String surname3 = input.nextLine();
                    System.out.println("What's your phone number?\n");
                    String pnum = input.nextLine();
                    System.out.println("What's your E-Mail?\n");
                    String mail = input.nextLine();
                    System.out.println("Create a password.\n");
                    String pw = input.nextLine();
                    try {
                        customers.insert(new BrandCustomer(name3, surname3, pnum, mail, pw));
                        System.out.println("Your account is succesfully created.\n");
                        break;
                    }
                    catch(IllegalArgumentException e){
                        System.out.println("This mail is already signed-up!");
                    }
                    break;
                case 1:
                    for(int i=0;i<admins.size();i++){
                        System.out.println( (i+1) + ") " +admins.at(i));
                    }
                    if(admins.size()>0) {
                        System.out.println("Choose an admin:");
                        int adminIndex = input.nextInt();
                        admin((Administrator) admins.at(adminIndex-1),employees);
                    }
                    else
                        System.out.println("There is no admin account. You can create one.");
                    break;
                case 2:
                    for(int i=0;i<employees.size();i++){
                        System.out.println( (i+1) + ") " +employees.at(i));
                    }
                    if(employees.size()>0) {
                        System.out.println("Choose an employee:");
                        int employeeIndex = input.nextInt();
                        employee((BranchEmployee) employees.at(employeeIndex - 1),customers);
                    }
                    else
                        System.out.println("There is no employee account. You can create one.");

                    break;
                case 3:
                    boolean logInFlag=false;
                    System.out.println("Enter your E-mail: ");
                    String email =input.nextLine();
                    for(int i=0;i<customers.size();i++){
                        if( ((BrandCustomer) customers.at(i)).geteMail().equals(email)){
                            customer((BrandCustomer) customers.at(i));
                            logInFlag=true;
                        }
                    }
                    if(!logInFlag)
                        System.out.println("You entered a wrong email.");
                    break;
                case 7:
                    break;
                default:
                    System.out.println("This is not a valid option.");


            }
        }
    }

    public static void admin(Administrator a, CompanyContainer<User> employees) {
        Scanner input = new Scanner(System.in);
        int opt;
        System.out.println("Hello " + a.getName() + " What would you like to do?\n");
        while (true) {
            System.out.println("1) Add branch\n2) Remove branch\n3) List branch codes\n4) Add Employee to a branch\n" +
                    "5) Remove an Employee from the branch \n" + "6) List employees\n" + "7) Query a branch for needs\n" +
                    "8) Create a new product in a branch\n" + "9) Add stock to a product\n"
                        + "10) Fire employee\n" +"11) List Products at a branch\n" +"12) Back to Main Menu.\n");
            opt = input.nextInt(); input.nextLine();
            if (opt == 1) {
                System.out.println("Input new branch code (create one): ");
                int brCode= input.nextInt();
                input.nextLine();
                try {
                    Administrator.addBranch(brCode);
                    System.out.println("Branch is successfully created.");
                } catch (IllegalStateException e) {
                    System.out.println("A branch with this code already exists. Try again.");
                }
            } else if (opt == 2) {
                System.out.println("Input branch code: ");
                int brCD = input.nextInt(); input.nextLine();
                try {
                    Administrator.removeBranch(brCD);
                    System.out.println("Branch is succesfully removed.");
                } catch (NoSuchElementException e) {
                    System.out.println("This branch doesn't exist.");
                }
            } else if (opt == 3) {
                if (Administrator.branches.size() > 0) {
                    for (int i = 0; i < Administrator.branches.size(); i++) {
                        System.out.println("Branch Code: " + Administrator.branches.at(i) + "\n");
                    }
                    System.out.println("Press anything to continue");
                    input.nextLine();
                } else
                    System.out.println("There is no branch.\n");
            } else if (opt == 4) {
                if (employees.size() > 0 && Administrator.branches.size() > 0) {
                    for (int i = 0; i < employees.size(); i++) {
                        System.out.println((i + 1) + ") " + employees.at(i));
                    }
                    System.out.println("Select an employee to add.");
                    int opt1 = input.nextInt();
                    input.nextLine();

                    for (int i = 0; i < Administrator.branches.size(); i++) {
                        System.out.println((i + 1) + ") " + Administrator.branches.at(i));
                    }
                    System.out.println("Select a branch to add the employee.");
                    int opt2 = input.nextInt();
                    input.nextLine();
                    try {
                        Administrator.addEmployee(Administrator.
                                branches.at(opt2 - 1).getBranchID(), (BranchEmployee) employees.at(opt1 - 1));
                        System.out.println("Employee succesfully added to branch.");
                    } catch (IllegalArgumentException e) {
                        System.out.println("This employee works in this branch already.");
                    } catch (NoSuchElementException e) {
                        System.out.println("This branch doesn't exist. Try again.");
                    } catch (ArrayIndexOutOfBoundsException e) {
                        System.out.println("Wrong input!");
                    }
                } else
                    System.out.println("There is no branch or employee.");
            } else if (opt == 5) {
                System.out.println("Enter the employee ID. (you can list employees from the previous menu.)");
                int id = input.nextInt();
                input.nextLine();
                System.out.println("Enter the ID of the branch (you can list  branches from the previous menu.");
                int id2 = input.nextInt();
                input.nextLine();
                try {
                    Administrator.removeEmployee(id2, id);
                    System.out.println("Employee is succesfully removed.");
                } catch (NoSuchElementException e) {
                    System.out.println("This Employee or the branch doesn't exist.");
                }
            } else if (opt == 6) {
                if (employees.size() > 0) {
                    for (int i = 0; i < employees.size(); i++)
                        System.out.println(employees.at(i));
                    System.out.println("Press anything to continue");
                    input.nextLine();
                } else
                    System.out.println("There is no employee.");
            }
            else if (opt == 7) {
                if (Administrator.branches.size() > 0) {
                    for (int i = 0; i < Administrator.branches.size(); i++) {
                        System.out.println((i + 1) + ") " + Administrator.branches.at(i) + "\n");
                    }
                    System.out.println("Choose a branch to query for needs");
                    int opt2 = input.nextInt();
                    input.nextLine();
                    if (Administrator.query(Administrator.branches.at(opt2 - 1)).size() > 0) {
                        System.out.println(Administrator.query(Administrator.branches.at(opt2 - 1)));
                        System.out.println("This branch needs the products above.");
                    }
                    else
                        System.out.println("-------There is no need in this branch.-------");
                } else
                    System.out.println("There is no branch.");
            }
            else if(opt == 8){
                System.out.println("Enter the branch ID");
                int brID = input.nextInt(); input.nextLine();
                System.out.println("Enter the new Product ID");
                int prID = input.nextInt(); input.nextLine();
                System.out.println("Enter the name of the new Product");
                String pr = input.nextLine();
                System.out.println("Enter the color of the new Product");
                String cl = input.nextLine();
                try {
                    Administrator.createProduct(brID,prID,pr,cl);
                    System.out.println("Product is succesfully created");
                }
                catch(IllegalArgumentException e){
                    System.out.println("This product already exists.");
                }
                catch(NoSuchElementException e){
                    System.out.println("There is no branch with this ID.");
                }
            }
            else if(opt==9){
                System.out.println("Enter the branch ID");
                int brID = input.nextInt(); input.nextLine();
                try {
                    if(Administrator.getBranch(brID)!=null){
                        for (int i = 0; i < Administrator.getBranch(brID).getProducts().size(); i++) {
                            System.out.println(Administrator.getBranch(brID).getProducts().at(i));
                        }
                    }
                    else
                        System.out.println("There is no branch with this ID");
                }
                catch(NullPointerException e){
                    System.out.println("There is no product in this branch.");
                }
                System.out.println("Enter the Product Code (model): ");
                int prID = input.nextInt();
                input.nextLine();
                System.out.println("Enter the amount");
                int amt = input.nextInt();
                input.nextLine();
                try {
                    Administrator.addProduct(brID, prID, amt);
                    System.out.println("Stocks are succesfully updated.");
                } catch (NoSuchElementException e) {
                    System.out.println("There is no branch with this ID or this product doesn't exist.\n");
                }
                catch(IllegalArgumentException e){
                    System.out.println("You can't add 1 or less products!");
                }
            }
            else if(opt==10){
                boolean fireFlag = false;
                System.out.println("Enter the ID of the employee");
                int empID = input.nextInt(); input.nextLine();
                for(int i=0;i<employees.size();i++){
                    if(employees.at(i).getId() == empID) {
                        employees.erase(employees.at(i));
                        System.out.println("Employee is succesfully deleted. ");
                        fireFlag =true;
                    }
                }
                if(!fireFlag)
                    System.out.println("There is no employee with this ID.");
            }
            else if(opt==11){
                boolean branchFlag=false;
                System.out.println("Enter the branch ID.");
                int branchID =  input.nextInt(); input.nextLine();
                try {
                        if(Administrator.getBranch(branchID).getProducts().size() == 0)
                            System.out.println("There is no product in this branch.");
                        System.out.println(Administrator.getBranch(branchID).getProducts());
                }
                catch(NullPointerException e){
                    System.out.println("This branch doesn't exist!");
                }
            }
            else if(opt==12)
                break;
            else
                System.out.println("Wrong option.\n");
        }
    }
    public static void employee(BranchEmployee e,CompanyContainer<User> c){
        Scanner input = new Scanner(System.in);
        int opt;
        System.out.println("Hello " + e.getName() + " What would you like to do?\n");
        while (true) {
            System.out.println("1) List products\n2) Inquire a product for needs\n" +
                    "3) Make Sale (face-to-face)\n" + "4) Access previous orders of a customer\n" +
                    "5) Create subscription(face-to-face)\n" + "6) List Customers\n" +"7) Back to Main Menu.\n");
            opt = input.nextInt(); input.nextLine();
            if(opt==1){
                try {
                    if(e.getEmployeesBranch().getProducts()!=null) {
                        try {
                            System.out.println(e.getEmployeesBranch().getProducts());
                        }catch (IllegalStateException ex){
                            System.out.println("There is no product in this branch!");
                        }
                    }
                }
                catch(IllegalArgumentException ex){
                    System.out.println("This employee doesn't have a branch yet!");
                }
            }
            else if(opt==2){
                System.out.println("Enter the product ID: ");
                int prID = input.nextInt(); input.nextLine();
                try{
                    if(e.inquireProduct(prID)) {
                        System.out.println("This product is low on amount. Do you want to inform the manager?\n" +
                                "1) Yes)\n2) No");
                        int opt2 = input.nextInt();
                        input.nextLine();
                        if (opt2 == 1) {
                            e.informAdmin(prID);
                            System.out.println("Admin is informed.");
                        }
                    }
                    else
                        System.out.println("This product has enough stock.");
                }
                catch(NullPointerException ex){
                    System.out.println("There is no product with given ID.");
                }
            }
            else if(opt==3){
                System.out.println("Enter the ID of to product to sell(you can check the product list from main menu" +
                                                                                                "(List Products): ");
                int prID= input.nextInt(); input.nextLine();
                System.out.println("Enter the ID of to customer: (You can find Customer IDs from the main menu" +
                        " (List customers) or create a subscription for this customer.");
                int cusNum = input.nextInt(); input.nextLine();
                System.out.println("How many of this product would you like to sell?");
                int amt= input.nextInt(); input.nextLine();
                try {
                    e.makeSale(prID, cusNum, amt);
                    System.out.println("Sale is successfully made.");
                }
                catch(NoSuchElementException ex){
                    System.out.println("There is no product with this ID or " +
                            "there is no customer with this ID. You can create subscription for " +
                            "customers in main menu. But only Administrators can create products.");
                }
                catch(IllegalArgumentException ex){
                    System.out.println("You can't sell zero or less products!");
                }
                catch(IllegalStateException ex){
                    System.out.println("There is not enough products to sell!");
                }
            }
            else if(opt==4){
                System.out.println("Enter the customer ID: ");
                int cstID = input.nextInt();
                if(e.getPreviousOrders(cstID)!=null) {
                    System.out.println(e.getPreviousOrders(cstID));
                    System.out.println("\nPrevious orders are listed above.");
                }
                else
                    System.out.println("There is no previous orders of this customer.");
            }
            else if(opt==5){
                System.out.println("Is the customer already signed-up to online store?\n" +
                        "1) Yes (Subscription for this branch)\n2) No( New Customer)");
                int opt2 = input.nextInt(); input.nextLine();
                if(opt2 == 1){
                    System.out.println("What is his/her ID?");
                    opt2 = input.nextInt(); input.nextLine();
                    boolean addFlag=false;
                    for(int i=0;i<c.size();i++) {
                        if (opt2 == c.at(i).getId()) {
                            try {
                                e.addCustomer((BrandCustomer) c.at(i));
                                addFlag = true;
                                System.out.println("Customer is succesfully subscribed to branch.");
                            }
                            catch(IllegalArgumentException ex){
                                System.out.println("This customer is already subscribed to this branch.");
                                addFlag=true;
                            }

                        }
                    }
                    if(!addFlag)
                        System.out.println("There is no customer signed-up to online store with this ID.");
                }
                else if(opt2==2) {
                    System.out.println("Enter the new customers name: ");
                    String nm = input.nextLine();
                    System.out.println("Enter the new customers surname: ");
                    String snm = input.nextLine();
                    System.out.println("Enter the new customers phone number: ");
                    String pnm = input.nextLine();
                    System.out.println("Enter the new customers e-mail: ");
                    String mail = input.nextLine();
                    System.out.println("Enter the new customers password: ");
                    String pw = input.nextLine();
                    try {
                        BrandCustomer newCus = new BrandCustomer(nm, snm, pnm, mail, pw);
                        c.insert(newCus);
                        e.createSubscription(newCus);

                        System.out.println("Subscription is successfully created.");
                    } catch (IllegalArgumentException ex) {
                        System.out.println("This mail is already signed up.");
                    } catch (IllegalStateException ex) {
                        System.out.println("ID can't be negative number.");
                    }
                }
                else
                    System.out.println("Wrong option!");
            }
            else if(opt==6){
                try {
                    if(e.getEmployeesBranch().getCustomers().size()>0) {
                        System.out.println(e.getEmployeesBranch().getCustomers());
                        System.out.println("\nCustomers of this branch are listed above.");
                    }
                    else
                        System.out.println("This branch doesn't have any customers yet.");
                }
                catch(IllegalArgumentException ex){
                    System.out.println("This employee doesn't have a branch yet.");
                }
            }
            else if(opt==7)
                break;
        }
    }
    public static void customer(BrandCustomer c){
        Scanner input = new Scanner(System.in);
        int opt;
        System.out.println("Hello " + c.getName() + " What would you like to do?\n");
        while (true) {
            System.out.println("1) Login\n2) Search for products\n"+ "3) List Branches\n" +"4) List Products\n" +
                    "5) Shop Online\n" + "6) View your previous orders\n" +"7) Back to Main Menu.\n");
            opt = input.nextInt(); input.nextLine();
            if(opt==1){
                System.out.println("Enter your mail adress: ");
                String mail = input.nextLine();
                System.out.println("Enter your password: ");
                String password = input.nextLine();
                c.login(mail,password);
            }
            else if(opt==2){
                System.out.println("1) Search by name\2) Search by ID");
                int opt2 = input.nextInt(); input.nextLine();
                switch (opt2){
                    case 1:
                        System.out.println("Enter the name of the product.");
                        String nm = input.nextLine();
                        try {
                            c.search(nm);
                        }
                        catch(IllegalStateException e){
                            System.out.println("You are not logged in!");
                        }
                        break;
                    case 2:
                        System.out.println("Enter the ID of the product.");
                        int id = input.nextInt();
                        try {
                            c.search(id);
                        }
                        catch(IllegalStateException e){
                            System.out.println("You are not logged in!");
                        }
                        break;
                    default:
                        System.out.println("Wrong input!");
                        break;
                }
            }
            else if(opt==3){
                if(Administrator.branches.size()==0)
                    System.out.println("There is no branch yet!");
                else
                    System.out.println(Administrator.branches);
            }
            else if(opt==4){
                System.out.println("Enter a branch ID to list it's products. You can list branch ID's from previous menu.");
                int brID = input.nextInt(); input.nextLine();
                try {
                    c.listProducts(brID);
                }
                catch(NoSuchElementException e){
                    System.out.println("There is no branch with this ID.");
                }
                catch(IllegalStateException e){
                    System.out.println("You are not logged in!");
                }
            }
            else if(opt == 5){
                System.out.println("Enter the branch ID (You can list branch ID's previous menu):");
                int brID = input.nextInt(); input.nextLine();
                System.out.println("Enter the Product ID(model) you would like to buy. " +
                        "(You can list Product ID's from previous menu.):");
                int prID = input.nextInt(); input.nextLine();
                System.out.println("Enter the amount of products you would like to buy:");
                int amt = input.nextInt(); input.nextLine();
                try {
                    c.buyOnline(brID, prID, amt);
                }
                catch(NoSuchElementException e){
                    System.out.println("This branch or product doesn't exist!");
                }
                catch(NullPointerException e){
                    System.out.println("This product doesn't exist.");
                }
                catch(IllegalStateException e){
                    System.out.println("You are not logged in!");
                }
                catch(IllegalArgumentException e){
                    System.out.println("You can't buy zero or less products!");
                }
            }
            else if(opt == 6){
                if(c.getPreviousOrders().size()==0){
                    System.out.println("There is no previous orders.");
                }
                else {
                    System.out.println(c.getPreviousOrders());
                    System.out.println("Press anything to continue.");
                    input.nextLine();
                }
            }
            else if(opt == 7)
                break;
            else
                System.out.println("Wrong input!");
        }
    }
}