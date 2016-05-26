//
// Created by brad on 5/2/16.
//


#include <stdexcept>
#include <string>

class SymbolTableException : public std::runtime_error {
public:

    SymbolTableException(const std::string &msg)
            : std::runtime_error(msg) { }

};
