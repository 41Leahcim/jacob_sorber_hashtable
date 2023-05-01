#include "hashtable.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Entry{
    char *key;
    void *object;
    struct Entry *next;
} Entry;

typedef struct _HashTable{
    size_t size;
    Hashfunction *hash;
    Entry **elements;
} HashTable;

HashTable* hashTableCreate(uint32_t size, Hashfunction *hashFunction){
    HashTable *hashTable = (HashTable*)malloc(sizeof(*hashTable));
    if(hashTable == NULL){
        return NULL;
    }
    hashTable->size= size;
    hashTable->hash = hashFunction;
    hashTable->elements = (Entry**)calloc(sizeof(Entry*), hashTable->size);
    return hashTable;
}

void hashTableDestroy(HashTable *hashTable){
    for(size_t i = 0;i < hashTable->size;i++){
        Entry *previous, *current;
        for(previous = NULL, current = hashTable->elements[i];current != NULL;previous = current, current = current->next){
            if(previous != NULL){
                free(previous->key);
                free(previous->object);
                free(previous);
            }
        }
        if(previous != NULL){
            free(previous->key);
            free(previous->object);
            free(previous);
        }
    }
    free(hashTable->elements);
    free(hashTable);
}

void hashTablePrint(HashTable *hashTable){
    puts("Start Table");
    for(size_t i = 0;i < hashTable->size;i++){
        if(hashTable->elements[i] != NULL){
            printf("\t%ld\t ", i);
            for(Entry *current = hashTable->elements[i];
                current != NULL;current = current->next){
                printf("\"%s\"(%p) - ", current->key, current->object);
            }
            putchar('\n');
        }
    }
    puts("End Table");
}

static size_t hashTableIndex(HashTable *hashTable, const char *key){
    return hashTable->hash(key, strlen(key)) % hashTable->size;
}

bool hashTableInsert(HashTable *hashTable, const char *key, void *object){
    if(key == NULL || object == NULL || hashTable == NULL){
        return false;
    }
    size_t index = hashTableIndex(hashTable, key);

    if(hashTableLookup(hashTable, key) != NULL){
        return false;
    }

    // Create a new entry
    Entry *entry = (Entry*)malloc(sizeof(*entry));
    if(entry == NULL){
        return false;
    }
    entry->object = object;
    entry->key = (char*)malloc(strlen(key) + 1);
    if(entry->key == NULL){
        free(entry);
        return false;
    }
    strcpy(entry->key, key);

    // Insert entry
    entry->next = hashTable->elements[index];
    hashTable->elements[index] = entry;
    return true;
}

void* hashTableLookup(HashTable *hashTable, const char *key){
    if(key == NULL || hashTable == NULL){
        return false;
    }
    size_t index = hashTableIndex(hashTable, key);

    Entry *current;
    for(current = hashTable->elements[index];
        current != NULL && strcmp(current->key, key) != 0;
        current = current->next){
    }
    if(current == NULL){
        return NULL;
    }
    return current->object;
}

void* hashTableDelete(HashTable *hashTable, const char *key){
    if(key == NULL || hashTable == NULL){
        return false;
    }
    size_t index = hashTableIndex(hashTable, key);

    Entry *current, *previous;
    for(previous = NULL, current = hashTable->elements[index];
        current != NULL && strcmp(current->key, key) != 0;
        previous = current, current = current->next){
    }
    if(current == NULL){
        return NULL;
    }
    if(previous == NULL){
        // Deleting the head of the list
        hashTable->elements[index] = current->next;
    }else{
        // Deleting from the middle or end
        previous->next = current->next;
    }
    void *result = current->object;
    free(current);
    return result;
}