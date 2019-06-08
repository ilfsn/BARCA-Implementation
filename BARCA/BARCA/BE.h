#pragma once
#include <vector>
#include <sqlite3.h>
#include <stdio.h>
#include <string>
#include <iostream>
class Feature {

};


class BehEx {
public:
	std::vector<double> read_metric();
	BehEx(int n_metric, int n_feature, int size_window) : n_metric(n_metric), n_feature(n_feature), size_window(size_window) {
		int exit = 0;
		exit = sqlite3_open("mini.db", &DB);

		if (exit) {
			std::cerr << "Error open DB " << sqlite3_errmsg(DB) << std::endl;
			}
		else
			std::cout << "Opened Database Successfully!" << std::endl;

		get_table_name();

		for (auto v : list_name) {
			std::cout << v <<" ";
			std::cout << std::endl;
		}
	}

private:
	
	int n_metric;
	int n_feature;
	int size_window;
	int get_table_name();
	std::vector<std::string> list_name;
	sqlite3* DB;
	std::vector<double> feature_extract();
};