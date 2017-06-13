
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <glm/vec4.hpp>

#include <globjects/Texture.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/stages/interfaces/RenderInterface.h>
#include <gloperate/rendering/ScreenAlignedTriangle.h>

#include <gloperate-glkernel/gloperate-glkernel_api.h>


namespace gloperate_glkernel
{


/**
*  @brief
*    Stage that aggregates multiple subsequent frames into a single framebuffer
*/
class GLOPERATE_GLKERNEL_API MultiFrameAggregationStage : public gloperate::Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        MultiFrameAggregationStage, gloperate::Stage
      , ""
      , ""
      , ""
      , "Stage that aggregates multiple subsequent frames into a single texture"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v0.1.0"
    )


public:
    // Inputs
    Input<globjects::Texture *>  sourceTexture;     ///< Current frame texture
    Input<globjects::Texture *>  targetTexture;     ///< Aggregation target texture
    Input<glm::vec4>             viewport;          ///< Target viewport
    Input<float>                 aggregationFactor; ///< Weight of new frame in current aggregation

    // Outputs
    Output<globjects::Texture *> aggregatedTexture; ///< Aggregation target texture (passed for stage chaining)


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] environment
    *    Environment to which the stage belongs (must NOT be null!)
    *  @param[in] name
    *    Stage name
    */
    MultiFrameAggregationStage(gloperate::Environment * environment, const std::string & name = "MultiFrameAggregationStage");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~MultiFrameAggregationStage();


protected:
    // Virtual Stage interface
    virtual void onContextInit(gloperate::AbstractGLContext * context) override;
    virtual void onContextDeinit(gloperate::AbstractGLContext * context) override;
    virtual void onProcess() override;

protected:
    // Data
    std::unique_ptr<gloperate::ScreenAlignedTriangle> m_triangle; ///< Screen-aligned Triangle for 'blitting'
    std::unique_ptr<globjects::Framebuffer> m_fbo;                ///< Framebuffer for render targets
};


} // namespace gloperate_glkernel
