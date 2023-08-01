#ifndef STRUCT_TYPEDEF_H
#define STRUCT_TYPEDEF_H

#if defined ( __ICCARM__ ) /*!< IAR Compiler */


#elif defined ( __CC_ARM )  /* MDK ARM Compiler */

typedef signed char int8_t;
typedef signed short int int16_t;
typedef signed int int32_t;
typedef signed long long int64_t;

/* exact-width unsigned integer types */
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef unsigned char bool_t;
typedef float fp32;
typedef double fp64;

#elif defined ( __GNUC__ ) /* GNU Compiler */ 
#include <stdint.h>

/* exact-width unsigned integer types */

typedef unsigned char bool_t;
typedef float fp32;
typedef double fp64;
#endif




#endif
