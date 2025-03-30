#pragma once

#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>

const int ALPHABET_SIZE = 26;//because there are 26 letters in the alphabet (english)
const int TOP_N = 3;

// Function to read the letter frequency distribution from a file
void read_distribution(const char* filename, double distribution[ALPHABET_SIZE]);

// Function to compute the histogram of a text
void compute_histogram(const char* text, double histogram[ALPHABET_SIZE]);

// Distance metrics
double chi_squared_distance(const double hist1[ALPHABET_SIZE], const double hist2[ALPHABET_SIZE]);
double cosine_distance(const double hist1[ALPHABET_SIZE], const double hist2[ALPHABET_SIZE]);
double euclidian_distance(const double hist1[ALPHABET_SIZE], const double hist2[ALPHABET_SIZE]);

// Caesar cipher functions
void breaking_the_encrypted_text_using_frequency_analysis(const char* text, char* decrypted_text, double (*distance_function)(const double[], const double[]), int num_shifts);
void encrypting_a_text_with_a_specific_shift(const char* initial_text, char encrypted_text[], int shift);
void decrypting_a_text_with_a_specific_shift(const char* initial_text, char decrypted_text[], int shift);
void computing_and_displaying_the_frequency_distribution_of_a_text(const char* text);
void break_encrypted_text(const char* text, char decrypted_text[], double (*distance_function)(const double[], const double[]));
