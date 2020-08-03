#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <math.h>
using namespace sf;// ��� ��� �� �������� � ����������� sfml, ��� ����� ����� �� ���������� ������ ��� � 
                   //������������� ������, ��� ��� ���������� ������������ ���� sf

const int tileSize = 32;

double offsetX = 0, offsetY = 0; //���������� ���������� �� �������� ������

SoundBuffer coinGet;
Sound coinGetSound(coinGet);
SoundBuffer jump;
Sound jumpSound(jump);

std::string TileMap[] =                             //�����, �������������� ���������
{
    "                             C   B ",
    "  C                              B ",
    "B      00000  0   0  C           B ",
    "B      0     000 000    B        B ",
    "B      00000  0   0          B000B ",
    "B   G           g B          B000B ",
    "BBBBBBBBBBBBBBBBBBB          BBBBB ",
    "B00             000    B     B     ",
    "B                           BB     ",
    "B  B         B   B   B       B     ",
    "B               00           B     ",
    "B       B                    B     ",
    "B                       000  B     ",
    "Bg    BP XXXXXXXBBXXXBB000H  B     ",
    "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBB     ",
    "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBB     ",
    "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBB     ",
    "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBB     ",
    "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBB     ",

    
};
int H;
int W;

class Player // ����� ������
{
    double frameChangeVelosity = 0.00001;        // �������� ����� ����� �������� ���������
    double currentFrame;                         // ���������� ��������� ���� ������� ����� � �������� ���������
    enum Direction { LEFT, RIGHT, UP, DOWN };   // ������������ ���� �����������
    Direction lastLookDir;                      // ���� ��������� ��� ������� ��������
    const int characterSizeX = 32;
    const int characterSizeY = 32;
    const int characterNum = 0;
    void SetupCamera()
    {
        if (!(rect.left > 300))
        {
            offsetX = 0;
        }
        else
        {
            if (!(rect.left < (W - 6) * tileSize - 300))
            {
                offsetX = (W - 6) * tileSize - 600;
            }
            else
                offsetX = rect.left - 600 / 2;
        }
        if (rect.top > 175)
            offsetY = rect.top - 175;
        else
            offsetY = 0;


    }
    void Colision(Direction axisDir)            // ������� ��������� ��������, ��� ������ ������������ ����, 
                                                    // �������� ��������� �������� ����������� ��� ���������, �
                                                // � ����������� �� ���, ������� �������� ���� �� OX, ���� OY
    {

        onGround = false;                       // ��� �����, ��� ����, ����� �������� �� ��������� �������� onGround,
                                                // � ����������� ��������, �.� ���� onGround c����� true, � dy = 0
                                                // �������� onGround ��� � ��������� true, �� �� ������� ��, ������ ����
                                                // �� �� ��������
                                                // ����� ������ ��������, ��� �� ��������� � ���������(��� �������������)
                                                // � ����� � ���, �� �� ������


        // �������� ��������������� � ������, ����������� �� ��������� �������, ��� �� ������
        for (int i = rect.top / tileSize; i < (rect.top + rect.height) / tileSize; i++)         // ��������� ����� ����������� for, �� ��������� ����
            for (int j = rect.left / tileSize; j < (rect.left + rect.width) / tileSize; j++)    // �� ������ �����, �� ������� ����� sprite 
            {
                if (i < H && j < W + 4 && i > -1 && j > -1)
                {
                    if (TileMap[i][j] == 'B' || TileMap[i][j] == 'P' || TileMap[i][j] == '_')
                    {

                        if (dx > 0 && (axisDir == Direction::LEFT || axisDir == Direction::RIGHT))
                            rect.left = j * tileSize - rect.width;
                        if (dx < 0 && (axisDir == Direction::LEFT || axisDir == Direction::RIGHT))
                            rect.left = (j + 1) * tileSize;

                        if (dy > 0 && (axisDir == Direction::UP || axisDir == Direction::DOWN))
                        {
                            rect.top = i * tileSize - rect.height; dy = 0; onGround = true;
                        }
                        if (dy < 0 && (axisDir == Direction::UP || axisDir == Direction::DOWN))
                        {
                            rect.top = (i + 1) * tileSize; dy = 0;
                        }

                    }

                    if (TileMap[i][j] == '0')
                    {
                        TileMap[i][j] = ' ';
                        coinGetSound.play();
                    }

                    if (TileMap[i][j] == 'X')
                    {
                        for (int i = 0; i < H; i++)
                        {
                            for (int j = 0; j < W; j++)
                            {
                                if (TileMap[i][j] == 'S')
                                {
                                    rect.top = i * tileSize;
                                    rect.left = j * tileSize;
                                    SetupCamera();
                                    dx = 0;
                                    dy = 0;
                                }
                            }
                        }
                    }
                }
            }

    }


public:
    FloatRect rect;                                    // �������������, �������� ������� �������� sprite
    double dx, dy;                                    // �������� �� ���� ����          
    bool onGround;                                   // ����� �� ������: �� ����� �� ��?
    Sprite sprite;                                  // Sprite, ������������ �� �������� � ��������������,\
                                                        ������� ��� �� �������� �� �������� ������������ ��������

    void Respawn()
    {
        rect = FloatRect(32, (H - 9) * 32, 32, 32);      // ����� � �������� ���������� ������������\
                                                        ������ ������� 2-�� �����������


        lastLookDir = Direction::RIGHT;             //����������� ����������� �������
        for (int i = 0; i < H; i++)
        {
            for (int j = 0; j < W; j++)
            {
                if (TileMap[i][j] == 'S') { rect.top = i * tileSize; rect.left = j * tileSize; SetupCamera(); }
            }
        }
        onGround = false;
        dx = dy = 0;
        currentFrame = 0;
    }

    Player(Texture& image)                           //����� ����� ��������� ���������� ��������
    {
        sprite.setTexture(image);                                           //��������� ��� ��������, ��� �������� �������
        sprite.setTextureRect(IntRect(0, 0, characterSizeX, characterSizeY));// � ������� �� ��� ������������ �����
        Respawn();
    }

    void update(double time)
    {

        rect.left += dx * time;                     // rect.left - ������� sprite � ���� �� ��� ox
        Colision(Direction::LEFT);                  // �����! ��� ������ �� �������� ������� ���������
                                                    // �� ����� �� ����
                                                    // �� ������� ����� ��������� Colision, �� ���� ���
                                                    // �����, ���� �� ������� ������� ��������� �� ���� ����, ��
                                                    // ����� ������� � ����, � �����, ������� ����� �� ���������


        if (!onGround) dy = dy + 0.000000001 * time; // ���������� �� �������� �� ����������� � ����� 
                                                     // ������������� dx - �������� �������, ������������� dy - c������� ����

        rect.top += dy * time;                    //������ �� ������ �� �������� �������, �� �����-�� ��������?
                                                 //, � ���������� �������� ����������� �� ������?
                                                // ������ ���, ������� ��������� ����� �������, ��� ������� ����� ���������
                                               // ������ � ����� ��������� �� ����� �����!
                                              // ��� �������� ��������� ����������� ������, ��
                                             // � �������� � ���� ����� ������������ � ������ ���������, ��
                                            // ��� �� ���������� ����� ����� ������� ����������, �� �� ����������� �� 
                                           // ����������� ������ ���� ����������� �� ����������!
                                               // rect.top - ������� sprite � ���� �� ��� oy
        Colision(Direction::DOWN);

        currentFrame += frameChangeVelosity * time; // �������� �� ������ ����� �� ������ ����� � ����������� �� �������� ���������
        if (currentFrame > 4) currentFrame -= 4;

        if (dx == 0)                                // � ����������� �� ����������� �������� �� OX �������� ��������         
            switch (lastLookDir)
            {
            case Direction::LEFT:  sprite.setTextureRect(IntRect(characterSizeX, characterNum * characterSizeY, -characterSizeX, characterSizeY)); break;
            case Direction::RIGHT: sprite.setTextureRect(IntRect(characterSizeX, characterNum * characterSizeY, characterSizeX, characterSizeY)); break;
                // ������� �� IntRect
                // 1 � 2 �������� - X � Y ���������� ������ �������� ����, � �������� �������� ���������� �������� ��������
                // 3 � 4 �������� - ��������� ������, � � ����� ����������� ������� ����� ����������� �� OX(3 ��������) � OY(4 ��������)
                // 3 �������� ������������� - ������, ������������� �����
                // 4 �������� ������������� - ����, ������������� �����

            }
        if (dx > 0)
        {
            sprite.setTextureRect(IntRect(characterSizeX * int(currentFrame), characterNum * characterSizeY, characterSizeX, characterSizeY));
            lastLookDir = Direction::RIGHT;
        }
        if (dx < 0)
        {
            sprite.setTextureRect(IntRect(characterSizeX * (int(currentFrame) + 1), characterNum * characterSizeY, -characterSizeX, characterSizeY));
            lastLookDir = Direction::LEFT;;
        }

        sprite.setPosition(rect.left - offsetX, rect.top - offsetY);

        dx = 0;
        SetupCamera();
    }
};

class BadPlayer // ����� ������
{

    double frameChangeVelosity = 0.00001;        // �������� ����� ����� �������� ���������
    double currentFrame;                         // ���������� ��������� ���� ������� ����� � �������� ���������
    enum Direction { LEFT, RIGHT, UP, DOWN };   // ������������ ���� �����������
    Direction lastLookDir;                      // ���� ��������� ��� ������� ��������
    const int characterSizeX = 32;
    const int characterSizeY = 32;
    const int characterNum = 3;

    void Colision(Direction axisDir)            // ������� ��������� ��������, ��� ������ ������������ ����, 
                                                    // �������� ��������� �������� ����������� ��� ���������, �
                                                // � ����������� �� ���, ������� �������� ���� �� OX, ���� OY
    {

        onGround = false;                       // ��� �����, ��� ����, ����� �������� �� ��������� �������� onGround,
                                                // � ����������� ��������, �.� ���� onGround c����� true, � dy = 0
                                                // �������� onGround ��� � ��������� true, �� �� ������� ��, ������ ����
                                                // �� �� ��������
                                                // ����� ������ ��������, ��� �� ��������� � ���������(��� �������������)
                                                // � ����� � ���, �� �� ������


        // �������� ��������������� � ������, ����������� �� ��������� �������, ��� �� ������
        for (int i = rect.top / tileSize; i < (rect.top + rect.height) / tileSize; i++)         // ��������� ����� ����������� for, �� ��������� ����
            for (int j = rect.left / tileSize; j < (rect.left + rect.width) / tileSize; j++)    // �� ������ �����, �� ������� ����� sprite 
            {
                if (i < H && j < W + 4 && i > -1 && j > -1)
                {
                    if (TileMap[i][j] == 'B' || TileMap[i][j] == 'P' || TileMap[i][j] == '_')
                    {

                        if (dx > 0 && (axisDir == Direction::LEFT || axisDir == Direction::RIGHT))
                        {
                            dx = dx * -1;
                            rect.left = j * tileSize - rect.width;
                            continue;
                        }

                        if (dx < 0 && (axisDir == Direction::LEFT || axisDir == Direction::RIGHT))
                        {
                            dx = dx * -1;
                            rect.left = (j + 1) * tileSize;
                            continue;
                        }

                        if (dy > 0 && (axisDir == Direction::UP || axisDir == Direction::DOWN))
                        {
                            rect.top = i * tileSize - rect.height; dy = 0; onGround = true;
                        }
                        if (dy < 0 && (axisDir == Direction::UP || axisDir == Direction::DOWN))
                        {
                            rect.top = (i + 1) * tileSize; dy = 0;
                        }

                    }


                    if (TileMap[i][j] == 'X') { alive = false; }
                }
            }

    }


public:
    FloatRect rect;                                         // �������������, �������� ������� �������� sprite
    double dx, dy;                                          // �������� �� ���� ����          
    bool onGround, alive;                                   // ����� �� ������: �� ����� �� ��?
    Sprite sprite;                                          // Sprite, ������������ �� �������� � ��������������,\
                                                                ������� ��� �� �������� �� �������� ������������ ��������


    BadPlayer(Texture& image)                           //����� ����� ��������� ���������� ��������
    {
        sprite.setTexture(image);                                           //��������� ��� ��������, ��� �������� �������
        sprite.setTextureRect(IntRect(0, 0, characterSizeX, characterSizeY));// � ������� �� ��� ������������ �����

        alive = true;
        rect = FloatRect(32, (H - 9) * 32, 32, 32);      // ����� � �������� ���������� ������������\
                                                        ������ ������� 2-�� �����������


        lastLookDir = Direction::RIGHT;             //����������� ����������� �������
        for (int i = 0; i < H; i++)
        {
            for (int j = 0; j < W; j++)
            {
                if (TileMap[i][j] == 'E') { rect.top = i * tileSize; rect.left = j * tileSize; }
            }
        }
        onGround = false;
        dx = 0.0001;
        dy = 0;
        currentFrame = 0;
    }
    BadPlayer(Texture& image, int x, int y)
    {
        sprite.setTexture(image);                                           //��������� ��� ��������, ��� �������� �������
        sprite.setTextureRect(IntRect(0, 0, characterSizeX, characterSizeY));// � ������� �� ��� ������������ �����

        alive = true;
        rect = FloatRect(32, (H - 9) * 32, 32, 32);      // ����� � �������� ���������� ������������\
                                                        ������ ������� 2-�� �����������


        { rect.top = y * tileSize; rect.left = x * tileSize; }

        onGround = false;
        dx = 0.0001;
        dy = 0;
        currentFrame = 0;
    }
    void update(double time)
    {

        rect.left += dx * time;                     // rect.left - ������� sprite � ���� �� ��� ox
        Colision(Direction::LEFT);                  // �����! ��� ������ �� �������� ������� ���������
                                                    // �� ����� �� ����
                                                    // �� ������� ����� ��������� Colision, �� ���� ���
                                                    // �����, ���� �� ������� ������� ��������� �� ���� ����, ��
                                                    // ����� ������� � ����, � �����, ������� ����� �� ���������


        if (!onGround) dy = dy + 0.000000001 * time; // ���������� �� �������� �� ����������� � ����� 
                                                     // ������������� dx - �������� �������, ������������� dy - c������� ����

        rect.top += dy * time;                    //������ �� ������ �� �������� �������, �� �����-�� ��������?
                                                 //, � ���������� �������� ����������� �� ������?
                                                // ������ ���, ������� ��������� ����� �������, ��� ������� ����� ���������
                                               // ������ � ����� ��������� �� ����� �����!
                                              // ��� �������� ��������� ����������� ������, ��
                                             // � �������� � ���� ����� ������������ � ������ ���������, ��
                                            // ��� �� ���������� ����� ����� ������� ����������, �� �� ����������� �� 
                                           // ����������� ������ ���� ����������� �� ����������!
                                               // rect.top - ������� sprite � ���� �� ��� oy
        Colision(Direction::DOWN);

        currentFrame += frameChangeVelosity * time; // �������� �� ������ ����� �� ������ ����� � ����������� �� �������� ���������
        if (currentFrame > 4) currentFrame -= 4;

        if (dx == 0)                                // � ����������� �� ����������� �������� �� OX �������� ��������         
            switch (lastLookDir)
            {
            case Direction::LEFT:  sprite.setTextureRect(IntRect(characterSizeX, characterNum * characterSizeY, -characterSizeX, characterSizeY)); break;
            case Direction::RIGHT: sprite.setTextureRect(IntRect(characterSizeX, characterNum * characterSizeY, characterSizeX, characterSizeY)); break;
                // ������� �� IntRect
                // 1 � 2 �������� - X � Y ���������� ������ �������� ����, � �������� �������� ���������� �������� ��������
                // 3 � 4 �������� - ��������� ������, � � ����� ����������� ������� ����� ����������� �� OX(3 ��������) � OY(4 ��������)
                // 3 �������� ������������� - ������, ������������� �����
                // 4 �������� ������������� - ����, ������������� �����

            }
        if (dx > 0)
        {
            sprite.setTextureRect(IntRect(characterSizeX * int(currentFrame), characterNum * characterSizeY, characterSizeX, characterSizeY));
            lastLookDir = Direction::RIGHT;
        }
        if (dx < 0)
        {
            sprite.setTextureRect(IntRect(characterSizeX * (int(currentFrame) + 1), characterNum * characterSizeY, -characterSizeX, characterSizeY));
            lastLookDir = Direction::LEFT;;
        }

        sprite.setPosition(rect.left - offsetX, rect.top - offsetY);

    }
};

int main()
{

    sf::RenderWindow window(sf::VideoMode(600, 350), "Traveller");
    H = sizeof(TileMap)/sizeof(TileMap[0]);         //������ �����
    W = TileMap[0].size();                          //������ �����

    Texture charactersT;                            //��������� �������
    Texture sheetT;
    charactersT.loadFromFile("characters.png");
    sheetT.loadFromFile("sheet.png");

    Sprite coin;
    Texture coinT;
    coinT.loadFromFile("Coin.png");
    coin.setTexture(coinT);
    
    Sprite tile;
    tile.setTexture(sheetT);
    Player man(charactersT);
    BadPlayer man1(charactersT, 1 ,0);
    BadPlayer man2(charactersT, 3 ,0);
    BadPlayer man3(charactersT, 6 ,0);
    BadPlayer man4(charactersT, 4 ,0);
    BadPlayer man5(charactersT, 10 ,0);
    Clock timer;    //���� ������ ����� ������� �����, ����� ������� ������ ����������
    Clock musicTimer;
    
    Music backMusic;
    backMusic.openFromFile("backMusic.ogg");
    backMusic.setVolume(25);
    backMusic.play();
    musicTimer.restart();

    coinGet.loadFromFile("CoinSFX.ogg");
    jump.loadFromFile("JumpSFX.ogg");
    while (window.isOpen())
    {
        if (musicTimer.getElapsedTime().asSeconds() >= 153)
        {
            musicTimer.restart();
            backMusic.play();
        }
        double time = timer.getElapsedTime().asMicroseconds();
        Event event;
        timer.restart();
        if (window.pollEvent(event))       // window.pollEvent(event) ������� ������� 
        {
            if (event.type == Event::Closed) window.close();
        }

        if (window.hasFocus())
        {
            timer.restart();

            //��������� ������� 
            {
                if (Keyboard::isKeyPressed(Keyboard::Left)) // � ����������� �� ������� ������ �� �������� �������� ���������
                {
                    man.dx = -0.0002;
                }
                if (Keyboard::isKeyPressed(Keyboard::Right))
                {
                    man.dx = 0.0002;
                }
                if (Keyboard::isKeyPressed(Keyboard::Right) && Keyboard::isKeyPressed(Keyboard::Left))
                {
                    man.dx = 0;
                }
                if (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::Space))
                {
                    if (man.onGround) { man.dy = -0.0005; man.onGround = false; jumpSound.play();}
                }
            }
            man.update(time);
            man1.update(time);
            man2.update(time);
            man3.update(time);
            man4.update(time);
            man5.update(time);

            if (man.rect.intersects(man1.rect))
            {
                if (man1.alive)
                {
                    if (man.dy > 0) { man1.alive = false; man.dy = -0.0005; }
                    else man.Respawn();
                }
            }
            if (man.rect.intersects(man2.rect))
            {
                if (man2.alive)
                {
                    if (man.dy > 0) { man2.alive = false; man.dy = -0.0005; }
                    else man.Respawn();
                }
            }
            if (man.rect.intersects(man3.rect))
            {
                if (man3.alive)
                {
                    if (man.dy > 0) { man3.alive = false; man.dy = -0.0005; }
                    else man.Respawn();
                }
            }
            if (man.rect.intersects(man4.rect))
            {
                if (man4.alive)
                {
                    if (man.dy > 0) { man4.alive = false; man.dy = -0.0005; }
                    else man.Respawn();
                }
            }
            if (man.rect.intersects(man5.rect))
            {
                if (man5.alive)
                {
                    if (man.dy > 0) { man5.alive = false; man.dy = -0.0005; }
                    else man.Respawn();
                }
            }
            window.clear();  //������� ����

            
            offsetX = floor(offsetX);
            offsetY = floor(offsetY);

            //����� ����������� �����
            for (int i = 0; i < H; i++)
            {
                for (int j = 0; j < W - 1; j++)
                {
                    if (TileMap[i][j] == '-')   continue; else
                    if (TileMap[i][j] == '_')   continue; else
                    if (TileMap[i][j] == 'X')   tile.setTextureRect(IntRect(34 * 10, 34 * 2, tileSize, tileSize)); else
                    if (TileMap[i][j] == 'S')
                    {
                        tile.setTextureRect(IntRect(34 * 3, 34 * 4, tileSize, tileSize));
                        tile.setPosition(tileSize * j - offsetX, tileSize * i - offsetY);
                        window.draw(tile);

                        TileMap[i - 1][j] = '-';
                        tile.setTextureRect(IntRect(34 * 3, 34 * 3, tileSize, tileSize));
                        tile.setPosition(tileSize * (j)-offsetX, tileSize * (i - 1) - offsetY);
                        window.draw(tile);

                        TileMap[i - 1][j - 1] = '-';
                        tile.setTextureRect(IntRect(34 * 2, 34 * 3, tileSize, tileSize));
                        tile.setPosition(tileSize * (j - 1) - offsetX, tileSize * (i - 1) - offsetY);
                        window.draw(tile);

                        TileMap[i - 2][j] = '-';
                        tile.setTextureRect(IntRect(34 * 3, 34 * 2, tileSize, tileSize));
                        tile.setPosition(tileSize * (j)-offsetX, tileSize * (i - 2) - offsetY);
                        window.draw(tile);

                        continue;
                    } else
                    if (TileMap[i][j] == '0')
                    {
                        coin.setPosition(tileSize * j - offsetX, tileSize * i - offsetY);
                        window.draw(coin);
                        continue;
                    } else
                    if (TileMap[i][j] == 'B') tile.setTextureRect(IntRect(0, 0, tileSize, tileSize)); else
                    if (TileMap[i][j] == 'G')
                    {
                        TileMap[i][j - 1] = '-';
                        tile.setTextureRect(IntRect(34 * 2, 0, tileSize, tileSize));
                        tile.setPosition(tileSize * (j - 1) - offsetX, tileSize * i - offsetY);
                        window.draw(tile);

                        tile.setTextureRect(IntRect(34 * 3, 0, tileSize, tileSize));
                        tile.setPosition(tileSize * j - offsetX, tileSize * i - offsetY);
                        window.draw(tile);

                        TileMap[i][j + 1] = '-';
                        tile.setTextureRect(IntRect(34 * 4, 0, tileSize, tileSize));
                        tile.setPosition(tileSize * (j + 1) - offsetX, tileSize * i - offsetY);
                        window.draw(tile);

                        continue;
                    } else
                    if (TileMap[i][j] == 'g')
                    {

                        tile.setTextureRect(IntRect(34 * 2, 0, tileSize, tileSize));
                        tile.setPosition(tileSize * j - offsetX, tileSize * i - offsetY);
                        window.draw(tile);

                        TileMap[i][j + 1] = '-';
                        tile.setTextureRect(IntRect(34 * 4, 0, tileSize, tileSize));
                        tile.setPosition(tileSize * (j + 1) - offsetX, tileSize * i - offsetY);
                        window.draw(tile);

                        continue;
                    } else
                    if (TileMap[i][j] == 'C')
                    {

                        tile.setTextureRect(IntRect(0, 34 * 7, tileSize, tileSize));
                        tile.setPosition(tileSize * j - offsetX, tileSize * i - offsetY);
                        window.draw(tile);

                        TileMap[i + 1][j] = '-';
                        tile.setTextureRect(IntRect(0, 34 * 8, tileSize, tileSize));
                        tile.setPosition(tileSize * j - offsetX, tileSize * (i + 1) - offsetY);
                        window.draw(tile);

                        TileMap[i][j + 1] = '-';
                        tile.setTextureRect(IntRect(34, 34 * 7, tileSize, tileSize));
                        tile.setPosition(tileSize * (j + 1) - offsetX, tileSize * i - offsetY);
                        window.draw(tile);

                        TileMap[i + 1][j + 1] = '-';
                        tile.setTextureRect(IntRect(34, 34 * 8, tileSize, tileSize));
                        tile.setPosition(tileSize* (j + 1) - offsetX, tileSize* (i + 1) - offsetY);
                        window.draw(tile);

                        TileMap[i][j + 2] = '-';
                        tile.setTextureRect(IntRect( 34 * 2, 34 * 7, tileSize, tileSize));
                        tile.setPosition(tileSize * (j + 2) - offsetX, tileSize * (i)-offsetY);
                        window.draw(tile);

                        TileMap[i + 1][j + 2] = '-';
                        tile.setTextureRect(IntRect( 34 * 2, 34 * 8, tileSize, tileSize));
                        tile.setPosition(tileSize * (j + 2) - offsetX, tileSize * (i + 1) - offsetY);
                        window.draw(tile);

                        continue;
                    } else
                    if (TileMap[i][j] == 'P')
                    {
                        
                        tile.setTextureRect(IntRect(0, 34 * 3, tileSize, tileSize));
                        tile.setPosition(tileSize * (j) - offsetX, tileSize * (i) - offsetY);
                        window.draw(tile);

                        TileMap[i - 1][j] = '_';
                        tile.setTextureRect(IntRect(0, 34 * 2, tileSize, tileSize));
                        tile.setPosition(tileSize* (j)-offsetX, tileSize* (i - 1)-offsetY);
                        window.draw(tile);


                        TileMap[i][j + 1] = '_';
                        tile.setTextureRect(IntRect(34, 34 * 3, tileSize, tileSize));
                        tile.setPosition(tileSize* (j + 1)-offsetX, tileSize* (i) - offsetY);
                        window.draw(tile);


                        TileMap[i - 1][j + 1] = '_';
                        tile.setTextureRect(IntRect(34 , 34 * 2, tileSize, tileSize));
                        tile.setPosition(tileSize* (j + 1)-offsetX, tileSize* (i - 1) - offsetY);
                        window.draw(tile);

                        continue;
                    } else
                    if (TileMap[i][j] == 'H')
                    {
                        tile.setTextureRect(IntRect(34 * 1, 34 * 1, tileSize, tileSize));
                        tile.setPosition(tileSize* (j)-offsetX, tileSize* (i)-offsetY);
                        window.draw(tile);

                        TileMap[i - 1][j] = '-';
                        tile.setTextureRect(IntRect(34 * 1, 0 , tileSize, tileSize));
                        tile.setPosition(tileSize * (j)-offsetX, tileSize * (i - 1)-offsetY);
                        window.draw(tile);

                        TileMap[i][j - 1] = '-';
                        tile.setTextureRect(IntRect(0 , 34 * 1, tileSize, tileSize));
                        tile.setPosition(tileSize* (j - 1)-offsetX, tileSize* (i) - offsetY);
                        window.draw(tile);

                        TileMap[i][j + 1] = '-';
                        tile.setTextureRect(IntRect(34 * 2, 34 * 1, tileSize, tileSize));
                        tile.setPosition(tileSize* (j + 1) - offsetX, tileSize* (i) - offsetY);
                        window.draw(tile);
                        continue;
                    } else  
                        tile.setTextureRect(IntRect(34 * 12, 34 * 6, tileSize, tileSize));
                    tile.setPosition(j * tileSize - offsetX, i * tileSize - offsetY);
                    window.draw(tile);
                }

            }
            window.draw(man.sprite);  //������ ������
            if (man1.alive) window.draw(man1.sprite);
            if (man2.alive) window.draw(man2.sprite);
            if (man3.alive) window.draw(man3.sprite);
            if (man4.alive) window.draw(man4.sprite);
            if (man5.alive) window.draw(man5.sprite);
            window.display();//������� ������ �� �����

        }


    }

    return 0;
}