package com.example.hexgame;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class MainMenu extends AppCompatActivity {
    Button play,exit;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main_menu);
        play = (Button) findViewById(R.id.play);
        exit = (Button) findViewById(R.id.exit);
        play.setOnClickListener(new View.OnClickListener(){
             @Override
             public void onClick(View v){
                openMenu2();
            }
        });
        exit.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                finish();
                System.exit(0);
            }
        });
    }

    public void openMenu2(){
        Intent intent = new Intent(this,Menu2.class);
        startActivity(intent);
    }
}