// Framework for CMP201-AX1.3: Searching
// William Kavanagh, Abertay University, w.kavanagh@abertay.ac.uk
// Header file.

#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>

#pragma once

// A simple struct representing a film
struct Movie {
	int id;
	std::string title;
	int year;
	bool bCertifiedFresh;
};

// ~~~~~~~~~~~~~~~~
// PROVIDED METHODS
// ~~~~~~~~~~~~~~~~

// takes a vector of films and returns a sorted copy.
std::vector<Movie> getSortedCopy(const std::vector<Movie>& movies);

// creates a vector of randomly generated movies of variable size
std::vector<Movie> generateMovieDatabase(int numMovies);

// ~~~~~~~~~~~~~~~~
// ASSESSED METHODS 
// ~~~~~~~~~~~~~~~~

// Return index position of movie in movies that matched target id
// return -1 not movie with targetID is in the collection.
// Search using a naive scan.
int LinearScan(const std::vector<Movie>& movies, int targetID);

// Return index position of movie in movies that matched target id
// return -1 not movie with targetID is in the collection.
// Search using a binary scan.
int BinarySearch(const std::vector<Movie>& movies, int targetID);

// Build an unordered_map from the vector for fast lookups.
// The map should use the film's ID for the index and the movie object as the value
std::unordered_map<int, Movie> BuildMovieMap(const std::vector<Movie>& movies);

// Use the map's constant-time lookup to find if a movie with the target ID exists.
// Note: The map itself should not change.
// Return: Movie object if the movie is in the map, Movie struct with an id of -1 if it is not present.
Movie MapSearch(std::unordered_map<int, Movie>& movieMap, int targetID);

// Takes a vector of movies to search and a vector of movie IDs to search for using linear scan.
// Returns as a double the time (in nanoseconds) for all IDs to be searched for.
double timeLinearScan(const std::vector<Movie>& movies, const std::vector<int>& targetIDs);

// Takes a vector of movies to search and a vector of movie IDs to search for using binary search.
// Returns a pair of timing results, 
// -> the first should be the time to sort the vector using the provided function
// -> the second should be the total time to search for each ID in the sorted vector
// note the second timing should not include the time taken to sort the vector
std::pair<double, double> timeBinarySearch(const std::vector<Movie>& movies, const std::vector<int>& targetIDs);

// Takes a vector of movies to search and a vector of movie IDs to search for using map search.
// Returns a pair of timing results, 
// -> the first should be the time to generate the map from the vector using assessed method BuildMovieMap
// -> the second should be the total time to search for each ID in the map
// note the second timing should not include the time taken to create the map
std::pair<double, double> timeMapSearch(const std::vector<Movie>& movies, const std::vector<int>& targetIDs);
