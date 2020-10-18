#include "Common.h"

signed_integer Error(char* message)
{
    #ifdef DEBUG
        printf("%s\n\n", message);
        return ERROR_RESULT;
    #else
        #define Error(message) (ERROR_RESULT)
    #endif
}

signed_integer ErrorWithCode(char* message, signed_integer errorCode)
{
    #ifdef DEBUG
        printf("%s Error code: %" PRId64 ".\n\n", message, errorCode);
        return SUCCESS_RESULT == errorCode ? ERROR_RESULT : errorCode;
    #else
        #define ErrorWithCode(message, errorCode) (ERROR_RESULT)
    #endif
}

void DebugInfo(char* message)
{
    #ifdef DEBUG
        printf("%s\n", message);
    #else
        ;
    #endif
}

