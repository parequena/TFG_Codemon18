#include <Game.hpp>
// -------------------------------------------------------- private functions.
// Update.
void Game::update()
{
    // m_map->update();
    // if(m_window.hasFocus())
    // {
    //     if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
    //     {
    //         sf::Vector2i pos = sf::Mouse::getPosition(m_window);
    //         std::cout << "posX: " << pos.x << "\tposY: " << pos.y << "\n";
    //     }
    // }
}

// Draw.
void Game::draw()
{
    // m_map->draw(m_window);
}

// -------------------------------------------------------- public functions.
// Ctor.
Game::Game():
    m_window(sf::VideoMode(df_gameWidth, df_gameHeight), df_gameName, sf::Style::Titlebar | sf::Style::Close)
{
    // Move the window at the middle of the screen.
    m_window.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width / 2 - df_gameWidth / 2, 
        sf::VideoMode::getDesktopMode().height / 2 - df_gameHeight / 2)); 

    m_mainMenu = new MainMenu(); // Create the main menu.

    std::ifstream gFile(df_gameConfig, std::ifstream::binary); // Read the json file.
    gFile >> m_cfg;

    // If we set the sourceCodes, update-it.
    if(m_cfg.isMember("sourceCodes"))
        GameObject::m_codeFolder = m_cfg["sourceCodes"].asString();

    int nMaps = m_cfg["maps"].size();
    std::vector<std::string> mapNames;
    mapNames.reserve(nMaps);
    for(int i = 0; i < nMaps; ++i)
        mapNames.emplace_back(m_cfg["maps"][i]["name"].asString());

    m_mapMenu = new MapMenu(mapNames);
    mapNames.clear();
    m_map = nullptr;
}

// Dtor.
Game::~Game()
{
    m_window.clear();
    m_window.close();

    if(m_mainMenu)
    {
        m_mainMenu = nullptr;
        delete m_mainMenu;
    }

    if(m_mapMenu)
    {
        m_mapMenu = nullptr;
        delete m_mapMenu;
    }
}

// Play
void Game::play()
{
    float s = 0.0f;
    auto selectedMap = -1;
    while(m_play)
    {
        sf::Event event;
        if((m_window.hasFocus() && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) || event.type == sf::Event::Closed)
            m_play = false;

        switch(m_mainMenu->display(m_window))
        {
            case MainMenuOptions::PLAY:
                selectedMap = m_mapMenu->display(m_window);
                if(selectedMap != -1)
                {
                    m_map = new Map(m_cfg["maps"][selectedMap]);

                    s = m_map->play(m_window);
                    if(s != std::numeric_limits<float>::infinity())
                        std::cout << "You've been playing the map for " << s << " seconds. \n";
                    else
                        std::cout << "YOU DIDN'T PASSED THE MAP, THERE IS NO TIME ON THIS ATTEMPT\n";

                    update();
                    draw();
                }
            break;

            // case MainMenuOptions::OPTIONS:
            //     std::cout << "Options\n";
            // break;
            
            case MainMenuOptions::ABOUT:
                // std::cout << "About\n";
                std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nCodemon:\nhttps://github.com/ABPMultimediaUA/codemon18\nDone by:\n\tPablo Requena Gonzalez\n\tTwitter: @conPdePABLO\n\tGithub : https://github.com/parequena\nUsed font: Pokemon X and Y by Luis Silva on dafont\nhttps://www.dafont.com/es/luis-silva.d6380\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
            break;

            case MainMenuOptions::EXIT:
                // std::cout << "Exit\n";
                m_play = false;
            break;
        }
    }
}