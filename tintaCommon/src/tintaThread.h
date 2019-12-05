/*  Copyright (C) 2011 - 2019 Mikhail Evdokimov  
    tintapix.com
    tintapix@gmail.com  */

#ifndef _TINTA_THREAD_H_
#define _TINTA_THREAD_H_

#include <thread>
#include <mutex>   
#include <atomic>

#include <condition_variable>
#include "tintaConfig.h"


#define  MUTEX_T std::mutex
#define  MUTEX_RECURSIVE_T std::recursive_mutex
#define  TINTA_LOCK_RECUR_MUTEX(name)  std::lock_guard<MUTEX_RECURSIVE_T> lock(name)
#define  TINTA_LOCK_RECUR_MUTEX_NAME(NAME, LOCK_NAME) std::lock_guard<MUTEX_RECURSIVE_T> LOCK_NAME(NAME)
#define  TINTA_LOCK_RECUR_MUTEX_AUTO std::lock_guard<MUTEX_RECURSIVE_T> lock(MUTEX_NAME)

#define  TINTA_LOCK_MUTEX(name)  std::lock_guard<MUTEX_T> lock(name)
#define  TINTA_LOCK_MUTEX_NAME(name, LOCK_NAME)  std::unique_lock<MUTEX_T> LOCK_NAME(name)
//#define  TINTA_LOCK_MUTEX_NAME_T(name, LOCK_NAME)  std::unique_lock<MUTEX_T> LOCK_NAME;

#define  AUTO_MUTEX mutable MUTEX_RECURSIVE_T MUTEX_NAME


#define THREAD_WAIT(sync, mutex, lock) sync.wait(lock)
#define THREAD_NOTIFY_ONE(sync) sync.notify_one() 
#define THREAD_NOTIFY_ALL(sync) sync.notify_all() 
#define TINTA_MUTEX(NAME) std::mutex NAME;
#define MUTEX_RECURSIVE(name) mutable MUTEX_RECURSIVE_T name;	

#define THREAD_HARDWARE_CONCURRENCY std::thread::hardware_concurrency()
#define THREAD_CURRENT_ID std::this_thread::get_id()
#define THREAD_CURRENT_ID_TYPE std::thread::id
#define THREAD_SLEEP(ms) std::this_thread::sleep_for(std::chrono::milliseconds(ms))
#define THREAD_SLEEP_MSG(ms, msg ) std::cout<<"sleep point: " << msg << "\n"; std::this_thread::sleep_for(std::chrono::milliseconds(ms))

#define THREAD_TYPE std::thread
#define THREAD_CREATE(name, worker) std::thread* name = new std::thread(worker) //NEW_T(boost::thread, MEMCATEGORY_GENERAL)(worker);
#define THREAD_DESTROY(name) delete name; //DELETE_T(name, thread, MEMCATEGORY_GENERAL)
#define THREAD_CONDITION(sync) std::condition_variable sync


class tintaSpinLock
{
private:
    std::atomic_flag spin_lock;
    std::atomic<bool> isLocked;

public:
    inline tintaSpinLock()
    {
        spin_lock.clear();
    }
    ~tintaSpinLock(){
        unlock();        
    }

    void lock()
    {   
        isLocked = true;
        while (spin_lock.test_and_set(std::memory_order_acquire))
        {
        }

    }

    void unlock()
    {        
        spin_lock.clear(std::memory_order_release);
        isLocked = false;
    }


};


struct tintaAutoSpinLock{

private:
    tintaAutoSpinLock() = delete;
    tintaAutoSpinLock(const tintaAutoSpinLock& v) = delete;
    tintaAutoSpinLock& operator=(const tintaAutoSpinLock&) = delete;
    tintaSpinLock &mLock;
public:
    tintaAutoSpinLock(tintaSpinLock &lock) :mLock(lock){
        mLock.lock();
    }
    ~tintaAutoSpinLock(){ mLock.unlock(); }
};

/*
    Keeps atomic reference on value  and changes on destroy
*/
template<typename T>
struct AtomicKeeper {
    AtomicKeeper() = delete;
    AtomicKeeper(const AtomicKeeper& v) = delete;
    AtomicKeeper& operator=(const AtomicKeeper&) = delete;

    AtomicKeeper(std::atomic<T> &val, T valOnDel) :
        Val(val),
        mValOnDel(valOnDel) {}

    ~AtomicKeeper() {
        Val.store(mValOnDel);
        std::cout << Val << "\n";
    }

    std::atomic<T> &Val;
    T mValOnDel;

};




#endif
