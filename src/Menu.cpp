#include <Menu.hpp>
// -------------------------------------------------------- protected functions.
// Draw.
void Menu::draw(sf::RenderWindow& window) const
{
    window.clear(sf::Color::White); // Clear to white.
    window.draw(*m_title);          // Draw the title.
    for(auto t : m_texts)           // Draw all texts.
        window.draw((*t));
}

// -------------------------------------------------------- public functions.
// Dtor.
Menu::~Menu()
{
    if(m_title)
    {
        m_title = nullptr;
        delete m_title;
    }

    if(!m_texts.empty())
    {
        for(auto &t : m_texts)
            delete t;

        m_texts.clear();
    }

    if(!m_strs.empty())
        m_strs.clear();
}