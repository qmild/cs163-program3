// Bryce Diedrich
// bryc3@pdx.edu
// CS163 Program #3
//
// <- header comments go here!

class UserData
{
   public:
    UserData();
    ~UserData();
    int init(char* name, 
            char** directors, 
            char** genres, 
            int use_age, 
            char* user_gender, 
            char* user_location,
            float user_rating,
            int user_watch_time,
            int num_directors,
            int num_genres);
    int display();

    char* name;
    char** directors;
    char** genres;
    int user_age;
    char * user_gender;
    char * user_location;
    float user_rating;
    int user_watch_time;
    int num_directors;
    int num_genres;
};

