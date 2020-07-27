#include <SFML/Graphics.hpp>
using namespace sf;

int main()
{
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!"); //�������� ����
    Texture t;                                                       //�������������� ��������
    t.loadFromFile("man.png");                                       //��������� �������� �� �����

    Sprite s;                                       // �������������� ������ = ����������� �������� ������ �\
                                                        ������������ ����� ��������
    s.setTexture(t);                                // ������������� �������� �������
    s.setTextureRect(IntRect(0, 0, 32, 32));        // ������������� Rect - ������������� ����� ��������
    s.setPosition(0, 0);                            // ������������� ������ � ������������ ����� � ����

    int lastKeyPressed = Keyboard::Right;// ��������� ������� �������
    float currentFrame = 0;              // ���������� ��������� ���� ������� ����� � ��������. 
    float moveVelosity = 0.0001;         // �������� �������� ���������
    float spriteFrameVelosity = 0.00001; // �������� ����� �����
    Clock clock;                         // ���������� ������ ����, ��������� �����
    
    
    while (window.isOpen()) 
    {
        float time = clock.getElapsedTime().asMicroseconds();//
        clock.restart();
        Event event;
        while (window.pollEvent(event))       // ��� ������� ������� ������� ���� �������� 
        {                                    //� ��������� ���� ���� ��� ���������
            if (event.type == Event::Closed)
                window.close();
        }
        if (Keyboard::isKeyPressed(Keyboard::Left)) 
        {
            lastKeyPressed = Keyboard::Left;

            s.move(-moveVelosity * time, 0); //������� ��������� �����, � ������������ ���������
                                             //Note: ��� �������� ��������� �� ���� �����������, �.� ������� ��
                                             // �� �������� ����������, � �� �������. 
                                             // �� ���� �� �� �������� ������ ������� c ���������� moveVelocity, ��
                                             // ������� ����������� ��, � ����� ���������, � ����� �������� ���� while
                                             // �������� �������� ������� ������ �� �������������� ��

            currentFrame += spriteFrameVelosity * time; //���-�� �������� sprite ���������, � �����������
            if (currentFrame > 8) currentFrame -= 8;    // �� ���������� currentFrame, ������� ���������� ������� Rect.
            s.setTextureRect(IntRect(32 + 32 * int(currentFrame), 32, -32, 32));
        }
        if (Keyboard::isKeyPressed(Keyboard::Right))
        {
            lastKeyPressed = Keyboard::Right;

            s.move(moveVelosity * time, 0);

            currentFrame += spriteFrameVelosity * time;
            if (currentFrame > 8) currentFrame -= 8;
            s.setTextureRect(IntRect(32 * int(currentFrame), 32, 32, 32));


        }
        if (Keyboard::isKeyPressed(Keyboard::Down))
        {
            s.move(0, moveVelosity * time);
        }
        if (Keyboard::isKeyPressed(Keyboard::Up))
        {
            s.move(0, -moveVelosity * time);
        }
        window.clear();
        window.draw(s);
        window.display();
        if (!
            (
                Keyboard::isKeyPressed(Keyboard::Left) ||
                Keyboard::isKeyPressed(Keyboard::Right) ||
                Keyboard::isKeyPressed(Keyboard::Up) ||
                Keyboard::isKeyPressed(Keyboard::Down)
                )
            )
        {
            currentFrame = 0;
            if (lastKeyPressed == Keyboard::Left)
                s.setTextureRect(IntRect(32, 0, -32, 32));
            else
                s.setTextureRect(IntRect(0, 0, 32, 32));
        }
        window.clear();  //������� ����
        window.draw(s);  //������ ������
        window.display();//������� ������ �� �����
    }

    return 0;
}