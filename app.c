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
    int length = strnlen(name, MAX_NAME);
    unsigned int hash_value = 0;
    for(int i = 0;i < length;i++){
        hash_value += name[i];
        hash_value = (hash_value * name[i]) % TABLE_SIZE;
    }
    return hash_value;
}

void init_hash_table(){
    for(int i = 0;i < TABLE_SIZE;i++){
        hash_table[i] = NULL;
    }
}

void print_table(){
    puts("Start\n");
    for(int i = 0;i < TABLE_SIZE;i++){
        if(hash_table[i] == NULL){
            printf("\t%d\t---\n", i);
        }else{
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
    if(person == NULL) return false;
    const int index = hash(person->name);
    person->next = hash_table[index];
    hash_table[index] = person;
    return true;
}

Person* hash_table_lookup(const char *name){
    int index = hash(name);
    Person *current;
    for(current = hash_table[index];
        current != NULL && strncmp(current->name, name, MAX_NAME) != 0;
        current = current->next){}
    return current;
}

Person* hash_table_delete(const char *name){
    int index = hash(name);
    Person *current, *previous;
    for(current = hash_table[index], previous = NULL;
        current != NULL && strncmp(current->name, name, MAX_NAME) != 0;
        previous = current, current = current->next){}
    if(current == NULL){
        return NULL;
    }
    if(previous != NULL){
        previous->next = current->next;
    }else{
        hash_table[index] = NULL;
    }
    return current;
}

int main(){
    init_hash_table();

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

    for(int i = 0;i < sizeof(people) / sizeof(*people);i++){
        hash_table_insert(&people[i]);
    }

    print_table();

    Person *person = hash_table_lookup("Mpho");
    if(person == NULL){
        puts("Not found!");
    }else{
        printf("Found %s.\n", person->name);
    }
    
    person = hash_table_lookup("George");
    if(person == NULL){
        puts("Not found!");
    }else{
        printf("Found %s.\n", person->name);
    }

    hash_table_delete("Mpho");
    person = hash_table_lookup("Mpho");
    if(person == NULL){
        puts("Not found!");
    }else{
        printf("Found %s.\n", person->name);
    }

    print_table();
    return 0;
}
