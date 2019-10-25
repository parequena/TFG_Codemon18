#ifndef ENTITY_HPP
#define ENTITY_HPP
#include <GameObject.hpp>
#include <queue>

class Entity : public GameObject
{
protected:  // -------------------------------------------------------- protected variables.

    // Clock.
    sf::Clock* m_clock;
    
    // Timer.
    sf::Time* m_time;
    
    // Texture.
    sf::Texture* m_texture = nullptr;

    // Allow movement.
    bool m_allowMove = false;

    // Movement speed.
    float m_speed = 0.0f;

    // Manage pipe.
    std::string managePipe(const std::string& toChild) const;

    // Actions.
    std::queue<std::string> m_actions;
    // std::string m_actions;

    // Check action syntax.
    virtual bool checkSyntax(const std::string& base) const;

    // Tokenize.
    void tokenizeActions(const std::string& code);

    // Ctor.
    Entity();

public:     // -------------------------------------------------------- public functions. .
    // Param Ctor.
    Entity(const uint8_t id, const std::string type, const uint8_t stepSize, const Json::Value& ent);

    // operator=.
    Entity& operator&=(Entity e);

    // Dtor.
    virtual ~Entity();

    // Update.
    virtual void update();
    
    // Draw.
    void draw(sf::RenderWindow& window) const;

    // Controlls collisions.
    bool touched(const GameObject& ent);
};

#endif