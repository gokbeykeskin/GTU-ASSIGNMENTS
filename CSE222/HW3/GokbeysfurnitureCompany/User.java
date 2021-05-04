package GokbeysfurnitureCompany;

/**
 * Person class is extended by all the automation system users. It implements Comparable interface.
 * @author gokbey_gazi_keskin_19010402631
 * */
public abstract class User implements Comparable<User>{
    private String name;
    private String surname;
    private int id;
    private static final KWArrayList<Integer> ids = new KWArrayList<>();

    /**Constructor for Person class
     * @param name person's name.
     * @param surname person's surname.
     * @param id person's ID number.
     * @throws IllegalStateException if the given ID already exists.
     * @throws IllegalArgumentException if the ID doesn't have 5 digits,starts with 0 or is a negative number.
     *
     * */
    public User(String name, String surname, int id) throws IllegalArgumentException,IllegalStateException{
        setName(name);
        setSurname(surname);
        if(String.valueOf(id).length()!=5) throw new IllegalArgumentException("ID should have 5 digits.");
        if(id<0) throw new IllegalArgumentException("ID can't be negative number.");
        if(ids.contains(id))
            throw new IllegalStateException("ID already exists!");
        else {
            setId(id);
            ids.add(id);
        }
    }

    /**
     * Getter for Ids container (holds the previous Person IDs for preventing confliction.
     * @return ids Previosuly created persons IDs.
     */
    public static KWArrayList<Integer> getIds(){return ids;}

    /** Getter function for person's name
     * @return Person's name*/
    public String getName() {
        return name;
    }
    /** Setter function for person's name
     * @param name Person's name
     * */
    public void setName(String name) {
        this.name = name;
    }
    /** Getter function for person's surname
     * @return Person's ID number*/
    public String getSurname() {
        return surname;
    }
    /** Setter function for person's surname
     * @param surname Person's surname*/
    public void setSurname(String surname) {
        this.surname = surname;
    }
    /** Getter function for person's ID number
     * @return Person's ID number*/
    public int getId() {
        return id;
    }
    /** Setter function for person's ID number
     * @param id Person's ID number
     * @throws IllegalStateException if the given ID already exists.
     * */
    public void setId(int id) throws IllegalStateException {
        if(ids.contains(id))
            throw new IllegalStateException("This ID already exists.");
        else
            this.id = id;
    }

    /**
     * compareTo method implementation for Person class
     * @param other Other Person to compare with.
     * @return -1 if the other object is null or not equal to this object. 0 if they are the same.
     */
    @Override
    public int compareTo(User other)
    {
        if(other == null)
            return -1;

        if(getName().equals(other.getName()) && getSurname().equals(other.getName()) && getId() == other.getId())
            return 0;

        return -1;
    }

    /**
     * Checks if the two person are the same.
     * @param other Another Object to compare with this Person.
     * @return Returns true if two person have same name, surname and ID otherwise, returns false.
     */
    @Override
    public boolean equals(Object other)
    {
        if(other == null)
            return false;
        if(!(other instanceof User))
            return false;

        return getName().equals( ((User)other).getName()) && getSurname().equals(
                ( (User) other).getSurname()) && getId() == ( (User) other).getId();
    }

    /**
     * Create a string representation of the container.
     * @return a string representation of the container.
     */
    @Override
    public String toString()
    {
        return String.format("Name: %s\nSurname: %s\nID: %d",getName(),getSurname(),getId());
    }
}
