#pragma once

#include <fstream>
#include <fileStream/path.h>

#include <utility/logger.h>

class BinReader {
	std::ifstream is{};
	const std::string path;
	const std::string fileName;
	const std::string fullPath;

public:

	void open()
	{
		is.open(fullPath, std::ios::binary);

		log(
			"[BinReader] Opening file "
			+ fullPath
			+ " | Status: "
			+ std::to_string(is.is_open()));

		if (!is.is_open()) {
			throw(strerror(errno));
		}
	}

	BinReader(const std::string& path, const std::string& fileName, bool openFile = true) : 
		path(path), 
		fileName(fileName), 
		fullPath(path::parse({ path, fileName }))
	{
		if (openFile) { open(); }
	};

	template<class Obj>
	void read(Obj* obj) {
		is.read(reinterpret_cast<char*>(obj), sizeof(Obj));
		log(
			"[BinReader] Reading from file "
			+ fullPath
			+ " | Success: "
			+ std::to_string(is.good()));
	}

	template<class Obj>
	void readArray(Obj* obj, uint32_t length) {
		is.read(reinterpret_cast<char*>(obj), length * sizeof(Obj));
		log(
			"[BinReader] Reading from file "
			+ fullPath
			+ " | Success: "
			+ std::to_string(is.good()));
	}

	template<class Obj>
	void readObj(Obj& obj) {
		this->read(&obj);
	}

	template<class Obj>
	void readArrayObj(Obj& obj, uint32_t length) {
		this->readArray(&obj, length);
	}

	~BinReader() {
		is.close();
	}
};