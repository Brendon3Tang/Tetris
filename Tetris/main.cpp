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

struct Point{
    int x, y;
}curBlock[4], lastBlock[4];

// 定义下降速度
const float SPEED_NORMAL = 0.5;
const float SPEED_QUICK = 0.05;
float delay = SPEED_NORMAL;  //实际选用的速度

Sound sound;
Text textScore, scoreTitle;
Font font;
int score = 0;

void checkOver(){
    
}

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
    if(blockIndex == 7) return; //如果方块是“田”型，不需要旋转
    Point p = curBlock[1]; //定义旋转中心
    for(int i = 0; i < 4; i++){
        lastBlock[i] = curBlock[i]; //备份旧点
        curBlock[i].x = p.x - lastBlock[i].y + p.y;
        curBlock[i].y = lastBlock[i].x - p.x + p.y;
    }
    if(!check()){
        for(int i = 0; i < 4; i++){
            curBlock[i] = lastBlock[i];
        }
    }
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
    
    int n = blockIndex - 1; //n表示从block[i][j]中取i.index为n（第n+1行）的block
    
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

void clearLine(){
    int k = ROW_COUNT - 1;  //k为慢指针，i为快指针。每当该行的count != col_num时，说明该行不需要消除，k随着i一起往上移动；如果count == col_num时，说明该行需要消除，此时k不上移，当i上移时，“table[k][j] = table[i][j]” 这行代码会将第i行的格子全部copy到第k行（此时k在i的下一行，k也是我们要消除的那一行），这样就相当于把第k行消除了。随着循环，k始终小于i，所以始终会把上一行的格子移动到下一行，所以也完成了上方其余格子的移动
    for(int i = ROW_COUNT-1; i >= 0; i--){
        int count = 0;
        for(int j = 0; j < COL_COUNT; j++){
            if(table[i][j] != 0) count++;
            
            table[k][j] = table[i][j];
        }
        if(count < COL_COUNT){
            k--;
        }
        else{
            sound.play();   //播放消除声音
            //累计分数
            score += 10;
        }
    }
    
    char tmp[16];
    sprintf(tmp, "%d", score);
    textScore.setString(tmp);
}

void initialScore(){
    if(!font.loadFromFile("/Users/brandon3tang/gameDev/Tetris/Tetris/sansation.ttf"))   exit(1);
    
    textScore.setFont(font);
    textScore.setCharacterSize(30);
    textScore.setColor(Color::Blue);
    textScore.setStyle(Text::Bold);
    textScore.setPosition(155, 445);
    textScore.setString("0");
    
    scoreTitle.setFont(font);
    scoreTitle.setCharacterSize(30);
    scoreTitle.setColor(Color:: Blue);
    scoreTitle.setStyle(Text::Bold);
    scoreTitle.setPosition(110, 405);
    scoreTitle.setString("SCORE");
}

int main(void){
    srand(time(0)); //生成一个随机种子
    
    Music music;
    if(!music.openFromFile("/Users/brandon3tang/gameDev/Tetris/Tetris/audio/background.wav"))   return -1;
    music.setLoop(true);
    music.play();
    
    SoundBuffer clear;
    if(!clear.loadFromFile("/Users/brandon3tang/gameDev/Tetris/Tetris/audio/remove.wav"))   return -1;
    sound.setBuffer(clear);
    
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
    
    initialScore();
    
    //3. generate the fisrt block
    newBlock();
    
    // 计时器clock
    Clock clock;    //sf::Clock,生成时就会启动
    float timer = 0;
    
    //getting into the game loop
    while(window.isOpen()){// 如果窗口未被关闭
        float time = clock.getElapsedTime().asSeconds(); //获取计时器从上一次重启/启动到现在所经过的时间
        clock.restart();
        timer += time;
        //waiting for players to press a key, and then dealing with it
        keyEvent(&window);
        
        if(timer > delay){
            //降落
            drop(); //方块下降一个位置
            timer = 0;    //总时间清零，等待累计到下一次下降的时间
        }
        
        clearLine();
        
        delay = SPEED_NORMAL; //初始化速度，以防在keyEvent中加速过。为什么在这里初始化？？
        
        //绘制游戏
        window.clear(Color::White);
        window.draw(spriteBg);
        window.draw(spriteFrame);
        window.draw(scoreTitle);
        window.draw(textScore);
        //绘制方块
        drawBlock(&spriteTiles, &window);
        
        window.display();
    }
    
    //cin.get();
    return 0;
}
