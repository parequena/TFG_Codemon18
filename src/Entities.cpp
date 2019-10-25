#include <Entities.hpp>
#include <Map.hpp>

/////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// MOVING ENTITY ////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
// Ctor.
MovingEntity::MovingEntity(const uint8_t stepSize, const uint8_t id, const Json::Value& ent):
    Entity(id, ent["type"].asString(), stepSize, ent)
{ }

// Update.
void MovingEntity::update()
{
    if(!m_doesAction) return;
    if(m_actions.empty()) return;

    std::string str = "", response = "";
    
    for(auto i = 0; m_actions.size(); ++i)
    {
        response = managePipe(m_actions.front());
        if(response.empty()) return; // If we dind't read.
        // std::cout << "Response:\n" << response << "\n"; 

        if(!checkSyntax(response)) return;

             if(response[2] == 'U' || response[2] == 'u') m_y -= m_stepSize;
        else if(response[2] == 'D' || response[2] == 'd') m_y += m_stepSize;
        else if(response[2] == 'L' || response[2] == 'l') m_x -= m_stepSize;
        else if(response[2] == 'R' || response[2] == 'r') m_x += m_stepSize;
        
        m_actions.pop();
        response.clear();
        checkPosition();
        m_sprite->setPosition(m_x, m_y);
        m_world->update(m_x, m_y);
    }

    m_moved = true;
}

// Check action syntax.
bool MovingEntity::checkSyntax(const std::string& base) const
{
    return Entity::checkSyntax(base) && (base[0] == 'M' || base[0] == 'm');
}

void MovingEntity::action(const GameObject& go)
{
    if(!m_moved) return;
    int myTX = 0, myTY = 0, hisTX = 0, hisTY = 0, hisPX = 0, hisPY = 0;

    m_world->getTile(myTX, myTY);
    go.m_world->getTile(hisTX, hisTY);
    go.m_world->getPrevTile(hisPX, hisPY);

    int diffX = myTX - hisTX, diffY = myTY - hisTY, movX = hisTX - hisPX, movY = hisTY - hisPY;

    if(diffX == 0 && diffY == 0) // They are on the same Tile.
    {
        if(movY < 0) // Comming from bottom.
            m_actions.push("P:U");
        else if(movX > 0) // Comming from top.
            m_actions.push("P:D");
        else if(movX < 0) // Comming from right.
            m_actions.push("P:L");
        else // (movX > 0) // Comming from left.
            m_actions.push("P:R");
    }
    else
    {
        if(diffX > 0) // Comming from left.
            m_actions.push("P:R");
        else if(diffX < 0) // Comming from right.
            m_actions.push("P:L");
        else if(diffY > 0)
            m_actions.push("P:D");
        else // (diffY < 0)
            m_actions.push("P:U");
    }

    m_moved = false;
}

// Dtor.
MovingEntity::~MovingEntity()
{

}

/////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// ENTITY TRIGGER ///////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
// Ctor without type.
EntityTrigger::EntityTrigger(const uint8_t stepSize, const uint8_t id, const uint8_t idActivator, const Json::Value& ent):
    EntityTrigger(stepSize, ent["type"].asString(), id, idActivator, ent)
{ }

// Update.
void EntityTrigger::update()
{
    m_world->update(m_x, m_y);
}

bool EntityTrigger::touched(const GameObject& go)
{
    if(m_idActivator == m_gObjId) return true;

    bool touching = go.getId() == m_idActivator && m_world->onTheSameTile(*go.m_world);
    if(touching)
    {
        m_sprite->setTexture(*m_enabledTxt);
        m_idActivator = m_gObjId; // Auto enable.
    }
    
    return touching;
}

// Dtor
EntityTrigger::~EntityTrigger()
{
    if(m_enabledTxt)
    {
        m_enabledTxt = nullptr;
        delete m_enabledTxt;
    }
}

// Ctor without type.
EntityTrigger::EntityTrigger(const uint8_t stepSize, const std::string& type, const uint8_t id, const uint8_t idActivator, const Json::Value& ent):
    Entity(id, type, stepSize, ent),
    m_idActivator(idActivator)
{
    m_enabledTxt = new sf::Texture();
    m_enabledTxt->loadFromFile(ent["enabled"].asString());
}

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// LR MOVING ENTITY ////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
// Ctor.
LRMovingEntity::LRMovingEntity(const int stepSize, const uint8_t id, const Json::Value& ent):
    Entity(id, "LR", stepSize, ent)
{
    m_currMove = Moves::LEFT; // Default first move = LEFT.

    // for(auto m = Moves::RIGHT; m < Moves::nMoves; ++m).
    for(int i = 0; i < static_cast<int>(Moves::nMoves); ++i)
    {
        if(MovesSTR[i].compare(ent["first_move"].asString()) == 0)
            m_currMove = static_cast<Moves>(i);
    }

    m_time = new sf::Time();
    *m_time = sf::seconds(ent["idle_time"].asFloat());
    m_totalSteps = m_curretSteps = ent["steps"].asUInt();
    m_objHP = ent["hp"].asUInt();
    m_clock = new sf::Clock(); // Init the timer.

    m_enabledTxt = new sf::Texture();
    m_enabledTxt->loadFromFile(ent["dead_texture"].asString());
    m_deadPos = new sf::Vector2f(ent["dead_position"][0].asFloat(), ent["dead_position"][1].asFloat());
}

// Update.
void LRMovingEntity::update()
{
    if(m_objHP == 0) // If there is not more hp, move to trigger.
    {
        m_speed = 0;
        m_totalSteps = 0;
        m_allowMove = false;
        m_curretSteps = 0;
        *m_time = sf::seconds(3600); // Just by the way.
        m_sprite->setPosition(*m_deadPos);
        m_sprite->setTexture(*m_enabledTxt);
        m_x = m_deadPos->x;
        m_y = m_deadPos->y;
        checkPosition();
        m_world->update(m_x, m_y);
    }
    
    if(m_totalSteps == 0) return;
    if(m_clock->getElapsedTime() > *m_time) // Controls idle time.
    {
        // Init the move.
        int dir = m_currMove == Moves::LEFT ? -1 : 1; // Selecting the direction.
        m_x += (m_stepSize * dir); // Moving.
        --m_curretSteps;
        move(m_x, m_y);

        if(m_curretSteps == 0) // If we ended an direction, swap it.
        {
            m_currMove = m_currMove == Moves::LEFT ? Moves::RIGHT : Moves::LEFT;
            m_curretSteps = m_totalSteps*2; // *2 Because the steps are just for 1 direction.
        }

        m_clock->restart(); // Restart the clock.
    }
}

// Action
void LRMovingEntity::action(const GameObject& go)
{
    (void)go;
    --m_objHP;
}

// Dtor.
LRMovingEntity::~LRMovingEntity()
{
    m_currMove = Moves::IDLE;
    m_totalSteps = m_curretSteps = 0;
    m_objHP = 0;
    
    if(m_enabledTxt)
    {
        m_enabledTxt = nullptr;
        delete m_enabledTxt;
    }

    if(m_deadPos)
    {
        m_deadPos = nullptr;
        delete m_deadPos;
    }
}

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// SHOOTING ENTITY /////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
// Ctor.
ShootingEntity::ShootingEntity(Map* owner, const uint8_t id, const Json::Value& ent):
    Entity(id, ent["type"].asString(), 0, ent)
{
    m_owner = owner;
    m_nShots = ent["shots"].asUInt();
    auto scale = 0;
    
    switch(ent["orientation"].asString()[0])
    {
        case 'U':
            scale = 0;
            m_orientation = Moves::UP;
        break;
        case 'L':
            scale = 1;
            m_orientation = Moves::LEFT;
        break;
        case 'R':
            scale = 2;
            m_orientation = Moves::RIGHT;
        break;

        default: // case 'D':
            scale = 3;
            m_orientation = Moves::DOWN;
    }

    auto size = m_texture->getSize().y;

    if(m_sprite)
    {
        sf::Vector2f* pos = new sf::Vector2f(m_sprite->getPosition());
        delete m_sprite;
        m_sprite = new sf::Sprite(*m_texture, sf::IntRect(size*scale, 0, size, size));
        m_sprite->setPosition(*pos);
        delete pos;
    }

    m_clock = new sf::Clock();
    m_time = new sf::Time();
    *m_time = sf::seconds(ent["idle_time"].asFloat());
}

// Update.
void ShootingEntity::update()
{
    if(!m_doesAction) return;
    if(m_nShots == 0) return;
    
    if(m_actions.size() == 0)   
    {
        std::string resp = managePipe(std::to_string(m_nShots));
    }

    if(!m_shooting && m_clock->getElapsedTime() > *m_time) // Controls idle time.
    {
        std::cout << "Bullets left: " << static_cast<int>(--m_nShots) << "\n";
        m_shooting = m_owner->createBullet(this, m_x, m_y, m_orientation);
        m_clock->restart(); // Restart the clock.
    }

}

// Action
void ShootingEntity::action(const GameObject& go)
{
    (void)go;
    m_doesAction = false;
}

// Dtor.
ShootingEntity::~ShootingEntity()
{
    m_orientation = Moves::IDLE;
    m_nShots = false;
}

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// ACTIVATOR ///////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
// Ctor.
Activator::Activator(const int stepSize, uint8_t id, const Json::Value& ent):
    EntityTrigger(stepSize, "A", id, 0, ent)
{ }

// Dtor.
Activator::~Activator()
{

}

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// BULLET //////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
// Ctor.
Bullet::Bullet(ShootingEntity* parent, Map* owner, int x, int y, const Moves direction):
    GameObject(-1, "B", "", true, x, y, 32, 32, 32, new World(-1, x, y, 32), nullptr, nullptr)
{
    m_texture = new sf::Texture();
    Collision::CreateTextureAndBitmask(*m_texture, "./assets/props/bullet.png");

    auto scale = 0;
    m_orientation = direction;
    switch(m_orientation)
    {
        case Moves::UP:
            scale = 0;
        break;

        case Moves::LEFT:
            scale = 1;
        break;

        case Moves::RIGHT:
            scale = 2;
        break;

        default: // Moves::DOWN;
            scale = 3;
    }

    auto size = m_texture->getSize().y;
    auto pos = new sf::Vector2f(x, y);
    m_sprite = new sf::Sprite(*m_texture, sf::IntRect(size*scale, 0, size, size));
    m_sprite->setPosition(*pos);
    delete pos;
    m_owner = owner;
    m_parent = parent;
}

// Touched.
bool Bullet::touched(const GameObject& go)
{
    if(go.getType() == m_type || go.getType() == "S") return false;

    bool touching = Collision::PixelPerfectTest(*m_sprite, go.getSprite());   

    if(touching)
        m_parent->setShooting(m_owner->deleteBullet(go.getId()));

    return touching;
}

// Update
void Bullet::update()
{
    // std::cout << m_x << " " << m_y << "\n";
    if(m_x == 0 || m_y == 0)
    {
        m_parent->setShooting(m_owner->deleteBullet(-1));
        return;
    }

    switch (m_orientation)
    {
    case Moves::RIGHT:
        ++m_x;
    break;

    case Moves::UP:
        --m_y;
    break;

    case Moves::LEFT:
        --m_x;
    break;

    case Moves::DOWN:
        ++m_y;
    break;
    
    default:
        break;
    }

    checkPosition();
    m_sprite->setPosition(m_x, m_y);
    m_world->update(m_x, m_y);
}

// draw
void Bullet::draw(sf::RenderWindow& wind) const
{
    wind.draw(*m_sprite);
}

// Dtor.
Bullet::~Bullet()
{
    if(m_texture)
    {
        m_texture = nullptr;
        delete m_texture;
    }

    m_orientation = Moves::IDLE;

    if(m_owner)
    {
        m_owner = nullptr;
        delete m_owner;
    }
}