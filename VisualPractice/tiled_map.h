#ifndef TE_TILED_MAP_H
#define TE_TILED_MAP_H

#include "gl.h"

#include <string>
#include <memory>
#include <vector>

namespace te
{
    class Texture;

    class TiledMap
    {
    public:
        TiledMap(const std::string& path, const std::string& filename);

        TiledMap(TiledMap&& tm);
        TiledMap& operator=(TiledMap&& tm);

        ~TiledMap();

        void draw() const;

    private:
        TiledMap(const TiledMap&) = delete;
        TiledMap& operator=(const TiledMap&) = delete;

        struct Tileset
        {
            std::shared_ptr<Texture> pTexture;
            GLint tileWidth;
            GLint tileHeight;
            GLint width;
            GLint height;
            unsigned firstGID;
        };

        struct Buffers
        {
            GLuint vao;
            GLuint vbo;
            GLuint ebo;
        };

        GLuint mShaderProgram;
        std::vector<Tileset> mTilesets;
        std::vector<Buffers> mBuffers;

        void destroy();
    };
}

#endif
