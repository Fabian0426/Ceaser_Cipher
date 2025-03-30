#include "menu.h"
#include "cipher_functions.h"
#include <fstream>
#include <iostream>
#include <string>

void run_menu() {
    int command;
    while (true) {
        std::cout << "Choose command:\n-1.Reading a text from the keyboard\n-2.Reading a text from a file\n-3.Encrypting a text with a specific shift\n-4.Decrypting a text with a known shift\n-5.Computing and displaying the frequency distribution of a text\n-6.Breaking the encrypted text using frequency analysis\n-0.Exit\n" << std::endl;
        std::cin >> command;
        std::cin.ignore(); 
        if (command == 0) {
            break;
        }
        else if (command == 1) {
            std::cout << "Enter the text: ";
            std::string text;
            std::getline(std::cin, text);
            std::cout << text << std::endl;
        }
        else if (command == 2) {
            std::cout << "Enter the file name: ";
            std::string filename;
            std::getline(std::cin, filename);
            std::ifstream file(filename);
            if (file.is_open()) {
                std::string line;
                while (std::getline(file, line)) {
                    std::cout << line << std::endl;
                }
                file.close();
            }
            else {
                std::cerr << "Unable to open file" << std::endl;
            }
        }
        else if (command == 3) {
            int shift;
            std::string text, encrypted_text;
            std::cout << "Enter the shift: ";
            std::cin >> shift;
            std::cin.ignore();
            std::cout << "Enter the text that you want to encrypt: " << std::endl;
            std::getline(std::cin, text);
            encrypted_text.resize(text.size());
            encrypting_a_text_with_a_specific_shift(text.c_str(), &encrypted_text[0], shift);
            std::cout << "Encrypted text: " << encrypted_text << std::endl;
        }
        else if (command == 4) {
            int shift;
            std::string text, decrypted_text;
            std::cin.ignore();
            std::cout << "Enter the decrypted text: ";
            std::getline(std::cin, text);

            std::cout << "Enter the shift value: ";
            std::cin >> shift;
            std::cin.ignore();

            decrypted_text.resize(text.size());
            decrypting_a_text_with_a_specific_shift(text.c_str(), &decrypted_text[0], shift);
            std::cout << "Decrypted text: " << decrypted_text << std::endl;

        }
        else if (command == 5) {
            std::string text;
            std::cin.ignore();
            std::cout << "Enter the text: ";
            std::getline(std::cin, text);

            computing_and_displaying_the_frequency_distribution_of_a_text(text.c_str());
        }
        else if (command == 6) {
            std::string text;
            std::cout << "Enter the encrypted text: ";
            std::cin.ignore();
            std::getline(std::cin, text);

            int sub_command;
            std::cout << "Choose sub-command:\n-1.Encrypt using Chi-Squared\n-2.Encrypt using Cosine\n-3.Encrypt using Euclidean\n" << std::endl;
            std::cin >> sub_command;

            std::string decrypted_text;
            decrypted_text.resize(text.size());
            switch (sub_command) {
            case 1:
                std::cout << "Encrypting using Chi-Squared Distance" << std::endl;
                breaking_the_encrypted_text_using_frequency_analysis(text.c_str(), &decrypted_text[0], chi_squared_distance, 1);
                break;
            case 2:
                std::cout << "Encrypting using Cosine Distance" << std::endl;
                breaking_the_encrypted_text_using_frequency_analysis(text.c_str(), &decrypted_text[0], cosine_distance, ALPHABET_SIZE);
                break;
            case 3:
                std::cout << "Encrypting using Euclidean Distance" << std::endl;
                breaking_the_encrypted_text_using_frequency_analysis(text.c_str(), &decrypted_text[0], euclidian_distance, ALPHABET_SIZE);
                break;
            default:
                std::cerr << "Invalid sub-command\n" << std::endl;
                break;
            }
        }
        else {
            std::cerr << "Invalid command\n" << std::endl;
        }
    }
}
