#ifndef DIFF_PATCH_DIFF_HPP
#define DIFF_PATCH_DIFF_HPP

#include <stdexcept>
#include <iostream>
#include <sstream>
#include "defs.hpp"

namespace differ
{
    enum class Operation
    {
        DELETE,
        INSERT,
        EQUAL
    };

    class Diff
    {
    public:
        Diff(Operation operation, const String& text)
            : op_(operation), text_(text) { }

        bool operator==(const Diff& other) const
        {
            return (this->op_ == other.op_) && (this->text_ == other.text_);
        }

        bool operator!=(const Diff& other) const
        {
            return !(*this == other);
        }

        const String ToString() const
        {
            std::stringstream formattedText;
            formattedText << "Diff(\""
                << std::get<String>(Diff::OperationData(op_))
                << ",\""
                << text_
                << "\"";

            return formattedText.str();
        }

        inline const Operation GetOperation() const { return op_; }

        inline String Text() const { return text_; }

    public:
        static std::tuple<Char, String> OperationData(Operation op)
        {
            switch (op)
            {
            case Operation::DELETE:
                return { '-', "DELETE" };
            case Operation::INSERT:
                return { '+', "INSERT" };
            case Operation::EQUAL:
                return { ' ', "EQUAL" };
            }
            throw std::invalid_argument("Invalid input operation.");
        }

    private:
        Operation op_;
        const String& text_;
    };
}

#endif // DIFF_PATCH_DIFF_HPP
