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
int blocks[7][4] = {
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
}curBlock[4], lastBlock[4];

// 定义下降速度
const float SPEED_NORMAL = 0.5;
const float SPEED_QUICK = 0.05;
float delay = SPEED_NORMAL;  //实际选用的速度

bool check(){
    for(int i = 0; i < 4; i++){
        if(curBlock[i].x < 0
           || curBlock[i].x >= COL_COUNT
           || curBlock[i]. y >= ROW_COUNT
           || table[curBlock[i].y][curBlock[i].x] != 0 )
            return false;
    }
    return true;
}

void moveLeftRight(int offset){
    for(int i = 0; i < 4; i++){
        lastBlock[i] = curBlock[i];
        //cout << "offset is: " << offset << endl;
        curBlock[i].x += offset;
    }
    
    if(!check()){
        for(int i = 0; i < 4; i++)
            curBlock[i] = lastBlock[i];
    }
    
}

void doRotate(){
    
}

void keyEvent(RenderWindow *window){
    bool rotate = false;    //是否旋转
    int offset = 0;
    Event e;    //事件变量
    
    //  pollEvent从事件队列中取出一个事件
    //如果没有事件了，就返回false
    while(window->pollEvent(e)){
        if(e.type == sf::Event::Closed)
            window->close();
        if(e.type == Event::KeyPressed){
            switch (e.key.code) {
                case Keyboard::Up:
                    rotate = true;
                    break;
                case Keyboard::Left:
                    offset = -1;
                    break;
                case Keyboard::Right:
                    offset = 1;
                    break;
                case Keyboard::Down:
                    delay = SPEED_QUICK;
                    break;
                default:
                    break;
          }
        }
        //处理移动
        if(offset !=0 ) moveLeftRight(offset);
        //处理旋转
        if(rotate)  doRotate();
    }
    
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
        curBlock[i].x = blocks[n][i] % 2;
        curBlock[i].y = blocks[n][i] / 2;
    }
}

void drawBlock(Sprite *spriteTiles, RenderWindow *window){
    //  1. 已经降落到底部的俄罗斯方块
    for(int i = 0; i < ROW_COUNT; i++){
        for(int j = 0; j < COL_COUNT; j++){
            if(table[i][j] != 0){//如果table有方块，则把他画出来
                //画方块，需要使用sprite来表示完整的方块图片
                spriteTiles->setTextureRect(IntRect(table[i][j] * 18, 0, 18, 18));   //set the sub-rec of the texture with the (x, y, width, height)
                spriteTiles->setPosition(j * 18, i * 18);   //j is column, so j is x; i is row, so i is y
                //设置偏移量
                spriteTiles->move(27, 30);
                //draw it out
                window->draw(*spriteTiles);
            }
        }
    }
    
    //  2. 正在降落中的俄罗斯方块（当前方块）只需要画当前的4个blocks
    for(int i = 0; i < 4; i++){ //blockIndex记录了当前方块的种类
        spriteTiles->setTextureRect(IntRect(blockIndex * 18, 0, 18, 18));
        spriteTiles->setPosition(curBlock[i].x * 18, curBlock[i].y * 18);
        //设置偏移量
        spriteTiles->move(27, 30);
        //draw it out
        window->draw(*spriteTiles);
    }
}

// drop()会使当前方块下降一个位置
void drop(){
    for(int i = 0; i < 4; i++){
        lastBlock[i] = curBlock[i];
        curBlock[i].y++;
    }
    
    if(!check()){
        //当curBlock到达底部或者与别的block接触时，固化处理
        for(int i = 0; i < 4; i++){
            table[lastBlock[i].y][lastBlock[i].x] = blockIndex;
        }
        //生成一个新方块
        newBlock();
    }
}

int main(void){
    srand(time(0)); //生成一个随机种子
    //create an interface
    // 1. create a window
    RenderWindow window(
                        VideoMode(320, 480),    //window size
                        "Tetris-BT2023"
                        );
    // 2. render background & tiles
    Texture background;
    background.loadFromFile("/Users/brandon3tang/gameDev/Tetris/Tetris/images/background.png"); //把背景图片加载到内存
    Sprite spriteBg(background);    //根据图片创造sprite
    
    Texture frame;
    frame.loadFromFile("/Users/brandon3tang/gameDev/Tetris/Tetris/images/frame.png");
    Sprite spriteFrame(frame);
    
    Texture tiles;
    tiles.loadFromFile("/Users/brandon3tang/gameDev/Tetris/Tetris/images/tiles.png");
    //把背景图片加载到内存
    Sprite spriteTiles(tiles);    //根据图片创造sprite
    
    //3. generate the fisrt block
    newBlock();
    
    // 计时器clock
    Clock clock;    //sf::Clock,生成时就会启动
    float overallTime = 0;
    
    //getting into the game loop
    while(window.isOpen()){// 如果窗口未被关闭
        float time = clock.getElapsedTime().asSeconds(); //获取计时器从上一次重启/启动到现在所经过的时间
        clock.restart();
        overallTime += time;
        //waiting for players to press a key, and then dealing with it
        keyEvent(&window);
//        sf::Event event;
//        while (window.pollEvent(event))
//        {
//            // Close window: exit
//            if (event.type == sf::Event::Closed) {
//                window.close();
//            }
//
//            // Escape pressed: exit
//            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
//                window.close();
//            }
//        }
        
        if(overallTime > delay){
            //降落
            drop(); //方块下降一个位置
            overallTime = 0;    //总时间清零，等待累计到下一次下降的时间
        }
        
        delay = SPEED_NORMAL; //初始化速度，以防在keyEvent中加速过。为什么在这里初始化？？
        
        //绘制游戏
        window.clear(Color::White);
        window.draw(spriteBg);
        window.draw(spriteFrame);
        //绘制方块
        drawBlock(&spriteTiles, &window);
        
        window.display();
    }
    
    cin.get();
    return 0;
}
