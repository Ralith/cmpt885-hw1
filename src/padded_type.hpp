#ifndef PADDEDTYPE_HPP
#define PADDEDTYPE_HPP

#include <stdint.h>

#define PADDEDTYPE_CACHE_SIZE (64)

namespace padded_type {

	template<class T>
	class __attribute__((aligned(PADDEDTYPE_CACHE_SIZE))) ptype
	{
		#pragma pack(push,1)
		private: volatile uint8_t left[PADDEDTYPE_CACHE_SIZE/2 - sizeof(T)];
		public:  volatile T value;
		private: volatile uint8_t right[PADDEDTYPE_CACHE_SIZE/2];
		#pragma pack(pop)

		/*public:
		inline T& operator=(const T &rhs) {
			value = rhs;
	        return rhs;
		}

		inline operator T() {
			return value;
		}*/
	};

} // end namespace padded_type

#endif
