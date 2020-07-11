#include "..\\controls\\csv.h"

void csv::line::init(std::istream& inp) {
	char temp[1000];
	inp.getline(temp, 1000);

	columns = std::count(temp, temp + strlen(temp), ',') + 1;
	data = new char[strlen(temp) + 1];
	pdata = new char* [columns];

	strcpy_s(data, strlen(temp) + 1, temp);
	pdata[0] = data;
	for (int i = 0, cnt = 1; data[i] != '\0'; ++i) {
		if (data[i] == ',') {
			data[i] = '\0';
			pdata[cnt++] = data + i + 1;
		}
	}
}

csv::line& csv::line::operator=(const line& x) {
	int data_size = 0;
	for (int i = 0; i < x.columns; ++i) {
		data_size += strlen(x.pdata[i]) + 1;
	}

	delete[] data;
	delete[] pdata;

	id = x.id;
	columns = x.columns;
	data = new char[data_size];
	pdata = new char* [columns];

	pdata[0] = data;
	for (int i = 0, cnt = 1; i < data_size; ++i) {
		data[i] = x.data[i];
		if (cnt < columns && data[i] == '\0') {
			pdata[cnt++] = data + i + 1;
		}
	}
	return *this;
}

csv::csv(const char* FILE) {
	std::ifstream inp(FILE);
	if (!inp.is_open()) 
		throw std::runtime_error("data file is not exist");
	
	rows = std::count(std::istreambuf_iterator<char>(inp), std::istreambuf_iterator<char>(), '\n') - 1;
	inp.seekg(0, inp.beg);

	mark.init(inp);
	data = new line[rows];
	for (int i = 0; i < rows; ++i) {
		data[i].init(inp);
		data[i].id = i;
	}
	inp.close();
}