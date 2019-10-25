#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <Entity.hpp>
#include <Inventory.hpp>

class Player : public Entity
{
    private:  // -------------------------------------------------------- private variables.
        Inventory* m_inventory = nullptr;

        // Open inventory.
        bool m_openInvetory = false;

    public:  // -------------------------------------------------------- public variables.
        // Ctor.
        Player(const uint8_t stepSize, const sf::Vector2f* initPos);

        // Dtor.
        ~Player();

        // Update.
        void update() override;

        // Draw.
        void draw(sf::RenderWindow& window) const override;

        // Set code to an entity.
        void setCode(Entity& ent);
};

#endif