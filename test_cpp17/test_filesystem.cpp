#include <string>
#include <iostream>
#include <filesystem>

int main()
{
    std::string path = "."; 
	for (const auto & entry : std::filesystem::directory_iterator(path))
	{
    //	if(entry.path().extension()==".cpp")
            std::cout << entry.path() << std::endl;
	}																											 
																															 //	if(entry.is_regular_file())
	return 0;
}
