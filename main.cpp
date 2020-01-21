#include<iostream>
constexpr auto FILEPATH = "C:/Users/Peter/Desktop/tests/test9.txt";
const auto MAX_FILEPATH_LENGTH = 256;

extern bool parse(const char[], const char[]);
extern bool validate(const char[]);

int main() {

	char filename[MAX_FILEPATH_LENGTH];
	char outname[MAX_FILEPATH_LENGTH];

	//std::cout << "Enter the filepath of an existing input file: ";
	//std::cin >> filename;
	std::cout << "Enter the name of an ouput file: ";
	std::cin >> outname;
	std::cout << "Validating..." << std::endl;
	if (validate(FILEPATH)) {
		std::cout << "Validation complete." << std::endl;
		std::cout << "Parsing..." <<std::endl;
		if (parse(FILEPATH, outname))std::cout << "Parsing complete." << std::endl;
		else std::cout << "Parsing failed." << std::endl;
	}
	else std::cout << "Validation failed.";
}
