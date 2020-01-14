#include<fstream>
#include<iostream>
#include<stack>
#include<string>

static const auto MAX_FILEPATH_LENGTH = 256;

//Same tags?
static bool is_not_srt_letter(char c) {
	return c != 'A' && c != 'D' && c != 'S' && c != 'C';
}
static bool is_valid(std::string tag) {
	return tag == "SRT-REV" || tag == "SRT-DST" || tag == "AGG-SUM" || tag == "AGG-PRO" || tag == "AGG-FST" 
		|| tag == "AGG-AVG" || tag == "AGG-LST" || tag == "MAP-INC" || tag == "MAP-MLT" || tag == "SRT-SLC" || tag == "SRT-ORD";
}
static bool is_number(const char& c) {
	return c >= '0' && c <= '9';
}
static bool is_double(const std::string number_string) {
	bool point_flag = false;
	bool negative_flag = false;
	for (auto ch : number_string) {
		if (ch == '-') {
			if (negative_flag == true)return false;
			negative_flag = true;
			continue;
		}
		if (ch == '.') {
			if (point_flag == true)return false;
			point_flag = true;
			continue;
		}
		if (!is_number(ch))return false;
	}
	return true;
}
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

		//Line counter.
		if (ch == '\n') { lines++; continue; }

		//Skip space outside tags.
		if (ch == ' ')continue;

		//Encounter a digit or a sign.
		if (is_number(ch) || ch == '-') {
		
			std::string number_string = "";
			while (ch != ' ' && ch != '<' && ch != '\n') {
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
			while (ch != '>' && ch != '\n') {

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
								if (is_not_srt_letter(ch)) {
									std::cerr << "Invalid symobls in parameter in tag SRT-ORD: line " << lines << std::endl;
									return false;
								}
								parameter += ch;
								input_stream.get(ch);
							}
							if (parameter != "ASC" && parameter != "DSC") {
								std::cerr << "Invalid parameter in tag SRT-ORD: line " << lines << ". Expecting parameter: ASC or DSC" << std::endl;
								return false;
							}
							input_stream.get(ch);
							if(ch != '>'){
								std::cerr << "Invalid symbols in tag SRT-ORD, after parameter: line " << lines << std::endl;
								return false;
							}
							else break;
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
								std::cerr << "Invalid symbols in function SRT-SLC, after parameter: line " << lines << std::endl;
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
								}
								input_stream.get(ch);
								if (ch != '>') {
									std::cerr << "Invalid symbols in " << tag << ", after parameter: line " << lines << std::endl;
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
