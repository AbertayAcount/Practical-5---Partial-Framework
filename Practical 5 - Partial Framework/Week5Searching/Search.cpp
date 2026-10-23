// Framework for CMP201-AX1.3: Searching
// William Kavanagh, Abertay University, w.kavanagh@abertay.ac.uk
// Partial Framework

#include "Search.h"
#include <vector>
#include <iostream>
#include <chrono>
#include <numeric>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <random>

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::nanoseconds;

std::vector<Movie> getSortedCopy(const std::vector<Movie>& movies)
{
	// Provided
	std::vector<Movie> sorted_movies = movies;
	std::sort(sorted_movies.begin(), sorted_movies.end(), [](const Movie& a, const Movie& b) {
		return a.id < b.id;
	});
	return sorted_movies;
}

std::vector<Movie> generateMovieDatabase(int numMovies)
{
	// Provided
	std::vector<Movie> movies;
	movies.reserve(numMovies);

	std::vector<int> ids(numMovies);
	std::iota(ids.begin(), ids.end(), 0); // 0, 1, 2, ...
	std::random_device rd;
	std::mt19937 gen(rd());
	std::shuffle(ids.begin(), ids.end(), gen);
	std::vector<std::string> first_half = { "The", "A", "Journey to", "Escape from", "Attack of the", "Revenge of the", "Chronicles of", "Legend of the", "Rise of the", "Dawn of the" };
	std::vector<std::string> second_half = { "Planet", "Robot", "Ninja", "Zombie", "Wizard", "Dragon", "Mountain", "Apocalypse", "Galaxy", "Future" };
	std::uniform_int_distribution<> title_dist(0, (int)first_half.size() - 1);
	std::uniform_int_distribution<> year_dist(1950, 2025);

	for (int i = 0; i < numMovies; ++i)
	{
		std::string title = first_half[title_dist(gen)] + " " + second_half[title_dist(gen)];
		int year = year_dist(gen);
		bool isFresh = false;
		movies.push_back({ ids[i], title, year, isFresh });		
	}
	return movies;
}

// ###########################################
// ############# SEARCHING ZONE ##############
// ###########################################

int LinearScan(const std::vector<Movie>& movies, int targetID)
{
	// TODO: Assessed [0.5]
	/*Int LinearScan(const std::vector<Movie>& movies, int targetID) [0.5] uses a
naïve O(n) search algorithm to search all element returning the index of the
movie with the targetID in the movies vector, or -1 if it cannot find the element.
You can assume that the vector will be sorted*/
	for (int i = 0; i < movies.size(); i++) 
	{
		if (movies[i].id == targetID) 
		{
			return i;
		}
	}
	return -1;
}

int BinarySearch(const std::vector<Movie>& movies, int targetID)
{
	// TODO: Assessed [1.0]
	/*Int BinarySearch(const std::vector<Movie>& movies, int targetID) [1.0] uses
an informed search O(log(n)) that only works on sorted data, returning -1 if an
element is not in the list. It will also return -1 if it finds an element in the vector
that is unsorted. It does not need to check that the full vector is sorted*/
	int low = 0, high = movies.size() - 1;

	while (low <= high) 
	{
		int mid = ((high - low) / 2) + low;

		if (movies[mid].id == targetID)
		{
			return mid;
		}
		else if (movies[mid].id > targetID)
		{
			high = mid + 1;
		}
		else 
		{
			low = mid + 1;
		}
	}
	return -1;
}

std::unordered_map<int, Movie> BuildMovieMap(const std::vector<Movie>& movies)
{
	// TODO: assessed [0.5]
	/*std::unordered_map<int, Movie> BuildMovieMap(const std::vector<Movie>&
movies) [0.5] transforms a vector of movies into an unsorted map of {<int> id -
<Movie> movie} pairs.*/
	std::unordered_map<int, Movie> movieMap;
	movieMap.reserve(movies.size());

	for (auto& movie : movies)
	{
		movieMap.emplace(movie.id, movie);
	}

	return movieMap;

}

Movie MapSearch(std::unordered_map<int, Movie>& movieMap, int targetID)
{
	// TODO: assessed [0.5]
	/*Movie MapSearch(std::unordered_map<int, Movie>& movieMap, int targetID)
[0.5] if the id exists as a key, returns the associated movie object or returns a
Movie object with an id of -1 if there is no object with the target id. Note: this
method should not add new elements to the map only failed searche*/
	auto it = movieMap.find(targetID);
	if (it != movieMap.end()) 
	{
		return it->second;
	}
	else 
	{
		Movie notFoundMovie{};
		notFoundMovie.id = -1;
		return notFoundMovie;
	}

}

// ###########################################
// ############# TIMING ZONE #################
// ###########################################

double timeLinearScan(const std::vector<Movie>& movies, const std::vector<int>& targetIDs)
{
	// TODO: Assessed [0.5]
	/*double timeLinearScan(const std::vector<Movie>& movies, const
std::vector<int>& targetIDs) [0.5] Uses the linear scan implementation and
times how long it takes to search the provided vector for each of the target IDs
in turn returning as a double the total time taken in nanoseconds*/
	auto startTime = std::chrono::high_resolution_clock::now();

	volatile int searchResult = -1;
	for (int targetID : targetIDs)
	{
		searchResult = LinearScan(movies, targetID);
	}

	auto endTime = std::chrono::high_resolution_clock::now();
	return static_cast<double>(duration_cast<std::chrono::nanoseconds>(endTime - startTime).count());

}

std::pair<double, double> timeBinarySearch(const std::vector<Movie>& movies, const std::vector<int>& targetIDs)
{
	// TODO: Assessed [1.0]
	/*std::pair<double,double> timeBinarySearch(const std::vector<Movie>&
movies, const std::vector<int>& targetIDs) [1.0] Stores the time taken in
nanoseconds to sort the vector using the provided getSortedCopy function as a
double as the first element of the pair. As with timeLinearScan, returns the total
time to search for each element in targetIDs in nanoseconds as a double, stored
as the second element of the pair.*/
	auto sortStartTime = std::chrono::high_resolution_clock::now();
	const std::vector<Movie> sortedMovies = getSortedCopy(movies);
	auto sortEndTime = std::chrono::high_resolution_clock::now();

	volatile int searchResult = -1;
	auto searchStartTime = std::chrono::high_resolution_clock::now();

	for (int targetID : targetIDs)
	{
		searchResult = BinarySearch(sortedMovies, targetID);
	}

	auto searchEndTime = std::chrono::high_resolution_clock::now();

	const double sortTime = static_cast<double>(duration_cast<std::chrono::nanoseconds>(sortEndTime - sortStartTime).count());

	const double searchTime = static_cast<double>(duration_cast<std::chrono::nanoseconds>(searchEndTime - searchStartTime).count());

	return { sortTime, searchTime };
}

std::pair<double, double> timeMapSearch(const std::vector<Movie>& movies, const std::vector<int>& targetIDs)
{
	// TODO: Assessed [1.0]
	/*std::pair<double, double> timeMapSearch(const std::vector<Movie>& movies,
const std::vector<int>& targetIDs) [1.0] As with timeBinarySearch(), but
stores time to generate map with assessed BuildMovieMap function and then
time to search for all elements in targetIDs.*/
	auto mapStartTime = std::chrono::high_resolution_clock::now();
	std::unordered_map<int, Movie> movieMap = BuildMovieMap(movies);
	auto mapEndTime = std::chrono::high_resolution_clock::now();

	volatile int searchResult = -1;
	auto searchStartTime = std::chrono::high_resolution_clock::now();

	for (int targetID : targetIDs)
	{
		searchResult = MapSearch(movieMap, targetID).id;
	}

	auto searchEndTime = std::chrono::high_resolution_clock::now();

	const double mapTime = static_cast<double>(duration_cast<std::chrono::nanoseconds>(mapEndTime - mapStartTime).count());

	const double searchTime = static_cast<double>(duration_cast<std::chrono::nanoseconds>(searchEndTime - searchStartTime).count());

	return { mapTime, searchTime };
}

// MAIN() provided for testing purposes. Comment out or remove before submission or a mark will be deducted from your grade.
// Note: The testing coverage in Main() is not sufficient to ensure you will get full marks. 
int main()
{
	// 1. Setup 

	// Experimental constants (muck about with these)
	const int DB_SIZE = 100000;
	const int NUM_SEARCHES = 10000;

	// RNG setup and prompting
	std::cout << "Generating the movie database with " << DB_SIZE << " films with shuffled IDs...\n"; // slow process so have a little prompt.
	auto movieDB = generateMovieDatabase(DB_SIZE);
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(0, DB_SIZE - 1);

	// Choose a film at random, swap it out with The Matrix. Also make Face/Off that is not in the database.
	int randomPosition = dist(gen);
	std::cout << "random position selected as " << randomPosition << ", Inserting 'The Matrix' into this position\n\n";
	Movie theMatrix = {
		movieDB[randomPosition].id,
		"The Matrix",
		1999,
		true
	};
	Movie faceOff = {
		DB_SIZE + 1,
		"Face/Off",
		1997,
		true
	};
	movieDB[randomPosition] = theMatrix;

	// 2. Test that the searching algorithms work.

	int linearScanForTheMatrix = LinearScan(movieDB, theMatrix.id);
	std::cout << "Linear Scan found The Matrix at position " << linearScanForTheMatrix << "\n";
	std::cout << "The film as this position is called: " << movieDB[linearScanForTheMatrix].title << "\n";
	int LinearScanForFaceOff = LinearScan(movieDB, faceOff.id);
	std::cout << "Linear Scan found Face/off at position " << LinearScanForFaceOff << "\n\n";

	std::cout << "Making sorted copy of the database for binary search...\n";
	auto sortedDB = getSortedCopy(movieDB);
	int binarySearchForTheMatrix = BinarySearch(sortedDB, theMatrix.id);
	std::cout << "Binary search found The Matrix at position " << binarySearchForTheMatrix << "\n";
	std::cout << "The movie title at this position is: " << sortedDB[binarySearchForTheMatrix].title << "\n";
	int binarySearchForFaceOff = BinarySearch(sortedDB, faceOff.id);
	std::cout << "Binary search found Face/Off at position " << binarySearchForFaceOff << "\n\n";

	std::cout << "Generating the unordered map for map search...\n";
	auto movieMap = BuildMovieMap(movieDB);
	Movie mapSearchForTheMatrix = MapSearch(movieMap, theMatrix.id);
	mapSearchForTheMatrix.title == "The Matrix" ? std::cout << "map search found The Matrix\n" : std::cout << "map search did not find The Matrix\n";
	Movie mapSearchForFaceOff = MapSearch(movieMap, faceOff.id);
	mapSearchForFaceOff.title == "Face/Off" ? std::cout << "map search found Face/Off\n\n" : std::cout << "map search did not find Face/Off\n\n";

	// 3. Testing the tests

	// Time to find one element.
	std::vector<int> singleSearch = { dist(gen) };	// Just one element
	std::vector<int> searchIDs;
	for (int i = 0; i < NUM_SEARCHES; ++i) {
		searchIDs.push_back(movieDB[dist(gen)].id);
	}
	
	auto timeTaken = timeLinearScan(movieDB, singleSearch);
	std::cout << "Linear scan searched for one element in " << std::fixed << std::setprecision(2) << timeTaken << " ns\n";
	timeTaken = timeLinearScan(movieDB, searchIDs);
	std::cout << "Linear scan search for " << NUM_SEARCHES << " elements in " << std::fixed << std::setprecision(2) << timeTaken << " ns\n\n";

	std::pair<double,double> timesTaken = timeBinarySearch(movieDB, singleSearch);
	std::cout << "Sorted database in " << std::fixed << std::setprecision(2) << timesTaken.first << " ns\n";
	std::cout << "Binary searched for one element in " << std::fixed << std::setprecision(2) << timesTaken.second << " ns\n";
	
	timesTaken = timeBinarySearch(movieDB, searchIDs);
	std::cout << "Sorted database in " << std::fixed << std::setprecision(2) << timesTaken.first << " ns\n";
	std::cout << "Binary searched for " << NUM_SEARCHES << " elements in " << std::fixed << std::setprecision(2) << timesTaken.second << " ns\n\n";

	timesTaken = timeMapSearch(movieDB, singleSearch);
	std::cout << "Generated map in " << std::fixed << std::setprecision(2) << timesTaken.first << " ns\n";
	std::cout << "Map searched for one element in " << std::fixed << std::setprecision(2) << timesTaken.second << " ns\n";

	timesTaken = timeMapSearch(movieDB, searchIDs);
	std::cout << "Generated map in " << std::fixed << std::setprecision(2) << timesTaken.first << " ns\n";
	std::cout << "Map searched for " << NUM_SEARCHES << " elements in " << std::fixed << std::setprecision(2) << timesTaken.second << " ns\n\n";

	return 0;
}
