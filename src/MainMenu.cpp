#include <MainMenu.hpp>
// -------------------------------------------------------- protected functions.
// Set the text.
void MainMenu::setText(const std::string& text, const int position)
{
    m_texts[position] = new sf::Text(text, *m_font, 20);    // Create the new text.
    m_texts[position]->setFillColor(sf::Color::Black);      // Set it to black.

    // Center into a position.
    m_texts[position]->setOrigin(m_texts[position]->getLocalBounds().width/2, m_texts[position]->getLocalBounds().height/2);
    m_texts[position]->setPosition(sf::Vector2f(m_h * 0.75, m_w / (m_strs.size() + 1) * (position + 0.3) + m_y));
}

// Update.
void MainMenu::update()
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
}

// -------------------------------------------------------- public functions.
// Ctor.
MainMenu::MainMenu()
{
    m_font = new sf::Font(); // Create the font.
    if(!m_font->loadFromFile("assets/fonts/PkmnXandY.ttf"))
        std::cerr << "Font: assets/fonts/PkmnXandY.ttf not found\n";
    
    m_title = new sf::Text("Codemon\n\n      Main Menu", *m_font, 50);
    m_title->setPosition(m_x, m_y);
    m_title->setFillColor(sf::Color::Black);

    // m_strs = {"Play", "Options", "About", "Exit"};
    m_strs = {"Play", "About", "Exit"};

    uint8_t strSize = m_strs.size();        // Get the string size.
    m_texts.resize(strSize);                 // Resize the text vector.
    for(uint8_t i = 0; i < strSize; ++i)    // Set texts.
        setText(m_strs[i], i);

    m_texts[m_selectedIdx]->setFillColor(sf::Color::Red);
}

// Dtor.
MainMenu::~MainMenu()
{

}

// Displays.
MainMenuOptions MainMenu::display(sf::RenderWindow& window)
{
    do
    {
        if(window.hasFocus() && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            return MainMenuOptions::EXIT;

        draw(window); // Draw texts.

        if(window.hasFocus()) // Update if we have the focus.
            update();

        window.display(); // Display the window.

        if(window.hasFocus() && sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
            break;
    } while(true);
    waitUntilRelease(sf::Keyboard::Return);

    return static_cast<MainMenuOptions>(m_selectedIdx);
}