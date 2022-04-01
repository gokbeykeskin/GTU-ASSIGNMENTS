package com.example.hexgame;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class Menu2 extends AppCompatActivity {
    public int gameMode;
    public static final String EXTRA_INFO_GM = "com.example.hexgame.EXTRA_INFO_GM";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_menu2);
        Button pvp = (Button) findViewById(R.id.pvp);
        Button pve = (Button) findViewById(R.id.pve);
        pvp.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
                gameMode=0;
                startGame();
            }
        });
        pve.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
               pve.setText("Not Yet.");
            }
        });
    }
    public void startGame(){
        Intent intent = new Intent(this,MainActivity.class);
        intent.putExtra(EXTRA_INFO_GM,gameMode);
        startActivity(intent);
    }
}