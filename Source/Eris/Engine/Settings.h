//
// Copyright (c) 2008-2015 the Eris project.
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

#pragma once

#include "Core/Object.h"
#include "Core/Context.h"
#include "Resource/INIFile.h"

namespace Eris
{
    class Settings : public Object
    {
    public:
        Settings(Context* context);

        void load();
        void save();

        bool getBool(const std::string& path, bool default);
        glm::i32 getI32(const std::string& path, glm::i32 default);
        glm::i64 getI64(const std::string& path, glm::i64 default);
        glm::f32 getF32(const std::string& path, glm::f32 default);
        glm::f64 getF64(const std::string& path, glm::f64 default);
        std::string getString(const std::string& path, std::string default);

        void setBool(const std::string& path, bool value);
        void setI32(const std::string& path, glm::i32 value);
        void setI64(const std::string& path, glm::i64 value);
        void setF32(const std::string& path, glm::f32 value);
        void setF64(const std::string& path, glm::f64 value);
        void setString(const std::string& path, std::string value);

    private:
        std::pair<std::string, std::string> getSectionKey(const std::string& path);

        SharedPtr<INIFile> m_ini;
    };

    template<> inline void Context::registerModule(Settings* module)
    {
        m_settings = SharedPtr<Settings>(module);
    }

    template<> inline Settings* Context::getModule()
    {
        ERIS_ASSERT(m_settings);
        return m_settings.get();
    }
}
