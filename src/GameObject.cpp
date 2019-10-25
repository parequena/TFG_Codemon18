#include <GameObject.hpp>
// Initialize m_codeFolder.
std::string GameObject::m_codeFolder = "./assets/sourceCodes";

void GameObject::checkPosition()
{
    if(m_x <= 0)
        m_x = 0;
    else if(m_x + (m_sprite->getTexture()->getSize().x) >= df_gameWidth)
        m_x = df_gameWidth - (m_sprite->getTexture()->getSize().x);

    if(m_y <= 0)
        m_y = 0;
    else if(m_y + (m_sprite->getTexture()->getSize().y) >= df_gameHeight)
        m_y = df_gameHeight - (m_sprite->getTexture()->getSize().y);
}

void GameObject::move(const int x, const int y)
{
    checkPosition();
    m_sprite->setPosition(x, y);
    m_world->update(x, y);
}

// Ctor.
GameObject::GameObject():
    GameObject(0, "", "", false, 0, 0, df_gameWidth, df_gameHeight, 0, new World(-1, -1, -1, -1))
{ }

// -------------------------------------------------------- public functions.
// Copy Ctor.
GameObject::GameObject(const GameObject& go):
    GameObject(go.m_gObjId, go.m_type, go.m_code, go.m_doesAction, go.m_x, go.m_y, go.m_w, go.m_h, go.m_stepSize, go.m_world, go.m_font, go.m_sprite)
{ }

// Param Ctor without sprite, font and world.
GameObject::GameObject(uint8_t id, const std::string& type, const std::string& code, bool doesAction,
    float x, float y, float w, float h, uint8_t stepSize):
        GameObject(id, type, code, doesAction, x, y, w, h, stepSize, new World(id, x, y, stepSize), nullptr, nullptr)
{ }

// Param Ctor without sprite and font.
GameObject::GameObject(uint8_t id, const std::string& type, const std::string& code, bool doesAction,
    float x, float y, float w, float h, uint8_t stepSize, const World* world):
        GameObject(id, type, code, doesAction, x, y, w, h, stepSize, world, nullptr, nullptr)
{ }

// Param Ctor.
GameObject::GameObject(uint8_t id, const std::string& type, const std::string& code, bool doesAction,
    float x, float y, float w, float h, uint8_t stepSize,
    const World* world, const sf::Font* font, const sf::Sprite* sprite):
        m_gObjId(id), m_type(type), m_code(code), m_doesAction(m_doesAction), m_x(x), m_y(y), m_w(w), m_h(h), m_stepSize(stepSize)
{
    if(font)
        m_font = new sf::Font(*font);

    if(sprite)
        m_sprite = new sf::Sprite(*sprite);

    if(world)
        m_world = new World(*world);
}

// Dtor.
GameObject::~GameObject()
{
    if(m_font)
    {
        m_font = nullptr;
        delete m_font;
    }

    if(m_sprite)
    {
        m_sprite = nullptr;
        delete m_sprite;
    }

    if(m_world)
    {
        m_world = nullptr;
        delete m_world;
    }
}

// Wait until release
void GameObject::waitUntilRelease(const sf::Keyboard::Key key)
{
    while(sf::Keyboard::isKeyPressed(key));
}

//////////////////////////////////////////////////////////////////////////////// WORLD
World::World():
    m_entityID(1), m_entityX(1), m_entityY(1), m_tileSize(1), m_tileX(1), m_tileY(1), 
    m_prevTileX(1), m_prevTileY(1)
{ }

// Ctor.
World::World(const uint8_t id, const int x, const int y, const int tileSize):
    m_entityID(id), m_entityX(x), m_entityY(y), m_tileSize(tileSize)
{
    if(m_tileSize == 0)
        m_tileSize = 1;

    m_prevTileX = m_tileX = x / m_tileSize;
    m_prevTileY = m_tileY = y / m_tileSize;
}

// Copy Ctor.
World::World(const World& w):
    m_entityID(w.m_entityID), m_entityX(w.m_entityX), m_entityY(w.m_entityY), m_tileX(w.m_tileX),
    m_tileY(w.m_tileY), m_prevTileX(w.m_prevTileX), m_prevTileY(w.m_prevTileY), m_tileSize(w.m_tileSize)
{ }

// Get tile.
void World::getTile(int&x, int& y)
{
    x = m_tileX;
    y = m_tileY;    
}

// Get previous tile.
void World::getPrevTile(int&x, int& y)
{
    x = m_prevTileX;
    y = m_prevTileY;
}

// Update.
void World::update(const int x, const int y)
{
    m_entityX = x; m_entityY = y;

    int newTileX = m_entityX / m_tileSize,
        newTileY = m_entityY / m_tileSize;

    if(newTileX != m_tileX || newTileY != m_tileY)
    {
        m_prevTileX = m_tileX;
        m_prevTileY = m_tileY;

        m_tileX = newTileX;
        m_tileY = newTileY;
    }
}

// Print (cout doesn't work).
void World::print()
{
    std::cout << "id: "   << m_entityID
            << "\tx: "  << m_entityX   << "\ty: "  << m_entityY
            << "\ttx: " << m_tileX     << "\tty: " << m_tileY
            << "\tpx: " << m_prevTileX << "\tpy: " << m_prevTileY
            << "\tts: " << m_tileSize
            << "\n";   
}

// Check if two worlds are on the same tile.
bool World::onTheSameTile(const World& w)
{
    return m_tileX == w.m_tileX && m_tileY == w.m_tileY;
}

// Cout <<
std::ostream& operator<<(std::ostream& os, const World& w)
{
    return os << "id: " << w.m_entityID << "\tx: " << w.m_entityX << "\ty: " << w.m_entityY
              << "\ttx: " << w.m_tileX << "\tty: " << w.m_tileY << "\tts: " << w.m_tileSize << "\n";
}