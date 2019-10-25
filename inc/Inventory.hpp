#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include <GameObject.hpp>

class Inventory : private GameObject
{
    private:     // -------------------------------------------------------- private functions.

        // Event handler.
        sf::Event* m_event;

        // Code folder.
        std::string m_codeFolder;
        
        // Selected index.
        int m_idx = 0;

        // Exec codes.
        std::vector<std::string> m_codes;

        // Manage if its open.
        bool m_isOpen = false;

        // Read the codes folder.
        void readCodeFolder();

        // touched.
        inline bool touched(const GameObject&) { return false; };

    public:     // -------------------------------------------------------- public functions.
        // Default Ctor.
        Inventory();
        
        // Ctor.
        Inventory(const std::string& folder);

        // Copy ctor.
        Inventory(const Inventory& i);

        // operator=.
        Inventory& operator=(Inventory i);

        // Dtor.
        ~Inventory();

        // open.
        std::string open();

        // isOpen.
        bool isOpen();
        
        // update.
        void update() override;

        // draw.
        void draw(sf::RenderWindow& window) const;
};

#endif