#pragma once
#include "Polzovatel.h"
class Messenger {
	const std::string from_;
	const std::string to_;
	const std::string text_;
public:
	Messenger (const std::string& from, const std::string to, const std::string text)
		:from_(from), to_(to), text_(text) {
	}
	const std::string& GetFrom()const {
		return from_;
	}
	const std::string& GetTo()const {
		return to_;
	}
	const std::string& GetText()const {
		return text_;
	}
};
