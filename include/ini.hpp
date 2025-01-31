#pragma once

#include <map>
#include <string>
#include <vector>

namespace ini {

class Document;

typedef std::map<std::string, std::string> smap;

struct section {
  std::string name;
  smap entries;
};

class Document {
  std::vector<section> sections;

public:
  int get_section(std::string section_name);

  void set(std::string entry_path, std::string value);
  std::string get(std::string entry_path);
  std::string get_df(std::string entry_path, std::string df);

  friend bool save(std::string path, Document &doc);
  friend bool load(std::string path, Document &doc);
};

} // namespace ini
