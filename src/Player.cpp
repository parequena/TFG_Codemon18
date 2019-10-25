#include <Player.hpp>
// -------------------------------------------------------- private variables.
// -------------------------------------------------------- public variables.
// Ctor.
Player::Player(const uint8_t stepSize, const sf::Vector2f* initPos)
{
    // This is so bad, you never should do it, never, ever load a file 2 times.
    Json::Value cfg;
    std::ifstream file(df_gameConfig, std::ifstream::binary); // Read the json file.
    file >> cfg;

    Json::Value pj = cfg["mainCh"];

    // Construct the object.
    // Entity(0, "P", stepSize*speed, pj);
    m_allowMove = true; // The player always can move.
    m_x = initPos->x;   // Set the init position.
    m_y = initPos->y;

    m_gObjId = 0;
    m_type = "P";

    m_speed = pj["speed"].asFloat();
    if(!m_texture)
    {
        m_texture = new sf::Texture();
        Collision::CreateTextureAndBitmask(*m_texture, pj["texture"].asString());
    }

    if(!m_sprite)
        m_sprite = new sf::Sprite(*m_texture);
    m_sprite->setPosition(*initPos);
    m_inventory = new Inventory(cfg.isMember("sourceCodes") ?
        cfg["sourceCodes"].asString() : GameObject::m_codeFolder); // Create the Inventory.

    if(m_world)
    {
        delete m_world;
        m_world = new World(m_gObjId, m_x, m_y, stepSize);
    }
    
    m_doesAction = false;
}

// Dtor.
Player::~Player()
{
    if(m_inventory)
    {
        m_inventory = nullptr;
        delete m_inventory;
    }
}

// Update
void Player::update()
{
    // Update the boolean.
    m_openInvetory = m_inventory->isOpen();

    if(m_openInvetory)
    {
        m_code = m_inventory->open();
        std::cout << "m_code : " << m_code << "\n";
    }

    // TODO: Falta arreglar la gestion de los codigos, hay que hacer un esquema de como
    // se selecinan desde el inventario, porque no tengo forma de crear algo que se autogestione,
    // y no tenemos tiempo para perderlo, ahora mismo, solo trabajaremos con un unico codigo.
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::I))
    {
        m_openInvetory = true;
        waitUntilRelease(sf::Keyboard::I);
        m_code =  m_inventory->open();
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::E))
    {
        // std::cout << "code: " << m_code << "\n";
        m_doesAction = true;
        waitUntilRelease(sf::Keyboard::E);
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        m_y -= m_speed;
        // waitUntilRelease(sf::Keyboard::W);
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        m_y += m_speed;
        // waitUntilRelease(sf::Keyboard::S);
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        m_x -= m_speed;
        // waitUntilRelease(sf::Keyboard::A);
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        m_x += m_speed;
        // waitUntilRelease(sf::Keyboard::D);
    }

    move(m_x, m_y);
}

// Draw.
void Player::draw(sf::RenderWindow& window) const
{
    if(m_openInvetory)
        m_inventory->draw(window);

    Entity::draw(window);
}

// Set code to an entity
void Player::setCode(Entity& ent)
{
    ent.setCode(m_code);
}