package ShoppingSystem;

import java.io.*;

/**
 * ShoppingSystem.Person interface
 */
interface Person {

    /**
     * Getter for ID
     * @return the ID of the user
     */
     String getID();

    /**
     * Getter for name
     * @return name of the user
     */
     String getName();

    /**
     * Proccess the given csv file and create products.txt
      * @throws IOException if one of the files couldn't open
     */
    static void processCSV() throws IOException {
        File file = new File("e-commerce-samples.csv");
        FileWriter fw=new FileWriter("products.txt");


        BufferedReader br = new BufferedReader(new FileReader(file));

        String line;
        String[] dataElements;
        br.readLine();
        while ((line = br.readLine()) != null) {
            dataElements = line.split(";");
            for(String i : dataElements) {
                i = i.replace("\"[\"\"","");
                i = i.replace("\"\"]\"","");
                fw.append(i).append("\n");
            }
            fw.append("\n");

        }
        br.close();
        fw.close();


    }


}
