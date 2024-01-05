#pragma once

#include <string>

namespace cg {
enum class ErrorCode : uint8_t {
    NotFound,
    OperationFailed
};

class Error {
public:
    Error(ErrorCode code, std::string message = "") : code_(code), message_(std::move(message)){};

    ErrorCode code() const { return code_; }
    const std::string& message() const { return message_; }

    bool operator==(const ErrorCode code) const { return code == code_; }

private:
    ErrorCode code_;
    std::string message_;
};
} // namespace cg