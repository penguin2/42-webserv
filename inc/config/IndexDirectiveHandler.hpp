#ifndef INDEX_DIRECTIVE_HANDLER_HPP
#define INDEX_DIRECTIVE_HANDLER_HPP

#include "ADirectiveHandler.hpp"

class IndexDirectiveHandler : public ADirectiveHandler {
 public:
  IndexDirectiveHandler();
  virtual bool isValid() const;
  virtual void setConfig(long unsigned int server_num,
                         std::string location_path);
};

#endif
