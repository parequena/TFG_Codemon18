// Prototipo de mapChange
// g++ -o map main.cpp -std=c++1z -lsfml-graphics -lsfml-window -lsfml-system
// ./map

// SPRITES.push_back({tex, sf::IntRect(r*factor, c*factor, factor, factor)});

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

#define theScale 4
#define factor 32
#define df_Map theScale*factor

struct Entidad
{
    int x = 0, y = 0;
    sf::Sprite sprite;

    Entidad(int x, int y, const sf::Sprite& sp)
    {
        this->x = x;
        this->y = y;
        sprite = sp;
    }

    void draw(sf::RenderWindow& w)
    {
        sprite.setPosition(y, x);
        w.draw(sprite);
    }
};

int main()
{
    std::vector<std::vector<uint8_t>> map = {
        {0, 1, 1, 2},
        {8, 9, 9, 10},
        {16, 17, 17, 18},
        {25, 25, 25, 25}
    };      

    sf::RenderWindow window(sf::VideoMode(df_Map, df_Map), "SFML works!");
    sf::Texture tex;
    std::vector<sf::Sprite> sps;
    sf::Sprite aux;
    std::vector<Entidad> entities;

    tex.loadFromFile("../../assets/tiles/prueba.png");

    for(int i = 0; i < tex.getSize().y; i+=factor)
        for(int j = 0; j < tex.getSize().x; j+=factor)
            sps.push_back({tex, sf::IntRect(j, i, factor, factor)});

    Entidad e(75, 15, sps[56]);
    sf::Sprite ent2 = sps[56];
    ent2.scale(-1, 1);
    Entidad e2(75, 80, ent2);
    entities.push_back(e);
    entities.push_back(e2);

    window.clear(sf::Color::White);
    window.display();

    while(window.isOpen())
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            window.close();

        // Draw Map.
        for(int i = 0; i < theScale; ++i)
        {
            for(int j = 0; j < theScale; ++j)
            {
                sf::Sprite sp = sps[map[i][j]];
                sp.setPosition(j*factor, i*factor);
                window.draw(sp);
            }
        }
        // End Draw Map.

        // Draw Entities.
        for(auto e: entities)
            e.draw(window);
        // End Draw Entities.

        window.display();
    }
}