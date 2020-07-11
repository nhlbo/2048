#ifndef CSV_H
#define CSV_H
#pragma warning(disable : 4996)
#include <Windows.h>
#include <iostream>
#include <fstream>}

class csv {
public:
	class line {
	public: 
		int id;			// Location row
	private:
		int columns;	// Number of columns
		char** pdata;
		char* data;

	public:
		~line() { delete[] data; delete[] pdata; }
		void init(std::istream& inp);
		line& operator=(const line& x);
	};

private:
	int rows;			// Number of rows
	line mark;
	line* data;
	
public:
	csv(const char* FILE);
	~csv() { delete[] data; }
};
#endif