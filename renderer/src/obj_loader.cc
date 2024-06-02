#include "renderer/obj_loader.hh"

#include "tiny_obj_loader.h"
#include <map>

ObjLoader::ObjLoader(const std::string& path, std::shared_ptr<TextureAtlas> textureAtlas) {
    using namespace tinyobj;
    ObjReader objReader;
    objReader.ParseFromFile(path);
    if (!objReader.Valid()) {
        throw std::runtime_error(path + " is not a valid obj file.");
    }
    const std::vector<material_t>& materials = objReader.GetMaterials();
    for (const material_t& material : materials) {
    }
    const attrib_t& attrib = objReader.GetAttrib();
    const std::vector<real_t>& vertices = attrib.vertices;
    const std::vector<real_t>& normals = attrib.normals;
    const std::vector<real_t>& texcoords = attrib.texcoords;
    const std::vector<shape_t>& shapes = objReader.GetShapes();
    std::map<int, std::pair<std::vector<MeshVertex>, std::vector<uint32_t>>> parsedMeshes;
    for (const shape_t& shape : shapes) {
        const mesh_t& mesh = shape.mesh;
        const std::vector<index_t>& indices = mesh.indices;
        const std::vector<int>& materialIds = mesh.material_ids;
        const std::vector<uint>& numFaceVertices = mesh.num_face_vertices;
        for (size_t i = 0; i < materialIds.size(); i++) {
            if (numFaceVertices.at(i) == 3) {
                std::vector<MeshVertex>& meshVertices = parsedMeshes[materialIds.at(i)].first;
                std::vector<uint32_t>& meshIndices = parsedMeshes[materialIds.at(i)].second;
                for (size_t j = 0; j < 3; j++) {
                    index_t index = indices.at(i * 3 + j);
                }
            } else {
                throw std::runtime_error("Faces with number of vertices different from 3 in obj not supported.");
            }
        }
    }
}