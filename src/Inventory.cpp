#include <Inventory.hpp>

// Read the codes folder.
void Inventory::readCodeFolder()
{
	m_codes.clear();
	std::string str;
	for(auto& f : std::filesystem::directory_iterator(m_codeFolder))
	{
		str = f.path().string();

		if(str.find(".cpp") != std::string::npos || str.find(".c") != std::string::npos || str.find(".cc") != std::string::npos)
			continue;

		m_codes.push_back(str);
	}

	// m_codes.push_back(f.path().string());
}

// Controls if we pressed a key number.
/*bool Inventory::pressNums(const sf::Keyboard key);
{
	std::cout << "key: " << key << "\n";
	return sf::Keyboard::Num1 <= key && key <= sf::Keyboard::Num9;
}*/

// Default Ctor.
Inventory::Inventory():
	Inventory("assets/sourceCodes/")
{ }

// Ctor.
Inventory::Inventory(const std::string& folder):
	m_codeFolder(folder),
	m_isOpen(false),
	m_event(new sf::Event())
{ }

// Copy ctor.
Inventory::Inventory(const Inventory& in):
	m_codeFolder(in.m_codeFolder),
	m_isOpen(false),
	m_codes(in.m_codes),
	m_event(in.m_event)
{ }

// operator=
Inventory& Inventory::operator=(Inventory in)
{
	std::swap(m_x, in.m_x);
	std::swap(m_y, in.m_y);
	std::swap(m_h, in.m_h);
	std::swap(m_w, in.m_w);
	std::swap(m_font, in.m_font);

	std::swap(m_codeFolder, in.m_codeFolder);
	std::swap(m_codes, in.m_codes);

	return *this;
}

// Dtor.
Inventory::~Inventory()
{
	m_codeFolder.clear();
	m_idx = 0;
	m_codes.clear();
}

// update
void Inventory::update()
{
	m_idx = -1;
	auto maxCodes = m_codes.size();
	do
	{
		for(auto i = 0; i < maxCodes; ++i)
			std::cout << i+1 << ". " << m_codes[i] << "\n";

		std::cout << "Get the code: (1-"<< maxCodes << ")\n";
		std::cin >> m_idx;
	} while (m_idx < 0 || m_idx > maxCodes);
}

// open
std::string Inventory::open()
{
	m_isOpen = true;

	readCodeFolder();
	update();

	m_isOpen = false;

	std::string ret = m_codes[m_idx-1];
	return ret;
}

// isOpen
bool Inventory::isOpen()
{
	return m_isOpen;
}

void Inventory::draw(sf::RenderWindow& window) const
{
    /*window.clear(sf::Color::Black);
	window.display();
	bool exit = false;
	while(window.pollEvent(*m_event));


	sf::Keyboard::Key k = m_event->key.code;
	while(!exit)
	{
		while(window.pollEvent(*m_event))
		{
			k = m_event->key.code;
			std::cout << "0: " << sf::Keyboard::Num0 << "\tk: " << k << "\t9: " << sf::Keyboard::Num9 << "\t";
			std::cout << ">= : " << (k >= sf::Keyboard::Num0) << "\t";
			std::cout << "<  : " << (k < sf::Keyboard::Num9) << "\n";

			if((k >= sf::Keyboard::Num0) && (k < sf::Keyboard::Num9))
			{
				std::cout << "double true\n";
				exit = true;
				break;
			}
		}
	}*/
}