#include "renderer/obj_loader.hh"

#include "tiny_obj_loader.h"
#include <map>

ObjLoader::ObjLoader(const std::string& path) {
    tinyobj::ObjReader objReader;
    objReader.ParseFromFile(path);
    if (!objReader.Valid()) {
        throw std::runtime_error(path + " is not a valid obj file.");
    }
    auto& materials = objReader.GetMaterials();
    std::vector<AtlasTexture> textures;
    textures.reserve(materials.size());
    for (auto& material : materials) {

    }
    auto& attrib = objReader.GetAttrib();
    auto& vertices = attrib.vertices;
    auto& normals = attrib.normals;
    auto& texcoords = attrib.texcoords;
    auto& shapes = objReader.GetShapes();
}