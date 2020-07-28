#include <SFML/Graphics.hpp>
using namespace sf;// ��� ��� �� �������� � ����������� sfml, ��� ����� ����� �� ���������� ������ ��� � 
                   //������������� ������, ��� ��� ���������� ������������ ���� sf

int ground = 150;  // ������ �� ������� ����������� �����

class Player // ����� ������
{
    FloatRect rect;                         // �������������, �������� ������� �������� sprite
    float frameChangeVelosity = 0.00001;    // �������� ����� �����
    float currentFrame;                     // ���������� ��������� ���� ������� ����� � ��������
    enum MoveDirection { LEFT, RIGHT, UP, DOWN };
    MoveDirection lastDir;
public: 
    float dx, dy;                           // �������� �� ���� ����          
    bool onGround;                          // ����� �� ������: �� ����� �� ��?
    Sprite sprite;                          // ������
    

    Player(Texture& image)                  //����� ����� ��������� ���������� ������
    {
        sprite.setTexture(image);   
        sprite.setTextureRect(IntRect(0, 0, 32, 32));
        rect = FloatRect(0, 0, 32, 32);    
        

        lastDir = MoveDirection::RIGHT;
        onGround = false;
        dx = dy = 0;
        currentFrame = 0;
    }

    void update(float time)
    {
        rect.left += dx * time;                     // rect.left - ������� sprite � ���� �� ��� ox
        if (!onGround) dy = dy + 0.000000002 * time; 

        rect.top += dy * time;                      // rect.top - ������� sprite � ���� �� ��� oy

        if (rect.top > ground) { rect.top = ground; dy = 0; onGround = true; }

        currentFrame += frameChangeVelosity * time; // �������� �� ������ ����� �� ������ ����� � ����������� �� ��������
        if (currentFrame > 8) currentFrame -= 8;

        if (dx == 0)                                // � ����������� �� �������� �������� ��������         
            switch (lastDir) 
            { 
                case MoveDirection::LEFT:  sprite.setTextureRect(IntRect(32, 0, -32, 32)); break;
                case MoveDirection::RIGHT: sprite.setTextureRect(IntRect(0, 0, 32, 32)); break;
            }
        if (dx > 0) 
        {
            sprite.setTextureRect(IntRect(32 * int(currentFrame), 32, 32, 32));
            lastDir = MoveDirection::RIGHT;
        }
        if (dx < 0)
        {
            sprite.setTextureRect(IntRect(32 + 32 * int(currentFrame), 32, -32, 32));
            lastDir = MoveDirection::LEFT;;
        }

        sprite.setPosition(rect.left, rect.top);

        dx = 0;
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    Texture t;
    t.loadFromFile("man.png");

    Player man(t);

    Clock clock;
    
    while (window.isOpen()) 
    {
        float time = clock.getElapsedTime().asMicroseconds();//
        clock.restart();
        Event event;
        if (window.pollEvent(event))       // window.pollEvent(event) ������� ������� ���� �������� � ��������� ���� ���� ��� ���������
        {                                    
            if (event.type == Event::Closed)
                window.close();
        }
        if (Keyboard::isKeyPressed(Keyboard::Left)) 
        {
            man.dx += -0.0002;
        }
        if (Keyboard::isKeyPressed(Keyboard::Right))
        {
            man.dx += 0.0002;
        }

        if (Keyboard::isKeyPressed(Keyboard::Up))
        {
            if (man.onGround) { man.dy = -0.0005; man.onGround = false; }
        }
        man.update(time);
        window.clear();  //������� ����
        window.draw(man.sprite);  //������ ������
        window.display();//������� ������ �� �����
    }

    return 0;
}