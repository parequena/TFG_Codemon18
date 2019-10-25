#ifndef ENTITIES_HPP
#define ENTITIES_HPP
#include <Entity.hpp>

class Map;

/////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// MOVING ENTITY ////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
struct MovingEntity : Entity
{
    // Ctor.
    MovingEntity(const uint8_t stepSize, const uint8_t id, const Json::Value& ent);

    // Update.
    void update() override;

    // Pushing action.
    void action(const GameObject& go) override;

    // Dtor.
    ~MovingEntity();

private:
    // Check action syntax.
    bool checkSyntax(const std::string& base) const override;

    // Bool that controls to add just 1 movement
    bool m_moved = true;
};

/////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// ENTITY TRIGGER ///////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
struct EntityTrigger : Entity
{
    // Ctor without type.
    EntityTrigger(const uint8_t stepSize, const uint8_t id, const uint8_t idActivator, const Json::Value& ent);

    // Update.
    virtual void update() override;

    // Touched.
    virtual bool touched(const GameObject& go) override;

    // Dtor
    virtual ~EntityTrigger();

protected:
    // Ctor.
    EntityTrigger(const uint8_t stepSize, const std::string& type, const uint8_t id, const uint8_t idActivator, const Json::Value& ent);

    // Enabled texture;
    sf::Texture* m_enabledTxt = nullptr;

    // Id that enables the trigger.
    int m_idActivator = -1;
};

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// LR MOVING ENTITY ////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
struct LRMovingEntity : Entity
{
    // Ctor.
    LRMovingEntity(const int stepSize, const uint8_t id, const Json::Value& ent);

    // Update.
    void update() override;

    // Action
    void action(const GameObject& go) override;

    // Dtor.
    ~LRMovingEntity();

private:
    // Controls the current move.
    Moves m_currMove;
    
    // Controls the number of steps.
    uint8_t m_totalSteps, m_curretSteps;

    // Object HP.
    uint8_t m_objHP;

    // Dead texture.
    sf::Texture* m_enabledTxt = nullptr;

    // Dead position.
    sf::Vector2f* m_deadPos = nullptr;
};

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// SHOOTING ENTITY /////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
struct ShootingEntity : Entity
{
    // Ctor.
    ShootingEntity(Map* owner, const uint8_t id, const Json::Value& ent);

    // Update.
    void update() override;

    // Action
    void action(const GameObject& go) override;

    // Get Orientation
    inline Moves getOrientation() const { return m_orientation; };

    // Dtor.
    ~ShootingEntity();

    // Set shooting
    inline void setShooting(bool sh) { m_shooting = sh; };

private:
    // Owner
    Map* m_owner = nullptr;

    // Orientation
    Moves m_orientation;

    // How many shots has this entity.
    uint8_t m_nShots;

    // Bool shooting
    bool m_shooting = false;
};

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// ACTIVATOR ///////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
struct Activator : EntityTrigger
{
    // Ctor.
    Activator(const int stepSize, uint8_t id, const Json::Value& ent);

    // Dtor
    ~Activator();
};

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// BULLET //////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
struct Bullet : GameObject
{
    // Ctor.
    Bullet(ShootingEntity* parent, Map* owner, int x, int y, const Moves direction);

    // Touched.
    bool touched(const GameObject& go) override;

    // Update
    void update() override;

    // draw
    void draw(sf::RenderWindow& wind) const override;

    // Dtor.
    ~Bullet();

private:
    // Texture
    sf::Texture* m_texture = nullptr;

    // Orientation
    Moves m_orientation;

    // Owner
    Map* m_owner = nullptr;

    // Parent
    ShootingEntity* m_parent = nullptr;
};

#endif