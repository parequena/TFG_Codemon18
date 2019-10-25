#include <Map.hpp>
// -------------------------------------------------------- private functions.
// Checks if we finished the map.
bool Map::checkEndMap(const GameObject& go) const
{
    return m_end &&
        (go.getX() >= m_endCoord[0] && go.getX() <= m_endCoord[2]
        && go.getY() >= m_endCoord[1] && go.getY() <= m_endCoord[3]);
}

// Find type
uint8_t Map::findType(const std::string& type)
{
    static const std::vector<std::string> strTypes = {"M", "T", "LR", "S", "A"};

    for(uint8_t i = 0; i < strTypes.size(); ++i)
        if(type.compare(strTypes[i]) == 0)
            return i;

    return -1;
}

// Load sprites from folder
void Map::loadSprites(std::string& folder)
{
    m_texture = new sf::Texture();
    if(!m_texture->loadFromFile(folder))
        std::cerr << "Error, texture " << folder << "does not exists.\n";
    else
    {
        for(auto i = 0; i < m_texture->getSize().y; i += m_tileSizeOnMap)
            for(auto j = 0; j < m_texture->getSize().x; j += m_tileSizeOnMap)
                m_tileSet.push_back(new sf::Sprite(*m_texture, sf::IntRect(j, i, m_tileSizeOnMap, m_tileSizeOnMap)));
    }
}

// void setCode
void Map::setCode(const GameObject& go1, GameObject& go2)
{
    go2.setCode(go1.getCode());
    go2.setAction(go1.getAction());
}

// touched
bool Map::touched(const GameObject& go)
{
    m_collisions = std::vector<bool>(m_nEnts, false);

    bool ret = false;

    for(auto i = 0; i < m_nEnts - 1; ++i) // YOU CANT COLIDE WITH YOU
    {
        if(m_objs[i]->touched(go)) // If it's touching.
        {
            // std::cout << "Go: " << go.getCode() << "\t[i]: " << m_objs[i]->getCode() << "\n"; 
            m_objs[i]->action(go); // Set the action.
            ret = true;
            if(go.getAction())
                setCode(go, *m_objs[i]);
        }
    }

    return ret;
}

// Update.
void Map::update()
{
    m_end = true; // Lets asume the map is over.
    for(auto b: m_enabledTriggers)
    {
        if(!b) // If there is a non enabled trigger.
        {
            m_end = false; // Keep playing.
            break;
        }
    }

    if(checkEndMap(*m_objs[m_nEnts - 1]))
        return;

    for(auto i : m_killingEnts)
    {
        if(m_objs[m_nEnts - 1]->touched(*m_objs[i]))
        {
            m_killed = true;
            m_end = true;
            return;
        }
    }

    for(auto i = 0, idx = 0; i < m_triggerIdx.size(); ++i, idx = 0) // Check all triggers.
    {
        // Set current to touched or not.
        m_enabledTriggers[i] = m_objs[m_triggerIdx[i]]->touched(*m_objs[0]);

        // If its false, keep looping
        while(idx < m_objs.size() && !m_enabledTriggers[i])
            m_enabledTriggers[i] = m_objs[m_triggerIdx[i]]->touched(*m_objs[idx++]);
    }

    if(touched(*m_objs[m_nEnts - 1]))
    {   
        m_objs[m_nEnts - 1]->setAction(false);
    }

    for(auto& o : m_objs)
        o->update();
}

// Draw.
void Map::draw(sf::RenderWindow& window) const
{
    // if(window.hasFocus())
    // {
    //     if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
    //     {
    //         sf::Vector2i pos = sf::Mouse::getPosition(window);
    //         std::cout << "posX: " << pos.x << "\tposY: " << pos.y << "\n";
    //     }
    // }

    // Draw map.
    sf::Sprite* sp = new sf::Sprite();
    for(auto i = 0; i < m_tileRows*m_tileCols; ++i)
    {
        sp = m_tileSet[m_tileMap[i]];
        sp->setPosition(i%m_tileCols * m_tileSizeOnMap, i/m_tileCols * m_tileSizeOnMap);
        window.draw(*sp);
    }

    // Draw the end map.
    window.draw(*m_endDraw);

    for(auto &o : m_objs)
        o->draw(window);
    window.display();
}

// -------------------------------------------------------- public functions.        
// Ctor.
Map::Map(const Json::Value& map):
    m_tileRows(map["nTiles"][0].asInt()),
    m_tileCols(map["nTiles"][1].asInt()),
    m_tileSizeOnMap(map["tileSizeOnMap"].asFloat()),
    m_spawnCoord(new sf::Vector2f(map["spawnCoord"][0].asInt(), map["spawnCoord"][1].asInt()))
{
    // Get map size.
    // Get tile size on map.
    // Get map coordinates.
    float w = map["endMapCoord"][2].asFloat();
    float h = map["endMapCoord"][3].asFloat();

    m_endCoord[0] = map["endMapCoord"][0].asInt();
    m_endCoord[1] = map["endMapCoord"][1].asInt();
    m_endCoord[2] = m_endCoord[0] + static_cast<uint16_t>(w);
    m_endCoord[3] = m_endCoord[1] + static_cast<uint16_t>(h);

    m_endDraw = new sf::RectangleShape(sf::Vector2f(w, h));
    // m_endDraw->setPosition(static_cast<float>(m_endCoord[0]), static_cast<float>(m_endCoord[0]));
    m_endDraw->setPosition(m_endCoord[0], m_endCoord[1]);
    m_endDraw->setFillColor(sf::Color(255, 255, 0, 100)); // Create a full black rectangle.
    // Load sprites.
    std::string f = map["tileSet"].asString();
    loadSprites(f);

    // Resize the map.
    m_tileMap.reserve(m_tileRows*m_tileCols);
    for(auto r = 0; r < m_tileRows; ++r)
        for(auto c = 0; c < m_tileCols; ++c)
            m_tileMap.emplace_back(map["tileMap"][r][c].asInt());

    std::cout << "----------------------\n";
    std::cout << "MAP HELP\n";
    std::cout << "----------------------\n";
    if(map.isMember("help") && !map["help"].asString().empty())
        std::cout << map["help"].asString();
    else std::cout << "There is no help on this level...\n\n\n\n";

    // Resize the m_obj vector.
    m_nEnts = map["entities"].size() + 1;
    m_objs.reserve(m_nEnts);
    int nShots = -1;
    // ShootingEntity* parent = nullptr;

    // Filling with map entities.
    for(auto i = 0; i < m_nEnts - 1; ++i)
    {
        switch(findType(map["entities"][i]["type"].asString()))
        {
            case 0:
                m_objs.emplace_back(new MovingEntity(m_tileSizeOnMap, i+1, map["entities"][i]));
            break;

            case 1:
                if(i+1 == map["entities"][i]["activator"].asInt())
                    std::cerr << "Trigger activator with id: " << i+1 << "can not been activated by himself\n";
                else
                {
                    m_enabledTriggers.emplace_back(false); // Emplace a false.
                    m_triggerIdx.emplace_back(i); // Set the trigger idx.
                    m_objs.emplace_back(new EntityTrigger(m_tileSizeOnMap, i+1, map["entities"][i]["activator"].asInt(), map["entities"][i]));
                }
            break;

            case 2:
                m_killingEnts.emplace_back(i); // Set a killing entity.
                m_objs.emplace_back(new LRMovingEntity(m_tileSizeOnMap, i+1, map["entities"][i]));
            break;

            case 3:
                // Create all bullets.

                m_objs.emplace_back(new ShootingEntity(this, i+1, map["entities"][i]));
                // parent = nullptr;
            break;

            case 4:
                m_enabledTriggers.emplace_back(false);
                m_triggerIdx.emplace_back(i);
                m_objs.emplace_back(new Activator(m_tileSizeOnMap, i+1, map["entities"][i]));
            break;

            default:
                m_objs.emplace_back(new Entity(i+1, "E", m_tileSizeOnMap, map["entities"][i]));
                // m_objs.emplace_back(new Entity(m_tileSizeOnMap, i+1, map["entities"][i]));
        }
    }

    // At the end, set the Player.
    m_objs.emplace_back(new Player(m_tileSizeOnMap  , m_spawnCoord));
    m_nEnts = m_objs.size();
    m_enableCode = false;
    m_collisions = std::vector<bool>(m_nEnts, false);
}

// Copy Ctor.
Map::Map(const Map& m)
    /*m_tileRows(m.m_tileRows),
    m_tileCols(m.m_tileCols),
    m_tileSizeOnMap(m.m_tileSizeOnMap)*/
{
    //**//
}

// operator=
Map& Map::operator&=(Map m)
{
    return *this;
}

// Dtor.
Map::~Map()
{
    for(auto &o : m_objs)
        delete o;
    m_objs.clear();
    m_triggerIdx.clear();
    m_enabledTriggers.clear();
    m_tileRows = 0, m_tileCols = 0;
    m_tileSizeOnMap = 0;
    m_tileMap.clear();

    if(m_texture != nullptr)
    {
        m_texture = nullptr;
        delete m_texture;
    }

    for(auto &t : m_tileSet)
        delete t;
    m_tileSet.clear();
    
    // delete [] m_endCoord;

    if(m_endDraw != nullptr)
    {
        m_endDraw = nullptr;
        delete m_endDraw;
    }

    if(m_spawnCoord != nullptr)
    {
        m_spawnCoord = nullptr;
        delete m_spawnCoord;
    }
    
    m_enableCode = false;
    m_nEnts = 0;

    m_collisions.clear();
    m_falses.clear();
    m_end = false;
}

// Play the map
float Map::play(sf::RenderWindow& window)
{
    sf::Clock* clock = new sf::Clock();

    while(!checkEndMap(*m_objs[m_nEnts - 1]))
    {
        update();
        if(m_killed)
        {
            std::cout << "Oh! You've been killed!\n";
            return std::numeric_limits<float>::infinity();
        }
        draw(window);

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        {
            waitUntilRelease(sf::Keyboard::Q);
            return std::numeric_limits<float>::infinity();
        }
    }

    float secs = clock->getElapsedTime().asSeconds();
    delete clock;
    return secs;
}


// Create bullet.
bool Map::createBullet(ShootingEntity* parent, int x, int y, Moves direction)
{
    auto it = m_objs.end();
    --it;
    m_objs.insert(--it, new Bullet(parent, this, x, y, direction));
    m_killingEnts.push_back(m_nEnts-2);
    m_triggerIdx.push_back(m_nEnts-2);
    m_enabledTriggers.push_back(false);
    ++m_nEnts;

    return true;
}

bool Map::deleteBullet(int id)
{
    auto it = m_objs.end();
    --it;
    --it;

    if(id != -1)
    {   
        for(auto i = 0; i < m_objs.size(); ++i)
        {
            if(m_objs[i]->getId() == id)
            {
                m_objs[i]->action(*this);
                break;
            }
        }
    }

    m_objs.erase(--it);
    m_killingEnts.pop_back();
    m_triggerIdx.pop_back();
    m_enabledTriggers.pop_back();
    --m_nEnts;

    return false;
}
