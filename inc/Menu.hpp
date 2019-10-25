#ifndef MENU_HPP
#define MENU_HPP

#include <GameObject.hpp>

class Menu : protected GameObject
{
    private:  // -------------------------------------------------------- private functions.

        // Touched.
        inline bool touched(const GameObject&) { return false; };

    protected:  // -------------------------------------------------------- protected variables.
        // Tittle.
        sf::Text* m_title = nullptr;

        // Selected Index.
        uint8_t m_selectedIdx = 0;

        // SFML Texts.
        std::vector<sf::Text*> m_texts;

        // Menu strings.
        std::vector<std::string> m_strs;

    protected:  // -------------------------------------------------------- protected functions.
        // Set the text.
        virtual void setText(const std::string& text, const int position) = 0;

        // Update.
        virtual void update() = 0;

        // Draw.
        virtual void draw(sf::RenderWindow& window) const;

    public:     // -------------------------------------------------------- public functions.
        // Dtor.
        virtual ~Menu();
};

#endif