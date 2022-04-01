package ShoppingSystem;

import java.io.IOException;
import java.util.Hashtable;
import java.util.TreeSet;

/**
 * Abstract user class for Shopping System.
 */
public abstract class User implements Person{
    protected static final Hashtable<String,User> loginInfo = new Hashtable<>();
    private final String ID;
    protected String pw;
    private final String name;

    private static final TreeSet<String> existingIDs = new TreeSet<>();

    /**
     * Constructor for user
     * @param ID ID of the user
     * @param name Name of the user
     * @param pw Password of the user
     */
    public User(String ID, String name, String pw) {
        if(ID.length()!=8)
            throw new IllegalArgumentException("ID has to be 8 digits.");
        if(pw.length()!=6)
            throw new IllegalArgumentException("Password has to be 6 digits.");
        if(existingIDs.contains(ID))
            throw new IllegalStateException("ID already exists.");

        existingIDs.add(ID);
        this.ID = ID;
        this.pw = pw;
        loginInfo.put(ID,this);
        this.name = name;
    }



    /**
     * ShoppingSystem.User has to validate before using the system.
     * @param id ID of the user
     * @param pw Password of the user
     * @return true if id and the password is true.
     */
    public abstract boolean login(String id,String pw) throws IOException;


    /**
     * Getter for ID
     * @return the ID of the user
     */
    public String getID() {
        return ID;
    }

    /**
     * Getter for name
     * @return name of the user
     */
    public String getName() {
        return name;
    }



    /**
     * toString implementation for user class.
     * @return String representation of the user.
     */
    @Override
    public String toString() {
        return ID + "\n" + pw + "\n";
    }


}
