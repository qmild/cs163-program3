#include "table.h"
#include "unistd.h"
#include <cstring>
#include <ostream>
#include <iostream>

using namespace std;

const int MAX_NAME = 50;

int run_table_insert(Table & my_table)
{
  UserData my_data;
  int result;
  char again;

  char name[MAX_NAME];
  int user_age;
  char user_gender[MAX_NAME];
  char user_location[MAX_NAME];
  float user_rating;
  int user_watch_time;
  int num_directors;
  int num_genres;

  cout << endl 
       << "We have to enter some example user data before we can insert it into our hash table!" << endl
       << "Note: our test.cpp does not include type checking on your input, so be careful to enter the correct data" << endl;
  do 
  {
    cout << endl
         << "Enter Name (string): ";
    cin.getline(name, MAX_NAME);

    cout << "How Many Directors? (int): ";
    cin >> num_directors;
    cin.ignore();
    char** directors = new char*[num_directors];
    for(int i = 0; i < num_directors; ++i) {
      directors[i] = new char[MAX_NAME];
    }
    for(int i = 0; i < num_directors; ++i) {
      cout << "Enter Direcor #" << num_directors <<" (string): ";
      cin.getline(directors[i], MAX_NAME);
    }

    cout << "How Many Genres? (int): ";
    cin >> num_genres;
    cin.ignore();
    char** genres = new char*[num_genres];
    for(int i = 0; i < num_genres; ++i) {
      genres[i] = new char[MAX_NAME];
    }
    for(int i = 0; i < num_genres; ++i) {
      cout << "Enter Genre #" << num_genres << " (string): ";
      cin.getline(genres[i], MAX_NAME); 
    }

    cout << "Enter User Age (int): ";
    cin >> user_age; cin.ignore();


    cout << "Enter User Gender (string): ";
    cin.getline(user_gender, MAX_NAME); 

    cout << "Enter User Location (string): ";
    cin.getline(user_location, MAX_NAME); 

    cout << "Enter User Rating (float): ";
    cin >> user_rating; cin.ignore();

    cout << "Enter User Watch Time (int): ";
    cin >> user_watch_time; cin.ignore();

    int init_result = my_data.init(name, 
        directors, 
        genres, 
        user_age, 
        user_gender, 
        user_location, 
        user_rating, 
        user_watch_time, 
        num_directors, 
        num_genres);
    cout << endl 
         << "Calling insert method..." << endl;
    result = my_table.insert(my_data);
    sleep(1);

    switch (result) {
      case 0:
        cout << "Method call returned sucess!" << endl;
        sleep(1);
        break;
      case 1:
        cout << "Method aborted with an error value of 1!" << endl 
             << "This indicates one or more of your input strings were uninitialized (nullptr)" << endl
             << "Please try again!"
             << endl;
        sleep(1);
        break;
      case 2:
        cout << "Method aborted with an error value of 2!" << endl 
             << "This indicates one  or more of your input strings were empty!" << endl
             << "Please try again!"
             << endl;
        sleep(1);
        break;
    }

    cout << "Run again? (y/n): ";
    cin >> again; cin.ignore();

  }while(again == 'Y' || again == 'y');

  cout << endl;
  sleep(1);
  return 0;
}

int run_loadFromFile(Table & my_table)
{
  int result;
  char again;
  char * filename;

  do {
    cout << "Please enter the filename: ";
    cin.getline(filename, MAX_NAME);
    sleep(1);

    cout << "Loading data from" << filename << endl;

    switch (result) {
      case 0:
        cout << "Method call returned success!" << endl;
        sleep(1);
        break;
      case 1:
        cout << "Method aborted with an error value of 1!" << endl 
             << "This indicates the file failed to open (possibly due to bad file name)" << endl 
             << "Please try again!" 
             << endl;
        sleep(1);
        break;
    }
  
  }while (again == 'Y' || again == 'y');

  return 0;
}

int main()
{
  Table my_table(200);
  int response = 1;

  
  do {
    cout << "\e[0m"   << "-------------------------------------------------------------------------------------------------------------------------------" << "\e[0m" << endl
         << "\e[1;4m" << "| Welcome to CS163 Program #3 test routine! Please select from the public methods below by entering the corresponding number. |" << "\e[0m" << endl
         << "\e[0m"   << "|                                                                                                                             |" << "\e[0m" << endl
         << "\e[m"    << "| 1. insert(UserData & to_insert)                                                                                             |" << "\e[0m" << endl 
         << "\e[0m"   << "-------------------------------------------------------------------------------------------------------------------------------" << "\e[0m" << endl
         << endl;

    cout << "Response: ";
    cin >> response; cin.ignore();

    switch (response) {
      case 1:
        run_table_insert(my_table);
        break;
      case 2:
        run_loadFromFile(my_table);
    }

  }while(response != 0);

  return 0;
}
