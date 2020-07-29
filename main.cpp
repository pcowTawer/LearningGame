#include <SFML/Graphics.hpp>
using namespace sf;// Так как мы работаем с библиотекой sfml, нам будет легче не обращаться каждый раз к 
                   //определенному классу, так что используем пространство имен sf

const int H = 11;   //Высота карты
const int W = 40;   //Ширина карты
const int tileSize = 32;
float offsetX = 0, offsetY = 0; //Переменные отвечающие за смещение камеры

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



class Player // Класс игрока
{
    FloatRect rect;                             // Прямоугольник, задающий позицию картинки sprite
    float frameChangeVelosity = 0.00001;        // Скорость смены кадра анимации персонажа
    float currentFrame;                         // Переменная считающая кадр который будет в анимации персонажа
    enum Direction { LEFT, RIGHT, UP, DOWN };   // Перечисление типа Направление
    Direction lastLookDir;                      // Куда последний раз смотрел персонаж

    void Colision(Direction axisDir)            // Функция обработки коллизий, для разных координатных осей, 
                                                    // параметр принимает значение направления оси координат, и
                                                // В зависимости от нее, считает коллизию либо по OX, либо OY
    {

        onGround = false;                       // Это нужно, для того, чтобы коллизия не сохраняла значение onGround,
                                                // а динамически изменяла, т.к если onGround cтанет true, и dy = 0
                                                // значение onGround так и останется true, тк он менялся бы, только если
                                                // бы мы прыгнули
                                                // Такой случай означает, что мы двигались с платформы(или возвышенности)
                                                // и сошли с нее, но не падаем


        // Проверка соприкосновения с картой, построенной на плиточной системе, или на Тайлах
        for (int i = rect.top / tileSize; i < (rect.top + rect.height) / tileSize; i++)         // Благодаря такой организации for, мы проверяем лишь
            for (int j = rect.left / tileSize; j < (rect.left + rect.width) / tileSize; j++)    // те плитки карты, на которые попал sprite 
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
    float dx, dy;                                    // Скорость по двум осям          
    bool onGround;                                   // Ответ на вопрос: На земле ли мы?
    Sprite sprite;                                   // Sprite, образованный из текстуры и прямоугольника,\
                                                        который как бы высекает из текстуры определенную картинку
    

    Player(Texture& image)                           //Класс будет принимать параметром текстуру
    {
        sprite.setTexture(image);                    //Установит эту текстуру, как текстуру спрайта
        sprite.setTextureRect(IntRect(0, 0, 32, 32));// И вырежет из нее определенную часть

        rect = FloatRect(32, (H-2)*32, 32, 32);      // Здесь я указываю координаты изначального\
                                                        спавна первыми 2-мя параметрами
        

        lastLookDir = Direction::RIGHT;             //Изначальное направление взгляда
        onGround = false;
        dx = dy = 0;
        currentFrame = 0;
    }

    void update(float time)
    {

        rect.left += dx * time;                     // rect.left - позиция sprite в окне по оси ox
        Colision(Direction::LEFT);                  // Важно! Как только мы изменяем позицию персонажа
                                                    // по одной из осей
                                                    // Мы должжны сразу проверить Colision, по этой оси
                                                    // Иначе, если мы изменим позицию персонажа по всем осям, он
                                                    // Может попасть в угол, и тогда, функция может не сработать
                                                    

        if (!onGround) dy = dy + 0.000000001 * time; // Ускоряемся со временем по направлению к земле 
                                                     // Положительный dx - скорость направо, положительный dy - cкорость вниз

        rect.top += dy * time;                    //Почему мы просто не изменяем позицию, на какое-то значение?
                                                 //, а пользуемся формулой перемещения из физики?
                                                // Потому что, изменяя положение таким образом, наш песонаж будет двигаться
                                               // всегда с одной скоростью на любом компе!
                                              // Раз скорость обработки компьютеров разная, то
                                             // и значение в коде будет прибавляться с разной скоростью, НО
                                            // Раз мы изпользуем время между тактами процессора, то мы избавляемся от 
                                           // зависимости нашего кода перемещения от компьютера!
                                               // rect.top - позиция sprite в окне по оси oy
        Colision(Direction::DOWN);

        currentFrame += frameChangeVelosity * time; // Двигаясь по спрайт листу мы меняем кадры в зависимости от скорости персонажа
        if (currentFrame > 8) currentFrame -= 8;

        if (dx == 0)                                // В зависимости от направления скорости по OX изменяем анимацию         
            switch (lastLookDir) 
            {                                                       
                case Direction::LEFT:  sprite.setTextureRect(IntRect(32, 0, -32, 32)); break; 
                case Direction::RIGHT: sprite.setTextureRect(IntRect(0, 0, 32, 32)); break;
                    // Немного об IntRect
                    // 1 и 2 параметр - X и Y координаты ЛЕВОГО верхнего угла, с которого начнется считывание пикселей текстуры
                    // 3 и 4 параметр - Насколько далеко, и в каком направлении пиксели будут считываться по OX(3 параметр) и OY(4 параметр)
                    // 3 параметр положительный - Вправо, отрицательный Влево
                    // 4 параметр положительный - Вниз, отрицательный Вверх

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
    Clock timer;    //Этот таймер будет считать время, между тактами работы процессора
    RectangleShape rectangle(Vector2f(32, 32));

    while (window.isOpen()) 
    {
        float time = timer.getElapsedTime().asMicroseconds();
        timer.restart();                                    
        Event event;

        if (window.pollEvent(event))       // window.pollEvent(event) ожидает событие 
        {            
            if (event.type == Event::Closed) window.close();
        }


        if (window.hasFocus())
        {
            if (Keyboard::isKeyPressed(Keyboard::Left)) // В зависимости от нажатой кнопки мы сообщаем скорость персонажу
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

            window.clear();  //Очищаем окно


            for (int i = 0; i < H; i++) //Здесь обновляется карта
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
            window.draw(man.sprite);  //Рисуем спрайт
            window.display();//Выводим спрайт на экран

        }

        
    }

    return 0;
}