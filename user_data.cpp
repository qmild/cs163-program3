// Bryce Diedrich
// bryc3@pdx.edu
// CS163 Program #3
//
// <- header comments go here!

#include "user_data.h"
#include <cstring>
#include <iostream>
#include <ostream>

using namespace std;

UserData::UserData()
{
      this->name = nullptr;
      this->directors = nullptr;
      this->genres = nullptr;
      this->user_age = 0;
      this->user_gender = nullptr;
      this->user_location = nullptr;
      this->user_rating = 0;
      this->user_watch_time = 0;
      this->num_directors = 0;
      this->num_genres = 0;
}

UserData::~UserData()
{

  if(name != nullptr) {
    delete name;
  }

  for(int i = 0; i < num_directors ;++i) {
    if(directors[i] != nullptr) {
      delete[] directors[i];
    }
  }
  delete[] directors;

  for(int i = 0; i < num_genres ;++i) {
    if(genres[i] != nullptr) {
      delete[] genres[i];
    }
  }
  delete[] genres;

  if(user_gender != nullptr) {
    delete user_gender;
  }
  if(user_location != nullptr) {
    delete user_location;
  }
}

int UserData::init(char* name, 
    char** directors, 
    char** genres, 
    int user_age, 
    char* user_gender, 
    char* user_location,
    float user_rating,
    int user_watch_time,
    int num_directors,
    int num_genres)

{


  int size = 0;
  // set calling objects number data to match corresponding arguments
  this->user_age = user_age;
  this->user_watch_time = user_watch_time;
  this->num_directors = num_directors;
  this->num_genres = num_genres;
  this->user_rating = user_rating;

  // loop through directors char* array to count it's size
  while (directors[size] != nullptr) {
    ++size;
  }
  // allocate directors char* array to size + 1
  this->directors = new char*[size + 1];
  // for each element in directors array allocate
  // character array and copy it from argument
  for(int i = 0; i < num_directors; ++i) {
    int length = strlen(directors[i]) + 1;
    this->directors[i] = new char[strlen(directors[i])];
    strcpy(this->directors[i], directors[i]); 
  }

  // loop through genres char* array to count it's size
  while (genres[size] != nullptr) {
    ++size;
  }
  // allocate genres char* array to size + 1
  this->genres = new char*[size + 1];
  // for each element in genres array allocate
  // character array and copy it from argument
  for(int i = 0; i < num_genres; ++i) {
    int length = strlen(genres[i]) + 1;
    this->genres[i] = new char[strlen(genres[i])];
    strcpy(this->genres[i], genres[i]); 
  }

  // allocate all dynamic array data members
  // then copy them from corresponding arguments
  this->name = new char[strlen(name)];
  strcpy(this->name, name);
  this->user_gender = new char[strlen(user_gender)];
  strcpy(this->user_gender, user_gender);
  this->user_location = new char[strlen(user_location)];
  strcpy(this->user_location, user_location);

  return 0;
}

int UserData::display()
{

  cout << "\e[4mShow Info\e[0m" << endl;
  cout << "Name: " << name << endl;
  cout << "Directors: ";
  for(int i = 0; i < num_directors; ++i) {
    cout << directors[i] << " ";
  }
  cout << endl;

  cout << "Genres: ";
  for(int i = 0; i < num_genres ; ++i) {
    cout << genres[i] << " ";
  }
  cout << endl;

  cout << "\e[4mUser Data\e[0m" << endl;
  cout << "Age: " << user_age << endl;
  cout << "Gender: " << user_gender << endl;
  cout << "Location: " << user_location << endl;
  cout << "Rating: " << user_rating << endl;
  cout << "Watch Time: " << user_watch_time << endl;

  return 0;
}


