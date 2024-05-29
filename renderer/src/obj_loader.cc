#include "renderer/obj_loader.hh"

#include "OBJ_Loader.hh"

ObjLoader::ObjLoader(const std::string& path) {
    objl::Loader loader;
    bool loadout = loader.LoadFile(path);
    if (!loadout) throw std::runtime_error("Failed to open " + path + ".");
    for (int i = 0; i < loader.LoadedMeshes.size(); i++) {
    }
}