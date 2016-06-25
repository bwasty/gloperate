
#include <gloperate/base/Canvas.h>

#include <cppassist/logging/logging.h>

#include <gloperate/base/Environment.h>
#include <gloperate/base/TimeManager.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/input/MouseDevice.h>
#include <gloperate/input/KeyboardDevice.h>
#include <gloperate/tools/AbstractVideoExporter.h>
#include <gloperate/tools/ImageExporter.h>


using namespace cppassist;


namespace gloperate
{


Canvas::Canvas(Environment * environment)
: AbstractCanvas(environment)
, m_pipelineContainer(environment)
, m_frame(0)
, m_mouseDevice(new MouseDevice(m_environment->inputManager(), "Canvas"))
, m_keyboardDevice(new KeyboardDevice(m_environment->inputManager(), "Canvas"))
, m_videoExporter(nullptr)
, m_imageExporter(nullptr)
, m_requestVideo(false)
{
    // Mark render output as required and redraw when it is invalidated
    m_pipelineContainer.rendered.setRequired(true);
    m_pipelineContainer.rendered.valueChanged.connect([this] (bool)
    {
        if (!m_pipelineContainer.rendered.isValid()) {
            this->redraw();
        }
    });
}

Canvas::~Canvas()
{
    if (m_videoExporter) delete m_videoExporter;
    if (m_imageExporter) delete m_imageExporter;
}

Pipeline * Canvas::rootPipeline() const
{
    return static_cast<Pipeline *>(const_cast<PipelineContainer *>(&m_pipelineContainer));
}

Stage * Canvas::renderStage() const
{
    return m_pipelineContainer.renderStage();
}

void Canvas::setRenderStage(Stage * stage)
{
    // De-initialize render stage
    if (m_pipelineContainer.renderStage() && m_openGLContext)
    {
        m_pipelineContainer.renderStage()->deinitContext(m_openGLContext);
    }

    // Set new render stage
    m_pipelineContainer.setRenderStage(stage);

    // Initialize new render stage
    if (m_pipelineContainer.renderStage() && m_openGLContext)
    {
        m_pipelineContainer.renderStage()->initContext(m_openGLContext);
    }
}

glm::vec4 Canvas::deviceViewport()
{
    return m_pipelineContainer.deviceViewport.value();
}

glm::vec4 Canvas::virtualViewport()
{
    return m_pipelineContainer.virtualViewport.value();
}

void Canvas::onUpdate()
{
    float timeDelta = m_environment->timeManager()->timeDelta();

    m_pipelineContainer.timeDelta.setValue(timeDelta);
}

void Canvas::onContextInit()
{
    cppassist::details() << "onContextInit()";

    // Initialize render stage in new context
    if (m_pipelineContainer.renderStage())
    {
        m_pipelineContainer.renderStage()->initContext(m_openGLContext);
    }

    if (!m_imageExporter)
    {
        m_imageExporter = new ImageExporter(this);
    }
}

void Canvas::onContextDeinit()
{
    cppassist::details() << "onContextDeinit()";

    // De-initialize render stage in old context
    if (m_pipelineContainer.renderStage())
    {
        m_pipelineContainer.renderStage()->deinitContext(m_openGLContext);
    }
}

void Canvas::onViewport(const glm::vec4 & deviceViewport, const glm::vec4 & virtualViewport)
{
    m_pipelineContainer.deviceViewport.setValue(deviceViewport);
    m_pipelineContainer.virtualViewport.setValue(virtualViewport);
}

void Canvas::onBackgroundColor(float red, float green, float blue)
{
    m_pipelineContainer.backgroundColor.setValue(glm::vec3(red, green, blue));
}

void Canvas::onRender(globjects::Framebuffer * targetFBO)
{
    cppassist::details() << "onRender()";

    // [TODO] This is necessary, because the actual render call (which will be called from the video/image exporter)
    // has to come from within the render thread.
    if (m_requestVideo)
    {
        m_requestVideo = false;
        m_videoExporter->createVideo([this] (int, int)
        {
            this->wakeup();
        }, true);
    }

    // [TODO] see above
    if (m_requestImage)
    {
        m_requestImage = false;
        m_imageExporter->save(true);
    }

    if (m_pipelineContainer.renderStage())
    {
        m_frame++;

        m_pipelineContainer.frameCounter.setValue(m_frame);
        m_pipelineContainer.targetFBO.setValue(targetFBO);

        m_pipelineContainer.renderStage()->process(m_openGLContext);
    }
}

void Canvas::onKeyPress(int key, int modifier)
{
    cppassist::details() << "onKeyPressed(" << key << ")";

    m_keyboardDevice->keyPress(key, modifier);
}

void Canvas::onKeyRelease(int key, int modifier)
{
    cppassist::details() << "onKeyReleased(" << key << ")";

    m_keyboardDevice->keyRelease(key, modifier);
}

void Canvas::onMouseMove(const glm::ivec2 & pos)
{
    cppassist::details() << "onMouseMoved(" << pos.x << ", " << pos.y << ")";

    m_mouseDevice->move(pos);
}

void Canvas::onMousePress(int button, const glm::ivec2 & pos)
{
    cppassist::details() << "onMousePressed(" << button << ", " << pos.x << ", " << pos.y << ")";

    m_mouseDevice->buttonPress(button, pos);
}

void Canvas::onMouseRelease(int button, const glm::ivec2 & pos)
{
    cppassist::details() << "onMouseReleased(" << button << ", " << pos.x << ", " << pos.y << ")";

    m_mouseDevice->buttonRelease(button, pos);
}

void Canvas::onMouseWheel(const glm::vec2 & delta, const glm::ivec2 & pos)
{
    cppassist::details() << "onMouseWheel(" << delta.x << ", " << delta.y << ", " << pos.x << ", " << pos.y << ")";

    m_mouseDevice->wheelScroll(delta, pos);
}

void Canvas::createVideo(std::string filename, int width, int height, int fps, int seconds, std::string backend)
{
    auto component = m_environment->componentManager()->component<AbstractVideoExporter>(backend);
    if (!component) return;

    if (m_videoExporter) delete m_videoExporter;
    m_videoExporter = component->createInstance();

    m_videoExporter->init(filename, this, width, height, fps, seconds);
    m_requestVideo = true;
}

void Canvas::exportImage(std::string filename, int width, int height, int renderIterations)
{
    m_imageExporter->init(filename, width, height, renderIterations);
    m_requestImage = true;
}

int Canvas::exportProgress()
{
    if (!m_videoExporter) return 0;

    return m_videoExporter->progress();
}

cppexpose::VariantArray Canvas::videoExporterPlugins()
{
    cppexpose::VariantArray plugins;
    for (auto component : m_environment->componentManager()->components())
    {
        if (strcmp(component->type(), "gloperate::AbstractVideoExporter") == 0)
        {
            plugins.push_back(cppexpose::Variant(component->name()));
        }
    }
    return plugins;
}


} // namespace gloperate