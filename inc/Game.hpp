#ifndef GAME_HPP
#define GAME_HPP

#include <MainMenu.hpp>
#include <MapMenu.hpp>

class Game
{
    private:  // -------------------------------------------------------- private variables.
        // Window.
        sf::RenderWindow m_window;

        // Controll the game loop;.
        bool m_play = true;

        // Main Menu.
        MainMenu* m_mainMenu = nullptr;

        // Map menu.
        MapMenu* m_mapMenu = nullptr;

        // Playing Map.
        Map* m_map = nullptr;

        // Config file.
        Json::Value m_cfg;


    private:  // -------------------------------------------------------- private functions.
        // Update.
        void update();

        // Draw.
        void draw();
    public:  // -------------------------------------------------------- public functions.
        // Ctor.
        Game();
        
        // Dtor.
        ~Game();

        // Play.
        void play();
};

#endif