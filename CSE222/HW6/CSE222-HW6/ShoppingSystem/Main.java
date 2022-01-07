package ShoppingSystem;

import java.io.*;
import java.util.*;

/**
 * Driver class for Shopping system
 */
public class Main {

    private static final List<User> traders = new ArrayList<>();
    private static final List<User> customers = new ArrayList<>();

    public static void main(String[] args) throws IOException {

        try {
            Person.processCSV();
        } catch (IOException e) {
            e.printStackTrace();
        }
        System.out.println("Which one would you like to run?\n1)Driver\n2)ShoppingSystem.User Interface (menu)");
        Scanner input = new Scanner(System.in);
        int opt = input.nextInt();
        if(opt==1)
            driver();
        else if(opt==2)
            mainMenu();
        else
            System.out.println("Wrong input.");

    }



    private static void driver() throws  IOException{
        System.out.println("-TEST CASES: Creating a trader-");
        System.out.println("Creating a trader:");
        Trader tr = new Trader("gokbey12","Gokbey's E-Market","123456");
        System.out.println("A trader is succesfully been created");

        System.out.println("Trying to create a ShoppingSystem.Trader with existing ID:");
        try {
            Trader tr2 = new Trader("gokbey12", "Gokbey's E-Market", "123456");
        }catch(IllegalStateException e){
            System.out.println("Caught Illegal State Exception.");
        }
        System.out.println("Trying to create a ShoppingSystem.Trader with non-proper ID:");
        try {
            Trader tr3 = new Trader("gokbey", "Gokbey's E-Market", "123456");
        }catch(IllegalArgumentException e){
            System.out.println("Caught Illegal Argument Exception.");
        }
        System.out.println("Trying to create a ShoppingSystem.Trader with non-proper password:");
        try {
            Trader tr4 = new Trader("gokbey12", "Gokbey's E-Market", "12345    ");
        }catch(IllegalArgumentException e){
            System.out.println("Caught Illegal Argument Exception.");
        }
        System.out.println("--------------------------------------------------------------");

        System.out.println("-TEST CASES: Creating a customer-");
        System.out.println("Creating a customer:");
        Customer cus = new Customer("gazi1234","Gokbey's E-Market","123456");
        System.out.println("A customer is successfully been created");

        System.out.println("Trying to create a ShoppingSystem.Customer with existing ID:");
        try {
            Customer cus2 = new Customer("gazi1234", "Gokbey's E-Market", "123456");
        }catch(IllegalStateException e){
            System.out.println("Caught Illegal State Exception.");
        }
        System.out.println("Trying to create a ShoppingSystem.Customer with non-proper ID:");
        try {
            Customer cus3 = new Customer("gazi", "Gokbey's E-Market", "123456");
        }catch(IllegalArgumentException e){
            System.out.println("Caught Illegal Argument Exception.");
        }

        System.out.println("Trying to create a ShoppingSystem.Customer with non-proper password:");
        try {
            Customer cus4 = new Customer("gazi4321", "Gokbey's E-Market", "12345");
        }catch(IllegalArgumentException e){
            System.out.println("Caught Illegal Argument Exception.");
        }
        System.out.println("--------------------------------------------------------------");
        System.out.println("-TEST CASE: ShoppingSystem.Trader login-");
        System.out.println("Login with wrong password:");
        System.out.println(tr.login("gokbey12","654321"));
        System.out.println("Login with wrong id:");
        System.out.println(tr.login("gokbey","123456"));
        System.out.println("Successful login:");
        System.out.println(tr.login("gokbey12","123456"));

        Trader alisha = new Trader("alisha12","Alisha","123456");
        alisha.login("alisha12","123456");
        Trader FabHomeDecor = new Trader("fabhome1","FabHomeDecor","654321");
        FabHomeDecor.login("fabhome1","654321");

        System.out.println("--------------------------------------------------------------");

        System.out.println("-TEST CASE: ShoppingSystem.Trader adding a product-");
        List<String> cat = new LinkedList<>();
        cat.add("Clothing");
        cat.add("Jean");
        tr.addProduct("Blue Jeans",cat,100,50,"The jeans you are looking for!");
        System.out.println("--------------------------------------------------------------");

        System.out.println("-TEST CASE: ShoppingSystem.Trader removing a product-");
        System.out.println("Trying to remove someone else's product:");
        tr.removeProduct("SRTEH2FF9KEDEFGF");
        System.out.println("Trying to remove non-existing product:");
        tr.removeProduct("lelele");
        System.out.println("Removing own product:");
        System.out.println(alisha.removeProduct("SRTEH2FF9KEDEFGF"));

        System.out.println("--------------------------------------------------------------");
        System.out.println("Trying to edit someone elses product:");
        tr.editProduct("SBEEH3QGU7MFYJFY");
        System.out.println("Trying to edit non-existing product:");
        tr.editProduct("lelele");
        System.out.println("Editing own product:");
        FabHomeDecor.editProduct("SBEEH3QGU7MFYJFY");
        System.out.println("!!You can check products.txt. New product (blue jeans) and edited product " +
                "(FabHomeDecor Fabric Double Sofa Bed) are added at the end.\n" +
                "ShoppingSystem.Product at the beginning (Alisha Solid Women's Cycling Shorts) is removed.");
        System.out.println("--------------------------------------------------------------");
        System.out.println("-TEST CASE:ShoppingSystem.Trader sees the existing products and accepts/decline them:");
        tr.seeWaitingOrders();
        System.out.println("You can check the orders.txt all the order status are changed from WAITING to ACCEPTED OR DECLINED");
        System.out.println("--------------------------------------------------------------");
        System.out.println("-TEST CASES: ShoppingSystem.Customer searching a product");
        System.out.println("Search with text \"Ballerina\" (you can sort-filter-buy from this listing");
        cus.searchProduct("Ballerina");
        System.out.println("Search with text \"Sandals\" (you can sort-filter-buy from this listing");
        cus.searchProduct("Sandals");


        System.out.println("Search with text Gaming Computer (does not exist.)");
        cus.searchProduct("Gaming Computer");
        System.out.println("--------------------------------------------------------------");
        System.out.println("-TEST CASE:Listing Traders-");
        cus.listTraders();
        System.out.println("List products of a trader (Alisha)");
        cus.listProductsOfTrader("Alisha");

    }

    private static void mainMenu() throws IOException {
        int opt;
        Scanner input = new Scanner(System.in);

        do {
            System.out.println("ShoppingSystem.Main-Menu\n1)Login as ShoppingSystem.Trader\n2)Login as ShoppingSystem.Customer\n3)Create account as ShoppingSystem.Trader" +
                    "\n4)Create account as ShoppingSystem.Customer\n5)Exit");
            opt = input.nextInt();
            switch (opt) {
                case 1:
                    traderMenu();
                    break;
                case 2:
                    customerMenu();
                    break;
                case 3:
                    createTrader();
                    break;
                case 4:
                    createCustomer();
                    break;
                case 5:
                    System.out.println("Goodbye!");
                default:
                    System.out.println("Wrong input.");
            }
        }while(opt!=5);
    }

    private static void createTrader(){
        Scanner input = new Scanner(System.in);

        try {
            System.out.println("Create a 8-digit ID:");
            String id,name,pw;
            id = input.nextLine();
            System.out.println("Your company name:");
            name = input.nextLine();
            System.out.println("Create a 6-digit password:");
            pw = input.nextLine();
            traders.add(new Trader(id,name,pw));
            System.out.println("Your trader account has succesfully been created.");
        }catch(IllegalStateException e){
            System.out.println("This ID already exists.");
        }
        catch(IllegalArgumentException e) {
            System.out.println("ID should have 8 digits.");
        } catch (IOException e) {
            e.printStackTrace();
        }


    }

    private static void createCustomer(){
        Scanner input = new Scanner(System.in);

        try {
            System.out.println("Create a 8-digit ID:");
            String id,name,pw;
            id = input.nextLine();
            System.out.println("Your name:");
            name = input.nextLine();
            System.out.println("Create a 6-digit password:");
            pw = input.nextLine();
            customers.add(new Customer(id,name,pw));
            System.out.println("Your customer account has succesfully been created.");
        }catch(IllegalStateException e){
            System.out.println("This ID already exists.");
        }
        catch(IllegalArgumentException e) {
            System.out.println("ID should have 8 digits.");
        } catch (IOException e) {
            e.printStackTrace();
            System.out.println("File couldn't open.");
        }
    }

    private static void customerMenu() throws IOException {
        Customer currentCustomer = null;
        Scanner input = new Scanner(System.in);
        boolean loggedin = false;
        System.out.println("Enter your ID:");
        String id = input.nextLine();
        System.out.println("Enter your password:");
        String pw = input.nextLine();

        for (User i : customers) {
            if (i.login(id, pw)) {
                System.out.println("Welcome," + i.getName() + "!");
                currentCustomer = (Customer) i;
                loggedin = true;
                break;
            }
        }
        if (!loggedin)
            System.out.println("Wrong ID or Password.");
        else {
            int opt = 0;
            while (opt != 3) {
                System.out.println("What would you like to do?\n1)List Traders\n2)Search Products\n3)Exit");
                opt = input.nextInt();
                input.nextLine(); //consume \n
                switch (opt) {
                    case 1:
                        currentCustomer.listTraders();
                        System.out.println("Which trader's products would you like to see? (Type name of trader)");
                        currentCustomer.listProductsOfTrader(input.nextLine());
                        break;
                    case 2:
                        System.out.println("Search text: ");
                        String  searchText = input.nextLine();
                        currentCustomer.searchProduct(searchText);
                        break;
                    case 3:
                        System.out.println("Redirecting to main menu.");
                        break;
                    default:
                        break;
                }
            }
        }
    }


    private static void traderMenu() throws IOException {
        Trader currentTrader = null;
        Scanner input = new Scanner(System.in);
        boolean loggedin = false;
        System.out.println("Enter your ID:");
        String id = input.nextLine();
        System.out.println("Enter your password:");
        String pw = input.nextLine();
        for(User i : traders){
            if(i.login(id,pw)){
                System.out.println("Welcome," + i.getName() + "!");
                currentTrader = (Trader)i;
                loggedin=true;
                break;
            }
        }
        if(!loggedin)
            System.out.println("Wrong ID or Password.");
        else {

            int opt = 0;
            while(opt!=5) {
                System.out.println("What would you like to do?\n1)Add a product\n2)Remove a ShoppingSystem.Product\n3)Edit a product" +
                        "\n4) See waiting orders(accept-decline them\n5)Quit");
                opt =input.nextInt(); input.nextLine(); //consume \n
                switch (opt) {
                    case 1:
                        System.out.println("Name of the product:");
                        String name = input.nextLine();
                        System.out.println("Categories (input one by one) (high-order to low-order)(type 0 to stop):");
                        List<String> category = new LinkedList<>();
                        String cat;
                        do {
                            cat = input.nextLine();
                            if(!cat.equals("0"))
                                category.add(cat);
                        } while (!cat.equals("0"));
                        System.out.println("Price:");
                        Integer price = input.nextInt();
                        input.nextLine();
                        System.out.println("Discount:");
                        Integer discount = input.nextInt(); input.nextLine();
                        System.out.println("Description:");
                        String description = input.nextLine();
                        currentTrader.addProduct(name, category, price, discount, description);
                        System.out.println("ShoppingSystem.Product is succesfully added.");
                        break;
                    case 2:
                        System.out.println("Type the ID of the product you want to remove.");
                        id = input.nextLine();
                        currentTrader.removeProduct(id);
                        System.out.println("This product is succesfully removed.");
                        break;
                    case 3:
                        System.out.println("Type the ID of the product you want to edit.");
                        id = input.nextLine();
                        currentTrader.editProduct(id);
                        break;
                    case 4:
                        currentTrader.seeWaitingOrders();
                        break;
                    case 5:
                        System.out.println("Redirecting to main menu.");
                    default:
                        System.out.println("Wrong input.");
                        break;
                }
            }
        }
    }
}
