#include <iostream>
#include <string>
#include <cassert>

namespace ciphers::XOR {
        std::string encrypt (const std::string &text, const int &key) {
            std::string encrypted_text = "";
            for (auto &c: text) {
                char encrypted_char = char(c ^ key);
                encrypted_text += encrypted_char;
            }
            return encrypted_text;
        }

        std::string decrypt (const std::string &text, const int &key) {
            std::string decrypted_text = "";
            for (auto &c : text) {
                char decrypted_char = char(c ^ key);
                decrypted_text += decrypted_char;
            }
            return decrypted_text;
        }
    }

void test() {
    // Test 1
    std::string text1 = "Hey! : What is your name...";
    std::string encrypted1 = ciphers::XOR::encrypt(text1, 17);
    std::string decrypted1 = ciphers::XOR::decrypt(encrypted1, 17);
    assert(text1 == decrypted1);
    std::cout << "Original text : " << text1;
    std::cout << " , Encrypted text (with key = 17) : " << encrypted1;
    std::cout << " , Decrypted text : "<< decrypted1 << std::endl;
    // Test 2
    std::string text2 = "->My name is Alex<-";
    std::string encrypted2 = ciphers::XOR::encrypt(text2, 29);
    std::string decrypted2 = ciphers::XOR::decrypt(encrypted2, 29);
    assert(text2 == decrypted2);
    std::cout << "Original text : " << text2;
    std::cout << " , Encrypted text (with key = 29) : " << encrypted2;
    std::cout << " , Decrypted text : "<< decrypted2 << std::endl;
}

int main() {
    test();
}
