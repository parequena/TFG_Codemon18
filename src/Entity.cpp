#include <Entity.hpp>
// -------------------------------------------------------- protected variables.
std::string Entity::managePipe(const std::string& toChild) const
{
    if(m_code.empty()) return "";
    std::string recivedCommand = "";
    
    // STDIN_FILENO    0       Standard input.
    // STDOUT_FILENO   1       Standard output.
    // STDERR_FILENO   2       Standard error output.
    // Creating FileDescripors.
    // p2c : Parent -> Child information flow.
    // C2p : Child -> Parent information flow.
    int fd_p2c[2], fd_c2p[2];

    // Create the pipes and check if they could be created.
    if(pipe(fd_p2c) || pipe(fd_c2p))
        std::cerr << "Error: Couldn't create pipes.\n";

    // Create the child process.
    int pid = fork();

    if(pid == -1)
    {
        std::cerr << "Error: Couldn't create the child process.\n";
    }
    else if (pid == 0) // Child process.
    {
        // Backup std in/out.
        int stdin_bak = dup(STDIN_FILENO), stdout_bak = dup(STDOUT_FILENO);

        // Close the unused pipe channels.
        close(fd_c2p[STDIN_FILENO]);  // Child won't write on c2p.
        close(fd_p2c[STDOUT_FILENO]); // Child won't read from p2c.

        // Duplicate the used channels.
        dup2(fd_c2p[STDOUT_FILENO], STDOUT_FILENO); // fd_c2p_out -> std::cin.
        dup2(fd_p2c[STDIN_FILENO], STDIN_FILENO);   // fd_p2c_in  -> std::cout.

        // FD's are suplicated on std in/out, so we can close the original ones.
        close(fd_c2p[STDOUT_FILENO]);
        close(fd_p2c[STDIN_FILENO]);

        // Exec the code with those arguments, then.
        char *const argv[] = { const_cast<char*>(m_code.c_str()), nullptr };
        execve(("./" + m_code).c_str(), argv, nullptr);

        // This code shouldn't execute, but restore just in case that execve fails.
        dup2 (stdin_bak, STDIN_FILENO);
        dup2 (stdout_bak, STDOUT_FILENO);
        close(stdin_bak);
        close(stdout_bak);

        std::cerr << "Error: " << m_code << "couln't be executed.\n";
    }
    else // Parent process.
    {
        // Backup std in/out.
        int stdin_bak = dup(STDIN_FILENO), stdout_bak = dup(STDOUT_FILENO);

        // Close the unused pipe channels.
        close(fd_c2p[STDOUT_FILENO]); // Parent won't write on c2p.
        close(fd_p2c[STDIN_FILENO]);  // Parent won't read on p2c.

        // Duplicate the used channels.
        dup2(fd_c2p[STDIN_FILENO], STDIN_FILENO);   // fd_c2p_in  -> std::cin.
        dup2(fd_p2c[STDOUT_FILENO], STDOUT_FILENO); // fd_p2c_out -> std::cout.

        // FD's are suplicated on std in/out, so we can close the original ones.
        close(fd_c2p[STDIN_FILENO]);
        close(fd_p2c[STDOUT_FILENO]);

        // for(auto& str : toChild).
        // {.
            // Now, write to the child process.
            // std::cout << str << "\n";.
            std::cout << toChild << "\n";

            // Get what the child process said.
            std::getline(std::cin, recivedCommand);
        // }.

        // Restore std in/out.
        dup2 (stdin_bak, STDIN_FILENO);
        dup2 (stdout_bak, STDOUT_FILENO);
        close(stdin_bak);
        close(stdout_bak);
    }

    return recivedCommand;
}

// Check action syntax.
bool Entity::checkSyntax(const std::string& base) const
{
    // This shouldn't be here.
    return base.length() == 3 && base[1] == ':';
}

// Tokenize.
void Entity::tokenizeActions(const std::string& code)
{
    /*std::istringstream ss(code);
    std::string token;

    while(std::getline(ss, token, ' '))
        m_actions.push(token);*/
}

// Ctor.
Entity::Entity():
    GameObject()
{ }

// Param Ctor.
Entity::Entity(const uint8_t id, const std::string type, const uint8_t stepSize, const Json::Value& ent):
    GameObject(id, type, "", false, ent["position"][0].asFloat(), ent["position"][1].asFloat(),
        0, 0, stepSize),
    m_clock(nullptr),
    m_time(nullptr)
{
    // *m_time = sf::seconds(0.5);.
    m_texture = new sf::Texture();
    Collision::CreateTextureAndBitmask(*m_texture, ent["texture"].asString());
    m_sprite = new sf::Sprite(*m_texture);
    m_allowMove = ent.isMember("move") ? ent["move"].asBool() : false;

    m_w = m_texture->getSize().x;
    m_h = m_texture->getSize().y;

    m_speed = ent.isMember("speed") ? ent["speed"].asFloat() : 0.0f;
    move(m_x, m_y);
    
    if(m_world)
    {
        delete m_world;
        m_world = new World(id, m_x, m_y, stepSize);
    }
}

// -------------------------------------------------------- public functions.

// Dtor.
Entity::~Entity()
{
    if(m_texture)
    {
        m_texture = nullptr;
        delete m_texture;
    }
}

// Update.
void Entity::update()
{
    checkPosition();
    m_sprite->setPosition(m_x, m_y);
    m_world->update(m_x, m_y);
}

// Draw.
void Entity::draw(sf::RenderWindow& window) const
{
    window.draw(*m_sprite);
}

// Controlls collisions.
bool Entity::touched(const GameObject& ent)
{
    if(ent.getType() == "B")
        return Collision::PixelPerfectTest(*m_sprite, *ent.m_sprite);

    return Collision::BoundingBoxTest(*m_sprite, *ent.m_sprite);
}