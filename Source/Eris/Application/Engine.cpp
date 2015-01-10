//
// Copyright (c) 2013-2015 the Eris project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include "Engine.h"
#include "Events.h"
#include "Locale.h"
#include "Settings.h"

#include "Core/Clock.h"
#include "Core/Log.h"
#include "Collections/Functions.h"
#include "Graphics/Graphics.h"
#include "Input/Input.h"
#include "IO/FileSystem.h"
#include "Resource/ResourceCache.h"

#include "../gitversion.h"

#include <VersionHelpers.h>

namespace Eris
{
    Engine::Engine(Context* context) :
        Object(context),
        m_exitcode(EXIT_OK),
        m_exiting(false)
    {        
        context->registerModule(new Log(context));
        context->registerModule(new Clock(context));
        context->registerModule(this);
        context->registerModule(new FileSystem(context));
        context->registerModule(new Graphics(context));
        context->registerModule(new Input(context));
        context->registerModule(new Locale(context));
        context->registerModule(new ResourceCache(context));
        context->registerModule(new Settings(context));

        subscribeToEvent(ExitRequestedEvent::getTypeStatic(), HANDLER(Engine, handleExitRequest));
    }

    void Engine::initialize()
    {
        FileSystem* fs = m_context->getModule<FileSystem>();
        Log* log = m_context->getModule<Log>();
        ResourceCache* rc = m_context->getModule<ResourceCache>();
        Graphics* graphics = m_context->getModule<Graphics>();
        Input* input = m_context->getModule<Input>();
        Settings* settings = m_context->getModule<Settings>();
        Locale* locale = m_context->getModule<Locale>();

        log->open(fs->getApplicationPreferencesDir() /= "log.log");

        logSystemInfo();

        fs->addPath(fs->getApplicationPreferencesDir());
        fs->addPath(fs->getDocumentsDir());
        fs->addPath(fs->getProgramDir());

        rc->initialize();
        rc->addDirectory(fs->getApplicationPreferencesDir());
        rc->addDirectory(fs->getProgramDir());
        rc->addDirectory(fs->getProgramDir() /= "Data");

        settings->load();
        locale->load(settings->getString("General/Language", "enGB"));

        if (!glfwInit())
        {
            setExitCode(EXIT_GLFW_INIT_ERROR);
            return;
        }

        graphics->setSize(settings->getI32("Graphics/Width", 0), settings->getI32("Graphics/Height", 0));
        graphics->setFullscreen(settings->getBool("Graphics/Fullscreen", true));
        graphics->setBorderless(settings->getBool("Graphics/Borderless", false));
        graphics->setResizable(settings->getBool("Graphics/Resizable", false));
        graphics->setVSync(settings->getBool("Graphics/VSync", true));
        graphics->setSamples(settings->getI32("Graphics/Multisamples", 4));
        graphics->initialize();

        input->initialize();
    }

    void Engine::run()
    {
        Graphics* graphics = m_context->getModule<Graphics>();
        Clock* clock = m_context->getModule<Clock>();
        Input* input = m_context->getModule<Input>();

        glm::f64 current_time = clock->getElapsedTime();

        while (!m_exiting)
        {
            glm::f64 new_time = clock->getElapsedTime();
            glm::f64 elapsed = new_time - current_time;
            current_time = new_time;

            clock->beginFrame(elapsed);

            UpdateEvent* event = m_context->getFrameAllocator().newInstance<UpdateEvent>();
            event->time_step = elapsed;

            sendEvent(UpdateEvent::getTypeStatic(), event);
            sendEvent(PostUpdateEvent::getTypeStatic());

            glfwSwapBuffers(graphics->getWindow());

            clock->endFrame();
            m_context->resetFrameAllocator();
        }
    }

    void Engine::terminate()
    {
        Graphics* graphics = m_context->getModule<Graphics>();
        ResourceCache* rc = m_context->getModule<ResourceCache>();
        Clock* clock = m_context->getModule<Clock>();
        Settings* settings = m_context->getModule<Settings>();

        graphics->terminate();
        rc->terminate();

        glm::f64 duration = clock->getElapsedTime() / 1000.0;
        glm::u64 frames = clock->getFrameNumber();

        glfwTerminate();

        settings->save();

        Log::rawf("Terminating - Ran for %d frames over %.2f seconds.", frames, duration);
    }

    const char* Engine::getVersion() const
    {
        return ERIS_VERSION;
    }

    void Engine::setExitCode(glm::i32 exitcode)
    {
        m_exitcode = exitcode;
    }

    void Engine::handleExitRequest(const StringHash& type, const Event* event)
    {
        m_exiting = true;
    }

    void Engine::logSystemInfo()
    {
        Log::raw("Initializing...");
        Log::rawf("\tVersion: %s", std::string_upper(std::string(getVersion())).c_str());

        SYSTEM_INFO sys_info;
        BOOL is_64 = FALSE;

#ifdef _WIN64
        is_64 = TRUE;
        GetSystemInfo(&sys_info);
#else
        if (IsWow64Process(GetCurrentProcess(), &is_64))
            GetNativeSystemInfo(&sys_info);
        else
            GetSystemInfo(&sys_info);
#endif

        if (is_64)
            Log::raw("\tPlatform: x64");
        else
            Log::raw("\tPlatform: Win32");

        if (IsWindows8Point1OrGreater())
            Log::raw("\tOS: Windows 8.1");
        else if (IsWindows8OrGreater())
            Log::raw("\tOS: Windows 8");
        else if (IsWindows7OrGreater())
            Log::raw("\tOS: Windows 7");
        else if (IsWindowsVistaOrGreater())
            Log::raw("\tOS: Windows Vista");
        else if (IsWindowsXPOrGreater())
            Log::raw("\tOS: Windows XP");
        else
            Log::raw("\tOS: Unsupported");

        glm::i32 cpu_info[4] = { -1 };
        glm::i32 n_ex_ids, i = 0;
        char cpu_brand_string[0x40];
        // Get the information associated with each extended ID.
        __cpuid(cpu_info, 0x80000000);
        n_ex_ids = cpu_info[0];
        for (i = 0x80000000; i <= n_ex_ids; ++i)
        {
            __cpuid(cpu_info, i);
            // Interpret CPU brand string
            if (i == 0x80000002)
                memcpy(cpu_brand_string, cpu_info, sizeof(cpu_info));
            else if (i == 0x80000003)
                memcpy(cpu_brand_string + 16, cpu_info, sizeof(cpu_info));
            else if (i == 0x80000004)
                memcpy(cpu_brand_string + 32, cpu_info, sizeof(cpu_info));
        }
        //string includes manufacturer, model and clockspeed
        Log::rawf("\tCPU: %s", std::string_ltrim(std::string(cpu_brand_string)).c_str());

        Log::rawf("\t\tCores: %d", sys_info.dwNumberOfProcessors);

        if (sys_info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 || sys_info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
            Log::raw("\t\tArchitecture: x64");
        else
            Log::raw("\t\tArchitecture: x86");

        MEMORYSTATUSEX state_ex;
        state_ex.dwLength = sizeof(state_ex);
        GlobalMemoryStatusEx(&state_ex);
        Log::rawf("\tMemory: %d%s", (state_ex.ullTotalPhys / 1024) / 1024, "MB");
    }

}
