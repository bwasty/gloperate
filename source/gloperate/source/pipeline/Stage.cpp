
#include <gloperate/pipeline/Stage.h>


namespace gloperate
{


Stage::Stage(ViewerContext * viewerContext)
: m_viewerContext(viewerContext)
{
}

Stage::~Stage()
{
}

ViewerContext * Stage::viewerContext() const
{
    return m_viewerContext;
}

void Stage::initContext(AbstractGLContext * context)
{
    onContextInit(context);
}

void Stage::deinitContext(AbstractGLContext * context)
{
    onContextDeinit(context);
}

void Stage::process(AbstractGLContext * context)
{
    onProcess(context);
}

void Stage::onContextInit(AbstractGLContext *)
{
}

void Stage::onContextDeinit(AbstractGLContext *)
{
}

void Stage::onProcess(AbstractGLContext *)
{
}


} // namespace gloperate
