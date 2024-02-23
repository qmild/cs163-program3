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

int countAndFormatList(char *listStart)
{
  char *iter = listStart;
  int count = 0;
  while(*iter != '|')
  {
    if(*iter == ';')
    {
      ++count;
      *iter = '\0';
    }
    iter++;
  }
  return count;
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
  int index = 0;

  // here we parse the file
  // we load one line from file into buffer for each
  // cycle of the loop until reaching EOF or an error
  while(file.getline(line_buffer, sizeof(line_buffer)))
  {
    // skip the first |
    char *start = line_buffer + 1;

    // find the start of the directors section
    char *directorStart = strstr(start, "|") + 1;

    // get the size of the movie name
    int movieNameSize = directorStart - start - 1;

    // leave room for '\0'
    data[index].name = new char[movieNameSize + 1];

    // copy the movie name and add '\0'
    strncpy(data[index].name, start, movieNameSize);
    data[index].name[movieNameSize] = '\0';

    // Count the number of directors, replace ; with '\0'
    int numDirectors = countAndFormatList(directorStart);

    data[index].directors = new char*[numDirectors];
    data[index].num_directors = numDirectors;

    // Copy each director name
    for(int director = 0; director < numDirectors; director++)
    {
      data[index].directors[director] = new char[strlen(directorStart) + 1];
      strcpy(data[index].directors[director], directorStart);
      directorStart += strlen(directorStart) + 1;
    }

    // skip the "|" again, now we should point to the first genre
    start = directorStart + 1;

    int numGenres = countAndFormatList(start);

    data[index].genres = new char*[numGenres];
    data[index].num_genres = numGenres;

    for(int genre = 0; genre < numGenres; genre++)
    {
      data[index].genres[genre] = new char[strlen(start) + 1];
      strcpy(data[index].genres[genre], start);
      start += strlen(start) + 1;
    }
    
    // Move to the start of age
    start = start + 1;
    char *genderStart = strstr(start, "|") + 1;
    int ageSize = genderStart - start - 1;
    
    // leave room for '\0'
    char *ageStr = new char[ageSize + 1]; // TODO: DELETE THIS LATER

    // copy the movie name and add '\0'
    strncpy(ageStr, start, ageSize);
    ageStr[ageSize] = '\0';
    data[index].user_age = atoi(ageStr);
    
    char *locationStart = strstr(genderStart, "|") + 1;
    int genderSize = locationStart - genderStart - 1;
    data[index].user_gender = new char[genderSize + 1];
    strncpy(data[index].user_gender, genderStart, genderSize);
    data[index].user_gender[genderSize] = '\0';

    char *ratingStart = strstr(locationStart, "|") + 1;
    int locationSize = ratingStart - locationStart - 1;
    data[index].user_location = new char[locationSize + 1];
    strncpy(data[index].user_location, locationStart, locationSize);
    data[index].user_location[locationSize] = '\0';

    char *watchTimeStart = strstr(ratingStart, "|") + 1;
    int ratingSize = watchTimeStart - ratingStart - 1;
    char *ratingStr = new char[ratingSize + 1]; // TODO: DELETE THIS LATER
    strncpy(ratingStr, ratingStart, ratingSize);
    ratingStr[ratingSize] = '\0';
    data[index].user_rating = atof(ratingStr);

    int watchTimeSize = strlen(watchTimeStart);
    char *watchTimeStr = new char[watchTimeSize + 1]; // TODO: DELETE THIS LATER
    strcpy(watchTimeStr, watchTimeStart);
    data[index].user_watch_time = atoi(watchTimeStr);
    
    insert(data[index]);
    index++;
  }

  // This prints the table back out in the format it was read
  // for(int i = 0; i < index; i++)
  // {
  //   cout << data[i].name << "|";
  //   for(int j = 0; j < data[i].num_directors; j++)
  //   {
  //     cout << data[i].directors[j] << ";";
  //   }
  //   cout << "|";
  //   for(int k = 0; k < data[i].num_genres; k++)
  //   {
  //     cout << data[i].genres[k] << ";";
  //   }
  //   cout << "|";
  //   cout << data[i].user_age << "|" << data[i].user_gender << "|" << data[i].user_location << "|" << data[i].user_rating << "|" << data[i].user_watch_time << "|" << endl;
  // }

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
