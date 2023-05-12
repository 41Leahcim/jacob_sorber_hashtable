#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef uint64_t Hashfunction(const char*, size_t);
typedef void cleanupFunction(void*);
typedef struct _HashTable HashTable;

HashTable* hashTableCreate(uint32_t size, Hashfunction *hashFunction, cleanupFunction *cf);
void hashTableDestroy(HashTable *hashTable);
void hashTablePrint(HashTable *hashTable);
bool hashTableInsert(HashTable *hashTable, const char *key, void *object);
void* hashTableLookup(HashTable *hashTable, const char *key);
void* hashTableDelete(HashTable *hashTable, const char *key);
