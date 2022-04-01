package com.example.hexgame;

import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;


public class MainActivity extends AppCompatActivity {
    int turn;
    HexCell[][] hexCells;

    TextView WinText;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        turn = 0;
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Intent intent = getIntent();
        int gameMode = intent.getIntExtra(Menu2.EXTRA_INFO_GM,0);
        hexCells = new HexCell[10][10];
        WinText = findViewById(R.id.pwon);

        WinText.setVisibility(View.INVISIBLE);
        for(int i=0; i<10; i++) {
            hexCells[i] = new HexCell[10];
            for(int j=0; j<10; j++) {
                String buttonID = "button" + i + j;
                int resID = getResources().getIdentifier(buttonID, "id", getPackageName());
                hexCells[i][j] = new HexCell();
                hexCells[i][j].cell= findViewById(resID);
                int finalI = i;
                int finalJ = j;
                hexCells[i][j].cell.setOnClickListener(new View.OnClickListener(){
                    @Override
                    public void onClick(View v) {
                        if (gameMode == 0){
                            if (turn == 0 && hexCells[finalI][finalJ].getCellState() == R.drawable.hexagon) {
                                hexCells[finalI][finalJ].cell.setBackgroundResource(R.drawable.hexagonred);
                                hexCells[finalI][finalJ].setCellState(R.drawable.hexagonred);
                                turn = 1;
                                int k=0;
                                while(k<10) {
                                    if (winCheckP1(k, 0)) {
                                        WinText.setText("Player 1 has won!");
                                        WinText.setVisibility(View.VISIBLE);
                                    }
                                    k++;
                                }
                            } else if (turn == 1 && hexCells[finalI][finalJ].getCellState() == R.drawable.hexagon) {
                                hexCells[finalI][finalJ].cell.setBackgroundResource(R.drawable.hexagonblue);
                                hexCells[finalI][finalJ].setCellState(R.drawable.hexagonblue);
                                turn = 0;
                                int k=0;
                                while(k<10) {
                                    if (winCheckP2(0, k)) {
                                        WinText.setText("Player 2 has won!");
                                        WinText.setVisibility(View.VISIBLE);
                                        k=10;
                                    }
                                    k++;
                                }
                            }
                        }
                        else if(gameMode==1){
                            //USER PLAYS
                            hexCells[finalI][finalJ].cell.setBackgroundResource(R.drawable.hexagonred);
                            if(winCheckP1(0, 0)) {
                                WinText.setText("Player has won!");
                                WinText.setVisibility(View.VISIBLE);
                            }
                            //BOT PLAYS
                           /* Integer[] botsMove = BestMove(hexCells,1,1);
                            hexCells[botsMove[0]][botsMove[1]].cell.setBackgroundResource(R.drawable.hexagonblue);
                            Log.i("bot plays:"," i: " + botsMove[0] + "j: " + botsMove[1]);
                            //wincheck();*/
                        }
                    }
                });
            }
        }
    }


    private boolean winCheckP1(int i,int j){
        boolean returnFlag=false;
        if(i==0&&j==0){
            while(i<10 && hexCells[i][0].getCellState()!= R.drawable.hexagonred){
                i++;
            }
        }
        if((i<10) &&j==10-1 && hexCells[i][j].getCellState()== R.drawable.hexagonred){
            hexCells[i][j].setChecked(true);
            for(int u=0;u<10;u++){
                for(int p=0;p<10;p++){
                    if(hexCells[u][p].getCellState()== R.drawable.hexagonred && hexCells[u][p].getChecked())
                       hexCells[u][p].cell.setBackgroundResource(R.drawable.hexagongreen);
                }
            }
            return true;
        }
        else if((i<10) && hexCells[i][j].getCellState()== R.drawable.hexagonred){
            if((i+1<10 && j<10) &&  hexCells[i+1][j].getCellState()== R.drawable.hexagonred && !hexCells[i + 1][j].getChecked()){
                hexCells[i][j].setChecked(true);
                i++;
                returnFlag= true;
            }
            else if((i+1<10 && j+1<10) &&  hexCells[i+1][j+1].getCellState()== R.drawable.hexagonred && !hexCells[i + 1][j + 1].getChecked()){
                hexCells[i][j].setChecked(true);
                i++;
                j++;
                returnFlag= true;
            }
            else if((i>0 && j-1>0) && hexCells[i][j-1].getCellState()== R.drawable.hexagonred && !hexCells[i][j - 1].getChecked()){
                hexCells[i][j].setChecked(true);
                j--;
                returnFlag= true;
            }
            else if((j+1<10) && hexCells[i][j+1].getCellState()== R.drawable.hexagonred && !hexCells[i][j + 1].getChecked()){
                hexCells[i][j].setChecked(true);
                j++;
                returnFlag= true;
            }
            else if((i-1>0 && j-1>0) && hexCells[i-1][j-1].getCellState()== R.drawable.hexagonred && !hexCells[i - 1][j - 1].getChecked()){
                hexCells[i][j].setChecked(true);
                i--;
                j--;
                returnFlag= true;
            }
            else if((i-1>0 && j>0) && hexCells[i-1][j].getCellState()== R.drawable.hexagonred && !hexCells[i - 1][j].getChecked()){
                hexCells[i][j].setChecked(true);
                i--;
                returnFlag= true;
            }
            else{
                for(int p=0;p<10;p++){
                    for(int u=0;u<10;u++){
                        hexCells[p][u].setChecked(false);
                    }
                }
            }
            if(returnFlag){ //if there is 'x' path ahead, proceed to next one.
                return winCheckP1(i,j);
            }
        }
        return false;
    }
    private boolean winCheckP2(int i,int j){
        boolean returnFlag=false;
        if(i==0&&j==0){
            while(j<10 && hexCells[0][j].getCellState()!= R.drawable.hexagonblue){
                j++;
            }
        }

        if((j<10) && i==10-1 && hexCells[i][j].getCellState()== R.drawable.hexagonblue){
            hexCells[i][j].setChecked(true);
            for(int u=0;u<10;u++){
                for(int p=0;p<10;p++){
                    if(hexCells[u][p].getCellState()== R.drawable.hexagonblue && hexCells[u][p].getChecked())
                        hexCells[u][p].cell.setBackgroundResource(R.drawable.hexagonpurple);
                }
            }
            return true;
        }
        else if((j<10) && hexCells[i][j].getCellState()== R.drawable.hexagonblue) {
            if (i + 1 < 10 && hexCells[i + 1][j].getCellState() == R.drawable.hexagonblue && !hexCells[i + 1][j].getChecked()) {
                hexCells[i][j].setChecked(true);
                i++;
                returnFlag = true;
                Log.i("Direction", "down" + hexCells[i][j].getCellState() + "  " + R.drawable.hexagonblue +"i:" + i + "j:" + j + "   " + hexCells[i-1][j].getChecked());
            }
            else if ((i + 1 < 10 && j + 1 < 10) && hexCells[i + 1][j + 1].getCellState() == R.drawable.hexagonblue && !hexCells[i + 1][j + 1].getChecked()) {
                hexCells[i][j].setChecked(true);
                i++;
                j++;
                returnFlag = true;
                Log.i("Direction", "rightdown");
            }
            else if (j + 1 < 10 && hexCells[i][j + 1].getCellState() == R.drawable.hexagonblue && !hexCells[i][j + 1].getChecked()) {
                hexCells[i][j].setChecked(true);
                j++;
                returnFlag = true;
                Log.i("Direction", "right");
            }
            else if (j - 1 > 0 && hexCells[i][j - 1].getCellState() == R.drawable.hexagonblue && !hexCells[i][j - 1].getChecked()) {
                hexCells[i][j].setChecked(true);
                j--;
                returnFlag = true;
                Log.d("Direction", "left2");
            }
            else if ((i - 1 > 0 && j - 1 > 0) && hexCells[i - 1][j - 1].getCellState() == R.drawable.hexagonblue && !hexCells[i - 1][j - 1].getChecked()) {
                hexCells[i][j].setChecked(true);
                i--;
                j--;
                returnFlag = true;
                Log.i("Direction", "leftup");
            } else if (i - 1 > 0 && hexCells[i - 1][j].getCellState() == R.drawable.hexagonblue && !hexCells[i - 1][j].getChecked()) {
                hexCells[i][j].setChecked(true);
                i--;
                returnFlag = true;
                Log.i("Direction", "up");
            }

            else{
                for (int p = 0; p < 10; p++) {
                    for (int u = 0; u < 10; u++) {
                        hexCells[p][u].setChecked(false);
                    }
                }
            }
            if (returnFlag) { //if there is 'x' path ahead, proceed to next one.
                return winCheckP2(i, j);
            }
        }
        return false;
    }
}