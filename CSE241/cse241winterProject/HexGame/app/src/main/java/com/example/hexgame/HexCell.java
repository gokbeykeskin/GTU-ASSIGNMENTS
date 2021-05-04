package com.example.hexgame;

import android.widget.Button;


public class HexCell {
    private int state;
    public Button cell;
    private boolean checkedBefore;

    public HexCell(){
        setCellState(R.drawable.hexagon);
        setChecked(false);
    }
    public void setCellState(int x){ state = x;}
    public int getCellState(){return state;}
    public void setChecked(boolean x){checkedBefore = x;}
    public boolean getChecked(){return checkedBefore;}

}
