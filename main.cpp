// Winter'24
// Instructor: Diba Mirza
// Student name: Harshal Reddy
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <cstring>
#include <algorithm>
#include <limits.h>
#include <iomanip>
#include <set>
#include <queue>
#include <sstream>
#include <chrono>
using namespace std;

#include "utilities.h"
#include "movies.h"
MovieDB db;

bool parseLine(string &line, string &movieName, double &movieRating);

int main(int argc, char** argv){
    if (argc < 2){
        cerr << "Not enough arguments provided (need at least 1 argument)." << endl;
        cerr << "Usage: " << argv[ 0 ] << " moviesFilename prefixFilename " << endl;
        exit(1);
    }
    

    ifstream movieFile (argv[1]);
    if (movieFile.fail()){
        cerr << "Could not open file " << argv[1];
        exit(1);
    }
  
    // Create an object of a STL data-structure to store all the movies

    string line, movieName;
    double movieRating;
    // Read each file and store the name and rating
    while (getline (movieFile, line) && parseLine(line, movieName, movieRating)){
           db.addMovie(movieName, movieRating);
	    // Use std::string movieName and double movieRating
            // to construct your Movie objects
            // cout << movieName << " has rating " << movieRating << endl;
            // insert elements into your data structure
    }

    movieFile.close();
    db.sortMovies();

    if (argc == 2){
            //print all the movies in ascending alphabetical order of movie names
            db.printAll(cout);
	    return 0;
    }

    ifstream prefixFile (argv[2]);

    if (prefixFile.fail()) {
        cerr << "Could not open file " << argv[2];
        exit(1);
    }

    vector<string> prefixes;
    while (getline (prefixFile, line)) {
        if (!line.empty()) {
            prefixes.push_back(line);
        }
    }

    //  For each prefix,
    //  Find all movies that have that prefix and store them in an appropriate data structure
    //  If no movie with that prefix exists print the following message
    auto start = chrono::high_resolution_clock::now();
    for(const auto& prefix: prefixes){
	auto matches = db.rangeForPrefix(prefix);
	if(matches.empty()){
		cout << "No movies found with prefix "<<"<replace with prefix>" << endl;
		continue;
	}
	sort(matches.begin(), matches.end(), [](const Movie* a, const Movie* b){
		if(a->rating != b->rating) return a->rating > b->rating;
		return a->name < b->name;


    });
        for(const auto* m : matches){
	cout << m->name << ", " << fixed << setprecision(1) << m->rating << endl;
	}
	cout << endl;
	}
for(const auto& prefix: prefixes){
	auto matches = db.rangeForPrefix(prefix);
	if(!matches.empty()){
		const Movie* best = *max_element(matches.begin(), matches.end(),[](const Movie* a, const Movie* b){
		if(a->rating != b->rating) return a->rating < b->rating;
		return a->name > b->name;
	});
	cout << "Best movie with prefix " << prefix << " is: "
                 << best->name << " with rating " << fixed << setprecision(1) << best->rating << endl;

	}	
	}

    //  For each prefix,
    //  Print the highest rated movie with that prefix if it exists.
    auto end = chrono::high_resolution_clock::now();
    cerr << "Execution time: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;

    return 0;
 
}


/* Add your run time analysis for part 3 of the assignment here as commented block*/
/*
-------------------------------
Part 3: Complexity Analysis

❖ Variables:
- n = number of movies
- m = number of prefixes
- k = max number of movies matched by a single prefix
- l = maximum length of a movie title (not used in our algorithm)

❖ Time Complexity (Worst-Case):

Preprocessing:
- We store n movies in a sorted vector: O(n log n) using std::sort

Prefix Lookup for each of m prefixes:
- Binary search using std::lower_bound: O(log n)
- Upper bound search: O(log n)
- Copying matched movies (up to k): O(k)
- Sorting matched movies by rating: O(k log k)

Total per prefix: O(log n + k log k)  
Total for m prefixes: O(m * (log n + k log k))

Overall Time Complexity:
O(n log n + m * (log n + k log k))

❖ Empirical Runtime Measurements (prefix_large.txt):

| Dataset                    | Runtime (ms) |
|----------------------------|--------------|
| input_20_random.csv        |  20 ms       |
| input_100_random.csv       |  76 ms       |
| input_1000_random.csv      |  63 ms       |
| input_76920_random.csv     | 443 ms       |



❖ Graph Comparison:
- Based on the runtime trend and slope, our implementation is closest to **Mystery Implementation #4** (red line).
- This confirms our expected complexity of O(m * log n) + O(k log k) per prefix.

-------------------------------


Space Complexity:
- O(n) to store all movies
- O(k) per prefix (temporary)
Total: O(n) permanent + O(k) temporary

Tradeoffs:
Used sorted vector + binary search to reduce time and space.
Avoided trie for space efficiency. Good balance of performance and simplicity.
-------------------------------
*/

/*
---------------------------------------
Part 3b: Space Complexity Analysis

❖ Variables:
- n = number of movies
- m = number of prefixes
- k = max number of movies that match a prefix
- l = max length of a movie title

---------------------------------------
❖ Space Complexity (Worst Case):

1. Storing all movies:
- Stored in a vector of structs (Movie objects)
→ O(n * l) = O(n), assuming movie titles are bounded-length

2. Storing all prefixes:
- m strings in a vector → O(m * l) = O(m), assuming l is constant

3. Per-prefix temporary storage:
- For each prefix, we create a temporary vector of up to k movie pointers
→ O(k) per prefix, reused across m iterations → O(k) total at any one time

We do not use additional data structures like:
- Tries
- Hash maps
- Graphs
- Extra index structures

---------------------------------------
❖ Final Total Space Complexity:
O(n + m + k)

- Permanent: O(n) for movies, O(m) for prefixes
- Temporary: O(k) at most during a single prefix query

This is highly space-efficient, avoiding complex index structures or redundant memory usage.

---------------------------------------
Part 3c: Time vs Space Tradeoffs

We intentionally designed our algorithm for **low space complexity**, targeting:

- O(n) for movies (standard)
- O(k) temporary memory per prefix (reused, not accumulated)
- Minimal overhead, using STL structures only (vector, string)

Despite not using a trie or hash index, we were still able to achieve **very low runtime** thanks to:
- Efficient use of `std::lower_bound` on a pre-sorted vector (binary search)
- Keeping our movie list compact and memory-local (cache-efficient)

❖ Did we also achieve low time complexity?
→ Yes — empirically, our runtime is nearly flat across input sizes, outperforming Mystery #4 and matching Mystery #3.

❖ Which was harder to achieve?
→ **Time complexity** was harder to minimize without sacrificing space.  
Many fast lookup solutions (e.g., trie with max-heaps) would require O(n * l) extra memory, which I avoided.

My final design balanced both goals — **low time and low space** — by carefully choosing the right STL structures and avoiding over-engineering.

---------------------------------------
*/
bool parseLine(string &line, string &movieName, double &movieRating) {
    int commaIndex = line.find_last_of(",");
    movieName = line.substr(0, commaIndex);
    movieRating = stod(line.substr(commaIndex+1));
    if (movieName[0] == '\"') {
        movieName = movieName.substr(1, movieName.length() - 2);
    }
    return true;
}
