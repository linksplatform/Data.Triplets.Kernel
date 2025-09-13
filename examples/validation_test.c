#include "../Platform.Data.Triplets.Kernel/Common.h"
#include "../Platform.Data.Triplets.Kernel/PersistentMemoryManager.h"

int main()
{
    printf("Testing file consistency validation implementation...\n");
    
    InitPersistentMemoryManager();
    
    char* testFilename = "test_validation.links";
    
    printf("Opening links file: %s\n", testFilename);
    signed_integer result = OpenLinks(testFilename);
    
    if (succeeded(result))
    {
        printf("File opened successfully. Testing validation functions:\n");
        
        // Test the validation functions directly
        printf("\n1. Testing mapped links validation...\n");
        signed_integer mappedResult = ValidateMappedLinks();
        printf("Mapped links validation result: %s\n", succeeded(mappedResult) ? "SUCCESS" : "FAILED");
        
        printf("\n2. Testing freed links validation...\n");
        signed_integer freedResult = ValidateFreedLinks();
        printf("Freed links validation result: %s\n", succeeded(freedResult) ? "SUCCESS" : "FAILED");
        
        printf("\n3. Testing comprehensive links validation...\n");
        signed_integer allResult = ValidateAllLinks();
        printf("Comprehensive links validation result: %s\n", succeeded(allResult) ? "SUCCESS" : "FAILED");
        
        printf("\nLinks count: %" PRIu64 "\n", (uint64_t)GetLinksCount());
        
        printf("\nClosing links file...\n");
        CloseLinks();
        printf("File closed successfully.\n");
    }
    else
    {
        printf("Failed to open links file.\n");
        return 1;
    }
    
    printf("\nValidation test completed successfully!\n");
    return 0;
}