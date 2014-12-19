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

#pragma once

#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>

namespace Eris
{
    // Event handling forward decls.
    class Object;
    class EventHandler;

    // Module forward decls.
    class Graphics;
    class Input;
    class Time;

    class Context
    {
        friend class Object;

    public:
        Context();
        virtual ~Context();

        Object* getEventSender() const;
        EventHandler* getEventHandler() const { return eventHandler_; }

        Graphics* getGraphics() { return graphics; }
        Input* getInput() { return input; }
        Time* getTime() { return time; }

        template<class T>
        T* createEvent()
        {
            return nullptr;
        }

    protected:
        Graphics* graphics;
        Input* input;
        Time* time;

    private:
        void addEventReceiver(Object* reciever, const std::string& eventType);
        void addEventReceiver(Object* reciever, Object* sender, const std::string& eventType);

        void removeEventReceiver(Object* reciever, const std::string& eventType);
        void removeEventReceiver(Object* reciever, Object* sender, const std::string& eventType);
        void removeEventSender(Object* sender);

        std::unordered_set<Object*>* getEventReceivers(const std::string& eventType);
        std::unordered_set<Object*>* getEventReceivers(Object* sender, const std::string& eventType);
       
        void setEventHandler(EventHandler* handler) { eventHandler_ = handler; }
        void beginSendEvent(Object* sender) { eventSenders_.push_back(sender); }
        void endSendEvent() { eventSenders_.pop_back(); }

        std::unordered_map<std::string, std::shared_ptr<Object>> modules_;
        std::unordered_map<std::string, std::unordered_set<Object*>> eventRecievers_;
        std::unordered_map<Object*, std::unordered_map<std::string, std::unordered_set<Object*>>> specificEventRecievers_;
        std::vector<Object*> eventSenders_;
        EventHandler* eventHandler_;
    };
}