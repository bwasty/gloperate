
#include <gloperate/stages/base/TextureLoadStage.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/viewer/ViewerContext.h>


namespace gloperate
{


TextureLoadStage::TextureLoadStage(ViewerContext * viewerContext, const std::string & name, Pipeline * parent)
: Stage(viewerContext, name, parent)
, filename(this, "filename")
, texture (this, "texture")
{
}

TextureLoadStage::~TextureLoadStage()
{
}

void TextureLoadStage::onContextInit(AbstractGLContext *)
{
}

void TextureLoadStage::onContextDeinit(AbstractGLContext *)
{
}

void TextureLoadStage::onProcess(AbstractGLContext *)
{
    // Check if texture needs to be rebuilt
    if (!texture.isValid())
    {
        // Load texture
        loadTexture();

        // Update outputs
        texture.setValue(m_texture.get());
    }
}

void TextureLoadStage::loadTexture()
{
    // Load texture from file
    m_texture = m_viewerContext->resourceManager()->load<globjects::Texture>(
        *filename
    );
}


CPPEXPOSE_COMPONENT(
    TextureLoadStage, gloperate::Stage
  , ""   // Tags
  , ""   // Icon
  , ""   // Annotations
  , "Stage that loads a texture from a file"
  , GLOPERATE_AUTHOR_ORGANIZATION
  , "v1.0.0"
)


} // namespace gloperate