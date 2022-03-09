#ifndef SCANNER_H
#define SCANNER_H

#include <string>

class Scanner {
 public:
  Scanner(const std::string& filename);

 private:
  std::string file_contents_;
};

#endif // SCANNER_H
