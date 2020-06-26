#include <fstream>
#include <iostream>

std::ifstream::pos_type filesize(const char* filename)
{
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg(); 
}

int main()
{
	std::cout << filesize("bolsar.html");
	return 0;
}