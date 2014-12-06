#include "Mutex.h"

namespace Eris
{ 
    void Mutex::Lock()
    {
        handle_.lock();
    }

    void Mutex::Unlock()
    {
        handle_.unlock();
    }

    MutexGuard::MutexGuard(Mutex& mutex) :
        mutex_(mutex_)
    {
        mutex_.Lock();
    }

    MutexGuard::~MutexGuard() _NOEXCEPT
    {
        mutex_.Unlock();
    }
}
