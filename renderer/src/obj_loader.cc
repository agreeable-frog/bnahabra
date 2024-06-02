#include "renderer/obj_loader.hh"

#include "tiny_obj_loader.h"
#include <map>

ObjLoader::ObjLoader(const std::string& path) {
    using namespace tinyobj;
    ObjReader objReader;
    objReader.ParseFromFile(path);
    if (!objReader.Valid()) {
        throw std::runtime_error(path + " is not a valid obj file.");
    }
    const std::vector<material_t>& materials = objReader.GetMaterials();
    std::vector<AtlasTexture> textures;
    textures.reserve(materials.size());
    for (const material_t& material : materials) {
    }
    const attrib_t& attrib = objReader.GetAttrib();
    const std::vector<real_t>& vertices = attrib.vertices;
    const std::vector<real_t>& normals = attrib.normals;
    const std::vector<real_t>& texcoords = attrib.texcoords;
    const std::vector<shape_t>& shapes = objReader.GetShapes();
    for (const shape_t& shape : shapes) {
        auto const& mesh = shape.mesh;
    }
}