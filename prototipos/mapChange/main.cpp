// Prototipo de mapChange
// g++ -o map main.cpp -std=c++1z -lsfml-graphics -lsfml-window -lsfml-system -ljsoncpp
// g++ -o map main.cpp Collision.o -std=c++1z -lsfml-graphics -lsfml-window -lsfml-system -ljsoncpp
// ./map

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "Collision.h"

const int minKBmap = sf::Keyboard::Num0;
#define df_A 200
bool currentMap[df_A][df_A];

class Entidad
{
        int _initX, _initY, _x, _y;
        sf::Sprite shape;
        sf::Text _text;
        sf::Font _f;

    public:
        inline sf::Sprite getSprite() { return shape; };
        Entidad(int x, int y, const float& size, const sf::Texture& t, const std::string& s)
        {
            _f.loadFromFile("../../assets/fonts/PkmnXandY.ttf");
            _initX = _x = x;
            _initY = _y = y;
            shape.setTexture(t);
            shape.setScale(size, size);
            shape.setPosition(x, y);
            _text.setFont(_f);
            _text.setFillColor(sf::Color::Black);
            _text.setPosition(220, 5);
            _text.setString(s);
        }

        bool operator!=(const Entidad& e)
        {
            return  _initX != e._initX && _initY != e._initY && _x != e._x && _y != e._y;
        }

        inline void dibujar(sf::RenderWindow& v)
        {
            v.draw(shape);
        };

        void update(int x, int y)
        {
            // Function to move.
            _x += x;
            _y += y;

            if(_x < 0  ) _x = 0;
            if(_x > df_A) _x = df_A;
            if(_y < 0  ) _y = 0;
            if(_y > df_A) _y = df_A;

            shape.setPosition(_x, _y);
        }

        bool meTocan(const Entidad& e)
        {
            return Collision::BoundingBoxTest(shape, e.shape);
        }

        void displayMsg(sf::RenderWindow& w)
        {
            w.draw(_text);
        }
};

class Personaje : public Entidad
{
        int _initX, _initY, _x, _y;
        sf::Sprite shape;

    public:
        Personaje(int x, int y, const float& size, const sf::Texture& t):
            Entidad(x, y, size, t, "")
        { }
};

class Mapa
{
        int _id;
        std::vector<Entidad> ents;
        sf::RectangleShape rect;
        
    public:
        Mapa(uint8_t id, const sf::Color& c)
        {
            _id = id;
            rect.setPosition(0, 0); // Todo el mapa.
            rect.setSize(sf::Vector2f(df_A, df_A));
            rect.setFillColor(c);
        }

        void dibujar(sf::RenderWindow& v)
        {
            // Dibujamos el mapa.
            v.draw(rect);
            // Dibujamos las entidades.

            for(auto e: ents)
                e.dibujar(v);
        }

        inline uint8_t id() { return _id; };
        inline void addEntidad(const Entidad& e) { ents.push_back(e); };
        void checkCollitions(const Personaje& pj, sf::RenderWindow& w)
        {
            bool b = false;
            for(auto e: ents)
            {
                b = e.meTocan(pj);
                if(b) e.displayMsg(w);
                std::cout << b << " ";
            }
            std::cout << "\n";
        }
        /*void updateEntities(int ent, int &x, int &y)
        {
            if(ent < 0) ent = 0;
            if(ent > ents.size()) ent = ents.size();
            bool colision = false;

            for(auto &e: ents)
            {
                if(!colision && ents[ent] != e) 
                    colision = Collision::PixelPerfectTest(ents[ent].getSprite(), e.getSprite());
            }

            for(size_t i = 0; i < ent && !colision; i++)
                colision = Collision::BoundingBoxTest(ents[ent].getSprite(), ents[i].getSprite());

            for(size_t i = ent + 1; i < ents.size() && !colision; i++)
                colision = Collision::BoundingBoxTest(ents[ent].getSprite(), ents[i].getSprite());

            if(!colision)
                ents[ent].update(x, y);
            else
            {
                x = 0;
                y = 0;
            }
        }*/
};

void cargaMapa0(std::vector<Mapa> &v)
{
    sf::Texture t;
    Collision::CreateTextureAndBitmask(t, "../../assets/tiles/white.png");
    Entidad e1(5, 5, 1.0f, t, "11");
    Collision::CreateTextureAndBitmask(t, "../../assets/tiles/white.png");
    Entidad e2(180, 180, 1.0f, t, "12");
    Collision::CreateTextureAndBitmask(t, "../../assets/tiles/white.png");
    Entidad e3(10, 160, 0.5f, t, "13");

    Mapa m1(1, sf::Color::Black);
    m1.addEntidad(e1);
    m1.addEntidad(e2);
    m1.addEntidad(e3);

    v.push_back(m1);
}

void cargaMapa1(std::vector<Mapa> &v)
{
    sf::Texture t;
    Collision::CreateTextureAndBitmask(t, "../../assets/tiles/grey.png");
    Entidad e1(0, 0,  1.0f, t, "21");
    Collision::CreateTextureAndBitmask(t, "../../assets/tiles/grey.png");
    Entidad e2(100, 0, 1.0f, t, "22");

    Mapa m1(2, sf::Color::Blue);
    m1.addEntidad(e1);
    m1.addEntidad(e2);

    v.push_back(m1);
}

void moveDirection(int &m, int i)
{
    m += i;

    if(m < -1) m = -1;
    if(m > 1) m = 1;
}

int changeMap(int actualMap, int newMap, int& movX, int& movY)
{
    // std::cout << "ac: " << actualMap << "\tn: " << newMap << "\tX: " << movX << "\tY: " << movY << "\n";
    if(actualMap == newMap - minKBmap) return actualMap;

    // Si queremos cambiar de mapa.
    movX = 0;
    movY = 0;

    for(int i = 0; i < df_A; ++i)
        for(int j = 0; j < df_A; ++j)
            currentMap[i][j] = false;

    return newMap - minKBmap;
}

void drawCurrentMap()
{
    std::cout << std::string(50, '\n');
    for(int i = 0; i < df_A; ++i)
    {
        for(int j = 0; j < df_A; ++j)
            std::cout << currentMap[i][j] << " ";
        std::cout << "\n";
    }
}

int main()
{
    // Actua como el juego.
    std::vector<Mapa> mapas;
    int mapaActual = minKBmap, entActual = 0, movX = 0, movY = 0, newMap = minKBmap;

    cargaMapa0(mapas);
    cargaMapa1(mapas);

    sf::RenderWindow window(sf::VideoMode(df_A + 200, df_A), "SFML works!");
	window.setFramerateLimit(30); // Limite FPS,

    sf::Font FUENTE;
    FUENTE.loadFromFile("../../assets/fonts/PkmnXandY.ttf");
    sf::Text TEXTO("HOLA Pablo\nTE QUIERO", FUENTE);
    TEXTO.setFillColor(sf::Color::Blue);
    TEXTO.setPosition(220, 5);

    sf::Texture t;
    Collision::CreateTextureAndBitmask(t, "../../assets/tiles/red.png");
    Personaje pj(100, 100, 1.f, t);


    while (window.isOpen())
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            window.close();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            moveDirection(movY, -1);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            moveDirection(movY, 1);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            moveDirection(movX, -1);

        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            moveDirection(movX, 1);
        

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num0))
            newMap = sf::Keyboard::Num0; //  - minKBmap;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
            newMap = sf::Keyboard::Num1; //  - minKBmap;
        
        mapaActual = changeMap(mapaActual, newMap, movX, movY);
        
        /*if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad0))
            entActual = sf::Keyboard::Numpad0 - sf::Keyboard::Numpad0;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad1))
            entActual = sf::Keyboard::Numpad1 - sf::Keyboard::Numpad0;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2))
            entActual = sf::Keyboard::Numpad2 - sf::Keyboard::Numpad0;*/

        //std::cout << "Map: " << mapaActual << "\tEnt: " << entActual << "\tX: " << movX << "\tY: " << movY << "\n";
        //drawCurrentMap();

        window.clear(sf::Color::White);
        // mapas[mapaActual].updateEntities(entActual, movY, movX);
        pj.update(movY, movX);
        mapas[mapaActual].dibujar(window);
        // window.draw(TEXTO);
        mapas[mapaActual].checkCollitions(pj, window);
        pj.dibujar(window);
        window.display();
    }

    return 0;
}