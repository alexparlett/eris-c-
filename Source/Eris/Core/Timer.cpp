#include "Timer.h"

namespace Eris
{
    Timer::Timer() :
        m_start_time(glfwGetTime())
    {
    }

    glm::f64 Timer::elapsed(bool reset /*= false*/)
    {
        glm::f64 current_time = glfwGetTime();
        return m_start_time - current_time;
    }

    void Timer::reset()
    {
        m_start_time = glfwGetTime();
    }
}