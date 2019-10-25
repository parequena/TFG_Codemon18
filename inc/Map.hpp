#ifndef MAP_HPP
#define MAP_HPP

#include <Player.hpp>
#include <Entities.hpp>

class Map : public GameObject
{
    private:  // -------------------------------------------------------- private variables.
        // Interrupt out.
        bool m_killed = false;

        // Objects (Props, entities).
        std::vector<GameObject*> m_objs;

        // Killing entities.
        std::vector<uint8_t> m_killingEnts;

        // Trigger idxs.
        std::vector<uint8_t> m_triggerIdx;

        // Enabled triggers.
        std::vector<bool> m_enabledTriggers;

        // tileRows, tileCols.
        uint8_t m_tileRows, m_tileCols;

        // tileSizeOnMap.
        float m_tileSizeOnMap;

        // tileMap.
        std::vector<uint8_t> m_tileMap;

        // Map Texture.
        sf::Texture* m_texture;

        // tileSet.
        std::vector<sf::Sprite*> m_tileSet;

        // endCoords.
        uint16_t m_endCoord[4];

        // End map drawinh.
        sf::RectangleShape* m_endDraw;

        // Spawn coordinates.
        sf::Vector2f* m_spawnCoord;

        // Enables code givings.
        bool m_enableCode = false;

        // Controlls how many entities.
        uint8_t m_nEnts = 0;

        // Boolean entity collision.
        std::vector<bool> m_collisions;

        // Saving falses.
        std::vector<bool> m_falses;

        // Checks if we can end the map.
        bool m_end = false;

        // CheckEndMap.
        bool checkEndMap(const GameObject& go) const;

        // Find type.
        static uint8_t findType(const std::string& type);

    private:  // -------------------------------------------------------- private functions.
        // Load sprites from folder.
        void loadSprites(std::string& folder);

        // void setCode.
        void setCode(const GameObject& go1, GameObject& go2);

        // touched.
        bool touched(const GameObject& go);

        // Update.
        void update() override;

        // Draw.
        void draw(sf::RenderWindow& window) const override;

    public:     // -------------------------------------------------------- public functions.        .
        // Ctor.
        Map(const Json::Value& map);

        // Copy Ctor.
        Map(const Map& m);

        // operator=.
        Map& operator&=(Map m);

        // Dtor.
        ~Map();

        // Play the map.
        float play(sf::RenderWindow& window);

        // Create bullet.
        bool createBullet(ShootingEntity* parent, int x, int y, Moves direction);

        // Delete bullet.
        bool deleteBullet(int id);
};

#endif