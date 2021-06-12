#include "sha256.cpp"
#include <iostream> // for std::cout only, not needed for hashing library

int main(int, char**) 
{
    SHA256 sha256;
    std::cout << sha256("Hello World") << std::endl;

    return 0; 
}