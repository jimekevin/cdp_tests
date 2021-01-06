#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>

class Config {

private:
  Config() = default; // Disallow instantiation outside of the class.

public:
  Config(const Config&) = delete;
  Config& operator=(const Config &) = delete;
  Config(Config &&) = delete;
  Config& operator=(Config &&) = delete;

  static auto& instance() {
      static Config inst;
      return inst;
  }

  static bool parseSimple(const std::string& path);
};

#endif // CONFIG_H
