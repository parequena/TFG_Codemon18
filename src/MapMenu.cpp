#include <MapMenu.hpp>
// -------------------------------------------------------- private functions.
// Set the text.
void MapMenu::setText(const std::string& text, const int position)
{
    m_texts[position] = new sf::Text(text, *m_font, 20);    // Create the new text.
    m_texts[position]->setFillColor(sf::Color::Black);      // Set it to black.

    // Center into a position.
    m_texts[position]->setOrigin(m_texts[position]->getLocalBounds().width/2, m_texts[position]->getLocalBounds().height/2);
    m_texts[position]->setPosition(sf::Vector2f(m_h * 0.75, m_w / (m_strs.size() + 1) * (position + 0.3) + m_y));
    /*int size = 5;

    m_texts.resize(size);

    auto x = m_h * 0.75;
    auto y = m_w / 6;
    auto inc = 80;

    m_texts[0] = new sf::Text("Select Map", *m_font, 20);
    m_texts[0]->setFillColor(sf::Color::Black);

    auto topMidle = m_texts[0]->getLocalBounds().width / 2;
    m_texts[0]->setOrigin(topMidle, m_texts[0]->getLocalBounds().height / 2);
    m_texts[0]->setPosition(x, y);

    m_texts[1] = new sf::Text(std::to_string(m_hundred), *m_font, 20);
    m_texts[1]->setFillColor(sf::Color::Black);
    m_texts[1]->setOrigin(m_texts[1]->getLocalBounds().width / 2, m_texts[1]->getLocalBounds().height / 2);
    m_texts[1]->setPosition(x - topMidle, y + inc * 1);

    m_texts[2] = new sf::Text(std::to_string(m_tens), *m_font, 20);
    m_texts[2]->setFillColor(sf::Color::Black);
    m_texts[2]->setOrigin(m_texts[2]->getLocalBounds().width / 2, m_texts[2]->getLocalBounds().height / 2);
    m_texts[2]->setPosition(x, y + inc * 1);

    m_texts[3] = new sf::Text(std::to_string(m_units), *m_font, 20);
    m_texts[3]->setFillColor(sf::Color::Black);
    m_texts[3]->setOrigin(m_texts[3]->getLocalBounds().width / 2, m_texts[3]->getLocalBounds().height / 2);
    m_texts[3]->setPosition(x +  topMidle, y + inc * 1);

    m_texts[4] = new sf::Text("Back", *m_font, 20);
    m_texts[4]->setFillColor(sf::Color::Black);
    m_texts[4]->setOrigin(m_texts[4]->getLocalBounds().width / 2, m_texts[4]->getLocalBounds().height / 2);
    m_texts[4]->setPosition(x, y + inc * 4);

    m_selectedIdx = 2;*/
}

// -------------------------------------------------------- protected functions.
// Update.
void MapMenu::update()
{
    m_texts[m_selectedIdx]->setFillColor(sf::Color::Black);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        if(--m_selectedIdx == 255)
            m_selectedIdx = 0;

        waitUntilRelease(sf::Keyboard::W);
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        auto max = m_texts.size() - 1;
        if(++m_selectedIdx > max)
            m_selectedIdx = max;

        waitUntilRelease(sf::Keyboard::S);
    }

    m_texts[m_selectedIdx]->setFillColor(sf::Color::Red);
    /*m_texts[m_selectedIdx]->setFillColor(sf::Color::Black);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        if(m_selectedIdx == m_texts.size() - 1)
        {
            m_texts[1]->setFillColor(sf::Color::Red);
            m_texts[2]->setFillColor(sf::Color::Red);
            m_texts[3]->setFillColor(sf::Color::Red);

            m_selectedIdx = 2;
        }

        waitUntilRelease(sf::Keyboard::W);
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        m_selectedIdx = m_texts.size() - 1;
        waitUntilRelease(sf::Keyboard::S);
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        if(m_selectedIdx > 1 && m_selectedIdx <= 3)
            --m_selectedIdx;

        waitUntilRelease(sf::Keyboard::A);
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        if(m_selectedIdx >= 1 && m_selectedIdx < 3)
            ++m_selectedIdx;

        waitUntilRelease(sf::Keyboard::D);
    }

    m_texts[1]->setString(std::to_string(m_hundred));
    m_texts[2]->setString(std::to_string(m_tens));
    m_texts[3]->setString(std::to_string(m_units));*/ 
}

// -------------------------------------------------------- public functions.
// Ctor.
MapMenu::MapMenu(const std::vector<std::string>& mapNames)
{   
    m_font = new sf::Font(); // Create the font.
    if(!m_font->loadFromFile("assets/fonts/PkmnXandY.ttf"))
        std::cerr << "Font: assets/fonts/PkmnXandY.ttf not found\n";
    
    m_title = new sf::Text("Map Menu", *m_font, 50);
    m_title->setPosition(0, 0);
    m_title->setFillColor(sf::Color::Black);

    m_strs = mapNames;
    m_nGameMaps = mapNames.size();

    uint8_t strSize = m_strs.size();        // Get the string size.
    m_texts.resize(strSize);                 // Resize the text vector.
    for(uint8_t i = 0; i < strSize; ++i)    // Set texts.
        setText(m_strs[i], i);

    m_texts[m_selectedIdx]->setFillColor(sf::Color::Red);
}

// Dtor.
MapMenu::~MapMenu()
{

}

// Displays.
int MapMenu::display(sf::RenderWindow& window)
{
    do
    {
        if(window.hasFocus() && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            waitUntilRelease(sf::Keyboard::Escape);
            return -1;
        }

        draw(window); // Draw texts.

        if(window.hasFocus()) // Update if we have the focus.
            update();

        window.display(); // Display the window.

        if(window.hasFocus() && sf::Keyboard::isKeyPressed(sf::Keyturn))
            break;
    } while(true);
    waitUntilRelease(sf::Keyboard::Return);

    return m_selectedIdx;
}