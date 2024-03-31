#include "Request.hpp"

#include "RequestData.hpp"

Request::Request(void) : state_(START) { this->data_ = new RequestData; }

Request::~Request(void) {
  if (this->data_ != NULL) delete this->data_;
}

bool Request::parse(std::string& buffer) {
  if (this->state_ == END && this->data_ != NULL) {
    delete this->data_;
    this->data_ = new RequestData;
  }
  switch (this->state_) {
    case START:
      parseBlank(buffer);
      break;
    case METHOD:
      parseMethod(buffer);
      break;
    case BETWEEN_METHOD_AND_URI:
      parseBlank(buffer);
      break;
    case URI:
      parseUri(buffer);
      break;
    case BETWEEN_URI_AND_VERSION:
      parseBlank(buffer);
      break;
    case VERSION:
      parseVersion(buffer);
      break;
    case BETWEEN_VERSION_AND_HEADER:
      parseBlank(buffer);
      break;
    case HEADER:
      parseHeader(buffer);
      break;
    case BODY:
      parseBody(buffer);
      break;
    case CHUNKEDBODY:
      parseChunkedBody(buffer);
      break;
    case END:
      break;
    default:
      break;
  }
  return (this->state_ == END) ? true : false;
}

const RequestData* Request::getRequestData(void) const { return this->data_; }

void Request::parseMethod(std::string& buffer) {}
void Request::parseUri(std::string& buffer) {}
void Request::parseVersion(std::string& buffer) {}
void Request::parseHeader(std::string& buffer) {}
void Request::parseBody(std::string& buffer) {}
void Request::parseChunkedBody(std::string& buffer) {}
void Request::parseBlank(std::string& buffer) {}
void Request::determineParseBody(std::string& buffer) {}
