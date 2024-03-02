//
// Created by brendan on 3/2/24.
//

#ifndef ERROR_H
#define ERROR_H

#include <string>
#include <variant>

class Error {
public:
    Error(std::string message) : m_message(std::move(message))
    {}

    std::string m_message;
};

template <typename T>
class ErrorOr {
public:
    bool isError() const { return std::holds_alternative<Error>(m_error_or_value); }
private:
    std::variant<Error, T> m_error_or_value {};
};


#endif //ERROR_H
