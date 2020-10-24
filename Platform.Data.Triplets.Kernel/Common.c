#include "Common.h"

#ifdef DEBUG    
    #define ERROR_MESSAGE(message)  printf("%s\n\n", message);
#else
    #define ERROR_MESSAGE(message)
#endif

#ifdef DEBUG
    #define ERROR_MESSAGE_WITH_CODE(message, errorCode)  printf("%s Error code: %" PRId64 ".\n\n", message, errorCode);
#else
    #define ERROR_MESSAGE_WITH_CODE(message, errorCode);
#endif

#ifdef DEBUG
    #define DEBUG_MESSAGE(message) printf("%s\n", message)
#else
    #define DEBUG_MESSAGE(message)
#endif



