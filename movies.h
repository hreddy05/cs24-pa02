#pragma once
#include <string>
#include <vector>

struct Movie {
    std::string name;
    double rating;

    bool operator<(const Movie& other) const {
        return name < other.name;
    }
};

class MovieDB {
public:
    void loadCSV(const std::string& filename);
    void printAll(std::ostream& out) const;
    std::vector<const Movie*> rangeForPrefix(const std::string& prefix) const;
    void addMovie(const std::string& name, double rating);
    void sortMovies();
private:
    std::vector<Movie> movies_;
};

