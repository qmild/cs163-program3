// Bryce Diedrich
// bryc3@pdx.edu
// CS163 Program #3
//
// <- header comments go here!
#include "table.h"
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <ostream>
#include <iostream>
#include <cstring>
#include <limits.h>
#include <strings.h>

using namespace std;

Table::Table(int size)
{
  this->size = size;
  table = new Node*[size]();
  // initialize node pointers in table to null
  for(int i = 0; i < size; ++i)
  {
    table[i] = nullptr;
  }
}

Table::~Table()
{
  for(int i = 0; i < size; ++i) 
  {
    while(table[i] != nullptr) 
    {
      Node * deleter = table[i];
      table[i] = table[i]->next;
      delete deleter;
    }
  }
}

// this hash function will have a varied base
int Table::hash(char * key)
{
  unsigned int hash_result = 0;
  unsigned int power = 1;

  for(int i = 0; i < strlen(key); ++i)
  {
    hash_result = (hash_result + (key[i] * power)) % size;
    // increase the power by one degree, and take it modulo UINT_MAX to avoid overflow 
    power = (power * BASE) % UINT_MAX;
  }

  return hash_result;
}

int Table::insert(UserData &  to_insert)
{
  // allocate UserData object and initialize data
  UserData * data = new UserData();
  int result = data->init(to_insert.name, 
      to_insert.directors, 
      to_insert.genres, 
      to_insert.user_age, 
      to_insert.user_gender, 
      to_insert.user_location, 
      to_insert.user_rating, 
      to_insert.user_watch_time, 
      to_insert.num_directors, 
      to_insert.num_genres);
 
  // check if any char* inputs are uninitialized
  if( to_insert.name == nullptr || to_insert.user_gender == nullptr || to_insert.user_location == nullptr || to_insert.directors == nullptr || to_insert.genres == nullptr)
  {
    return 1; 
  }

  // check if any director[] argument strings are uninitialized
  for (int i = 0; i < to_insert.num_directors; ++i) {
    if (to_insert.directors[i] == nullptr) {
      return 1;
    }
  }

  // check if any genres[] argument strings are uninitialized
  for (int i = 0; i < to_insert.num_directors; ++i) {
    if (to_insert.genres[i] == nullptr) {
      return 1;
    }
  }

  // check if any char* inputs are empty
  if( strlen(to_insert.name) == 0 || strlen(to_insert.user_gender) == 0 || strlen(to_insert.user_location) == 0)
  {
    return 2; 
  }

  // check if any director[] argument strings are empty
  for (int i = 0; i < to_insert.num_directors; ++i) {
    if ( strlen(to_insert.directors[i]) == 0) {
      return 2;
    }
  }

  // check if any genres[] argument strings are empty
  for (int i = 0; i < to_insert.num_directors; ++i) {
    if (strlen(to_insert.genres[i]) == 0) {
      return 2;
    }
  }

  // allocate key. we use the shows name as the key
  char* key = new char[strlen(data->name)]();
  strcpy(key, data->name);
 
  // store hash of key as index into our table array
  int index = hash(key);

  // allocate node with key and data pointers and index
  Node * new_node = new Node(key,data,index);

  if (table[index] == nullptr) {
    table[index] = new_node;
    table[index]->next = nullptr;
  }
  else{
    new_node->next = table[index];
    table[index] = new_node;
  }

  return 0;
}

int Table::display(char* name)
{
  return 0; 
}

int Table::displayAll()
{
  Node * incrementer = nullptr;
  
  for (int i = 0; i < size; ++i) {
    incrementer = table[i];
    while(incrementer != nullptr) {
      incrementer->data->display();
      incrementer = incrementer->next;
    }
  }

  return 0; 
}

int Table::loadFromFile(char * filename)
{
  // assume no UserData member will recieve and entry over 50 characters
  const int MAX_ENTRY_LENGTH = 50;

  // declare character buffer to hold one line from the file (assume no more than 15 entries per line)
  char line_buffer[MAX_ENTRY_LENGTH * 15];

  // declare ifstream object "file" and open file into it
  ifstream file(filename);

  // make sure file opened succesfully
  // and return an error if not
  if (file.fail()) {
    return 1;
  }

  // assume file holds user data for at most 200 shows
  const int MAX_NUM_SHOWS = 200;
  // array to store show data
  UserData data[MAX_NUM_SHOWS];
  // index for data[]
  int index;

  // here we parse the file
  // we load one line from file into buffer for each
  // cycle of the loop until reaching EOF or an error
  while(file.getline(line_buffer, sizeof(line_buffer)))
  {
    // strtok() terminates each token into substrings with \0
    // and returns a pointer to a substring. we capture this
    // with entry_pointer.
    char* entry_ptr = strtok(line_buffer, "|");
      while(entry_ptr != nullptr)
      {
        // allocate name array on heap with size of name substring
        // then copy in the name entry
        data[index].name = new char[strlen(entry_ptr) + 1];
        strcpy(data[index].name, entry_ptr);
       
        // parsing directors takes more work as it is a char**
        // first we point to the next substring to start parsing directors entry
        // and then we declare a sub entru pointer to parse each director in the 
        // directors entry
        entry_ptr = strtok(line_buffer, "|");
        char* sub_entry_ptr = entry_ptr;
        // count how many directors are in this entry
        char* count_ptr = sub_entry_ptr;
        data[index].num_directors = 0;
         while (*count_ptr != '\0') {
           if(*count_ptr == ';') {
            ++data[index].num_directors;
           }
           ++count_ptr;
        }
        ++data[index].num_directors; // no ';' delimeter after last director!
       
        // allocate first dimension of directors to the number of directors in 
        // this entry
        data[index].directors = new char*[data[index].num_directors];

        // allocate and copy over each director string for each director[i]
        for (int i = 0; i < data[index].num_directors; ++i) {
          data[index].directors[i] = new char[strlen(sub_entry_ptr + 1)];
          strcpy(data[index].directors[i], sub_entry_ptr);
          // point to next sub entry
          sub_entry_ptr = strtok(NULL, ";");
        }

        // repeat above process for genres entry
        entry_ptr = strtok(line_buffer, "|");
        sub_entry_ptr = entry_ptr;

        count_ptr = sub_entry_ptr;
        data[index].num_genres = 0;
         while (*count_ptr != '\0') {
           if(*count_ptr == ';') {
            ++data[index].num_genres;
           }
           ++count_ptr;
        }
        ++data[index].num_genres; 
        
        data[index].genres = new char*[data[index].num_directors];

        for (int i = 0; i < data[index].num_genres; ++i) {
          data[index].genres[i] = new char[strlen(sub_entry_ptr + 1)];
          strcpy(data[index].genres[i], sub_entry_ptr);
          sub_entry_ptr = strtok(NULL, ";");
        }

        entry_ptr = strtok(NULL, "|");
        data[index].user_age = atoi(entry_ptr);

        entry_ptr = strtok(NULL, "|");
        data[index].user_gender = new char[strlen(entry_ptr) + 1];
        strcpy(data[index].user_gender, entry_ptr);

        entry_ptr = strtok(NULL, "|");
        data[index].user_location = new char[strlen(entry_ptr) + 1];
        strcpy(data[index].user_location, entry_ptr);

        entry_ptr = strtok(NULL, "|");
        data[index].user_rating = atoi(entry_ptr);

        entry_ptr = strtok(NULL, "|");
        data[index].user_watch_time = atoi(entry_ptr);

        ++index; 

      }
  }


  return 0;
}

Node* Table::searchByName(char* name)
{
  Node * incrementer = nullptr;
  Node * match = nullptr;

  for (int i = 0; i < size; ++i)
  {
    incrementer = table[i];
    while ((incrementer != nullptr) == 0) {
     if (strcmp(incrementer->data->name,name)) {
      i = size;
      match = incrementer;
      incrementer = incrementer->next;
     }
    }
  }

  return match;
}
