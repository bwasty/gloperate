
#pragma once


#include <gloperate/resources/Loader.h>

#include <gloperate-assimp/gloperate-assimp_api.h>
#include <glm/mat4x2.hpp>

struct aiMesh;
struct aiScene;
struct aiLight;

namespace gloperate
{
    class Scene;
    class PolygonalGeometry;
    class Light;
}


namespace gloperate_assimp
{


/**
*  @brief
*    Loader for meshes (PolygonalGeometry) that uses ASSIMP for import
*
*  Supported options:
*    "smoothNormals" <bool>: Generate smooth normals
*/
class GLOPERATE_ASSIMP_API AssimpSceneLoader : public gloperate::Loader<gloperate::Scene>
{
public:
    /**
    *  @brief
    *    Constructor
    */
    AssimpSceneLoader();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AssimpSceneLoader();

    // Virtual loader methods
    virtual bool canLoad(const std::string & ext) const override;
    virtual std::vector<std::string> loadingTypes() const override;
    virtual std::string allLoadingTypes() const override;
    virtual gloperate::Scene * load(const std::string & filename, const reflectionzeug::Variant & options, std::function<void(int, int)> progress) const override;


protected:
    /**
    *  @brief
    *    Convert an ASSIMP scene into a gloperate scene
    *
    *  @param[in] scene
    *    ASSIMP scene (must be valid!)
    *
    *  @return
    *    Scene
    */
    gloperate::Scene * convertScene(const aiScene * scene) const;

    /**
    *  @brief
    *    Convert ASSIMP mesh into gloperate mesh
    *
    *  @param[in] mesh
    *    ASSIMP mesh (must be valid!)
    *
    *  @return
    *    Mesh, must be destroyed by the caller
    */
	gloperate::PolygonalGeometry * convertGeometry(const aiMesh * mesh, const glm::mat4 & transformation) const;
    gloperate::Light * convertLight(const aiScene * scene, const aiLight * light) const;
};


} // namespace gloperate_assimp
