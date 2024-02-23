// Bryce Diedrich
// bryc3@pdx.edu
// CS163 Program #3
//
// <- header comments go here!

#include "user_data.h"
#include <cstring>
#include <fstream>

// base of polynomial in hash function
const int unsigned BASE = 31;

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
    int insert(UserData & to_insert);
    int remove(char* name);
    int retrieve(char* name, UserData & to_add);
    int display(char* name);
    int displayByGenre(char* genre);
    int displayAll();
    int loadFromFile(char * filename);

   private:
    // returns integer hash based on string argument
    //int hash(char * key);
    int hash(char * key);
    Node* searchByName(char* name);
    // array of pointers to nodes
    Node** table;
    // size of table
    int size;
};

