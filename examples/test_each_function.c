#include "../Platform.Data.Triplets.Kernel/Link.h"
#include "../Platform.Data.Triplets.Kernel/PersistentMemoryManager.h"
#include <stdio.h>

signed_integer printLink(link_index linkIndex) {
    printf("Link: %llu (Source: %llu, Linker: %llu, Target: %llu)\n", 
           linkIndex, 
           GetSourceIndex(linkIndex), 
           GetLinkerIndex(linkIndex), 
           GetTargetIndex(linkIndex));
    return 1; // Continue iteration
}

signed_integer printFirstThreeLinks(link_index linkIndex) {
    static int count = 0;
    printf("Link %d: %llu\n", ++count, linkIndex);
    return count < 3; // Stop after 3 links
}

int main() {
    printf("Testing Each function implementation.\n");
    
    OpenLinks("test_each_db.links");
    printf("Database opened.\n");
    
    // Create some test links
    link_index isA = CreateLink(itself, itself, itself);
    link_index isNotA = CreateLink(itself, itself, isA);
    link_index link1 = CreateLink(isA, isNotA, isA);
    link_index link2 = CreateLink(isA, isNotA, isNotA);
    
    printf("Created test links: isA=%llu, isNotA=%llu, link1=%llu, link2=%llu\n", isA, isNotA, link1, link2);
    
    // Test Each with single link query
    printf("\n=== Test 1: Single link query ===\n");
    link_index query1[] = {isA};
    Each(query1, 1, printLink);
    
    // Test Each with two-element query (source, target pattern)
    printf("\n=== Test 2: Two-element query (source, target) ===\n");
    link_index query2[] = {isA, null}; // Find all links with isA as source
    Each(query2, 2, printFirstThreeLinks);
    
    // Test Each with three-element query (exact match)
    printf("\n=== Test 3: Three-element exact match query ===\n");
    link_index query3[] = {isA, isNotA, isA}; // Search for exact link
    Each(query3, 3, printLink);
    
    printf("\nTest completed.\n");
    
    CloseLinks();
    printf("Database closed.\n");
    
    return 0;
}