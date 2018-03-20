
#pragma once


#include <gloperate/base/AbstractGLContextFactory.h>

#include <gloperate-qt/gloperate-qt_api.h>


class QSurface;
class QSurfaceFormat;


namespace gloperate_qt
{


/**
*  @brief
*    OpenGL context factory
*/
class GLOPERATE_QT_API GLContextFactory : public gloperate::AbstractGLContextFactory
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] surface
    *    Surface on which contexts are created (must NOT be null)
    */
    GLContextFactory(QSurface * surface);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~GLContextFactory();

    // Virtual gloperate::AbstractGLContextFactory functions
    virtual std::unique_ptr<gloperate::AbstractGLContext> createContext(const gloperate::GLContextFormat & format) const override;

    /**
    *  @brief
    *    Convert context format description into Qt version
    *
    *  @param[in] format
    *    OpenGL context format description (gloperate version)
    *
    *  @return
    *    OpenGL context format description (Qt version)
    */
    static QSurfaceFormat toQSurfaceFormat(const gloperate::GLContextFormat & format);


protected:
    QSurface * m_surface; ///< Surface on which contexts are created
};


} // namespace gloperate_qt
