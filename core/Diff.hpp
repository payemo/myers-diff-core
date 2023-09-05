#ifndef DIFF_PATCH_DIFF_HPP
#define DIFF_PATCH_DIFF_HPP

#include "defs.hpp"

#include <stdexcept>
#include <iostream>
#include <sstream>
#include <list>

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
        Diff(Operation operation, const String& text);

        bool operator==(const Diff& other) const;

        bool operator!=(const Diff& other) const;

        const String ToString() const;

        inline const Operation GetOperation() const { return op_; }

        inline String Text() const { return text_; }

        inline UInt32 GetTextLength() const { return text_.size(); }

    public:
        static std::tuple<Char, String> OperationData(Operation op);

    private:
        Operation op_;
        String text_;
    };

    using DiffList = typename std::list<Diff>;
}

#endif // DIFF_PATCH_DIFF_HPP
