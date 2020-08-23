// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <strings.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 20000;

// To be used for size function
int wordcounter = 0;

// Hash table
node *table[N];

// Function Declared
void deleteList(struct node *head);


// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Sets word into all lowercase so hash will be the same for upper and lowercase versions of the word

    char wordcopy [strlen(word) + 1];
    strcpy(wordcopy, word);

    for (int i = 0, n = strlen(word); i < n; i++)
    {
        wordcopy[i] = tolower(wordcopy[i]);

    }

    // Hash the lowercase word
    int hashval = hash(wordcopy);

    // Go to proper bucket in hashtable and go through linked list. If you find the word, return true

    if (table[hashval] != NULL)
    {
        for (node *temp = table[hashval]; temp != NULL; temp = temp -> next)
        {
            if (strcasecmp(wordcopy, temp->word) == 0)
            {
                return true;
            }
        }
    }
    return false;

}

// Hashes word to a number - Found online, djb2 hash function, modified slightly
unsigned int hash(const char *word)
{
    // Declare hash value and set equal to 5381, and declare int c
    unsigned int hash = 5381;
    int c;

    // Iterate through each letter of the word and store it in c, c++
    while ((c = *word++))
    {
        // Updating the value of hash through bitwise math, basically multiplying by 33 and adding c
        hash = ((hash << 5) + hash) + c;
    }

    // Mod hash to make sure it is within bucket size
    hash = hash % N;
    return hash;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize hashtable
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }
    
    // Open file and check that it opened correctly
    FILE *dfile = fopen(dictionary, "r");
    if (dfile == NULL)
    {
        return false;
    }

    // Creat buffer array for word, not dynamically
    char wordarr[LENGTH + 1];

    // While the file has not ended, do the following
    while (fscanf(dfile, "%s", wordarr) != EOF)
    {
        // Count words
        wordcounter++;
        
        // Create new node and check for worked
        node *newnode = malloc(sizeof(node));
        if (newnode == NULL)
        {
            free(newnode);
            return false;
        }

        // Copy word into the data portion of the node
        strcpy(newnode -> word, wordarr);

        // Hash node and find out where it should go in hash table
        unsigned int hashvalue = hash(wordarr);
        
        // First time putting something into the linked list
        
        if (table[hashvalue] == NULL)
        {
            newnode -> next = NULL;
            table[hashvalue] = newnode;
        }
        else
        {
            // Insert node in the hash-th value of hash table
            newnode -> next = table[hashvalue];
            table[hashvalue] = newnode;
        }
    }
    //Close dictionary file
    fclose(dfile);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return wordcounter;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        // Create temp pointer
        deleteList(table[i]);
    }
    return true;
}

void deleteList(struct node *head) 
{ 
    if (head == NULL)
    {
        return;
    }
    deleteList(head->next);  
    free(head); 
} 
  
