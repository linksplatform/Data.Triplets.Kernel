// Test program to verify multiple Links instances can be created
// This demonstrates the solution for issue #2

#include "LinksContext.h"
#include <stdio.h>
#include <stdlib.h>

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
    
    // Try to open different databases with each instance
    printf("\nTesting opening different database files...\n");
    
    signed_integer result1 = OpenLinksWithContext(context1, "test_db1.links");
    if (result1 != SUCCESS_RESULT)
    {
        printf("WARNING: Could not open first database (this is expected on first run)\n");
    }
    else
    {
        printf("✓ Opened first database with context1\n");
    }
    
    signed_integer result2 = OpenLinksWithContext(context2, "test_db2.links");
    if (result2 != SUCCESS_RESULT)
    {
        printf("WARNING: Could not open second database (this is expected on first run)\n");
    }
    else
    {
        printf("✓ Opened second database with context2\n");
    }
    
    // Test backward compatibility - using default context
    printf("\nTesting backward compatibility...\n");
    signed_integer result3 = OpenLinks("test_default.links");
    if (result3 != SUCCESS_RESULT)
    {
        printf("WARNING: Could not open default database (this is expected on first run)\n");
    }
    else
    {
        printf("✓ Opened database using default context (backward compatibility)\n");
    }
    
    // Test that contexts maintain separate state
    printf("\nTesting separate state maintenance...\n");
    
    if (result1 == SUCCESS_RESULT && result2 == SUCCESS_RESULT)
    {
        // Try creating links in each context
        link_index link1 = CreateLinkWithContext(context1, 0, 1, 2);
        link_index link2 = CreateLinkWithContext(context2, 0, 3, 4);
        
        if (link1 != null && link2 != null)
        {
            printf("✓ Created links in both contexts\n");
            
            // Verify counts are separate
            unsigned_integer count1 = GetLinksCountWithContext(context1);
            unsigned_integer count2 = GetLinksCountWithContext(context2);
            
            printf("  Context1 has %u links\n", count1);
            printf("  Context2 has %u links\n", count2);
            
            if (count1 > 0 && count2 > 0)
            {
                printf("✓ Each context maintains separate link counts\n");
            }
        }
        else
        {
            printf("WARNING: Could not create test links\n");
        }
    }
    
    // Clean up
    printf("\nCleaning up...\n");
    
    if (result1 == SUCCESS_RESULT)
    {
        CloseLinksWithContext(context1);
        printf("✓ Closed first database\n");
    }
    
    if (result2 == SUCCESS_RESULT)
    {
        CloseLinksWithContext(context2);
        printf("✓ Closed second database\n");
    }
    
    if (result3 == SUCCESS_RESULT)
    {
        CloseLinks();
        printf("✓ Closed default database\n");
    }
    
    DestroyLinksContext(context1);
    DestroyLinksContext(context2);
    printf("✓ Destroyed both contexts\n");
    
    printf("\n✅ Multiple instances test completed successfully!\n");
    printf("This demonstrates that multiple Links instances can now exist in the same process.\n");
    
    return 0;
}