#include "../Platform.Data.Triplets.Kernel/Common.h"
#include "../Platform.Data.Triplets.Kernel/Link.h"
#include "../Platform.Data.Triplets.Kernel/PersistentMemoryManager.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

// Function declarations
void OptimizedRecursiveTraversalCore(link_index nodeIndex, visitor visit);
unsigned_integer CountNodesCore(link_index nodeIndex);

// Global counters for performance measurement
static volatile unsigned_integer recursive_visits = 0;
static volatile unsigned_integer existing_visits = 0;

// Visitor functions for different approaches
void recursive_counter(link_index linkIndex) {
    recursive_visits++;
    // Simulate some work to make the measurement more significant
    volatile int dummy = 0;
    for (int i = 0; i < 10; i++) {
        dummy += i;
    }
}

void existing_counter(link_index linkIndex) {
    existing_visits++;
    // Simulate same work
    volatile int dummy = 0;
    for (int i = 0; i < 10; i++) {
        dummy += i;
    }
}

// Custom recursive implementation (optimized)
void OptimizedRecursiveTraversal(link_index rootIndex, visitor visit) {
    if (rootIndex == null) return;
    
    Link* root = GetLink(rootIndex);
    if (root->BySourceRootIndex == null) return;
    
    // Start from the actual tree root
    OptimizedRecursiveTraversalCore(root->BySourceRootIndex, visit);
}

void OptimizedRecursiveTraversalCore(link_index nodeIndex, visitor visit) {
    if (nodeIndex == null) return;
    
    Link* node = GetLink(nodeIndex);
    
    // In-order traversal
    OptimizedRecursiveTraversalCore(node->BySourceLeftIndex, visit);
    visit(nodeIndex);
    OptimizedRecursiveTraversalCore(node->BySourceRightIndex, visit);
}

// Create a more realistic tree structure for testing
link_index CreateBalancedTree(int maxDepth, link_index parent) {
    if (maxDepth <= 0) return null;
    
    // Create the main node
    link_index node = CreateLink(parent != null ? parent : itself, itself, itself);
    if (node == null) return null;
    
    // Create children if we haven't reached max depth
    if (maxDepth > 1) {
        // Create left and right children
        CreateBalancedTree(maxDepth - 1, node);
        CreateBalancedTree(maxDepth - 1, node);
    }
    
    return node;
}

// Function to count nodes in a tree
unsigned_integer CountNodes(link_index rootIndex) {
    if (rootIndex == null) return 0;
    
    Link* root = GetLink(rootIndex);
    if (root->BySourceRootIndex == null) return 0;
    
    return CountNodesCore(root->BySourceRootIndex);
}

unsigned_integer CountNodesCore(link_index nodeIndex) {
    if (nodeIndex == null) return 0;
    
    Link* node = GetLink(nodeIndex);
    return 1 + CountNodesCore(node->BySourceLeftIndex) + CountNodesCore(node->BySourceRightIndex);
}

void RunComprehensiveBenchmark() {
    printf("=== Comprehensive Tree Traversal Benchmark ===\n");
    printf("Testing with 32-bit integers (uint32_t/int32_t)\n\n");
    
    // Test cases with different tree configurations
    struct {
        int depth;
        int iterations;
        const char* description;
    } test_cases[] = {
        {6, 1000, "Small tree (depth 6)"},
        {8, 500, "Medium tree (depth 8)"},
        {10, 200, "Large tree (depth 10)"},
        {12, 50, "Very large tree (depth 12)"}
    };
    
    int num_tests = sizeof(test_cases) / sizeof(test_cases[0]);
    
    for (int test = 0; test < num_tests; test++) {
        printf("=== %s ===\n", test_cases[test].description);
        printf("Tree depth: %d, Iterations: %d\n", 
               test_cases[test].depth, test_cases[test].iterations);
        
        // Create test tree
        link_index root_link = CreateBalancedTree(test_cases[test].depth, null);
        if (root_link == null) {
            printf("ERROR: Failed to create test tree\n\n");
            continue;
        }
        
        // Count actual nodes
        unsigned_integer node_count = CountNodes(root_link);
        printf("Actual nodes in tree: %u\n", node_count);
        
        if (node_count == 0) {
            printf("WARNING: No referer tree structure found\n\n");
            continue;
        }
        
        // Test 1: Existing WalkThroughAllReferersBySource (mostly recursive)
        recursive_visits = 0;
        clock_t start1 = clock();
        
        for (int i = 0; i < test_cases[test].iterations; i++) {
            WalkThroughAllReferersBySource(root_link, recursive_counter);
        }
        
        clock_t end1 = clock();
        double time1 = ((double)(end1 - start1)) / CLOCKS_PER_SEC;
        
        // Test 2: Custom optimized recursive implementation
        existing_visits = 0;
        clock_t start2 = clock();
        
        for (int i = 0; i < test_cases[test].iterations; i++) {
            OptimizedRecursiveTraversal(root_link, existing_counter);
        }
        
        clock_t end2 = clock();
        double time2 = ((double)(end2 - start2)) / CLOCKS_PER_SEC;
        
        // Results
        printf("\nResults:\n");
        printf("Existing implementation (WalkThroughAllReferersBySource):\n");
        printf("  Time: %.6f seconds\n", time1);
        printf("  Nodes visited: %u\n", recursive_visits);
        printf("  Visits per iteration: %.1f\n", (double)recursive_visits / test_cases[test].iterations);
        printf("  Time per visit: %.9f seconds\n", 
               recursive_visits > 0 ? time1 / recursive_visits : 0);
        
        printf("Optimized recursive implementation:\n");
        printf("  Time: %.6f seconds\n", time2);
        printf("  Nodes visited: %u\n", existing_visits);
        printf("  Visits per iteration: %.1f\n", (double)existing_visits / test_cases[test].iterations);
        printf("  Time per visit: %.9f seconds\n", 
               existing_visits > 0 ? time2 / existing_visits : 0);
        
        // Performance comparison
        if (time1 > 0 && time2 > 0) {
            double ratio = time1 / time2;
            printf("\nPerformance comparison:\n");
            if (ratio > 1.0) {
                printf("  Optimized is %.2fx faster than existing\n", ratio);
                printf("  Improvement: %.1f%%\n", (ratio - 1.0) * 100);
            } else {
                printf("  Existing is %.2fx faster than optimized\n", 1.0/ratio);
                printf("  Existing advantage: %.1f%%\n", (1.0/ratio - 1.0) * 100);
            }
        }
        
        printf("\n");
        
        // Clean up (simplified)
        // In a real scenario, we would properly clean up all created links
    }
}

void AnalyzeInlineAsmApproach() {
    printf("=== Inline Assembly Analysis ===\n");
    printf("The inline assembly approach in SizeBalancedTree.h has the following characteristics:\n\n");
    
    printf("1. Manual Stack Management:\n");
    printf("   - Uses ESP register directly: mov initialStackPointer, ESP\n");
    printf("   - Manual push/pop: push element, pop element\n");
    printf("   - Stack comparison for termination\n\n");
    
    printf("2. Performance Considerations:\n");
    printf("   - Eliminates function call overhead\n");
    printf("   - Manual register management\n");
    printf("   - May prevent compiler optimizations\n");
    printf("   - Platform-specific (x86 only)\n\n");
    
    printf("3. Issues with Current Implementation:\n");
    printf("   - Comment states it's slower than recursive version\n");
    printf("   - Compiler can't optimize inline assembly\n");
    printf("   - Modern compilers are very good at optimizing recursion\n\n");
    
    printf("4. Impact of 32-bit Integer Change:\n");
    printf("   - Reduced memory usage: 32-bit vs 64-bit indices\n");
    printf("   - Better cache performance (more indices fit in cache lines)\n");
    printf("   - Faster arithmetic on some 32-bit architectures\n");
    printf("   - Potential for better vectorization\n\n");
    
    printf("5. Recommendation:\n");
    printf("   - Modern recursive approach is likely faster\n");
    printf("   - Compiler optimizations (tail call, inlining) are very effective\n");
    printf("   - 32-bit integers provide memory/cache benefits\n");
    printf("   - Consider iterative approach without inline assembly\n");
}

int main() {
    printf("Comprehensive Tree Traversal Performance Analysis\n");
    printf("=================================================\n");
    printf("This benchmark compares different tree traversal approaches\n");
    printf("with the updated 32-bit integer types.\n\n");
    
    // Initialize the database
    if (OpenLinks("comprehensive_benchmark.links") != SUCCESS_RESULT) {
        printf("ERROR: Failed to initialize links database\n");
        return 1;
    }
    
    // Run the comprehensive benchmark
    RunComprehensiveBenchmark();
    
    // Provide analysis of the inline assembly approach
    AnalyzeInlineAsmApproach();
    
    // Close database
    CloseLinks();
    
    printf("Benchmark completed. Check comprehensive_benchmark.links for database file.\n");
    return 0;
}