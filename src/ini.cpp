#include <array>
#include <fstream>
#include <ini.h>
#include <iostream>
#include <ostream>
#include <regex>
#include <sstream>
#include <string>

namespace ini {

const std::regex REGEX_SECTION("\\s*\\[([^\\]]*)\\]\\s*");
const std::regex
    REGEX_ENTRY("\\s*([a-zA-Z$_][a-zA-Z$_0-9]*)\\s*=\\s*([^\n]*)\\s*");

std::array<std::string, 2> get_entry_path_seg(std::string input) {
  int pIndex = input.find(".");

  if (pIndex == std::string::npos)
    return {};

  return {input.substr(0, pIndex),
          input.substr(pIndex + 1, input.length() - pIndex - 1)};
}

int Document::get_section(std::string section_name) {
  for (int i = 0; i < sections.size(); i++)
    if (sections[i].name == section_name)
      return i;

  return -1;
}

void Document::set(std::string entry_path, std::string value) {
  auto segments = get_entry_path_seg(entry_path);
  int s_index = get_section(segments[0]);

  if (s_index < 0) {
    s_index = sections.size();
    sections.push_back({.name = segments[0]});
  }

  sections[s_index].entries[segments[1]] = value;
}

std::string Document::get(std::string entry_path) {
  auto segments = get_entry_path_seg(entry_path);
  int s_index = get_section(segments[0]);

  if (s_index < 0)
    return ""; // even the section doesn't exists, so the entry also doesn't

  return sections[s_index].entries[segments[1]];
}

std::string Document::get_df(std::string entry_path, std::string df) {
  std::string value = get(entry_path);

  if (value.empty())
    return df;

  return value;
}

bool save(std::string path, Document &doc) {
  std::stringstream ss;

  for (auto s : doc.sections) {
    ss << "[" << s.name << "]" << std::endl;

    for (auto entry : s.entries)
      ss << entry.first << " = " << entry.second << std::endl;
  }

  std::ofstream file(path);

  if (file) {
    std::string content = ss.str();
    file.write(content.c_str(), content.size());
    return true;
  }

  return false;
}

bool load(std::string path, Document &doc) {
  std::ifstream file(path);

  if (file) {
    std::string line;

    std::string curr;
    std::smatch sm;
    while (std::getline(file, line)) {
      if (std::regex_match(line, sm, REGEX_SECTION)) {
        curr = sm[1];
        doc.sections.push_back({.name = curr});
        continue;
      }

      if (std::regex_match(line, sm, REGEX_ENTRY)) {
        doc.set(curr + "." + sm[1].str(), sm[2]);
        continue;
      }

      if (!line.empty())
        return false;
    }

    return true;
  }

  return false;
}

} // namespace ini
