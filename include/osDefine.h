#ifndef OSDEFINE_FILE_H
#define OSDEFINE_FILE_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define OS_WINDOWS				/**Windows*/	
#else
#define OS_LINUX				/**Linux*/
#endif

#endif