//Author: Petar Angelov
//Date: 21/01/2020

#pragma once
#include<iostream>
#include<fstream>
#include<stack>
#include<string>
#include<tuple>
#include "standart_library.h"

static const auto MAX_FILEPATH_LENGTH = 256;

static bool is_number(const char& c) {
	return c >= '0' && c <= '9';
 }
static double to_number(const std::string& s) {
	double d = std::stod(s);
	return d;
}

bool parse(const char filename[MAX_FILEPATH_LENGTH], const char outname[MAX_FILEPATH_LENGTH]) {


	 std::ifstream fin(filename, std::ios::in | std::ios::binary);
	 if (fin.fail()) {
		 std::cerr << "Error opening file!" << std::endl;
	 }
	 std::ofstream fout(outname, std::ios::out | std::ios::binary);

	 bool flag = false;
	 char ch;
	 std::string tag= "";

	 std::stack<std::tuple<std::string,std::string>> function_stack;
	 std::stack<std::list<double>> number_stack;


	 while (fin.get(ch)) {
		 if (ch == ' ')continue;
		 if (is_number(ch) || ch == '-') {
			 std::string number_string = "";
			 while (ch != ' ' && ch != '<' && ch != '\n' && !fin.eof()) {
				 number_string += ch;
				 fin.get(ch);
			 }
			 double number = stod(number_string);
			 
			 if (number_stack.empty()) {
				 std::list<double> temp;
				 temp.push_back(number);
				 number_stack.push(temp);
			 }
			 else {
				 std::list<double> temp = number_stack.top();
				 number_stack.pop();
				 temp.push_back(number);
				 number_stack.push(temp);
			 }
		 }
		 if (ch == '<') {
			 std::string parameter = "";
			 fin.get(ch);
			 while (ch != '>' && ch != '\n') {
				 if (ch == ' ') {
					 fin.get(ch);
					 continue;
				 }
				 if (ch == 34) {
					 fin.get(ch);
					 while (ch != 34) {
						 parameter += ch;
						 fin.get(ch);
					 }
					 break;
				 }
				
				 tag += ch;
				 fin.get(ch);
				 
			 }
			 if (tag[0] == '/') {
				 std::list<double> args = number_stack.top();
				 number_stack.pop();

				 std::tuple<std::string, std::string> function_param = function_stack.top();
				 std::string function = std::get<0>(function_param);
				 std::string parameter = std::get<1>(function_param);
				 function_stack.pop();
				 if (function == "SRT-SLC") {
					 if (stoul(parameter) >= args.size()) {
						 std::cerr << "Given parameter in function SRT-SLC exceed the size of the list, try with postion N, N e [0, Max Position]" << std::endl;
						 return false;
					 }
				 }
				 std::list<double> new_list = use_function(function, parameter, args);
				 if (number_stack.size() > 0) {
					 std::list<double> args2 = number_stack.top();
					 number_stack.pop();

					 std::list<double> new_list = use_function(function, parameter, args);
					 for (auto t : new_list) {
						 args2.push_back(t);
					 }
					 number_stack.push(args2);
				 }
				 else { number_stack.push(new_list); }
					
			 }
			 else {		
				
				 function_stack.push(std::make_tuple(tag,parameter));
				 std::list<double> empty;
				 number_stack.push(empty);
			 }
			 tag = "";
		 }
		 
 	 }
 
	 std::list<double> out_list = number_stack.top();
	 for (std::list<double>::iterator it = out_list.begin(); it != out_list.end(); ++it)
		 fout << *it << ' ';
	 return true;
	 
}