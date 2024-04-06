#ifndef TYPES_VARIABLES_H
#define TYPES_VARIABLES_H

#if defined(__linux__) && defined(__x86_64__)
typedef signed char int8;
typedef unsigned char uint8;
typedef signed short int16;
typedef unsigned short uint16;
typedef signed int int32;
typedef unsigned int uint32;
typedef signed long int64;
typedef unsigned long uint64;
#else
typedef signed char int8;
typedef unsigned char uint8;
typedef signed short int16;
typedef unsigned short uint16;
typedef signed long int32;
typedef unsigned long uint32;
typedef signed long long int64;
typedef unsigned long long uint64;
#endif

#endif // TYPES_VARIABLES_H
