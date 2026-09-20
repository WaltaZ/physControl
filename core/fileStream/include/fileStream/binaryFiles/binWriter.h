#pragma once

#include <fstream>
#include <filesystem>

class BinWriter {
	std::ofstream os;

public:

	BinWriter(const std::string& path, const std::string& fileName) 
	{
		std::filesystem::create_directories(path);
		const std::string fullPath = path::parse({ path, fileName });
		os = std::ofstream(fullPath, std::ios::binary);
	};

	template<class Obj>
	void write(const Obj* obj) {
		os.write(reinterpret_cast<const char*>(obj), sizeof(*obj));
	}

	template<class Obj>
	void writeArray(Obj* obj, uint32_t length) {
		os.write(reinterpret_cast<const char*>(obj), length * sizeof(*obj));
	}

	~BinWriter() {
		os.close();
	}
};