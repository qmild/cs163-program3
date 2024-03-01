// Bryce Diedrich
// bryc3@pdx.edu
// CS163 Program #3
//
// This file defines out hash table ADT as an array of linear linked lists
//
// The following public interface is provided
// - inseert: accepts a UserData object and inserts it onto the hash table, returning error info
// - retrieve: accepts a name to retrieve data from matching UserData object. This data is then copied
//   into the passed by reference UserData object.
// - remove: accepts a char* string name and removes UserData object with matching name
// - display: accepts char* name and displays UserData object with matching name
// - displayByGenre: accepts char* genre and displays all UserData objects with a matching genre
// - displayAll: no arguments. displays all UserData objects in table

#include "user_data.h"
#include <cstring>
#include <fstream>

// base of polynomial in hash function
const int unsigned BASE = 31;
const int unsigned BAD_BASE = 30;

 class Node
{
   public:
    // constructor
    Node(char* key, UserData* data, int index) {
        this->key = new char[strlen(key) +1]; 
        this->data = data;
        this->index = index;
        strcpy(this->key, key);
        next = nullptr;
    }
    //destructor
    ~Node()
    {
        delete[] key;
        delete data;
        if(this->next != nullptr) {
            delete this->next;
        }
    }

    Node* next;
    UserData* data;
    char* key;
    int index;
};

class Table
{
   public:
    Table(int size);
    ~Table();
    int insert(UserData & insert_data);
    int remove(char* name);
    int retrieve(char* name, UserData & out_data);
    int display(char* name);
    int displayByGenre(char* genre);
    int displayAll();
    int loadFromFile(char* filename);

   private:
    // returns integer hash based on string argument
    //int hash(char * key);
    int hash(char* key);
    int badhash(char* key);
    Node* searchByName(char* name, int & out_index);
    // array of pointers to nodes
    Node** table;
    // size of table
    int size;
};

