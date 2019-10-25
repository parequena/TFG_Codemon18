#ifndef MAINMENU_HPP
#define MAINMENU_HPP

#include <Menu.hpp>

enum class MainMenuOptions : uint8_t { PLAY, ABOUT, EXIT};

class MainMenu : public Menu
{
    protected:  // -------------------------------------------------------- protected functions.
        // Set the text.
        void setText(const std::string& text, const int position);
        
        // Update.
        void update() override;

    public:     // -------------------------------------------------------- public functions.
        // Ctor.
        MainMenu();

        // Dtor.
        ~MainMenu();
        
        // Displays.
        MainMenuOptions display(sf::RenderWindow& window);
};

#endif