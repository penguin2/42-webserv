#ifndef WEBSERV_CONFIG_H
#define WEBSERV_CONFIG_H

class Config {
 public:
  Config(const char* config_file);
  ~Config();

 private:
  Config();
  Config(const Config&);
  Config& operator=(const Config&);
};

#endif
