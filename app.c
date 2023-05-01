#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_NAME 256
#define TABLE_SIZE 10

typedef struct Person{
    char name[MAX_NAME];
    int age;
    struct Person *next;
} Person;

Person *hash_table[TABLE_SIZE];

unsigned int hash(const char *name){
    // Meaure the length of the name, only count upto the maximum name length
    int length = strnlen(name, MAX_NAME);

    // Calculate and return the hash
    unsigned int hash_value = 0;
    for(int i = 0;i < length;i++){
        hash_value += name[i];
        hash_value = (hash_value * name[i]) % TABLE_SIZE;
    }
    return hash_value;
}

void init_hash_table(){
    // Set every table entry as NULL
    for(int i = 0;i < TABLE_SIZE;i++){
        hash_table[i] = NULL;
    }
}

void print_table(){
    // For every entry in the table
    puts("Start\n");
    for(int i = 0;i < TABLE_SIZE;i++){
        // Check whether the entry is empty
        if(hash_table[i] == NULL){
            // Print a default line if it is
            printf("\t%d\t---\n", i);
        }else{
            // Print all items on this entry, if it isn't
            printf("\t%d\t ", i);
            for(Person *current = hash_table[i];current != NULL;current = current->next){
                printf("%s - ", current->name);
            }
            putchar('\n');
        }
    }
    puts("End");
}

bool hash_table_insert(Person *person){
    // Return false, if we got NULL
    if(person == NULL) return false;

    // Calculate the hash
    const int index = hash(person->name);

    // Add the person to the entry corresponding to the hash
    person->next = hash_table[index];
    hash_table[index] = person;

    // We successfully added the person to the hash table
    return true;
}

Person* hash_table_lookup(const char *name){
    // Calculate the hash
    int index = hash(name);

    // Try to find an item with that name
    Person *current;
    for(current = hash_table[index];
        current != NULL && strncmp(current->name, name, MAX_NAME) != 0;
        current = current->next){}
    
    // Return the item, if found.
    // Else, return NULL
    return current;
}

Person* hash_table_delete(const char *name){
    // Calculate the hash
    int index = hash(name);

    // Try to find an item with that name
    Person *current, *previous;
    for(current = hash_table[index], previous = NULL;
        current != NULL && strncmp(current->name, name, MAX_NAME) != 0;
        previous = current, current = current->next){}
    
    // Return NULL if not found
    if(current == NULL){
        return NULL;
    }

    // Make the previous node point to the next of the current node, if there was a previous
    // Set the entry to the next node, otherwise
    if(previous == NULL){
        hash_table[index] = current->next;
    }else{
        previous->next = current->next;
    }

    // Return the removed node
    return current;
}

int main(){
    // Initialize the hash table
    init_hash_table();

    // Create an array of people
    Person people[] = {
        {.name="Jacob", .age=256},
        {.name = "Kate", .age=27},
        {.name="Mpho", .age=14},
        {.name="Sarah", .age=54},
        {.name="Edna", .age=15},
        {.name="Maren", .age=25},
        {.name="Eliza", .age=34},
        {.name="Robert", .age=1},
        {.name="Jane", .age=75},
    };

    // Insert the people into the hash table
    for(size_t i = 0;i < sizeof(people) / sizeof(*people);i++){
        hash_table_insert(&people[i]);
    }

    // Print the table
    print_table();

    // Check whether the lookup function works with existing data
    Person *person = hash_table_lookup("Mpho");
    if(person == NULL){
        puts("Not found!");
    }else{
        printf("Found %s.\n", person->name);
    }
    
    // Check whether the lookup function works with not existing data
    person = hash_table_lookup("George");
    if(person == NULL){
        puts("Not found!");
    }else{
        printf("Found %s.\n", person->name);
    }

    // Check whether the delete function works with existing data
    hash_table_delete("Mpho");
    person = hash_table_lookup("Mpho");
    if(person == NULL){
        puts("Not found!");
    }else{
        printf("Found %s.\n", person->name);
    }

    // Print the table again
    print_table();
    return 0;
}
