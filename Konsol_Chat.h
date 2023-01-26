#pragma once
#include <vector>
#include <exception>
#include <memory>
#include "messenger.h"
struct UserLoginExp :public std::exception {//вызываем исключения, если логин занят
	const char* what() const noexcept override {
		return "error: login busy";
	}
};
class Konsol_Chat {
	bool ChatWork_ = false;//переменная, отвечающая за работу чата
	std::vector<Polzovatel> userList_;//создаем вектор, который хранит пользователей чата
	std::vector<Messenger> messageList_;//создаем вектор, который хранит сообщения в чате
	std::shared_ptr<Polzovatel> currentUser_ = nullptr;//shared указатель указывает на текущего пользователя

	void login();
	void signUp();
	void showChat()const;
	void showAllUsersName()const;
	void addMessenger();

	std::vector<Polzovatel>& getAllUsers() {
		return userList_;
	}
	std::vector<Messenger>& getAllMessages() {
		return messageList_;
	}
	std::shared_ptr<Polzovatel>getUserByLogin(const std::string& login)const;
	std::shared_ptr<Polzovatel>getUserByName(const std::string& name)const;
public:
	void start();
	bool ChatWork()const {
		return ChatWork_;
	}
	std::shared_ptr<Polzovatel>getCurrentUser()const {
		return currentUser_;
	}
	void showLoginMenu();
	void showUserMenu();
};
