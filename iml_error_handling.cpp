//Author: Petar Angelov
//Date: 21/01/2020

#include<fstream>
#include<iostream>
#include<stack>
#include<string>

static const auto MAX_FILEPATH_LENGTH = 256;


static bool is_valid(std::string tag) {
	std::string tags[11] = { "SRT-REV", "SRT-DST", "SRT-SLC", "SRT-ORD", "MAP-INC", "MAP-MLT",
	"AGG-AVG", "AGG-SUM", "AGG-PRO", "AGG-FST", "AGG-LAST" };
	
	for (int i = 0; i < 11; i++) {
		if (tag == tags[i])return true;
	}
	return false;

	/*return tag == "SRT-REV" || tag == "SRT-DST" || tag == "AGG-SUM" || tag == "AGG-PRO" || tag == "AGG-FST" 
		|| tag == "AGG-AVG" || tag == "AGG-LST" || tag == "MAP-INC" || tag == "MAP-MLT" || tag == "SRT-SLC" || tag == "SRT-ORD";*/
}
static bool is_number(const char& c) {
	return c >= '0' && c <= '9';
}
static bool is_double(const std::string number_string) {
	bool point_flag = false;
	int i = 0;
	if (number_string[0] == '-') { i = 1; }

	for (i; i < number_string.length(); i++) {
		if (number_string[i] == '.') {
			if (point_flag == true)return false;
			point_flag = true;
			continue;
		}
		if (!is_number(number_string[i]))return false;
	}
	return true;
}

//Same tags?
static bool check_tags(const std::string &lhs, const  std::string &rhs) {
	char c = 47;
	std::string newlhs = c + lhs;
	return newlhs == rhs;
}

unsigned int lines = 0;
bool validate(const char filename[MAX_FILEPATH_LENGTH]) {

	std::ifstream input_stream(filename, std::ios::in || std::ios::binary);
	if (input_stream.fail()) {
		std::cerr << "Error opening file." << std::endl;
	}

	std::stack<std::string> tag_stack;

	
	char ch;
	while (input_stream.get(ch)) {

		//Check for unnecessary symbols.
		if (ch != '\n' && ch != ' ' && !is_number(ch) && ch != '-' && ch != '<') {
			std::cerr << "Invalid character: \"" << ch << "\", line " << lines << std::endl;
			return false;
		}

		//Line counter.
		if (ch == '\n') { lines++; continue; }

		//Skip space outside tags.
		if (ch == ' ')continue;

		//Encounter a digit or a sign.
		if (is_number(ch) || ch == '-') {
		
			std::string number_string = "";
			while (ch != ' ' && ch != '<' && ch != '\n' && !input_stream.eof()) {
				number_string += ch;
				input_stream.get(ch);
			}
			if (!is_double(number_string)) {
				std::cerr << "Invalid data/number: " << number_string << ", line " << lines << std::endl;
				return false;
			}
		}

		//Enter a tag.
		if (ch == '<') {
			input_stream.get(ch);
			std::string tag;
			while (ch != '>' && ch != '\n' && !input_stream.eof()) {

				//Handle parameter.
				if (ch == ' ') {
					if (tag == "SRT-ORD") {
						//Expecting specific parameter	
						input_stream.get(ch);
						if (ch != 34) {
							std::cerr << "Invalid symbols in tag SRT-ORD, before parameter: line " << lines << std::endl;
							return false;
						}
						else {
							std::string parameter;
							input_stream.get(ch);
							while (ch != 34 && !input_stream.eof()) {
								parameter += ch;
								input_stream.get(ch);
							}
							if (parameter != "ASC" && parameter != "DSC") {
								std::cerr << "Invalid parameter in tag SRT-ORD: line " << lines << ". Expecting parameter: ASC or DSC" << std::endl;
								return false;
							}
							input_stream.get(ch);
							if(ch != '>'){
								std::cerr << "Invalid symbols in tag SRT-ORD, after parameter in line " << lines << std::endl;
								return false;
							}
							else { break; }
						}
					}
					if (tag == "SRT-SLC") {
						//Expecting specific parameter	
						input_stream.get(ch);
						if (ch != 34) {
							std::cerr << "Invalid symbols in tag SRT-SLC, before parameter: line " << lines << std::endl;
							return false;
						}
						else {
							std::string parameter;
							input_stream.get(ch);
							while (ch != 34 && !input_stream.eof()) {
								if (!is_number(ch)) {
									std::cerr << "Invalid parameter in function SRT-SLC: line " << lines << ". Expecting parameter:  INTEGER N, N >= 0" << std::endl;
									return false;
								}
								parameter += ch;
								input_stream.get(ch);
							}
							input_stream.get(ch);
							if (ch != '>') {
								std::cerr << "Invalid symbols in function SRT-SLC: line " << lines << std::endl;
								return false;
							}
							else break;
						}
					}
					if (tag == "MAP-INC" || tag == "MAP-MLT") {
							//Expecting specific parameter	
							input_stream.get(ch);
							if (ch != 34) {
								std::cerr << "Invalid symbols in tag " << tag <<": line "<< lines<< std::endl;
								return false;
							}
							else {
								std::string parameter;
								input_stream.get(ch);
								while (ch != 34 && !input_stream.eof()) {
									parameter += ch;
									input_stream.get(ch);
								}
								if (!is_double(parameter)) {
									std::cerr << "Invalid parameter in tag " << tag << ": line " << lines << ". Expecting parameter: DOUBLE" << std::endl;
									return false;
								}
								input_stream.get(ch);
								if (ch != '>') {
									std::cerr << "Invalid symbols in tag " << tag << ": line " << lines << std::endl;
									return false;
								}
								else break;
							}

					}
				}
				tag += ch;
				input_stream.get(ch);
			}
			if (ch != '>') {
				std::cerr << "Expecting '>': line " << lines << std::endl;
				return false;
			}
			//Handle closing tag.
			if (tag[0] == '/') {
				std::string last_tag = tag_stack.top();
				if (!check_tags(last_tag, tag)) {
					std::cout << last_tag << tag;
					std::cerr << "Opening and ending tag mismatch: line " << lines << std::endl;
					return false;
				}
				else {
					tag_stack.pop();
					continue;
				}
			}
			if (!is_valid(tag)) {
				std::cerr << "Invalid tag: line " << lines << std::endl;
				return false;
			}
			else tag_stack.push(tag);
		}

	}

	if (!tag_stack.empty()) {
		std::cerr << "Expecting closing tag /" << tag_stack.top() << ": line " << lines << std::endl;
		return false;
	}
	else return true;

}