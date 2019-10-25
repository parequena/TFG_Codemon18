#ifndef MAPMENU_HPP
#define MAPMENU_HPP

#define df_extraMapPath "./assets/maps/"

#include <Menu.hpp>
#include <Map.hpp>

enum class MapMenuOptions : uint8_t { MAP,  BACK };

class MapMenu : public Menu
{
    private:  // -------------------------------------------------------- private variables.
        // Max maps that has our game.
        uint8_t m_nGameMaps = 0;

        // Extra maps names.
        std::vector<std::string> m_MapsNames;

        // Ints to controll the map value.
        // uint8_t m_hundred = 0, m_tens = 0, m_units = 0;.

        // Choosed Map.
        uint8_t m_choosedMap = 0;

    private:  // -------------------------------------------------------- private functions.
        // Set the text.
        // void setText();.
    protected:  // -------------------------------------------------------- protected functions.
        // Set the text.
        void setText(const std::string& text, const int position); // { (void)text; (void)position; return; };.
        
        // Update.
        void update() override;

    public:     // -------------------------------------------------------- public functions.
        // Ctor.
        MapMenu(const std::vector<std::string>& mapNames);

        // Dtor.
        ~MapMenu();
        
        // Displays.
        int display(sf::RenderWindow& window);
};

#endif