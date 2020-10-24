#include "Common.h"

#ifdef DEBUG    
    #define ERROR(message)  printf("%s\n\n", message);
#else
    #define ERROR(message)
#endif

#ifdef DEBUG
    #define ERROR_WITH_CODE(message, errorCode)  printf("%s Error code: %" PRId64 ".\n\n", message, errorCode);
#else
    #define ERROR_WITH_CODE(message, errorCode);
#endif

#ifdef DEBUG
    #define DEBUG(message) printf("%s\n", message)
#else
    #define DEBUG(message)
#endif



