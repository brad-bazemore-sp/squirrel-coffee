//
// Created by brad on 5/4/16.
//

#ifndef SEMANTICEXCEPTION_H
#define SEMANTICEXCEPTION_H

#include <iostream>
#include <stdexcept>
#include <string>

class SemanticException : public std::runtime_error {
public:

    SemanticException(const std::string &msg)
            : std::runtime_error(msg) {
        std::cout << "!!!!!!!!!!!Error detected!!!!!!!!!!!!" << std::endl;
        std::cout << "!!!!!!Compilation has stopped!!!!!!!!" << std::endl;
        std::cout << msg << std::endl;
        exit(EXIT_FAILURE);
    }

};

#endif //TJC_PART_2_SEMANTICEXCEPTION_H
