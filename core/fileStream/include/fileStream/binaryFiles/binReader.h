#pragma once

#include<fstream>

class BinReader {
	std::ifstream is;

public:

	BinReader(const std::string& path, const std::string& fileName)
	{
		const std::string fullPath = path::parse({ path, fileName });
		is = std::ifstream(fullPath, std::ios::binary);
		if (!is.is_open()) {
			throw(strerror(errno));
		}
	};

	template<class Obj>
	void read(Obj* obj) {
		is.read(reinterpret_cast<char*>(obj), sizeof(Obj));
	}

	template<class Obj>
	void readArray(Obj* obj, uint32_t length) {
		is.read(reinterpret_cast<char*>(obj), length * sizeof(Obj));
	}

	~BinReader() {
		is.close();
	}
};