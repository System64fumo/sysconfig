#include "inimanager.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>

void ini_manager::load(const std::string& filename) {
	std::ifstream file(filename);
	std::string line;
	std::string current_section;

	available = file.is_open();

	if (available) {
		file_path = filename;
		while (std::getline(file, line)) {
			line = trim(line);

			if (line.empty() || line[0] == ';' || line[0] == '#') {
				continue;
			}
			else if (line[0] == '[' && line[line.size() - 1] == ']') {
				current_section = line.substr(1, line.size() - 2);

				if (data.find(current_section) == data.end()) {
					section_order.push_back(current_section);
					key_order[current_section] = std::vector<std::string>();
				}
			}
			else {
				size_t delimiter_pos = line.find('=');
				if (delimiter_pos != std::string::npos && !current_section.empty()) {
					std::string key = trim(line.substr(0, delimiter_pos));
					std::string value = trim(line.substr(delimiter_pos + 1));
					data[current_section][key] = value;

					auto& keys = key_order[current_section];
					if (std::find(keys.begin(), keys.end(), key) == keys.end()) {
						keys.push_back(key);
					}
				}
			}
		}
		file.close();
	}
	else {
		std::fprintf(stderr, "Unable to open file: %s\n", filename.c_str());
	}
}

void ini_manager::save() {
	std::ofstream file(file_path);
	if (!file.is_open()) {
		std::fprintf(stderr, "Failed to save: %s\n", file_path.c_str());
		return;
	}

	for (const auto& section_name : section_order) {
		file << "[" << section_name << "]\n";

		if (key_order.find(section_name) != key_order.end()) {
			for (const auto& key_name : key_order[section_name]) {
				if (data[section_name].find(key_name) != data[section_name].end()) {
					file << key_name << "=" << data[section_name][key_name] << "\n";
				}
			}
		}
		file << "\n";
	}

	file.close();
}

std::string ini_manager::trim(const std::string& str) {
	const size_t first = str.find_first_not_of(' ');
	if (std::string::npos == first) {
		return str;
	}
	const size_t last = str.find_last_not_of(' ');
	return str.substr(first, (last - first + 1));
}
