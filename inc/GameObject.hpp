#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#define df_gameName     "Codemon"
#define df_fps          30
#define df_headerSize   0
#define df_gameHeight   df_headerSize + 480
#define df_gameWidth    640
#define df_gameConfig   "./assets/game.json"

#include <SFML/Graphics.hpp>
#include <json/json.h>
#include <iostream>
#include <cstdint>
#include <vector>
#include <filesystem>
#include <fstream>
#include "Collision.h"
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <cstring>

class World;

enum class Moves {IDLE, RIGHT, RIGHT_UP, UP, LEFT_UP, LEFT, LEFT_DOWN, DOWN, RIGHT_DOWN, nMoves};
static const std::string MovesSTR[] = {"IDLE", "RIGHT", "RIGHT_UP", "UP", "LEFT_UP", "LEFT", "LEFT_DOWN", "DOWN", "RIGHT_DOWN"};

class GameObject
{
    protected:  // -------------------------------------------------------- protected variables.
        // GameObject X.
        float m_x = 0;

        // GameObject Y.
        float m_y = df_headerSize;

        // GameObject H.
        float m_h = df_gameHeight;

        // GameObject W.
        float m_w = df_gameWidth;

        // Step size.
        uint8_t m_stepSize = 0;

        // Game font.
        sf::Font* m_font = nullptr;

        //
        bool m_doesAction = false;

        // Code from execute.
        std::string m_code = "";

        // GameObject ID.
        int m_gObjId = -1;

        // type.
        std::string m_type = "O";

        // Check position.
        void checkPosition();

        // Move to a position.
        void move(const int x, const int y);

        // Ctor.
        GameObject();

    public:     // -------------------------------------------------------- public functions.
        // World.
        World* m_world = nullptr;

        // Sprite.
        sf::Sprite* m_sprite = nullptr;

        // Code folder.
        static std::string m_codeFolder;

        // Copy Ctor.
        GameObject(const GameObject& go);

        // Param Ctor without sprite, font and world.
        GameObject(uint8_t id, const std::string& type, const std::string& code, bool doesAction,
                    float x, float y, float w, float h, uint8_t stepSize);

        // Param Ctor without sprite and font.
        GameObject(uint8_t id, const std::string& type, const std::string& code, bool doesAction,
                    float x, float y, float w, float h, uint8_t stepSize, const World* world);

        // Param Ctor.
        GameObject(uint8_t id, const std::string& type, const std::string& code, bool doesAction,
                    float x, float y, float w, float h, uint8_t stepSize,
                    const World* world, const sf::Font* font, const sf::Sprite* sprite);
        
        // Dtor.
        virtual ~GameObject();

        // Update.
        virtual void update() = 0;

        // Draw.
        virtual void draw(sf::RenderWindow& window) const = 0;

        // Getters.
        inline int getX() const             { return m_x; };
        inline int getY() const             { return m_y; };
        inline int getHeight() const        { return m_h; };
        inline int getWidth() const         { return m_w; };
        inline uint8_t getStep() const      { return m_stepSize; };
        inline bool getAction() const       { return m_doesAction; };
        inline std::string getCode() const  { return m_code; };
        inline int getId() const            { return m_gObjId; };
        inline std::string getType() const  { return m_type; };
        inline sf::Sprite getSprite() const { return *m_sprite; };

        // Setters.
        inline void setAction(const bool b) { m_doesAction = b; };
        inline void setCode(const std::string& s) { m_code = s; };

        // Wait until realease.
        static void waitUntilRelease(const sf::Keyboard::Key key);

        // Controlls collisions.
        virtual bool touched(const GameObject& go) = 0;

        // Set type.
        inline std::string getType() { return m_type; };

        // Does an action caused by an other object.
        virtual void action(const GameObject& go) { (void)go; return; };
};

class World
{
    int m_entityID  = -1;
    int m_entityX   = -1, m_entityY   = -1;
    int m_tileX     = -1, m_tileY     = -1;
    int m_prevTileX = -1, m_prevTileY = -1;
    int m_tileSize  = -1;
    
public:
    //
    World();
    
    // Ctor.
    World(const uint8_t id, const int x, const int y, const int tileSize);
    
    // Copy Ctor.
    World(const World& w);

    // Get tile.
    void getTile(int& x, int& y);
    // Get previous tile.
    void getPrevTile(int& x, int& y);
    // Update.
    void update(const int x, const int y);
    // Print (cout doesn't work).
    void print();
    // Check if two worlds are on the same tile.
    bool onTheSameTile(const World& w);
    // Cout <<.
    friend std::ostream& operator<<(std::ostream& os, const World& w);
};

#endif