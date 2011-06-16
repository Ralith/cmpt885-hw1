#include "../padded_type.hpp"
#include "../ryatomic.h"

#define MAX_THREADS 64

class Lock
{
        padded_type::ptype<int>
                wait_pos,
                lock_pos,
                spinners[MAX_THREADS]
        ;

public:
        Lock()
        {
                wait_pos.value=-1;
                lock_pos.value=0;

                /* initialize the structure as [T][F]...[F] */
                spinners[0].value=true;
                for(u_int8_t i=1;i<MAX_THREADS;i++) spinners[i].value=false;
        }

        inline void lock()
        {
                /* increment and return spin position */
                int local_wait_pos=atomic_inc(&wait_pos.value)%MAX_THREADS;
                if(local_wait_pos<0) local_wait_pos+=MAX_THREADS;

                /* spin */
                while(!spinners[local_wait_pos].value);

                /* free spin position */
                spinners[local_wait_pos].value=false;
        }

        inline void unlock()
        {
                /* increment global pointer */
                lock_pos.value=(lock_pos.value+1)%MAX_THREADS;

                /* "signal" the thread waiting on lock_pos */
                spinners[lock_pos.value].value=true;
        }
};

