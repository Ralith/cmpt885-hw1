/*
 *  ryatomic.cpp
 *  Cascade-test
 *
 *  Created by Craig Mustard on 08/10/09.
 *  Copyright 2009 Simon Fraser University. All rights reserved.
 *
 */
#include "ryatomic.h"
#include <iostream>

#ifdef WINDOWS
//nothing
#else
#if defined(__APPLE__) && defined(__MACH__)

#else
long casFailures = 0;
long casSuccesses = 0;

//GCC
bool cas(int volatile* ptr, int old, int _new)
{
	#if defined(_TM)
	__tm_atomic {
		if(*ptr == old){
			*ptr = _new;
			return true;
		} else{
			return false;
		}
	}
	#else
	if(__sync_bool_compare_and_swap(ptr,old,_new)){
			casSuccesses++;
			return true;
		} else {
			casFailures++;
			return false;
	}
	#endif
	//return __sync_bool_compare_and_swap(ptr,old,_new);
}

long printCasStatus(){
	long cs = getCasSuccesses();
	long cf = getCasFailures();
	long ct = cs+cf;
	double sucRatio = (double)cs / (double)ct;
	std::cout << "Cas Successes: " << cs << " Fails: " << cf << " Success Ratio: " << sucRatio << std::endl;	
}

void resetCasStatus(){
	casFailures = 0;
	casSuccesses = 0;
}

long getCasFailures(){
	return casFailures;
}

long getCasSuccesses(){
	return casSuccesses;
}

#endif
#endif