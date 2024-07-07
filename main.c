#include <stdio.h>
#include <stdlib.h>
#include <math.h>  // For log2 function

#define L1_SIZE (16 * 1024)  // 16KB
#define L2_SIZE (32 * 1024)  // 32KB
#define L3_SIZE (2 * 1024) // 2MB
#define BLOCK_SIZE 64  // Assuming block size is 64 bytes
#define ADDRESS_BITS 32  // Assuming a 32-bit address space

unsigned int hits = 0; //Sum all hits
unsigned int misses = 0; // Sum all misses
unsigned int total_commands = 0; //Sum all commands load and store

/// @brief This struct represents a cache line.
typedef struct {
    int valid;        // Valid bit
    unsigned int tag; // Tag
} CacheLine;

/// @brief This function allocate memory for a cache line and return a pointer.
/// @param cache_size 
/// @return 
CacheLine* initialize_cache(int cache_size) {
    int num_lines = cache_size / BLOCK_SIZE;
    CacheLine* cache = (CacheLine*)malloc(num_lines * sizeof(CacheLine));
    if (cache == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < num_lines; i++) {
        cache[i].valid = 0; // Set all valid bits to 0
        cache[i].tag = 0;   // Initialize tag to 0
    }
    return cache;
}

/// @brief This function can take any cache Lx and update the cache.
/// @param cache 
/// @param cache_size 
/// @param cache_name 
void update_cache(CacheLine* cache, unsigned int address, int cache_size) {
    int index_bits = (int)log2(cache_size / BLOCK_SIZE);
    int index_mask = (1 << index_bits) - 1;
    int index = (address >> (int)log2(BLOCK_SIZE)) & index_mask;
    unsigned int tag = address >> (index_bits + (int)log2(BLOCK_SIZE));

    cache[index].valid = 1;
    cache[index].tag = tag;
}


/// @brief This function update L1 cache 
/// @param L1 
/// @param address 
void update_cache_L1(CacheLine* L1, unsigned int address) {
    int index_bits = (int)log2(L1_SIZE / BLOCK_SIZE); // L1 specific calculations
    int index_mask = (1 << index_bits) - 1;
    int index = (address >> (int)log2(BLOCK_SIZE)) & index_mask;
    unsigned int tag = address >> (index_bits + (int)log2(BLOCK_SIZE));

    L1[index].valid = 1;
    L1[index].tag = tag;
}

/// @brief This function update L2 cache 
/// @param L2 
/// @param address 
void update_cache_L2(CacheLine* L2, unsigned int address) {
    int index_bits = (int)log2(L2_SIZE / BLOCK_SIZE); // L2 specific calculations
    int index_mask = (1 << index_bits) - 1;
    int index = (address >> (int)log2(BLOCK_SIZE)) & index_mask;
    unsigned int tag = address >> (index_bits + (int)log2(BLOCK_SIZE));

    L2[index].valid = 1;
    L2[index].tag = tag;
}

/// @brief This function update L3 cache 
/// @param L3 
/// @param address 
void update_cache_L3(CacheLine* L3, unsigned int address) {
    int index_bits = (int)log2(L3_SIZE / BLOCK_SIZE); // L3 specific calculations
    int index_mask = (1 << index_bits) - 1;
    int index = (address >> (int)log2(BLOCK_SIZE)) & index_mask;
    unsigned int tag = address >> (index_bits + (int)log2(BLOCK_SIZE));

    L3[index].valid = 1;
    L3[index].tag = tag;
}

/// @brief This function check if Lx cache is valid and have the same tag.
/// @param cache 
/// @param address 
/// @param cache_size 
/// @return 
int is_in_cache(CacheLine* cache, unsigned int address, int cache_size) {
    int index_bits = (int)log2(cache_size / BLOCK_SIZE);
    int index_mask = (1 << index_bits) - 1;
    int index = (address >> (int)log2(BLOCK_SIZE)) & index_mask;
    unsigned int tag = address >> (index_bits + (int)log2(BLOCK_SIZE));

    return cache[index].valid && (cache[index].tag == tag);
}

/// @brief Check if the valid bit is set for the given address in the specified cache.
/// @param cache The cache in which to check the address.
/// @param address The memory address to check.
/// @param cache_size The size of the cache.
/// @return 1 if the valid bit is set for the given address, 0 otherwise.
    int is_valid_bit_set(CacheLine* cache, unsigned int address, int cache_size) {
    int num_lines = cache_size / BLOCK_SIZE;  // Number of lines in the cache
    int index_bits = (int)log2(num_lines);    // Calculate the number of index bits needed
    int index_mask = (1 << index_bits) - 1;   // Create a mask to extract the index
    int index = (address >> (int)log2(BLOCK_SIZE)) & index_mask; // Calculate the index from the address

    return cache[index].valid; // Return the state of the valid bit for the calculated index
}


/// @brief Check if the given address is in L1 cache.
/// @param L1 The L1 cache.
/// @param address The memory address to check.
/// @return 1 if in cache and valid, 0 otherwise.
int is_in_cache_L1(CacheLine* L1, unsigned int address) {
    int index_bits = (int)log2(L1_SIZE / BLOCK_SIZE); // L1 specific calculations
    int index_mask = (1 << index_bits) - 1;
    int index = (address >> (int)log2(BLOCK_SIZE)) & index_mask;
    unsigned int tag = address >> (index_bits + (int)log2(BLOCK_SIZE));

    return L1[index].valid && (L1[index].tag == tag);
}

/// @brief Check if the given address is in L2 cache.
/// @param L2 The L2 cache.
/// @param address The memory address to check.
/// @return 1 if in cache and valid, 0 otherwise.
int is_in_cache_L2(CacheLine* L2, unsigned int address) {
    int index_bits = (int)log2(L2_SIZE / BLOCK_SIZE); // L2 specific calculations
    int index_mask = (1 << index_bits) - 1;
    int index = (address >> (int)log2(BLOCK_SIZE)) & index_mask;
    unsigned int tag = address >> (index_bits + (int)log2(BLOCK_SIZE));

    return L2[index].valid && (L2[index].tag == tag);
}

/// @brief Check if the given address is in L3 cache.
/// @param L3 The L3 cache.
/// @param address The memory address to check.
/// @return 1 if in cache and valid, 0 otherwise.
int is_in_cache_L3(CacheLine* L3, unsigned int address) {
    int index_bits = (int)log2(L3_SIZE / BLOCK_SIZE); // L3 specific calculations
    int index_mask = (1 << index_bits) - 1;
    int index = (address >> (int)log2(BLOCK_SIZE)) & index_mask;
    unsigned int tag = address >> (index_bits + (int)log2(BLOCK_SIZE));

    return L3[index].valid && (L3[index].tag == tag);
}


void print_cache_values(CacheLine* cache, int cache_size, const char* cache_name) {
    int num_lines = cache_size / BLOCK_SIZE;
    printf("%s Cache Contents:\n", cache_name);
    printf("Index | Valid | Tag\n");
    for (int i = 0; i < num_lines; i++) {
        printf("%5d | %5d | %08X\n", i, cache[i].valid, cache[i].tag);
    }
}

/// @brief By given a cache size the name of the cache the function will print how many Index Bits, Tag Bits,  Offset Bits, 1 Valid Bit
/// @param cache_size 
/// @param cache_name 
void print_cache_info(int cache_size, const char* cache_name) {
    int num_lines = cache_size / BLOCK_SIZE;
    int offset_bits = (int)log2(BLOCK_SIZE);
    int index_bits = (int)log2(num_lines);
    int tag_bits = ADDRESS_BITS - index_bits - offset_bits;
    printf("%s: %d Index Bits, %d Tag Bits, %d Offset Bits, 1 Valid Bit\n",
        cache_name, index_bits, tag_bits, offset_bits);
}

/// @brief Function to print the index and tag for a given address and cache size.
/// @param address uint
/// @param cache_size int 
/// @param cache_name string
void print_index_and_tag(unsigned int address, int cache_size, const char* cache_name) {
    int index_bits = (int)log2(cache_size / BLOCK_SIZE);
    int index_mask = (1 << index_bits) - 1;
    int index = (address >> (int)log2(BLOCK_SIZE)) & index_mask;
    unsigned int tag = address >> (index_bits + (int)log2(BLOCK_SIZE));

    printf("%s Address %08X: Index = %d, Tag = %08X\n", cache_name, address, index, tag);
}

/// @brief Function to reconstruct the full address from index and tag stored in a cache line.
/// @param index Index of the cache line.
/// @param tag Tag stored in the cache line.
/// @param cache_size Size of the cache to determine the number of index bits.
/// @return The reconstructed full address.
unsigned int get_full_address(int index, unsigned int tag, int cache_size) {
    int index_bits = (int)log2(cache_size / BLOCK_SIZE);  // Calculate the number of index bits
    int offset_bits = (int)log2(BLOCK_SIZE);            // Calculate the number of offset bits

    unsigned int address = tag << (index_bits + offset_bits); // Shift tag left to make space for index and offset
    address |= (index << offset_bits);                        // Insert index into the correct position
    // Offset is assumed to be zero since we're usually dealing with entire blocks

    return address;
}


void full_catch_logic(CacheLine* L1, CacheLine* L2, CacheLine* L3, unsigned int address) {
    if (is_valid_bit_set(L1, address, L1_SIZE) == 0) {
        printf("Miss L1 Valid=0 Address %08X. L1 was updated\n", address);
        update_cache_L1(L1, address);
        misses++;
        return;
    } else if (is_in_cache_L1(L1, address)) {
        printf("Hit L1 Address %08X \n", address);
        hits++;
        return;
    } else {
        // If valid bit is set but tag mismatch, get the actual address from the cache and the check if L2 is valid ==0 then update l1 and l2
        int index = (address >> (int)log2(BLOCK_SIZE)) & ((1 << (int)log2(L1_SIZE / BLOCK_SIZE)) - 1);
        unsigned int storedTag = L1[index].tag;
        unsigned int currentL1Address = get_full_address(index, storedTag, L1_SIZE);
        printf("The current address is: %08X, but the tag matches for address: %08X\n", address, currentL1Address);
        if(is_valid_bit_set(L2,currentL1Address,L2_SIZE)==0) {
            update_cache_L2(L2, currentL1Address);
            update_cache_L1(L1,address);
            return;
        }
        else if (is_in_cache_L2(L2, address)) {
            printf("Hit L2 Address %08X. Swapping L1 and L2 contents.\n", address);
            
            
            // Swap contents: Move current L2 data to L1 and move current L1 data to L2
            update_cache_L1(L1, address);
            update_cache_L2(L2, currentL1Address);
            
            hits++; // Count this as a hit since we accessed data from L2
        }else{
            printf("Miss L2 Address %08X.check on L3 \n", address);
            if(is_valid_bit_set(L3,address,L3_SIZE)==0){
                misses++;
                int index1 = (address >> (int)log2(BLOCK_SIZE)) & ((1 << (int)log2(L1_SIZE / BLOCK_SIZE)) - 1);
                unsigned int storedTag1 = L1[index1].tag;
                unsigned int currentL1Address = get_full_address(index1, storedTag1, L1_SIZE);
        
        int index = (address >> (int)log2(BLOCK_SIZE)) & ((1 << (int)log2(L2_SIZE / BLOCK_SIZE)) - 1);
                unsigned int storedTag = L2[index].tag;
                unsigned int currentL2Address = get_full_address(index, storedTag, L2_SIZE);
             update_cache_L3(L3, currentL2Address);
             update_cache_L2(L2, currentL1Address);
             update_cache_L1(L1,address);
             printf("Miss L3 Address %08X. L3 was updated\n", address);
            }
            else if (is_in_cache_L3(L3,address)){
                printf("Hit L3 Address %08X. Swapping L1, L2 and L3 contents.\n", address);
                 int index1 = (address >> (int)log2(BLOCK_SIZE)) & ((1 << (int)log2(L1_SIZE / BLOCK_SIZE)) - 1);
                unsigned int storedTag1 = L1[index1].tag;
                unsigned int currentL1Address = get_full_address(index1, storedTag1, L1_SIZE);
        
        int index = (address >> (int)log2(BLOCK_SIZE)) & ((1 << (int)log2(L2_SIZE / BLOCK_SIZE)) - 1);
                unsigned int storedTag = L2[index].tag;
                unsigned int currentL2Address = get_full_address(index, storedTag, L2_SIZE);
             
                // Swap contents: Move current L3 data to L2, current L2 data to L1 and move current L1 data to L3
                update_cache_L3(L3, currentL2Address);
                update_cache_L2(L2, currentL1Address);
                update_cache_L1(L1, address);
                hits++; // Count this as a hit since we accessed data from L3
             
            }
            else{
                printf("Miss L3 Address %08X. No hit in L3)\n", address);
                misses++;
                
                printf("Hit L3 Address %08X. Swapping L1, L2 and L3 contents.\n", address);
                 int index1 = (address >> (int)log2(BLOCK_SIZE)) & ((1 << (int)log2(L1_SIZE / BLOCK_SIZE)) - 1);
                unsigned int storedTag1 = L1[index1].tag;
                unsigned int currentL1Address = get_full_address(index1, storedTag1, L1_SIZE);
        
        int index = (address >> (int)log2(BLOCK_SIZE)) & ((1 << (int)log2(L2_SIZE / BLOCK_SIZE)) - 1);
                unsigned int storedTag = L2[index].tag;
                unsigned int currentL2Address = get_full_address(index, storedTag, L2_SIZE);
             
             int index3 = (address >> (int)log2(BLOCK_SIZE)) & ((1 << (int)log2(L2_SIZE / BLOCK_SIZE)) - 1);
                unsigned int storedTag3 = L2[index3].tag;
                unsigned int currentL3Address = get_full_address(index3, storedTag3, L3_SIZE); // store the L3 address to DRAM memory.

             update_cache_L3(L3, currentL2Address);
             update_cache_L2(L2, currentL1Address);
             update_cache_L1(L1, address);
             printf("address %08x is wrtie back to DRAM\n", currentL3Address);
             }
            
        }
    }
}

int main() {

    // Initialize caches
    CacheLine* L1 = initialize_cache(L1_SIZE);
    CacheLine* L2 = initialize_cache(L2_SIZE);
    CacheLine* L3 = initialize_cache(L3_SIZE);

    // Example address
    unsigned int address = 0x1A2B3C4D;

    // Print index and tag before updates
    print_index_and_tag(address, L1_SIZE, "L1");
       print_index_and_tag(address, L2_SIZE, "L2");
    print_index_and_tag(address, L3_SIZE, "L3");


   

    // Print cache contents and bit information
    print_cache_values(L1, L1_SIZE, "L1");
    print_cache_values(L2, L2_SIZE, "L2");
    print_cache_values(L3, L3_SIZE, "L3");
    print_cache_info(L1_SIZE, "L1");
     print_cache_info(L2_SIZE, "L2");
      print_cache_info(L3_SIZE, "L3");
    



      



    // Free allocated memory
    free(L1);
    free(L2);
    free(L3);
    printf("test2\n");

    return 0;
}