import java.awt.*;  //for gui
import javax.swing.*;  
import java.awt.event.*;
import java.util.Random;
import java.util.Vector;  
import java.io.FileWriter;   // for FileWriter class
import java.io.IOException;  // for Error Handling
import java.io.File;  // for File class
import java.io.FileNotFoundException;  // for Error Handling
import java.util.Scanner; // for File Reading

public class HexGame extends JPanel implements ActionListener{
   private JFrame frame; //game frame
   private JFrame firstFrame; //board size input frame
   private JButton[][] cells;
   private JButton reset,save,load,undo;
   private int size; //board size
   private int turn; // 0 or 1, holds player turn
   private int gameMode;
   private Boolean[][] checkedBefore; //for wincheck
   private JButton[] lastMoves; //for undo feature
   private int lastMovesIndex; //holds the very last moves index in lastMoves array.
   private JCheckBox pvp; //choose game mod pvp
   JCheckBox pve; //choose game mod pve
   Color p1c=new Color(255,0,0); //red
   Color p2c=new Color(0,0,255); //blue
   Color emptyc=new Color(255,255,255); //white
   public HexGame(int x){
        size = x;
        turn=0;
        lastMovesIndex=0;
        int buttonPlaceX=0;
        int buttonPlaceY=0;
        final int increment=25;
        int shifter=increment/2;
        pvp=new JCheckBox("PVP");
        pve=new JCheckBox("PVE");
        reset = new JButton("RESET");
        save = new JButton("SAVE");
        load = new JButton("LOAD");
        undo = new JButton("UNDO");
        firstFrame=new JFrame();
        firstFrame.setLayout(new FlowLayout());
        JTextField textField = new JTextField( "Choose game mode.", 20 ); 
        textField.setEditable(false);
        firstFrame.add(textField);
        firstFrame.setSize(new Dimension(250,250));
        firstFrame.add(pvp);
        firstFrame.add(pve);
        CheckBoxHandler handler = new CheckBoxHandler();
        pvp.addItemListener(handler);
        pve.addItemListener(handler);
        firstFrame.setVisible(true);
        cells = new JButton[size][size];
        lastMoves = new JButton[size*size];
        checkedBefore= new Boolean[size][size];
        for(int i=0;i<size;i++){
            cells[i]= new JButton[size];
            checkedBefore[i]= new Boolean[size];
            lastMoves[i] = new JButton();
            for(int j=0;j<size;j++){
                cells[i][j] = new JButton();
                cells[i][j].setBackground(new Color(255,255,255));
                cells[i][j].setBounds(buttonPlaceX,buttonPlaceY,increment,increment);
                cells[i][j].addActionListener(this);
                checkedBefore[i][j]=false;
                buttonPlaceX+=increment;
            }
            buttonPlaceX=shifter;
            shifter+=increment/2;
            buttonPlaceY+=increment;
        }

        frame = new JFrame("Hex Game");
        frame.setSize(new Dimension(increment*(size+size),increment*(size+size))); //frame size depends on the board size.
        frame.setLayout(null);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        reset.setBounds(increment*(size+size)/4+75,increment*(size+size)/2,50,50);
        save.setBounds(increment*(size+size)/4,increment*(size+size)/2,50,50);
        load.setBounds(increment*(size+size)/4-75,increment*(size+size)/2,50,50);
        undo.setBounds(increment*(size+size)/4+150,increment*(size+size)/2,50,50);
        reset.setBorder(null);
        save.setBorder(null);
        load.setBorder(null);
        undo.setBorder(null);
        reset.addActionListener(this);
        save.addActionListener(this);
        load.addActionListener(this);
        undo.addActionListener(this);
        frame.add(reset);
        frame.add(save);
        frame.add(load);
        frame.add(undo);
        for(int i=0;i<size;i++)
            for(int j=0;j<size;j++)
            frame.add(cells[i][j]);
    }
    public void actionPerformed(ActionEvent e){
        if(e.getSource()==reset){
            for(int i=0;i<size;i++){
                for(int j=0;j<size;j++){
                    cells[i][j].setBackground(emptyc);
                }
            }
        }
        else if(e.getSource()==undo){
            try{
                lastMoves[--lastMovesIndex].setBackground(new Color(255,255,255));
                if(turn==1) turn=0;
                else if(turn==0) turn=1;
            }
            catch(Exception E){
                JOptionPane.showMessageDialog(frame,"This is the first move. You can't undo!");
            }
        }
        else if(e.getSource()==save){
            String input = JOptionPane.showInputDialog("Enter the name of the save file (example : mySave.txt):");
            try {
              FileWriter myWriter = new FileWriter(input);
              myWriter.write(size+"-"+gameMode+"-");
              for(int i=0;i<size;i++)
                for(int j=0;j<size;j++){
                    if(cells[i][j].getBackground().equals(emptyc))
                        myWriter.write(Integer.toString(0));
                    if(cells[i][j].getBackground().equals(p1c))
                        myWriter.write(Integer.toString(1));
                    if(cells[i][j].getBackground().equals(p2c))
                        myWriter.write(Integer.toString(2));

                }
              myWriter.close();
              JOptionPane.showMessageDialog(frame,"Game is succesfully saved. You can keep playing.");
            } catch (IOException ex) {
              JOptionPane.showMessageDialog(frame,"An error occured while saving the game.");
            }
        }
        else if(e.getSource()==load){
            int k=0;
            String gameSave = new String();
            String input = JOptionPane.showInputDialog("Enter the name of the load file (example: mySave.txt");
            try{
                File loadFile = new File(input);
                Scanner myReader = new Scanner(loadFile);
                if(myReader.hasNextLine())
                    gameSave = myReader.nextLine();
                else
                    JOptionPane.showMessageDialog(frame,"This file is empty!");
                
                for(int i=0;i<size;i++){ //delete the old board
                    for(int j=0;j<size;j++){
                            frame.remove(cells[i][j]);
                            frame.revalidate();
                            frame.repaint();
                    }
                }

                size=Integer.parseInt(gameSave.substring(0,gameSave.indexOf('-')));
                gameSave=gameSave.substring(gameSave.indexOf('-')+1);
                gameMode=Integer.parseInt(gameSave.substring(0,gameSave.indexOf('-')));
                gameSave=gameSave.substring(gameSave.indexOf('-')+1);


                //recreate everything according to the new size.
                int buttonPlaceX=0;
                int buttonPlaceY=0;
                final int increment=25;
                int shifter=increment/2;
                cells = new JButton[size][size];
                lastMoves = new JButton[size*size];
                checkedBefore= new Boolean[size][size];
                for(int i=0;i<size;i++){
                    cells[i]= new JButton[size];
                    checkedBefore[i]= new Boolean[size];
                    lastMoves[i] = new JButton();
                    for(int j=0;j<size;j++){
                        cells[i][j] = new JButton();
                        cells[i][j].setBackground(new Color(255,255,255));
                        cells[i][j].setBounds(buttonPlaceX,buttonPlaceY,increment,increment);
                        cells[i][j].addActionListener(this);
                        checkedBefore[i][j]=false;
                        buttonPlaceX+=increment;
                    }
                    buttonPlaceX=shifter;
                    shifter+=increment/2;
                    buttonPlaceY+=increment;
                }

                frame.setSize(new Dimension(increment*(size+size),increment*(size+size)));
                reset.setBounds(increment*(size+size)/4+75,increment*(size+size)/2,50,50);
                save.setBounds(increment*(size+size)/4,increment*(size+size)/2,50,50);
                load.setBounds(increment*(size+size)/4-75,increment*(size+size)/2,50,50);
                undo.setBounds(increment*(size+size)/4+150,increment*(size+size)/2,50,50);

                for(int i=0;i<size;i++){
                    for(int j=0;j<size;j++){
                        frame.add(cells[i][j]);
                        if(gameSave.charAt(k)=='0') // 0 is empty cell
                            cells[i][j].setBackground(new Color(255,255,255));
                        else if(gameSave.charAt(k)=='1') //player 1 cell
                            cells[i][j].setBackground(new Color(255,0,0));
                        else if(gameSave.charAt(k)=='2') //player 2 cell
                            cells[i][j].setBackground(new Color(0,0,255));
                        k++;
                    }
                }
                myReader.close();
            }
            catch (FileNotFoundException ex){
                  JOptionPane.showMessageDialog(frame,"This file doesn't exists.");
            }
        }
        else if(gameMode==0){
            for(int i=0;i<size;i++){
                for(int j=0;j<size;j++){
                    if(e.getSource()==cells[i][j] && turn==0 && cells[i][j].getBackground().equals(emptyc)){
                        cells[i][j].setBackground(new Color(255,0,0));
                        lastMoves[lastMovesIndex] = cells[i][j];
                        lastMovesIndex++;
                        turn=1;
                        if(winCheckP1(0,0)==true){
                            JFrame winFrame = new JFrame();
                            winFrame.setLayout(new FlowLayout());
                            JTextField winText = new JTextField("Player1 Won!");
                            winText.setSize(new Dimension(250,50));
                            winFrame.setSize(new Dimension(250,250));
                            winText.setEditable(false);
                            winFrame.add(winText);
                            winFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
                            winFrame.setVisible(true);
                        }
                    }
                    else if(e.getSource()==cells[i][j] && turn==1 && cells[i][j].getBackground().equals(emptyc)){
                        cells[i][j].setBackground(new Color(0,0,255));
                        lastMoves[lastMovesIndex] = cells[i][j];
                        lastMovesIndex++;
                        turn=0;
                        if(winCheckP2(0,0)==true){
                            JFrame winFrame = new JFrame();
                            winFrame.setLayout(new FlowLayout());
                            JTextField winText = new JTextField("Player2 Won!");
                            winText.setSize(new Dimension(250,50));
                            winFrame.setSize(new Dimension(250,250));
                            winText.setEditable(false);
                            winFrame.add(winText);
                            winFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
                            winFrame.setVisible(true);
                        }
                    }
                }
            }
        }
        else if(gameMode==1){
            Random random = new Random();
            for(int i=0;i<size;i++){
                for(int j=0;j<size;j++){
                    if(e.getSource()==cells[i][j] && cells[i][j].getBackground().equals(emptyc)){
                        cells[i][j].setBackground(new Color(255,0,0));
                        lastMoves[lastMovesIndex] = cells[i][j];
                        lastMovesIndex++;
                        if(winCheckP1(0,0)==true){
                            JFrame winFrame = new JFrame();
                            winFrame.setLayout(new FlowLayout());
                            JTextField winText = new JTextField("Player Won!");
                            winText.setSize(new Dimension(250,50));
                            winFrame.setSize(new Dimension(250,250));
                            winText.setEditable(false);
                            winFrame.add(winText);
                            winFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
                            winFrame.setVisible(true);
                        }
                    }
                }
            }
            int randomX;
            int randomY;
            do{
                randomX=random.nextInt(size);
                randomY=random.nextInt(size);
            }while(!cells[randomX][randomY].getBackground().equals(emptyc));
            cells[randomX][randomY].setBackground(new Color(0,0,255));
            lastMoves[lastMovesIndex] = cells[randomX][randomY];
            lastMovesIndex++;
            if(winCheckP2(0,0)==true){
                JFrame winFrame = new JFrame();
                winFrame.setLayout(new FlowLayout());
                JTextField winText = new JTextField("Computer Won!");
                winText.setSize(new Dimension(250,50));
                winFrame.setSize(new Dimension(250,250));
                winText.setEditable(false);
                winFrame.add(winText);
                winFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
                winFrame.setVisible(true);
            }
        }
    }
    private class CheckBoxHandler implements ItemListener{ //gameMode
        public void itemStateChanged(ItemEvent event){
            if(event.getSource()==pvp){
                gameMode=0;
                frame.setVisible(true);
                firstFrame.setVisible(false);
            }
            else if(event.getSource()==pve){
                gameMode=1;
                frame.setVisible(true);
                firstFrame.setVisible(false);
            }
        }
    }
    private boolean winCheckP1(int i,int j){
        boolean returnFlag=false;
        while(returnFlag==false){
            if(i==0&&j==0){
                while(i<size && !cells[i][0].getBackground().equals(p1c)){
                    i++;
                }
            }
            if((i<size) &&j==size-1 && cells[i][j].getBackground().equals(p1c)){
                checkedBefore[i][j]=true;
                for(int u=0;u<size;u++){
                    for(int p=0;p<size;p++){
                        if(cells[u][p].getBackground().equals(p1c) && checkedBefore[u][p]==true)
                        cells[u][p].setBackground(new Color(255,124,0));
                    }
                }
                return true;
            }
            else if((i<size) && cells[i][j].getBackground().equals(p1c)){
                if((i<size && j+1<size) && cells[i][j+1].getBackground().equals(p1c) && checkedBefore[i][j+1]==false){
                    checkedBefore[i][j]=true;
                    j++;
                    returnFlag= true;
                }
                else if((i-1>0 && j+1>0 && i-1<size && j+1<size) && cells[i-1][j+1].getBackground().equals(p1c) && checkedBefore[i-1][j+1]==false){
                    checkedBefore[i][j]=true;
                    i--;
                    j++;
                    returnFlag= true;
                }
                else if((i+1<size && j<size) && cells[i+1][j].getBackground().equals(p1c) && checkedBefore[i+1][j]==false){
                    checkedBefore[i][j]=true;
                    i++;
                    returnFlag= true;
                }
                else if((i-1>0 && j>0) && cells[i-1][j].getBackground().equals(p1c) && checkedBefore[i-1][j]==false){
                    checkedBefore[i][j]=true;
                    i--;
                    returnFlag= true;
                }
                else if((i+1>0 && j-1>0 && i+1<size&&j-1<size) && cells[i+1][j-1].getBackground().equals(p1c) && checkedBefore[i+1][j-1]==false){
                    checkedBefore[i][j]=true;
                    i++;
                    j--;
                    returnFlag= true;
                }
                else if((i>0 && j-1>0) && cells[i][j-1].getBackground().equals(p1c) && checkedBefore[i][j-1]==false){
                    checkedBefore[i][j]=true;
                    j--;
                    returnFlag= true;
                }
                else{
                    for(int p=0;p<size;p++){
                        for(int u=0;u<size;u++){
                            checkedBefore[p][u]=false;
                        }
                    }
                }
                if(returnFlag==true){ //if there is 'x' path ahead, proceed to next one.
                    return winCheckP1(i,j);
                }
            }
            if(i<size)
                i++;
            else break;
        }
        return false;
    }
    private boolean winCheckP2(int i,int j){
        boolean returnFlag=false;
        while(returnFlag==false){
            if(i==0&&j==0){
                while(j<size&&!cells[0][j].getBackground().equals(p2c)){
                    j++;
                }
            }
            if((j<size) && i==size-1 && cells[i][j].getBackground().equals(p2c)){
                checkedBefore[i][j]=true;
                for(int u=0;u<size;u++){
                    for(int p=0;p<size;p++){
                        if(cells[u][p].getBackground().equals(p2c) && checkedBefore[u][p]==true)
                        cells[u][p].setBackground(new Color(0,255,0));
                    }
                }
                return true;
            }
            else if((j<size) && cells[i][j].getBackground().equals(p2c)){
                if((i<size && j+1<size) && cells[i][j+1].getBackground().equals(p2c) && checkedBefore[i][j+1]==false){
                    checkedBefore[i][j]=true;
                    j++;
                    returnFlag= true;
                }
                else if((i-1>0 && j+1>0 && i-1<size && j+1<size) && cells[i-1][j+1].getBackground().equals(p2c) && checkedBefore[i-1][j+1]==false){
                    checkedBefore[i][j]=true;
                    i--;
                    j++;
                    returnFlag= true;
                }
                else if((i+1<size && j<size) && cells[i+1][j].getBackground().equals(p2c) && checkedBefore[i+1][j]==false){
                    checkedBefore[i][j]=true;
                    i++;
                    returnFlag= true;
                }
                else if((i-1>0 && j>0) && cells[i-1][j].getBackground().equals(p2c) && checkedBefore[i-1][j]==false){
                    checkedBefore[i][j]=true;
                    i--;
                    returnFlag= true;
                }
                else if((i+1>0 && j-1>0 && i+1<size&&j-1<size) && cells[i+1][j-1].getBackground().equals(p2c) && checkedBefore[i+1][j-1]==false){
                    checkedBefore[i][j]=true;
                    i++;
                    j--;
                    returnFlag= true;
                }
                else if((i>0 && j-1>0) && cells[i][j-1].getBackground().equals(p2c) && checkedBefore[i][j-1]==false){
                    checkedBefore[i][j]=true;
                    j--;
                    returnFlag= true;
                }
                else{
                    for(int p=0;p<size;p++){
                        for(int u=0;u<size;u++){
                            checkedBefore[p][u]=false;
                        }
                    }
                }
                if(returnFlag==true){ //if there is 'x' path ahead, proceed to next one.
                    return winCheckP2(i,j);
                }
            }
            if(j<size)
                j++;
            else
                break;
        }
        return false;
    }
}