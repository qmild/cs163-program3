// Bryce Diedrich
// bryc3@pdx.edu
// CS163 Program #3
//
// This file implements our Table class. We have the following public methods.
// - insert:
// - remove
// - retrieve
// - display
// - displayByGenre
// - displayAll
// - loadFromFile
// The only method that modifies an argument is retrieve. It copies data into the UserData object passed
// by reference.

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

// this hash function takes a string and computes a polynomial, addding
// ASCI value of letters multiplied by a prime base to the power of the 
// letters position
int Table::hash(char*  key)
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

// used for efficiency test same as hash() but with a non-prime base
int Table::badhash(char*  key)
{
  unsigned int hash_result = 0;
  unsigned int power = 1;

  for(int i = 0; i < strlen(key); ++i)
  {
    hash_result = (hash_result + (key[i] * power)) % size;
    // increase the power by one degree, and take it modulo UINT_MAX to avoid overflow 
    power = (power * BAD_BASE) % UINT_MAX;
  }

  return hash_result;
}

// allocates a new node, and allocate a new UserData object pointed to by the node.
// then copies the data from to_insert into the allocated UserData object before
// inserting the node onto the hash table where index = hash(to_insert.name)
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
  for (int i = 0; i < to_insert.num_genres; ++i) {
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
  for (int i = 0; i < to_insert.num_genres; ++i) {
    if (strlen(to_insert.genres[i]) == 0) {
      return 2;
    }
  }


  // allocate key. we use the shows name as the key
  char* key = new char[strlen(data->name)]();
  strcpy(key, data->name);



  // store hash of key as index into our table array
  int index = hash(key);

  // uncomment to use badHash instead for testing
  //int index = badhash(key);

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

// calls searchByName() to get the index of data matching name
// if match is at the head of the LLL, increment head and delete node
// if not at head, use a prev pointer to delete node
int Table::remove(char *name)
{
  int index = 0;
  // calling searchByName for the index, not the Node* return value as we will need a prev ptr
  Node* to_remove = searchByName(name, index);
  Node* head = table[index];

  // return 1 if we did not find the matching name
  if (to_remove == nullptr) {
    return 1;
  }

  // if object to be deleted is at the head of the list 
  if(head != nullptr && strcmp(head->data->name, name) == 0)
  {
    Node* deleter = head;
    head = head->next;
    delete deleter;
    return 0;
  }

  // find previous pointer
  Node* prev = nullptr; 
  Node* inc = head;

  while (inc != nullptr && strcmp(inc->data->name, name)) {
    prev = inc;
    inc = inc->next;
  }

  // return 1 if we did not find the matching name
  if (inc == nullptr) {
    return 1;
  }

  // remove matchcing node
  prev->next = inc->next;
  delete inc;
  
  return 0; 
}

// calls searchByName to get a pointer to node with matching name
// we then call init UserData method on out_data to fill it 
// with data from matching name
int Table::retrieve(char *name, UserData &out_data)
{
  int index = 0;
  Node* to_retrieve = searchByName(name, index);

  if (to_retrieve == nullptr) {
    return 1;
  }

  int result = out_data.init(to_retrieve->data->name, 
      to_retrieve->data->directors, 
      to_retrieve->data->genres, 
      to_retrieve->data->user_age, 
      to_retrieve->data->user_gender, 
      to_retrieve->data->user_location, 
      to_retrieve->data->user_rating, 
      to_retrieve->data->user_watch_time, 
      to_retrieve->data->num_directors, 
      to_retrieve->data->num_genres);
 
  return result;
}

// calls searhByName to get pointer to node with matching name
// then calls UserData method display() on that node
int Table::display(char* name)
{
  int index;
  // get pointer to object with matching name
  // if no match is found, nullptr will be returned
  Node* to_display = searchByName(name, index);

    // return error due to no match
  if (to_display == nullptr) {
    return 1;
  }

  to_display->data->display();

  return 0; 
}

// outer for loop loops through the table
// inner while steps through the associated LLL at current element
// we then use a third for loop to check each genre[j] for a match
// with strcmp
int Table::displayByGenre(char *genre)
{
  Node * incrementer = nullptr;
  bool match = false;

  for (int i = 0; i < size; ++i) {
    incrementer = table[i];
    while(incrementer != nullptr) {
      for (int j = 0; j < incrementer->data->num_genres + 1; ++j) {
        if (strcmp(incrementer->data->genres[j],genre) == 0) {
          match = true;
          j = incrementer->data->num_genres + 1;
        }
      }

      if(match == true){
        incrementer->data->display();
      }
      incrementer = incrementer->next;
    }
  }

  return 0;
}

// loops through table, with an inner loop at each element
// to step through associated LLL, calling display (from
// UserData) on each node
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

// used in loadFromFile to count director and genre sub-entries
// uses starting pointer to step through each character and count the number of semicolons before
// reaching a | delimiter. Also replaces each semicolon with \0 null terminator when they are found
int countAndFormat(char* list_start)
{
  char* inc = list_start;
  int count = 0;
  while(*inc != '|')
  {
    if(*inc == ';')
    {
      ++count;
      *inc = '\0';
    }
    ++inc;
  }
  return count;
}

// loads from a file one line at a time with a line buffer. | delimeters mark an entry and some
// entries like directors have sub entries with ; delimeter. Uses pointer that point at the start of an entry
// and one that points to the start of the next entry to calculate the size of the entry via pointer arithmetic
// and then uses strncmp to copy a string by size instead of \o null character.
// the case of directors and genres is a little more difficult but that is explained in comments below
int Table::loadFromFile(char* filename)
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
  int index = 0;

  // parse file one line at a time
  while(file.getline(line_buffer, sizeof(line_buffer)))
  {
    // skip the first |
    char* entry_start = line_buffer + 1;
    // point to start of directors entry
    char* directors_start = strstr(entry_start, "|") + 1;
    // calculate the size of name with ptr arithmetic
    int size_name = directors_start - entry_start - 1;
    // allocate UserData object's name array
    data[index].name = new char[size_name + 1];
    // copy over name from the name entry
    strncpy(data[index].name, entry_start, size_name);
    data[index].name[size_name] = '\0';

    // this function returns a number of ; deliminated subentries
    // in a | deliminated entry and replaces ; with \0
    data[index].num_directors = countAndFormat(directors_start);
    data[index].directors = new char*[data[index].num_directors];

    // Copy each director name
    for(int director = 0; director < data[index].num_directors; ++director)
    {
      data[index].directors[director] = new char[strlen(directors_start) + 1];
      strcpy(data[index].directors[director], directors_start);
      directors_start += strlen(directors_start) + 1;
    }

    // skip the "|" again, now we should point to the first genre
    entry_start = directors_start + 1;

    int num_genres = countAndFormat(entry_start);

    data[index].genres = new char*[num_genres];
    data[index].num_genres = num_genres;

    for(int genre = 0; genre < num_genres; ++genre)
    {
      data[index].genres[genre] = new char[strlen(entry_start) + 1];
      strcpy(data[index].genres[genre], entry_start);
      entry_start += strlen(entry_start) + 1;
    }

    // Move to the start of age
    entry_start = entry_start + 1;
    char* gender_start = strstr(entry_start, "|") + 1;
    int age_size = gender_start - entry_start - 1;

    // leave room for '\0'
    char* age_str = new char[age_size + 1]; // TODO: DELETE THIS LATER

    // copy the movie name and add '\0'
    strncpy(age_str, entry_start, age_size);
    age_str[age_size] = '\0';
    data[index].user_age = atoi(age_str);

    // repeat for gender
    char* location_start = strstr(gender_start, "|") + 1;
    int gender_size = location_start - gender_start - 1;
    data[index].user_gender = new char[gender_size + 1];
    strncpy(data[index].user_gender, gender_start, gender_size);
    data[index].user_gender[gender_size] = '\0';

    // repeat for location
    char* rating_start = strstr(location_start, "|") + 1;
    int location_size = rating_start - location_start - 1;
    data[index].user_location = new char[location_size + 1];
    strncpy(data[index].user_location, location_start, location_size);
    data[index].user_location[location_size] = '\0';

    // repeat for rating
    char* watch_time_start = strstr(rating_start, "|") + 1;
    int rating_size = watch_time_start - rating_start - 1;
    char* rating_str = new char[rating_size + 1]; // TODO: DELETE THIS LATER
    strncpy(rating_str, rating_start, rating_size);
    rating_str[rating_size] = '\0';
    data[index].user_rating = atof(rating_str);

    // repeat for watch_time
    int watch_time_size = strlen(watch_time_start);
    char* watch_time_str = new char[watch_time_size + 1]; // TODO: DELETE THIS LATER
    strcpy(watch_time_str, watch_time_start);
    data[index].user_watch_time = atoi(watch_time_str);

    insert(data[index]);
    ++index;
  }
 
  return 0;
}

// loops through table with inner loop through each asociated LLL
// sets match pointer to the current incrementer and exits loop when
// strcmp shows a character match to name data
Node* Table::searchByName(char* name, int & out_index)
{
  Node * incrementer = nullptr;
  Node * match = nullptr;

  for (int i = 0; i < size; ++i)
  {
    incrementer = table[i];
    while ((incrementer != nullptr)) {
      if (strcmp(incrementer->data->name,name) == 0) {
        out_index = i;
        i = size;
        match = incrementer;
        incrementer = nullptr;
      }
      incrementer = incrementer->next;

    }
  }

  return match;
}
