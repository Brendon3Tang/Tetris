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

//block包含所有七种block类型，每当要创建新block时，便会从中取相对应的block用以转换成curBlock
int block[7][4] = {
    {1, 3, 5, 7},   //1字形
    {2, 4, 5, 7},   //Z 1型
    {3, 5, 4, 6},   //Z 2型
    {3, 5, 4, 7},   //T
    {2, 3, 5, 7},   //L
    {3, 5, 7, 6},   //J
    {2, 3, 4, 5}    //田
};

const int ROW_COUNT = 20;
const int COL_COUNT = 10;
//game zoon: table[i][j] = 0 means that the position (i,j) is empty, else is occupied by a block with the related index
int table[ROW_COUNT][COL_COUNT];//    20行10列

int blockIndex; //表示当前方块的种类

struct{
    int x, y;
}curBlock[4];

void keyEvent(){
     
}

// newBlock（）会把block的序号转化成坐标轴中的坐标，然后赋予curBlock，形成一个新的block
void newBlock(){
    blockIndex = 1 + rand() % 7;    //从1～7中取一个随机值
    
    int n = blockIndex - 1;
    
    // 把block里的序号转化为坐标轴上的坐标,并且放入curBlock，形成curBlock。
    /*  序号 % 2 = x坐标
     *  序号 / 2 = y坐标
     **/
    for(int i = 0; i < 4; i++){
        curBlock[i].x = block[n][i] % 2;
        curBlock[i].y = block[n][i] / 2;
    }
}

void drawBlock(Sprite *spriteTiles){
    //  1. 已经降落到底部的俄罗斯方块
    for(int i = 0; i < ROW_COUNT; i++){
        for(int j = 0; j < COL_COUNT; j++){
            if(table[i][j] != 0){//如果table有方块，则把他画出来
                //画方块，需要使用sprite来表示完整的方块图片
                spriteTiles->setTextureRect(IntRect());   //set the sub-rec of the texture
            }
        }
    }
    
    //  2. 正在降落中的俄罗斯方块（当前方块）
}

int main(void){
    srand(time(0)); //生成一个随机种子
    //create an interface
    // 1. create a window
    RenderWindow window(
                        VideoMode(320, 480),    //window size
                        "Tetris-BT2023"
                        );
    // 2. render background
    Texture background;
    background.loadFromFile("/Users/brandon3tang/gameDev/Tetris/Tetris/images/background.png"); //把背景图片加载到内存
    Sprite spriteBg(background);    //根据图片创造sprite
    
    //3. render tiles
    Texture tiles;
    tiles.loadFromFile("/Users/brandon3tang/gameDev/Tetris/Tetris/images/tiles.png");
    Sprite spriteTiles(tiles);
    
    window.draw(spriteBg);
    window.display();
    
    //generate the fisrt block
    newBlock();
    
    //getting into the game loop
    while(window.isOpen()){// 如果窗口未被关闭
        //waiting for players to press a key, and then dealing with it
        keyEvent();
        
        //绘制游戏
        
        //绘制方块
        drawBlock(&spriteTiles);
    }
    
    cin.get();
    return 0;
}
