#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <sstream>
#include "cipher.h"
#include <chrono>
#include <vector>
#include <thread>
#include <ncurses.h>

void initNcurses() {
    initscr();          
    cbreak();           
    noecho();           
    keypad(stdscr, TRUE); 
    curs_set(0);        
}

void drawMenu(int highlight, const std::vector<std::string>& choices) {
    int x, y, i;  
    x = 2;
    y = 2;
    box(stdscr, 0, 0);

    for(i = 0; i < choices.size(); ++i) {
        if(highlight == i + 1) { // High light the present choice
            attron(A_REVERSE); 
            mvwprintw(stdscr, y + i, x, "%s", choices[i].c_str());
            attroff(A_REVERSE);
        } else
            mvwprintw(stdscr, y + i, x, "%s", choices[i].c_str());
    }
    wrefresh(stdscr);
}


bool isValidKey(const std::string& key) {
    for (char ch : key) {
        if (!isalpha(ch)) {
            return false;
        }
    }
    return true;
}

// Pause for a moment and wait for user input
void pauseAndContinue() {
    std::cout << "Press enter to continue...";
    
    std::cin.get();  // Wait for user to press Enter
}

void explainCipher(int cipherChoice) {
    if (cipherChoice == 1) {
        std::cout << "\nCaesar Cipher Explanation:\n"
                  << "The Caesar cipher is one of the simplest and most widely known encryption techniques. "
                  << "It is a type of substitution cipher in which each letter in the plaintext is shifted a certain "
                  << "number of places down or up the alphabet. For example, with a shift of 1, 'A' would become 'B', 'B' would become 'C', etc.\n";
    } else if (cipherChoice == 2) {
        std::cout << "\nVigenère Cipher Explanation:\n"
                  << "The Vigenère cipher is a method of encrypting alphabetic text by using a simple form of polyalphabetic substitution. "
                  << "A key, which is a word or phrase, is used to repeat alongside the plaintext message, and each letter in the plaintext is shifted "
                  << "according to the corresponding letter in the key. It uses a different shift for each letter based on the keyword, making it more secure than the Caesar cipher.\n";
    }
    // Pause for the user to read the explanation
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << "Continuing...\n";
    pauseAndContinue();
}

void processText(const std::string& text, const std::string& key, bool encrypt, bool useCaesar, int shift) {
    std::string result;
    if (encrypt) {
        if (useCaesar) {
            result = caesarEncrypt(text, shift);
        } else {
            result = vigenereEncrypt(text, key);
        }
    } else {
        if (useCaesar) {
            result = caesarDecrypt(text, shift);
        } else {
            result = vigenereDecrypt(text, key);
        }
    }
    std::cout << (encrypt ? "Encrypted" : "Decrypted") << " Text: " << result << std::endl;
}

void processFile(const std::string& inputFilePath, const std::string& outputFilePath, const std::string& key, bool encrypt, bool useCaesar, int shift, bool bruteForce = false) {
    std::ifstream inFile(inputFilePath, std::ios::binary);
    if (!inFile.is_open()) {
        std::cout << "Failed to open input file." << std::endl;
        return;
    }

    std::ofstream outFile(outputFilePath, std::ios::binary);
    if (!outFile.is_open()) {
        std::cout << "Failed to open output file." << std::endl;
        inFile.close();
        return;
    }

    std::string line;
    while (getline(inFile, line)) {
        if (encrypt) {
            if (useCaesar) {
                line = caesarEncrypt(line, shift);
            } else {
                line = vigenereEncrypt(line, key);
            }
        } else {
            if (useCaesar) {
                if (bruteForce) {
                    for (int i = 1; i < 26; ++i) {
                        std::string decrypted = caesarDecrypt(line, i);
                        outFile << "Shift " << i << ": " << decrypted << "\n";
                    }
                } else {
                    line = caesarDecrypt(line, shift);
                }
            } else {
                line = vigenereDecrypt(line, key);
            }
        }
        if (!bruteForce) {
            outFile << line << "\n";
        }
    }

    inFile.close();
    outFile.close();
    if (bruteForce) {
        std::cout << "Brute force decryption completed. Check the output file for possible decryptions." << std::endl;
    } else {
        std::cout << "Process completed successfully." << std::endl;
    }
}

void startGame() {
    // Randomly choose between Caesar and Vigenère cipher
    int cipherChoice = rand() % 2 + 1; // 1 for Caesar, 2 for Vigenère
    std::string key;
    int shift;

    // Arrays of words by category
    const char* subjects[] = {"The cat", "A ghost", "She", "He"};
    const char* verbs[] = {"sat on", "runs", "is watching", "likes"};
    const char* adjectives[] = {"a funny", "a scary", "a large", "a small"};
    const char* objects[] = {"sofa.", "frog.", "movie.", "apple."};
    const char* keys[] = {"key", "sun", "art", "sky", "pie"}; // Keys for Vigenère

    int numSubjects = sizeof(subjects) / sizeof(subjects[0]);
    int numVerbs = sizeof(verbs) / sizeof(verbs[0]);
    int numAdjectives = sizeof(adjectives) / sizeof(adjectives[0]);
    int numObjects = sizeof(objects) / sizeof(objects[0]);
    int numKeys = sizeof(keys) / sizeof(keys[0]);

    const char* subject = subjects[rand() % numSubjects];
    const char* verb = verbs[rand() % numVerbs];
    const char* adjective = adjectives[rand() % numAdjectives];
    const char* object = objects[rand() % numObjects];

    std::string sentence = std::string(subject) + " " + std::string(verb) + " " +
                           std::string(adjective) + " " + std::string(object);

    std::string encryptedSentence;

    if (cipherChoice == 1) {
        shift = rand() % 26 + 1; // Random shift between 1 and 26 for Caesar
        encryptedSentence = caesarEncrypt(sentence, shift);
        std::cout << "\nEncrypted with Caesar Cipher (Shift " << shift << "): " << encryptedSentence << std::endl;
    } else {
        key = keys[rand() % numKeys];
        encryptedSentence = vigenereEncrypt(sentence, key);
        std::cout << "\nEncrypted with Vigenère Cipher (Key '" << key << "'): " << encryptedSentence << std::endl;
    }

    std::cout << "Your task is to decrypt the sentence." << std::endl;
    std::string userAttempt;
    std::cout << "Your decryption attempt: ";
    std::getline(std::cin, userAttempt);

    if (userAttempt == sentence) {
        std::cout << "Correct! Well done.\n";
    } else {
        std::cout << "Incorrect. The correct sentence was: " << sentence << std::endl;
    }
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    std::vector<std::string> choices = {
        "Text Encryption/Decryption",
        "File Encryption/Decryption",
        "Game",
        "Exit"
    };
    int choice = 0;
    int highlight = 1;

    while (true) {
        drawMenu(highlight, choices);
        int c = getch();  // Get user input
        switch (c) {
            case KEY_UP:
                if (highlight == 1)
                    highlight = choices.size();
                else
                    --highlight;
                break;
            case KEY_DOWN:
                if (highlight == choices.size())
                    highlight = 1;
                else
                    ++highlight;
                break;
            case 10:  // Enter key
                choice = highlight;
                break;
            default:
                break; 
        }

        if (choice != 0)  
            break;
    }

    endwin();

    int cipherChoice, action, shift;
    std::string text, inputFilePath, outputFilePath, key;

    switch (choice) {
        case 1:
            std::cout << "Enter text to encrypt/decrypt: ";
            std::getline(std::cin, text);

            // Validate the cipher choice
            do {
                std::cout << "Choose cipher (1 for Caesar, 2 for Vigenère): ";
                if (!(std::cin >> cipherChoice) || (cipherChoice != 1 && cipherChoice != 2)) {
                    std::cout << "Invalid choice. Please enter '1' for Caesar or '2' for Vigenère.\n";
                    std::cin.clear(); // Clear error state
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard bad input
                } else {
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear any leftover input
                    break; // Break loop if input is valid
                }
            } while (true);

            explainCipher(cipherChoice);

            
            do {
                std::cout << "Choose action (1 for Encrypt, 2 for Decrypt): ";
                if (!(std::cin >> action) || (action != 1 && action != 2)) {
                    std::cout << "Invalid choice. Please enter '1' for Encrypt or '2' for Decrypt.\n";
                    std::cin.clear(); 
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard bad input
                } else {
                    break; // Break loop if input is valid
                }
            } while (true);
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 

            if (cipherChoice == 1) {
                bool validShift = false;
                while (!validShift) {
                    std::cout << "Enter shift amount (number only, no spaces): ";
                    std::string shiftInput;
                    std::getline(std::cin, shiftInput);
                    std::istringstream iss(shiftInput);
                    if (iss >> shift && iss.eof()) {  // Check if only one integer is read
                        validShift = true;
                        processText(text, "", action == 1, true, shift);
                    } else {
                        std::cout << "Invalid input. Please enter a single numeric value without any spaces.\n";
                    }
                }
            } else {
                do {
                    std::cout << "Enter key: (Has to be alphabetical)";
                    std::getline(std::cin, key);
                } while (!isValidKey(key));
                processText(text, key, action == 1, false, 0);
            }
            break;
        case 2:
            std::cout << "Enter input file path: This works with absolute and relative filepaths. ";
            std::getline(std::cin, inputFilePath);
            std::cout << "Enter output file path: This will create a new file if it doesn't already exist ";
            std::getline(std::cin, outputFilePath);
            std::cout << "Choose cipher (1 for Caesar, 2 for Vigenère): ";
            std::cin >> cipherChoice;
            std::cout << "Choose action (1 for Encrypt, 2 for Decrypt): ";
            std::cin >> action;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (cipherChoice == 1) {
                std::cout << "Choose method: 1 for Regular, 2 for Brute Force: ";
                int method;
                std::cin >> method;
                if (method == 1) {
                    std::cout << "Enter shift amount: ";
                    std::cin >> shift;
                    processFile(inputFilePath, outputFilePath, "", action == 1, true, shift);
                } else {
                    processFile(inputFilePath, outputFilePath, "", false, true, 0, true);
                }
            } else if (cipherChoice == 2) {
                do {
                    std::cout << "Enter key: ";
                    std::getline(std::cin, key);
                } while (!isValidKey(key));
                processFile(inputFilePath, outputFilePath, key, action == 1, false, 0);
            }
            break;
        case 3:
            startGame();
            break;
            break;
        default:
            std::cout << "Invalid option!" << std::endl;
            return 1;
    }
    return 0;
}
