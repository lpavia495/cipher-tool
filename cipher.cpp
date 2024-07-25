#include "cipher.h"

std::string caesarEncrypt(const std::string &text, int shift) {
    std::string result = "";
    for (char c : text) {
        if (isalpha(c)) {
            char base = islower(c) ? 'a' : 'A';
            c = (c - base + shift) % 26 + base;  // Wrap around alphabet
        }
        result += c;
    }
    return result;
}

std::string caesarDecrypt(const std::string &text, int shift) {
    return caesarEncrypt(text, 26 - shift);  // Decrypting is just reversing the shift
}

std::string vigenereEncrypt(const std::string &text, const std::string &key) {
    std::string result;
    int keyLength = key.length();
    int keyIndex = 0;

    for (char c : text) {
        if (isalpha(c)) {
            char base = islower(c) ? 'a' : 'A';
            int shift = (key[keyIndex % keyLength] - base) % 26;
            c = (c - base + shift) % 26 + base;
            keyIndex++;
        }
        result += c;
    }
    return result;
}

std::string vigenereDecrypt(const std::string &text, const std::string &key) {
    std::string result;
    int keyLength = key.length();
    int keyIndex = 0;

    for (char c : text) {
        if (isalpha(c)) {
            char base = islower(c) ? 'a' : 'A';
            int shift = (key[keyIndex % keyLength] - base) % 26;
            c = (c - base - shift + 26) % 26 + base;  
            keyIndex++;
        }
        result += c;
    }
    return result;
}