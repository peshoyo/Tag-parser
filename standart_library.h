#pragma once
#include<list>
using List = std::list<double>;


//	SORTING  //
List srt_ord(List , std::string );
List srt_rev(List );
List srt_slc(List, size_t );
List srt_dst(List );

//  MAP	//
List map_inc(List, double);
List map_mlt(List, double);

//	AGGREGATE  //
List agg_sum(const List);
List agg_pro(const List);
List agg_avg(const List);
List agg_fst(const List);
List agg_lst(const List);

//dasdasadsdabhf
List use_function(std::string, std::string, List );