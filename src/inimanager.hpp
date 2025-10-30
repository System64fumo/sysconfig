#pragma once
#include <string>
#include <vector>
#include <unordered_map>

class ini_manager {
	public:
		void load(const std::string&);
		void save();
		std::unordered_map<std::string, std::unordered_map<std::string, std::string>> data;
		std::vector<std::string> section_order;
		std::unordered_map<std::string, std::vector<std::string>> key_order;

		bool available;

	private:
		std::string file_path;
		std::string trim(const std::string&);
};
