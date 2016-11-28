#include <gloperate-glkernel/HemisphereDistributionKernelStage.h>

#include <algorithm>

#include <glm/vec3.hpp>

#include <glkernel/sample.h>
#include <glkernel/scale.h>
#include <glkernel/sort.h>

#include <glbinding/gl/enum.h>

#include <globjects/Texture.h>
#include <globjects/Buffer.h>


namespace gloperate_glkernel
{


CPPEXPOSE_COMPONENT(HemisphereDistributionKernelStage, gloperate::Stage)


HemisphereDistributionKernelStage::HemisphereDistributionKernelStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, name)
, kernelSize("kernelSize", this, 1)
, regenerate("regenerate", this, true)
, kernel("kernel", this)
, texture("texture", this)
, m_texture(nullptr)
{
}


void HemisphereDistributionKernelStage::resizeKernel()
{
    m_kernel = glkernel::kernel3(static_cast<std::uint16_t>(*kernelSize));
}


void HemisphereDistributionKernelStage::regenerateKernel()
{
    glkernel::sample::hammersley_sphere(m_kernel);
    glkernel::sort::distance(m_kernel, {0.0f, 0.0f, 1.0f});
}


void HemisphereDistributionKernelStage::onContextInit(gloperate::AbstractGLContext * context)
{
    m_texture = new globjects::Texture(gl::GL_TEXTURE_2D);
}


void HemisphereDistributionKernelStage::onProcess(gloperate::AbstractGLContext * context)
{
    if (!m_texture)
    {
        onContextInit(context);
    }

    bool regenKernel = *regenerate;
    if (*kernelSize != m_kernel.extent().x)
    {
        resizeKernel();
        regenKernel = true;
    }

    if (regenKernel)
    {
        regenerateKernel();

        m_texture->image1D(0, gl::GL_RGB32F, *kernelSize, 0, gl::GL_RGB, gl::GL_FLOAT, m_kernel.data());

        kernel.setValue(m_kernel);
        texture.setValue(m_texture);
    }
}


} // namespace gloperate_glkernel
