
#include "TimerStage.h"

#include <cppassist/logging/logging.h>


using namespace cppassist;


CPPEXPOSE_COMPONENT(TimerStage, gloperate::Stage)


TimerStage::TimerStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, "TimerStage", name)
, timeDelta  ("timeDelta",   this, 0.0f)
, virtualTime("virtualTime", this, 0.0f)
, m_timer(environment)
, m_time(0.0f)
{
    // Setup timer
    m_timer.elapsed.connect([this] ()
    {
        // Update virtual time
        m_time += *timeDelta;
        virtualTime.setValue(m_time);
    });

    m_timer.start(0.0f);
}

TimerStage::~TimerStage()
{
}

void TimerStage::onContextInit(gloperate::AbstractGLContext *)
{
}

void TimerStage::onContextDeinit(gloperate::AbstractGLContext *)
{
}

void TimerStage::onProcess(gloperate::AbstractGLContext *)
{
}