#include "Diff.hpp"

namespace differ
{
	Diff::Diff(Operation operation, const String& text)
		: op_{ operation }, text_{ text }
	{
	}

	bool Diff::operator==(const Diff& other) const
	{
		return (this->op_ == other.op_) && (this->text_ == other.text_);
	}

	bool Diff::operator!=(const Diff& other) const
	{
		return !(*this == other);
	}

	const String Diff::ToString() const
	{
		std::stringstream formattedText;
		formattedText << "Diff(\""
			<< std::get<String>(Diff::OperationData(op_))
			<< ",\""
			<< text_
			<< "\"";

		return formattedText.str();
	}

	// STATIC

	std::tuple<Char, String> Diff::OperationData(Operation op)
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
}