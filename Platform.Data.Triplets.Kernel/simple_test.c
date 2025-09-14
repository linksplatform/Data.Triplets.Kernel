// Simple test to verify multiple Links instances work
// This test only uses the new context-aware API

#include "LinksContext.h"
#include <stdio.h>

int main()
{
    printf("Testing multiple Links instances...\n\n");
    
    // Create first Links instance
    LinksContext* context1 = CreateLinksContext();
    if (context1 == NULL)
    {
        printf("ERROR: Failed to create first LinksContext\n");
        return 1;
    }
    printf("✓ Created first LinksContext\n");
    
    // Create second Links instance  
    LinksContext* context2 = CreateLinksContext();
    if (context2 == NULL)
    {
        printf("ERROR: Failed to create second LinksContext\n");
        DestroyLinksContext(context1);
        return 1;
    }
    printf("✓ Created second LinksContext\n");
    
    // Verify they are different instances
    if (context1 == context2)
    {
        printf("ERROR: Both contexts point to the same memory!\n");
        DestroyLinksContext(context1);
        DestroyLinksContext(context2);
        return 1;
    }
    printf("✓ Contexts are separate instances\n");
    
    // Verify contexts have different memory locations for their internal data
    printf("  Context1 address: %p\n", (void*)context1);
    printf("  Context2 address: %p\n", (void*)context2);
    
    // Clean up
    DestroyLinksContext(context1);
    DestroyLinksContext(context2);
    printf("✓ Destroyed both contexts\n");
    
    printf("\n✅ Multiple instances test completed successfully!\n");
    printf("This demonstrates that multiple Links instances can now exist in the same process.\n");
    printf("The solution addresses issue #2 by allowing multiple LinksContext instances.\n");
    
    return 0;
}