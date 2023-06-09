#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashtable.h"

#define MAX_LINE 4096
#define FNV_PRIME 0x100000001b3
#define FNV_OFFSET 0xcbf29ce48422325UL

uint64_t hash(const char *data, size_t length){
    // Calculate and return the hash
    uint64_t hash_value = 0;
    for(size_t i = 0;i < length;i++){
        hash_value += data[i];
        hash_value *= data[i];
    }
    return hash_value;
}

uint64_t hash_fnv0(const char *data, size_t length){
    // Calculate and return the hash
    uint64_t hash_value = 0;
    for(size_t i = 0;i < length;i++){
        hash_value *= FNV_PRIME;
        hash_value ^= data[i];
    }
    return hash_value;
}

uint64_t hash_fnv1(const char *data, size_t length){
    // Calculate and return the hash
    uint64_t hash_value = FNV_OFFSET;
    for(size_t i = 0;i < length;i++){
        hash_value *= FNV_PRIME;
        hash_value ^= data[i];
    }
    return hash_value;
}

uint64_t hash_fnv1a(const char *data, size_t length){
    // Calculate and return the hash
    uint64_t hash_value = FNV_OFFSET;
    for(size_t i = 0;i < length;i++){
        hash_value ^= data[i];
        hash_value *= FNV_PRIME;
    }
    return hash_value;
}

void generateRandomWord(char *buffer, size_t length){
    for(size_t i = 0;i < length - 1;i++){
        buffer[i] = 'a' + (rand() % 26);
    }
    buffer[length - 1] = 0;
}

void mycleanup(void *p){
    printf("Destroying %p\n", p);
    free(p);
}

int main(int argc, const char **argv){
    if(argc != 3){
        printf("Usage: %s <wordlist filename> <num guesses>\n", argv[0]);
    }

    const char *filename = argv[1];
    uint32_t num_guesses = atoi(argv[2]);

    // 1_048_576 entries
    const int tablesize = (1 << 20);
    
    HashTable *table = hashTableCreate(tablesize, hash_fnv1a, NULL);

    FILE *file = fopen(filename, "r");
    char buffer[MAX_LINE];
    uint32_t numwords = 0;
    while(!feof(file) && fgets(buffer, MAX_LINE, file) != NULL){
        buffer[strcspn(buffer, "\n\r")] = 0;
        char *newentry = (char*)malloc(strlen(buffer) + 1);
        if(newentry == NULL){
            break;
        }
        strcpy(newentry, buffer);
        hashTableInsert(table, buffer, newentry);
        numwords++;
    }
    fclose(file);
    printf("Loaded %d words into the table.\n", numwords);
    printf("\t... with %lu collisions\n", hash_table_collisions(table));

    uint32_t good_guesses = 0;
    const int shortest_guess = 2;
    const int longest_guess = 15;
    for(uint32_t i = 0;i < num_guesses;i++){
        generateRandomWord(buffer, shortest_guess + (rand() % (longest_guess - shortest_guess)));
        if(hashTableLookup(table, buffer)){
            good_guesses++;
        }
    }

    printf("%u out of %u guesses were in the table.\n", good_guesses, num_guesses);

    hashTableDestroy(table);
}
