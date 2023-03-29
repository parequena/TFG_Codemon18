#ifndef MAP_H
#define MAP_H

#include <vector>
#include "TextureManager.cpp"
#include "../tileset/level0.cpp"

class Map
{
private:
    SDL_Rect src, dst;
    std::vector<SDL_Texture*> textures;
    int minimumTreadTile = 1; // If a tile is minus than this value, we can't step in.
    int map[][DF_mapCols];

public:
    Map(std::vector<std::string> filenames)
    {
        int size = filenames.size();

        for(int i = 0; i<size; ++i)
            textures.push_back(TextureManager::LoadTexture(filenames[i]));

        loadMap(lvl0);

        src.x = src.y = 0; // Source on (0,0)
        dst.x = dst.y = 0; // Source on (0,0)
        src.w = dst.w = DF_TileSize; // 20x20 Tile
        src.h = dst.h = DF_TileSize; // 20x20 Tile
    }

    ~Map()
    {

    }

    void loadMap(const unsigned char arr[][DF_mapCols])
    {
        for(int r = 0; r < DF_mapRows; ++r)
        {
            for(int c = 0; c < DF_mapCols; ++c)
                map[r][c] = (int)arr[r][c];
        }
    }

    inline void drawMap()
    {
        int type = -1;
        for(int r = 0; r < DF_mapRows; r++)
        {
            for(int c = 0; c < DF_mapCols; c++)
            {
                type = map[r][c];
                dst.x = c * DF_TileSize; // Move DF_TileSize px.
                dst.y = r * DF_TileSize; // Move DF_TileSize px.

                std::cout << "dst.x" << dst.x << "\tdst.y" << dst.y << std::endl;

                TextureManager::Draw(textures[type], src, dst);
            }
        }
    }
};

#endif