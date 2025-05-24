#include "movies.h"
#include <fstream>
#include <sstream>
#include <algorithm>

void MovieDB::loadCSV(const std::string& filename) {
    std::ifstream inFile(filename);
    std::string line;
    while (std::getline(inFile, line)) {
        std::stringstream ss(line);
        std::string title;
        std::getline(ss, title, ',');
        std::string ratingStr;
        std::getline(ss, ratingStr);
        Movie m;
        m.name = title;
        m.rating = std::stod(ratingStr);
        movies_.push_back(m);
    }
    std::sort(movies_.begin(), movies_.end());
}

void MovieDB::printAll(std::ostream& out) const {
    for (const auto& m : movies_) {
        out << m.name << ", " << m.rating << "\n";
    }
}

std::vector<const Movie*> MovieDB::rangeForPrefix(const std::string& prefix) const {
    auto comp = [](const Movie& m, const std::string& p) {
        return m.name.compare(0, p.length(), p) < 0;
    };
    auto lower = std::lower_bound(movies_.begin(), movies_.end(), prefix, comp);

    std::string next = prefix;
    if (!next.empty()) {
        ++next.back();
    }
    auto upper = std::lower_bound(movies_.begin(), movies_.end(), next, comp);

    std::vector<const Movie*> result;
    for (auto it = lower; it != upper; ++it) {
        result.push_back(&(*it));
    }
    return result;
}
void MovieDB::addMovie(const std::string& name, double rating){
	movies_.push_back(Movie{name, rating});
}

void MovieDB::sortMovies(){
	std::sort(movies_.begin(), movies_.end());
}
