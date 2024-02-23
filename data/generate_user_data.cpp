#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <set>
#include <cmath>

const int MAX_LENGTH = 256;

// Function to generate a random integer following a normal distribution
int generateRandomAge(int mean, int stddev) {
    static bool hasSpare = false;
    static double spare;
    if(hasSpare) {
        hasSpare = false;
        return mean + stddev * spare;
    }
    hasSpare = true;
    double u, v, s;
    do {
        u = (rand() / ((double)RAND_MAX)) * 2.0 - 1.0;
        v = (rand() / ((double)RAND_MAX)) * 2.0 - 1.0;
        s = u * u + v * v;
    } while (s >= 1 || s == 0);
    s = sqrt(-2.0 * log(s) / s);
    spare = v * s;
    return mean + stddev * u * s;
}

// Function to generate a random gender
std::string generateRandomGender() {
    std::string genders[] = {"Male", "Female", "Other"};
    return genders[rand() % 3];
}

// Function to generate a random location
std::string generateRandomLocation() {
    std::string locations[] = {
        "New York", "Los Angeles", "Chicago", "Miami", "San Francisco",
        "London", "Paris", "Rome", "Berlin", "Madrid",
        "Tokyo", "Seoul", "Beijing", "Sydney", "Moscow",
        "Toronto", "Mexico City", "Rio de Janeiro", "Buenos Aires", "São Paulo",
        "Cairo", "Dubai", "Mumbai", "Bangkok", "Singapore"
    };
    return locations[rand() % 25];
}

// Function to generate a random movie name
std::string generateRandomName() {
    std::string names[] = {
        "The Matrix", "Inception", "Pulp Fiction", "The Godfather", "Forrest Gump", "The Shawshank Redemption",
        "Jurassic Park", "The Dark Knight", "Fight Club", "Gladiator", "Titanic", "Avatar",
        "The Lion King", "The Lord of the Rings", "Star Wars", "Indiana Jones", "Back to the Future", "E.T.",
        "The Avengers", "Harry Potter", "The Terminator", "Die Hard", "Alien", "The Exorcist",
        "Casablanca"
    };
    return names[rand() % 25];
}

// Function to generate a random director
std::string generateRandomDirector() {
    std::string directors[] = {
        "Christopher Nolan", "Quentin Tarantino", "Francis Ford Coppola", "Martin Scorsese", "Steven Spielberg",
        "James Cameron", "Alfred Hitchcock", "Stanley Kubrick", "Clint Eastwood", "Ridley Scott",
        "Tim Burton", "David Fincher", "Spike Lee", "George Lucas", "Akira Kurosawa",
        "Woody Allen", "Peter Jackson", "Coen Brothers", "Wes Anderson", "David Lynch",
        "Ang Lee", "Roman Polanski", "Michael Bay", "Paul Thomas Anderson", "John Woo"
    };
    return directors[rand() % 25];
}

// Function to generate a random genre
std::string generateRandomGenre() {
    std::string genres[] = {"Action", "Adventure", "Comedy", "Drama", "Sci-Fi", "Thriller"};
    return genres[rand() % 6];
}

// Function to generate a random rating
float generateRandomRating() {
    return round(10 * (static_cast <float> ((rand()) / static_cast<float>((RAND_MAX/5))))) / 10; // Rating from 1 to 10
}

// Function to generate a random watch time
int generateRandomWatchTime() {
    return rand() % 180 + 60; // Watch time from 60 to 240 minutes
}

// Function to generate a random movie in the specified format
std::string generateRandomMovie() {
    std::string movie;
    movie += "|";
    movie += generateRandomName();
    movie += "|";
    // Generate random number of directors separated by ";"
    int numDirectors = rand() % 3 + 1; // Random number of directors (1 to 3)
    for (int i = 0; i < numDirectors; ++i) {
        if (i > 0) movie += ";";
        movie += generateRandomDirector();
    }
    movie += "|";
    // Generate random number of genres separated by ";"
    int numGenres = rand() % 3 + 1; // Random number of genres (1 to 3)
    for (int i = 0; i < numGenres; ++i) {
        if (i > 0) movie += ";";
        movie += generateRandomGenre();
    }
    movie += "|";
    movie += std::to_string(generateRandomAge(25, 10)); // Mean age 25, stddev 10
    movie += "|";
    movie += generateRandomGender();
    movie += "|";
    movie += generateRandomLocation();
    movie += "|";
    movie += std::to_string(generateRandomRating());
    movie += "|";
    movie += std::to_string(generateRandomWatchTime());
    movie += "\n";
    return movie;
}

int main() {
    // Seed random number generator
    srand(time(NULL));

    // Generate a list of unique random movies
    std::ofstream outputFile("user_data.txt");
    std::set<std::string> uniqueMovies;
    while (uniqueMovies.size() < 200) {
        uniqueMovies.insert(generateRandomMovie());
    }
    for (const std::string& movie : uniqueMovies) {
        outputFile << movie;
    }
    outputFile.close();

    std::cout << "Movies generated and saved to movies.txt" << std::endl;

    return 0;
}
