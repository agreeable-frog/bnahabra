#include "renderer/obj_loader.hh"

#include "tiny_obj_loader.h"

ObjLoader::ObjLoader(const std::string& path) {
    tinyobj::ObjReader objReader;
    objReader.ParseFromFile(path);
    if (!objReader.Valid()) {
        std::cout << path << " is not a valid obj file." << std::endl;
    }
    auto& attrib = objReader.GetAttrib();
    std::cout << "nb attrib vertices : " << attrib.vertices.size() << std::endl;
    auto& materials = objReader.GetMaterials();
    std::cout << "nb materials : " << materials.size() << std::endl;
    auto& shapes = objReader.GetShapes();
    std::cout << "nb shapes : " << shapes.size() << std::endl;
    for (auto& shape : shapes) {
        std::cout << "shape name : " << shape.name << std::endl;
        auto& mesh = shape.mesh;
        std::cout << "mesh indices size : " << mesh.indices.size() << std::endl;
        std::cout << "mesh material ids (" << mesh.material_ids.size() << ") : ";
        for (auto& materialId : mesh.material_ids) {
            std::cout <<  materialId << ", ";
        }
        std::cout << std::endl;
        std::cout << "mesh smoothing group ids (" << mesh.smoothing_group_ids.size() << ") : ";
        for (auto& smoothingGroupId : mesh.smoothing_group_ids) {
            std::cout << smoothingGroupId << ", ";
        }
        std::cout << std::endl;
        std::cout << "mesh num faces vertices (" << mesh.num_face_vertices.size() << ") : ";
        for (auto& numFaceVertex : mesh.num_face_vertices) {
            std::cout << numFaceVertex << ", ";
        }
        std::cout << std::endl;
    }
}