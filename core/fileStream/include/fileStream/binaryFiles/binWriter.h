#pragma once

#include <fstream>
#include <filesystem>
#include <fileStream/path.h>

#include <utility/logger.h>

class BinWriter {
	std::ofstream os{};
	const std::string path;
	const std::string fileName;
	const std::string fullPath;

public:

	BinWriter(const std::string& path, const std::string& fileName) : 
		path(path), 
		fileName(fileName), 
		fullPath(path::parse({ path, fileName }))
	{
		std::filesystem::create_directories(path);
		const std::string fullPath = path::parse({ path, fileName });
		os.open(fullPath, std::ios::binary);
		log(
			"[BinWriter] Opening file " 
			+ fullPath 
			+ " | Status: " 
			+ std::to_string(os.is_open()));
	};

	template<class Obj>
	void write(const Obj* obj)
	{
		os.write(reinterpret_cast<const char*>(obj), sizeof(Obj));
		log(
			"[BinWriter] Writing to file " 
			+ fullPath 
			+ " | Success: " 
			+ std::to_string(os.good()));
	}

	template<class Obj>
	void writeArray(const Obj* obj, uint32_t length)
	{
		os.write(reinterpret_cast<const char*>(obj), sizeof(Obj) * length);
		log(
			"[BinWriter] Writing array to file " 
			+ fullPath 
			+ " | Success: " 
			+ std::to_string(os.good()));
	}

	template<class Obj>
	void writeObj(const Obj& obj) 
	{
		this->write(&obj);
	}

	template<class Obj>
	void writeArrayObj(const Obj& obj, uint32_t length) 
	{
		this->writeArray(&obj, length);
	}

	~BinWriter() {
		os.close();
	}
};