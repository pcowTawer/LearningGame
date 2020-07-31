#include <SFML/Graphics.hpp>
using namespace sf;// ��� ��� �� �������� � ����������� sfml, ��� ����� ����� �� ���������� ������ ��� � 
                   //������������� ������, ��� ��� ���������� ������������ ���� sf

const int tileSize = 16;
const int characterSizeX = 32;
const int characterSizeY = 32;

double offsetX = 0, offsetY = 0; //���������� ���������� �� �������� ������

std::string TileMap[] =                             //�����, �������������� ���������
{
    "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
    "B             C--                B0000B",
    "B             ---                B0000B",
    "B      C--               C--     B0000B",
    "B      ---               ---     B   BB",
    "B                     B         BB    B",
    "B             B                  BB   B",
    "B       B                   BB   B   BB",
    "B  S                  B0000 BB        B",
    "B     BP              B0000 BBg-  G   B",
    "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
    "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
    "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
    "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
    "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",

    
};

const int H = sizeof(TileMap)/sizeof(TileMap[0]);           //������ �����
const int W = sizeof(TileMap[0]) / sizeof(TileMap[0][0]);   //������ �����

class Player // ����� ������
{
    FloatRect rect;                             // �������������, �������� ������� �������� sprite
    double frameChangeVelosity = 0.00001;        // �������� ����� ����� �������� ���������
    double currentFrame;                         // ���������� ��������� ���� ������� ����� � �������� ���������
    enum Direction { LEFT, RIGHT, UP, DOWN };   // ������������ ���� �����������
    Direction lastLookDir;                      // ���� ��������� ��� ������� ��������

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
                if (i < H && j < W)
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
                    }
                    
                    if (TileMap[i][j] == 'X')
                    {
                        for (int i = 0; i < H; i++)
                        {
                            for (int j = 0; j < W; j++)
                            {
                                if (TileMap[i][j] == 'S') { rect.top = i * tileSize; rect.left = j * tileSize; offsetX = 0; offsetY = 0; dx = 0; dy = 0; }
                            }
                        }
                    }
                }
            }
        
    }


public: 
    double dx, dy;                                    // �������� �� ���� ����          
    bool onGround;                                   // ����� �� ������: �� ����� �� ��?
    Sprite sprite;                                   // Sprite, ������������ �� �������� � ��������������,\
                                                        ������� ��� �� �������� �� �������� ������������ ��������
    

    Player(Texture& image)                           //����� ����� ��������� ���������� ��������
    {
        sprite.setTexture(image);                                           //��������� ��� ��������, ��� �������� �������
        sprite.setTextureRect(IntRect(0, 0, characterSizeX, characterSizeY));// � ������� �� ��� ������������ �����

        rect = FloatRect(32, (H-9)*32, 32, 32);      // ����� � �������� ���������� ������������\
                                                        ������ ������� 2-�� �����������
        

        lastLookDir = Direction::RIGHT;             //����������� ����������� �������
        for (int i = 0; i < H; i++)
        {
            for (int j = 0; j < W; j++)
            {
                if (TileMap[i][j] == 'S') { rect.top = i * tileSize; rect.left = j * tileSize; offsetX = 0; offsetY = 0; }
            }
        }
        onGround = false;
        dx = dy = 0;
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
                case Direction::LEFT:  sprite.setTextureRect(IntRect(characterSizeX , characterSizeY , -characterSizeX, characterSizeY)); break;
                case Direction::RIGHT: sprite.setTextureRect(IntRect(characterSizeX , characterSizeY , characterSizeX, characterSizeY)); break;
                    // ������� �� IntRect
                    // 1 � 2 �������� - X � Y ���������� ������ �������� ����, � �������� �������� ���������� �������� ��������
                    // 3 � 4 �������� - ��������� ������, � � ����� ����������� ������� ����� ����������� �� OX(3 ��������) � OY(4 ��������)
                    // 3 �������� ������������� - ������, ������������� �����
                    // 4 �������� ������������� - ����, ������������� �����

            }
        if (dx > 0) 
        {
            sprite.setTextureRect(IntRect(characterSizeX * int(currentFrame), characterSizeY, characterSizeX, characterSizeY));
            lastLookDir = Direction::RIGHT;
        }
        if (dx < 0)
        {
            sprite.setTextureRect(IntRect(characterSizeX * (int(currentFrame) + 1), characterSizeY, -characterSizeX, characterSizeY));
            lastLookDir = Direction::LEFT;;
        }

        sprite.setPosition(rect.left - offsetX, rect.top - offsetY);

        dx = 0;
        if (rect.left > 300 && rect.left < (W-1)*tileSize-300) offsetX = rect.left - 600/2;
        if (rect.top > 175) offsetY = rect.top  - 175;
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(600, 350), "SFML works!");


    Texture charactersT;                    //��������� �������
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
    Clock timer;    //���� ������ ����� ������� �����, ����� ������� ������ ����������




    while (window.isOpen())
    {
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
                    if (man.onGround) { man.dy = -0.0005; man.onGround = false; }
                }
            }
            man.update(time);

            window.clear();  //������� ����

            //����� ����������� �����
            for (int i = 0; i < H; i++)
            {

                for (int j = 0; j < W - 1; j++)
                {

                    if (TileMap[i][j] == '-')   continue;
                    if (TileMap[i][j] == '_')   continue;
                    if (TileMap[i][j] == 'X')   tile.setTextureRect(IntRect(34 * 10, 34 * 2, tileSize, tileSize));
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
                    };
                    if (TileMap[i][j] == '0')
                    {
                        coin.setPosition(tileSize * j - offsetX, tileSize * i - offsetY);
                        window.draw(coin);
                        continue;
                    };
                    if (TileMap[i][j] == 'B') tile.setTextureRect(IntRect(0, 0, tileSize, tileSize));
                    if (TileMap[i][j] == ' ') tile.setTextureRect(IntRect(34 * 12, 34 * 6, tileSize, tileSize));
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
                    }   // ������� �����
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
                    }   // ����� �����
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
                    }   // ������
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
                    }
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
                    }   
                    tile.setPosition(j * tileSize - offsetX, i * tileSize - offsetY);
                    window.draw(tile);
                }

            }
            window.draw(man.sprite);  //������ ������
            window.display();//������� ������ �� �����

        }


    }

    return 0;
}