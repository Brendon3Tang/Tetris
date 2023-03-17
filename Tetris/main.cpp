//
//  main.cpp
//  Tetris
//
//  Created by Brandon3Tang on 3/17/23.
//  Copyright © 2023 Brandon3Tang. All rights reserved.
//
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <iostream>


using namespace std;
using namespace sf;

void keyEvent(){
     
}

void newBlock(){
    
}

int main(void){
    //create an interface
    // 1. create a window
    RenderWindow window(
                        VideoMode(320, 480),    //window size
                        "Tetris-BT2023"
                        );
    // 2. render background
    Texture t1;
    t1.loadFromFile("/Users/brandon3tang/gameDev/Tetris/Tetris/images/background.png");
    
    Sprite spriteBg(t1);
    
    window.draw(spriteBg);
    window.display();
    
    //generate the fisrt block
    newBlock();
    
    //getting into the game loop
    while(window.isOpen()){
        //waiting for players to press a key, and then dealing with it
        keyEvent();
        
    }
    
    cin.get();
    return 0;
}
