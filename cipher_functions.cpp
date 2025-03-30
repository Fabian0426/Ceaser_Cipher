#include "cipher_functions.h"
#include <cstring>
#include <cmath>
#include <iostream>

void read_distribution(const char* filename, double distribution[ALPHABET_SIZE]) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error at opening the file\n";
        return;
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        file >> distribution[i];
    }

    file.close();
}

void compute_histogram(const char* text, double histogram[ALPHABET_SIZE]) {
    int frequency[ALPHABET_SIZE] = { 0 };
    int text_len = strlen(text);
    int nr_letters = 0;
    for (int i = 0; i < text_len; i++) {
        if (text[i] >= 'a' && text[i] <= 'z') {
            frequency[text[i] - 'a']++;
            nr_letters++;
        }
        else if (text[i] >= 'A' && text[i] <= 'Z') {
            frequency[text[i] - 'A']++;
            nr_letters++;
        }
    }

    if (nr_letters == 0) {
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            histogram[i] = 0.0;
        }
        return;
    }

    double value = 100.0 / (double)nr_letters;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        histogram[i] = frequency[i] * value;
    }
}

double chi_squared_distance(const double hist1[ALPHABET_SIZE], const double hist2[ALPHABET_SIZE]) {
    double distance = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (hist2[i] != 0) {
            distance += (hist1[i] - hist2[i]) * (hist1[i] - hist2[i]) / hist2[i];
        }
    }
    return distance;
}

double cosine_distance(const double hist1[ALPHABET_SIZE], const double hist2[ALPHABET_SIZE]) {
    double sum1 = 0;
    double sum2 = 0;
    double sum3 = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        sum1 += hist1[i] * hist2[i];
        sum2 += hist1[i] * hist1[i];
        sum3 += hist2[i] * hist2[i];
    }
    double distance = 1.0 - (sum1 / (sqrt(sum2) * sqrt(sum3)));
    return distance;
}

double euclidian_distance(const double hist1[ALPHABET_SIZE], const double hist2[ALPHABET_SIZE]) {
    double distance = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        distance += (hist1[i] - hist2[i]) * (hist1[i] - hist2[i]);
    }
    return sqrt(distance);
}

void break_caesar_cipher(const char* text, int& best_shift, double& best_distance, double (*distance_function)(const double[], const double[])) {
    size_t length = strlen(text);
    char* text_copy = new char[length + 1];
    double distribution[ALPHABET_SIZE], histogram[ALPHABET_SIZE];

    read_distribution("distribution.txt", distribution);

    best_distance = 1e9;//large number: 1 billion

    for (int shift = 0; shift < ALPHABET_SIZE; shift++) {
        strcpy_s(text_copy, length + 1, text);
        for (size_t i = 0; i < length; i++) {
            if (text_copy[i] >= 'a' && text_copy[i] <= 'z') {
                text_copy[i] = (text[i] - 'a' - shift + ALPHABET_SIZE) % ALPHABET_SIZE + 'a';
            }
            else if (text_copy[i] >= 'A' && text_copy[i] <= 'Z') {
                text_copy[i] = (text[i] - 'A' - shift + ALPHABET_SIZE) % ALPHABET_SIZE + 'A';
            }
        }

        compute_histogram(text_copy, histogram);
        double actual_distance = distance_function(histogram, distribution);

        if (actual_distance < best_distance) {
            best_distance = actual_distance;
            best_shift = shift;
        }
    }

    delete[] text_copy;
}

void encrypting_a_text_with_a_specific_shift(const char* initial_text, char encrypted_text[], int shift) {
    size_t text_len = strlen(initial_text);
    for (size_t i = 0; i < text_len; i++) {
        if (initial_text[i] >= 'a' && initial_text[i] <= 'z') {
            encrypted_text[i] = (initial_text[i] - 'a' + shift + ALPHABET_SIZE) % ALPHABET_SIZE + 'a';
        }
        else if (initial_text[i] >= 'A' && initial_text[i] <= 'Z') {
            encrypted_text[i] = (initial_text[i] - 'A' + shift + ALPHABET_SIZE) % ALPHABET_SIZE + 'A';
        }
        else {
            encrypted_text[i] = initial_text[i];
        }
    }
    encrypted_text[text_len] = '\0';
}

void decrypting_a_text_with_a_specific_shift(const char* initial_text, char* decrypted_text, int shift) {
    size_t text_len = strlen(initial_text);
    for (size_t i = 0; i < text_len; i++) {
        if (initial_text[i] >= 'a' && initial_text[i] <= 'z') {
            decrypted_text[i] = (initial_text[i] - 'a' - shift + ALPHABET_SIZE) % ALPHABET_SIZE + 'a';
        }
        else if (initial_text[i] >= 'A' && initial_text[i] <= 'Z') {
            decrypted_text[i] = (initial_text[i] - 'A' - shift + ALPHABET_SIZE) % ALPHABET_SIZE + 'A';
        }
        else {
            decrypted_text[i] = initial_text[i];
        }
    }
    decrypted_text[text_len] = '\0';
}

void breaking_the_encrypted_text_using_frequency_analysis(const char* text, char* decrypted_text, double (*distance_function)(const double[], const double[]), int num_shifts) {
    int best_shift;
    double best_distance;

    break_caesar_cipher(text, best_shift, best_distance, distance_function);

    decrypting_a_text_with_a_specific_shift(text, decrypted_text, best_shift);

    std::cout << "Decrypted text: " << decrypted_text << std::endl;
}

void display_encrypted_texts(const char* text, int num_shifts) {
    char decrypted_text[1001];

    std::cout << "Using Chi-Squared Distance:" << std::endl;
    breaking_the_encrypted_text_using_frequency_analysis(text, decrypted_text, chi_squared_distance, num_shifts);

    std::cout << "Using Cosine Distance:" << std::endl;
    breaking_the_encrypted_text_using_frequency_analysis(text, decrypted_text, cosine_distance, num_shifts);

    std::cout << "Using Euclidean Distance:" << std::endl;
    breaking_the_encrypted_text_using_frequency_analysis(text, decrypted_text, euclidian_distance, num_shifts);
}

void computing_and_displaying_the_frequency_distribution_of_a_text(const char* text) {
    double histogram[ALPHABET_SIZE];
    compute_histogram(text, histogram);
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        std::cout << char('A' + i) << ": " << histogram[i] << "%\n";
    }
}

