#pragma once
#include <string>
class Polzovatel {
	const std::string login_;
	std::string password_;
	std::string name_;

public:
	Polzovatel(const std::string& login, const std::string& password, const std::string& name, double& time)
		:login_(login), password_(password), name_(name) {
	}
	const std::string& GetUserLogin()const {
		return login_;
	}
	const std::string& GetUserPassword()const {
		return password_;

	}
	void SetUserPassword(const std::string& password) {
		/*this->*/ password_ = password;
	}
	const std::string& GetUserName() const {
		return name_;
	}
	void SetUserName(const std::string& name) {
		/*this->*/name_ = name;
	}
};

