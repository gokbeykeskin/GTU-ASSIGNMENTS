

import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.JTextArea;
 
public class HexTest {
 
     
    public static void main(String[] args) {
         

        int size,gmode;
        do{
            String input = JOptionPane.showInputDialog("ENTER THE BOARD SIZE (MINIMUM SIZE:6)");
            size = Integer.parseInt( input ); // convert input to int      
        }while(size<6);
        
        HexGame buttonFrame = new HexGame(size);  
    }
 
}