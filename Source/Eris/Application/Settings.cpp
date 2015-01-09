//
// Copyright (c) 2008-2014 the Eris project.
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

#include "Settings.h"

#include "Core/Log.h"
#include "Collections/Functions.h"
#include "IO/File.h"
#include "IO/FileSystem.h"
#include "Memory/Pointers.h"
#include "Resource/ResourceCache.h"

namespace Eris
{
    Settings::Settings(Context* context) :
        Object(context),
        m_ini(nullptr)
    {
    }

    void Settings::load()
    {
        ResourceCache* rc = m_context->getModule<ResourceCache>();
        m_ini = rc->getResource<INIFile>("settings.ini");
    }

    void Settings::save()
    {
        FileSystem* fs = m_context->getModule<FileSystem>();

        SharedPtr<File> file = SharedPtr<File>(new File(m_context, fs->getApplicationPreferencesDir() /= "settings.ini", FileMode::WRITE));
        m_ini->save(*file);
    }

    bool Settings::getBool(const std::string& path, bool default)
    {
        std::pair<std::string, std::string> section_key = getSectionKey(path);
        return m_ini->getSection(section_key.first)->getBool(section_key.second, default);
    }

    glm::i32 Settings::getI32(const std::string& path, glm::i32 default)
    {
        std::pair<std::string, std::string> section_key = getSectionKey(path);
        return m_ini->getSection(section_key.first)->getI32(section_key.second, default);
    }

    glm::i64 Settings::getI64(const std::string& path, glm::i64 default)
    {
        std::pair<std::string, std::string> section_key = getSectionKey(path);
        return m_ini->getSection(section_key.first)->getI64(section_key.second, default);
    }

    glm::f32 Settings::getF32(const std::string& path, glm::f32 default)
    {
        std::pair<std::string, std::string> section_key = getSectionKey(path);
        return m_ini->getSection(section_key.first)->getF32(section_key.second, default);
    }

    glm::f64 Settings::getF64(const std::string& path, glm::f64 default)
    {
        std::pair<std::string, std::string> section_key = getSectionKey(path);
        return m_ini->getSection(section_key.first)->getF64(section_key.second, default);
    }

    std::string Settings::getString(const std::string& path, std::string default)
    {
        std::pair<std::string, std::string> section_key = getSectionKey(path);
        return m_ini->getSection(section_key.first)->getString(section_key.second, default);
    }

    void Settings::setBool(const std::string& path, bool value)
    {
        std::pair<std::string, std::string> section_key = getSectionKey(path);
        m_ini->getSection(section_key.first)->setValue(section_key.second, value ? "true" : "false");
    }

    void Settings::setI32(const std::string& path, glm::i32 value)
    {
        std::pair<std::string, std::string> section_key = getSectionKey(path);
        m_ini->getSection(section_key.first)->setValue(section_key.second, std::to_string(value));
    }

    void Settings::setI64(const std::string& path, glm::i64 value)
    {
        std::pair<std::string, std::string> section_key = getSectionKey(path);
        m_ini->getSection(section_key.first)->setValue(section_key.second, std::to_string(value));
    }

    void Settings::setF32(const std::string& path, glm::f32 value)
    {
        std::pair<std::string, std::string> section_key = getSectionKey(path);
        m_ini->getSection(section_key.first)->setValue(section_key.second, std::to_string(value));
    }

    void Settings::setF64(const std::string& path, glm::f64 value)
    {
        std::pair<std::string, std::string> section_key = getSectionKey(path);
        m_ini->getSection(section_key.first)->setValue(section_key.second, std::to_string(value));
    }

    void Settings::setString(const std::string& path, std::string value)
    {
        std::pair<std::string, std::string> section_key = getSectionKey(path);
        m_ini->getSection(section_key.first)->setValue(section_key.second, value);
    }

    std::pair<std::string,std::string> Settings::getSectionKey(const std::string& path)
    {
        std::size_t pos = path.find('/');
        if (pos == std::string::npos)
        {
            Log::errorf("Settings path invalid: %s", path.c_str());
        }
        return std::make_pair(path.substr(0, pos), path.substr(pos + 1, std::string::npos));
    }
}