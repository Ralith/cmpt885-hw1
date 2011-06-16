/**
 * File ryatomic.h
 * author: Ryan Dickie
 * brief: atomic primitives
 */

#ifndef RYATOMIC_H
#define RYATOMIC_H

#ifndef WINDOWS
    #define int int
#else
    #define int LONG
#endif

//my big scheme for multi-platform support
#ifdef WINDOWS
    #include <Windows.h>
    #include <Winnt.h>
    static inline bool cas(int volatile* ptr, int old, int _new)
    {
        return InterlockedCompareExchange(ptr, _new, old) == old;
    }
    
//M$ says "This function is supported only on Itanium Processor Family (IPF)."
//retarded! lock;xadd is the x86 assembly instruction. in fact the gcc
//version down below doesn't have a native increment, only an add
//because (it supports risc processors)
/*    
    static inline int atomic_add(int volatile* ptr, int val)
    {
        return InterlockedAdd(ptr,(int)val);
    }
*/
    
    static inline int atomic_inc(int volatile* ptr)
    {
        return InterlockedIncrement(ptr);
    }
    
    static inline int atomic_dec(int volatile* ptr)
    {
        return InterlockedDecrement(ptr);
    }


#else
#if defined(__APPLE__) && defined(__MACH__)
    //leopard still uses older gcc that doesn't have the atomic
    //intrinsics so i use mach's instead.
	#include </usr/include/libkern/OSAtomic.h>
    static inline bool cas(int volatile* ptr, int old, int _new)
    {
        return OSAtomicCompareAndSwapIntBarrier(old,_new,ptr);
    }
    //returns the new value
    static inline int atomic_add(int volatile* ptr, int val)
    {
        return OSAtomicAdd32(val,ptr);
    }
    
    static inline int atomic_inc(int volatile* ptr)
    {
        return OSAtomicIncrement32(ptr);
    }
    
    static inline int atomic_dec(int volatile* ptr)
    {
        return OSAtomicDecrement32(ptr);
    }    
#else
#ifdef ICC_TM
	static inline bool cas(int volatile* ptr, int old, int _new) {
		//int _InterlockedCompareExchange(volatile int *Destination, int Exchange, int Comparand)
		return _InterlockedCompareExchange((void*)ptr, _new, old);
	}

	static inline int atomic_add(int volatile* ptr, int val) {
		return _InterlockedExchangeAdd((void*)ptr, val);
	}

	static inline int atomic_inc(int volatile* ptr) {
		return _InterlockedIncrement((void*)ptr);
	}

	static inline int atomic_dec(int volatile* ptr) {
		return _InterlockedDecrement((void*)ptr);
	}

#else
    //GCC
	//inline

	static inline bool cas(int volatile* ptr, int old, int _new){ 
		return __sync_bool_compare_and_swap(ptr,old,_new);
	}

	static inline bool cas( void volatile** ptr, void* old, void* _new){ 
		return __sync_bool_compare_and_swap(ptr,old,_new);
	}


	static inline int atomic_add(int volatile* ptr, int val) {
        return __sync_add_and_fetch(ptr,val);
    }
    
    static inline int atomic_inc(int volatile* ptr) {
        return __sync_add_and_fetch(ptr,1);
    }
    
    static inline int atomic_dec(int volatile* ptr) {
        return __sync_sub_and_fetch(ptr,1);
    }
#endif //ICC_TM
#endif // (__APPLE__) && defined(__MACH__)
#endif // WINDOWS

    static inline int atomic_inc_mod(int volatile* ptr, int mod)
    {
        while (true)
        {
            int old = *ptr;
            int _new = (old+1)%mod;
            if (cas(ptr, old, _new))
                return _new;
        }
    }

    static inline void spinlock_acquire(int volatile* ptr)
    {
        while (!cas(ptr, 0,1))
        {
        }
    }

    static inline void spinlock_release(int volatile* ptr)
    {
        *ptr = 0;
    }

#endif //RYATOMIC_H
