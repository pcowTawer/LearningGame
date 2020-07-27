#include <SFML/Graphics.hpp>
using namespace sf;

int main()
{
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!"); //Рендерим окно
    Texture t;                                                       //Инициализируем текстуру
    t.loadFromFile("man.png");                                       //Загружаем текстуру из файла

    Sprite s;                                       // Инициализируем спрайт = загруженной текстуре взятой в\
                                                        определенном месте картинки
    s.setTexture(t);                                // Устанавливаем текстуру спрайта
    s.setTextureRect(IntRect(0, 0, 32, 32));        // Устанавливаем Rect - прямоугольную часть текстуры
    s.setPosition(0, 0);                            // Устанавливаем спрайт в определенное место в окне

    int lastKeyPressed = Keyboard::Right;// Последняя нажатая клавиша
    float currentFrame = 0;              // Переменная считающая кадр который будет в анимации. 
    float moveVelosity = 0.0001;         // Скорость движения персонажа
    float spriteFrameVelosity = 0.00001; // Скорость смены кадра
    Clock clock;                         // Переменная класса часы, считающее время
    
    
    while (window.isOpen()) 
    {
        float time = clock.getElapsedTime().asMicroseconds();//
        clock.restart();
        Event event;
        while (window.pollEvent(event))       // Эта функция ожидает событие типа Закрытие 
        {                                    //и закрывает окно если оно произошло
            if (event.type == Event::Closed)
                window.close();
        }
        if (Keyboard::isKeyPressed(Keyboard::Left)) 
        {
            lastKeyPressed = Keyboard::Left;

            s.move(-moveVelosity * time, 0); //Двигаем персонажа влево, с определенной скоростью
                                             //Note: эта скорость одинакова на всех компьютерах, т.к зависит не
                                             // от скорости компьютера, а от времени. 
                                             // Те если бы мы оставили просто функцию c параметром moveVelocity, то
                                             // функция срабатывала бы, с такой скоростью, с какой работает цикл while
                                             // скорость которого зависит только от быстродействия ПК

            currentFrame += spriteFrameVelosity * time; //Так-же изменяем sprite персонажа, в зависимости
            if (currentFrame > 8) currentFrame -= 8;    // от переменной currentFrame, простым изменением позиции Rect.
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
        window.clear();  //Очищаем окно
        window.draw(s);  //Рисуем спрайт
        window.display();//Выводим спрайт на экран
    }

    return 0;
}