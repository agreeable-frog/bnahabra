#pragma once

#include <string>

#include "renderer/mesh.hh"
#include "renderer/texture_atlas.hh"

class ObjLoader {
private:
    std::vector<std::shared_ptr<Mesh>> _loadedMeshes;
    std::vector<std::shared_ptr<AtlasTexture>> _loadedTextures;
public:
    ObjLoader(const std::string& path, std::shared_ptr<TextureAtlas> textureAtlas);
};