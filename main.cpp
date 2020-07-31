#include <SFML/Graphics.hpp>
using namespace sf;// Так как мы работаем с библиотекой sfml, нам будет легче не обращаться каждый раз к 
                   //определенному классу, так что используем пространство имен sf

const int tileSize = 16;
const int characterSizeX = 32;
const int characterSizeY = 32;

double offsetX = 0, offsetY = 0; //Переменные отвечающие за смещение камеры

std::string TileMap[] =                             //Карта, закодированная символами
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

const int H = sizeof(TileMap)/sizeof(TileMap[0]);           //Высота карты
const int W = sizeof(TileMap[0]) / sizeof(TileMap[0][0]);   //Ширина карты

class Player // Класс игрока
{
    FloatRect rect;                             // Прямоугольник, задающий позицию картинки sprite
    double frameChangeVelosity = 0.00001;        // Скорость смены кадра анимации персонажа
    double currentFrame;                         // Переменная считающая кадр который будет в анимации персонажа
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
    double dx, dy;                                    // Скорость по двум осям          
    bool onGround;                                   // Ответ на вопрос: На земле ли мы?
    Sprite sprite;                                   // Sprite, образованный из текстуры и прямоугольника,\
                                                        который как бы высекает из текстуры определенную картинку
    

    Player(Texture& image)                           //Класс будет принимать параметром текстуру
    {
        sprite.setTexture(image);                                           //Установит эту текстуру, как текстуру спрайта
        sprite.setTextureRect(IntRect(0, 0, characterSizeX, characterSizeY));// И вырежет из нее определенную часть

        rect = FloatRect(32, (H-9)*32, 32, 32);      // Здесь я указываю координаты изначального\
                                                        спавна первыми 2-мя параметрами
        

        lastLookDir = Direction::RIGHT;             //Изначальное направление взгляда
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
        if (currentFrame > 4) currentFrame -= 4;

        if (dx == 0)                                // В зависимости от направления скорости по OX изменяем анимацию         
            switch (lastLookDir) 
            {                                                       
                case Direction::LEFT:  sprite.setTextureRect(IntRect(characterSizeX , characterSizeY , -characterSizeX, characterSizeY)); break;
                case Direction::RIGHT: sprite.setTextureRect(IntRect(characterSizeX , characterSizeY , characterSizeX, characterSizeY)); break;
                    // Немного об IntRect
                    // 1 и 2 параметр - X и Y координаты ЛЕВОГО верхнего угла, с которого начнется считывание пикселей текстуры
                    // 3 и 4 параметр - Насколько далеко, и в каком направлении пиксели будут считываться по OX(3 параметр) и OY(4 параметр)
                    // 3 параметр положительный - Вправо, отрицательный Влево
                    // 4 параметр положительный - Вниз, отрицательный Вверх

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


    Texture charactersT;                    //Установка текстур
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
    Clock timer;    //Этот таймер будет считать время, между тактами работы процессора




    while (window.isOpen())
    {
        double time = timer.getElapsedTime().asMicroseconds();
        Event event;
        timer.restart();
        if (window.pollEvent(event))       // window.pollEvent(event) ожидает событие 
        {
            if (event.type == Event::Closed) window.close();
        }

        if (window.hasFocus())
        {
            timer.restart();

            //Обработка нажатий 
            {
                if (Keyboard::isKeyPressed(Keyboard::Left)) // В зависимости от нажатой кнопки мы сообщаем скорость персонажу
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

            window.clear();  //Очищаем окно

            //Здесь обновляется карта
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
                    }   // Большая трава
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
                    }   // Малая трава
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
                    }   // Облако
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
            window.draw(man.sprite);  //Рисуем спрайт
            window.display();//Выводим спрайт на экран

        }


    }

    return 0;
}