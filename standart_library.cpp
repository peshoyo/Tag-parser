//Author: Petar Angelov
//Date: 21/01/2020

#include<list>
#include<string>
#include "standart_library.h"


/// SORTING
List srt_ord(List l, std::string param) {
	if (param == "ASC") {
		l.sort();
		return l;
	}
	else if (param == "DSC") {
		l.sort(std::greater<double>());
		return l;
	}
	else { return l; }

}
 List srt_rev(List l) {
	 l.reverse();
	 return l;
 }
 List srt_slc(List l, size_t index) {
	 while (index > 0) { 
		 l.pop_front();
		 index--;
	 }
	 return l;
 }
 List srt_dst(List l) {

	 List new_list;
	 while (!l.empty()) {
		 double temp = l.front();
		 l.remove(temp);
		 new_list.push_back(temp);
	 }
	 return new_list;
 }

 /// MAP
 List map_inc(List l, double x) {
	 for (std::list<double>::iterator it = l.begin(); it != l.end(); ++it)
		 *it += x;
	 return l;
 }
 List map_mlt(List l, double x) {
	 for (std::list<double>::iterator it = l.begin(); it != l.end(); ++it)
		 *it *= x;
	 return l;
 }

 /// AGGREGATE 
List agg_sum(const List l) {
	List new_l;
	double sum = 0;
	for (std::list<double>::const_iterator it = l.begin(); it != l.end(); ++it)
		sum += *it;
	new_l.push_front(sum);
	return new_l;
}
List agg_pro(const List l) {
	List new_l;
	 double pro = 1;
	 for (std::list<double>::const_iterator it = l.begin(); it != l.end(); ++it)
		 pro *= *it;

	 new_l.push_front(pro);
	 return new_l;
 }
List agg_avg(const List l) {
	List new_l;
	double sum = 0;
	size_t n = 0;
	for (std::list<double>::const_iterator it = l.begin(); it != l.end(); ++it)
	{
		sum += *it; n++;
	}

	new_l.push_front(sum/n);
	return new_l;
 }
List agg_fst(const List l) {
	List new_l;
	new_l.push_front( l.front());
	return new_l;
 }
List agg_lst(const List l) {
	List new_l;
	new_l.push_front(l.back());
	return new_l;
 }


List use_function(std::string function, std::string param, List l) {

	//Functions with parameters.
	 if (param != "") {
		 if (function == "SRT-ORD")return srt_ord(l, param);
		 if (function == "SRT-SLC")return srt_slc(l, stoi(param));

		 if (function == "MAP-INC")return map_inc(l, stod(param));
		 if (function == "MAP-MLT")return map_mlt(l, stod(param));
	 }
	 //Functions without parameters.
	 else {
		 if (function == "SRT-REV")return srt_rev(l);
		 if (function == "SRT-DST")return srt_dst(l);

		 if (function == "AGG-SUM")return agg_sum(l);
		 if (function == "AGG-PRO")return agg_pro(l);
		 if (function == "AGG-AVG")return agg_avg(l);
		 if (function == "AGG-FST")return agg_fst(l);
		 if (function == "AGG-LST")return agg_lst(l);	 
	 }
	 return l;
 }