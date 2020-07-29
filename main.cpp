#include <SFML/Graphics.hpp>
using namespace sf;// ��� ��� �� �������� � ����������� sfml, ��� ����� ����� �� ���������� ������ ��� � 
                   //������������� ������, ��� ��� ���������� ������������ ���� sf

const int H = 11;   //������ �����
const int W = 40;   //������ �����
const int tileSize = 32;
float offsetX = 0, offsetY = 0; //���������� ���������� �� �������� ������

std::string TileMap[H] = {
    "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
    "B                                B    B",
    "B                                B    B",
    "B                                B    B",
    "B                                B    B",
    "B                            BBBBB    B",
    "B                                B    B",
    "B          000           BB      B    B",
    "B                        BB           B",
    "B                        BB           B",
    "BBBBBBB   BBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
    
};



class Player // ����� ������
{
    FloatRect rect;                             // �������������, �������� ������� �������� sprite
    float frameChangeVelosity = 0.00001;        // �������� ����� ����� �������� ���������
    float currentFrame;                         // ���������� ��������� ���� ������� ����� � �������� ���������
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
                    if (TileMap[i][j] == 'B')
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
                    
                }
            }
        
    }


public: 
    float dx, dy;                                    // �������� �� ���� ����          
    bool onGround;                                   // ����� �� ������: �� ����� �� ��?
    Sprite sprite;                                   // Sprite, ������������ �� �������� � ��������������,\
                                                        ������� ��� �� �������� �� �������� ������������ ��������
    

    Player(Texture& image)                           //����� ����� ��������� ���������� ��������
    {
        sprite.setTexture(image);                    //��������� ��� ��������, ��� �������� �������
        sprite.setTextureRect(IntRect(0, 0, 32, 32));// � ������� �� ��� ������������ �����

        rect = FloatRect(32, (H-2)*32, 32, 32);      // ����� � �������� ���������� ������������\
                                                        ������ ������� 2-�� �����������
        

        lastLookDir = Direction::RIGHT;             //����������� ����������� �������
        onGround = false;
        dx = dy = 0;
        currentFrame = 0;
    }

    void update(float time)
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
        if (currentFrame > 8) currentFrame -= 8;

        if (dx == 0)                                // � ����������� �� ����������� �������� �� OX �������� ��������         
            switch (lastLookDir) 
            {                                                       
                case Direction::LEFT:  sprite.setTextureRect(IntRect(32, 0, -32, 32)); break; 
                case Direction::RIGHT: sprite.setTextureRect(IntRect(0, 0, 32, 32)); break;
                    // ������� �� IntRect
                    // 1 � 2 �������� - X � Y ���������� ������ �������� ����, � �������� �������� ���������� �������� ��������
                    // 3 � 4 �������� - ��������� ������, � � ����� ����������� ������� ����� ����������� �� OX(3 ��������) � OY(4 ��������)
                    // 3 �������� ������������� - ������, ������������� �����
                    // 4 �������� ������������� - ����, ������������� �����

            }
        if (dx > 0) 
        {
            sprite.setTextureRect(IntRect(32 * int(currentFrame), 32, 32, 32));
            lastLookDir = Direction::RIGHT;
        }
        if (dx < 0)
        {
            sprite.setTextureRect(IntRect(32 + 32 * int(currentFrame), 32, -32, 32));
            lastLookDir = Direction::LEFT;;
        }

        if (rect.top > 400) { rect.top = (H - 2) * 32; rect.left = 32; }
        sprite.setPosition(rect.left - offsetX, rect.top - offsetY);

        dx = 0;
        if (rect.left > 300 && rect.left < (W-1)*32-300) offsetX = rect.left - 600/2;
        if (rect.top > 175) offsetY = rect.top  - 175;
    }
};



int main()
{
    bool focus = 1;
    sf::RenderWindow window(sf::VideoMode(600, 350), "SFML works!");

    Texture t;
    t.loadFromFile("man.png");
    Player man(t);
    Clock timer;    //���� ������ ����� ������� �����, ����� ������� ������ ����������
    RectangleShape rectangle(Vector2f(32, 32));

    while (window.isOpen()) 
    {
        float time = timer.getElapsedTime().asMicroseconds();
        timer.restart();                                    
        Event event;

        if (window.pollEvent(event))       // window.pollEvent(event) ������� ������� 
        {            
            if (event.type == Event::Closed) window.close();
        }


        if (window.hasFocus())
        {
            if (Keyboard::isKeyPressed(Keyboard::Left)) // � ����������� �� ������� ������ �� �������� �������� ���������
            {
                man.dx += -0.0002;
            }
            if (Keyboard::isKeyPressed(Keyboard::Right))
            {
                man.dx += 0.0002;
            }
            if (Keyboard::isKeyPressed(Keyboard::Right) && Keyboard::isKeyPressed(Keyboard::Left))
            {
                man.dx = 0;
            }
            if (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::Space))
            {
                if (man.onGround) { man.dy = -0.0005; man.onGround = false; }
            }

            man.update(time);

            window.clear();  //������� ����


            for (int i = 0; i < H; i++) //����� ����������� �����
            {
                for (int j = 0; j < W - 1; j++)
                {
                    if (TileMap[i][j] == 'B')   rectangle.setFillColor(Color::White);
                    if (TileMap[i][j] == '0')   rectangle.setFillColor(Color::Green);
                    if (TileMap[i][j] == ' ')   continue;
                    rectangle.setPosition(j * 32 - offsetX, i * 32 - offsetY);
                    window.draw(rectangle);
                }

            }
            window.draw(man.sprite);  //������ ������
            window.display();//������� ������ �� �����

        }

        
    }

    return 0;
}