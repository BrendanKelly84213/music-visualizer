//
// Created by brendan on 3/2/24.
//

#ifndef ERROR_H
#define ERROR_H

#include <string>
#include <variant>
#include <expected>

#define TRY(expression, returnValue)                                        \
    ({                                                                      \
        auto&& temporary = expression;                                      \
        if (std::move(temporary).isError()) {                               \
          std::cout << "Error: " << std::move(temporary).error().message(); \
          return returnValue;                                               \
        }                                                                   \
        std::move(temporary).value();                                       \
    })                                                                      \



class Error {
public:
    Error(std::string message) : m_message(std::move(message))
    {}

    [[nodiscard]] std::string const& message() const { return m_message; }
private:
    std::string m_message;
};

template <typename T>
class Result {
public:

    Result(T value)
    : m_error_or_value(std::move(value))
    {}
    Result(Error error)
    : m_error_or_value(std::unexpected<Error>(std::move(error)))
    {}

    [[nodiscard]] bool isError() const { return !m_error_or_value.has_value(); }
    T&& value() { return std::move(*m_error_or_value); }
    [[nodiscard]] Error error() const { return m_error_or_value.error(); }
private:
    std::expected<T, Error> m_error_or_value {};
};


#endif //ERROR_H
