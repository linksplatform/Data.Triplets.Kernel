#include "Common.h"

#ifdef DEBUG
    signed_integer Error(char* message)
    {
        printf("%s\n\n", message);
        return ERROR_RESULT;
    }
#else
    #define Error(message) (ERROR_RESULT)
#endif

#ifdef DEBUG
    signed_integer ErrorWithCode(char* message, signed_integer errorCode)
    {
        printf("%s Error code: %" PRId64 ".\n\n", message, errorCode);
        return SUCCESS_RESULT == errorCode ? ERROR_RESULT : errorCode;
    }
#else
    #define ErrorWithCode(message, errorCode) (ERROR_RESULT)
#endif

#ifdef DEBUG
    void DebugInfo(char* message)
    {
        printf("%s\n", message);
    }
#endif
