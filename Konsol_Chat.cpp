#include<iostream>
#include <string>
#include "Konsol_Chat.h"
#include "Polzovatel.h"

void Konsol_Chat::start() {
	ChatWork_ = true;
}
std::shared_ptr<Polzovatel>Konsol_Chat::getUserByLogin(const std::string& login) const {//получаем login пользователя по его имени
	for (auto& user : userList_) {//проходим весь вектор
		if (login == user.GetUserLogin())//если передаваемая ссылка на login равна значению возвращаемому функцией GetUserLogin
			return std::make_shared<Polzovatel>(user);// то мы возвращаем указатель на этого пользователя 
	}
	return nullptr;
}
std::shared_ptr<Polzovatel>Konsol_Chat::getUserByName(const std::string& name)const {//то же самое, что и выше
	for (auto& user : userList_) {
		if (name == user.GetUserName())
			return std::make_shared<Polzovatel>(user);
	}
	return nullptr;
}
void Konsol_Chat::showLoginMenu() {
	currentUser_ = nullptr;//присываиваем текущему пользователю значение nullptr
	char simvol;
	do {
		std::cout << "(1)Логин" << std::endl;
		std::cout << "(2)Зарегистрироваться" << std::endl;
		std::cout << "(0)Закрыть" << std::endl;
		std::cout << "\033[36m" << ">>" << "\033[0m";
		std::cin >> simvol;
		switch (simvol) {
		case '1':
			login();//заходим под существующим логином и паролем
			break;
		case '2':
			try {
				signUp();//регистрируемся
			}
			catch (const std::exception& e) {//ловим исключение, вызванное в функции signup
				std::cout << e.what() << std::endl;
			}
			break;
		case '0':
			ChatWork_ = false;
			break;
		default:
			std::cout << "1 or 2..." << std::endl;
			break;
		}
	} while (!currentUser_ && ChatWork_);//цикл работает, пока не зарегистрирован пользователь, и пока чат работает
}
void Konsol_Chat::login() {
	std::string login, password;
	char simvol;
	do {
		std::cout << "Логин:";
		std::cin >> login;
		std::cout << "Пароль:";
		std::cin >> password;
		currentUser_ = getUserByLogin(login);//указатель на пользователя, который зарегистрировался
		if (currentUser_ == nullptr || (password != currentUser_->GetUserPassword()))//если данного пользователя нет в векторе, то возвращаем пустой указаетель, или если неверный пароль
		{
			currentUser_ = nullptr;
			std::cout << "Ошибка входа..." << std::endl;
			std::cout << "(0)выход или (любая клавиша) повторить: ";
			std::cin >> simvol;
			if (simvol == '0')
				break;
		}
	} while (!currentUser_);
}
void Konsol_Chat::showChat()const {
	std::string from;
	std::string to;
	std::cout << "---Чат---" << std::endl;
	for (auto& mess : messageList_) {
		if (currentUser_->GetUserLogin() == mess.GetFrom() || currentUser_->GetUserLogin() == mess.GetTo() || mess.GetTo() == "все") {//если текущий пользователь
			from = (currentUser_->GetUserLogin() == mess.GetFrom()) ? "мне" : getUserByLogin(mess.GetFrom())->GetUserName();
			if (mess.GetTo() == "все") {//если, to равно всем, то адресуем сообщение всем пользователям
				to = "все";
			}
			else {
				to = (currentUser_->GetUserLogin() == mess.GetTo()) ? "мне" : getUserByLogin(mess.GetTo())->GetUserName();
				//если текущее имя пользователя равно to, то отправляем сообщение самому себе, если нет, то получаем имя пользователя и присваиваем его значение полю to
			}
			std::cout << "Сообщение от " << from << " всем " << to << std::endl;
			std::cout << " Текст:" << mess.GetText() << std::endl;
		}
	}
	std::cout << "-----------" << std::endl;
}
void Konsol_Chat::signUp() {
	std::string login, password, name;
	double time;
	std::cout << "Логин: ";
	std::cin >> login;
	std::cout << "Пароль:";
	std::cin >> password;
	std::cout << "Имя:";
	std::cin >> name;
	if (getUserByLogin(login) || login == "все") //если login занят, то ошибка
	{
		throw UserLoginExp();
	}
	Polzovatel user = Polzovatel(login, password, name, time);// создаем объект класса Polzovatel
	userList_.push_back(user);//добавляем пользователя 
	currentUser_ = std::make_shared<Polzovatel>(user);//создаем указатель на текущего пользователя
}
void Konsol_Chat::showUserMenu() {
	char operation;
	std::cout << "Привет, " << currentUser_->GetUserName() << std::endl;
	while (currentUser_) {
		std::cout << "Меню: (1)ПоказатьЧат| (2)Добавить сообщение|(3)Пользователи|(0)Выйти" << std::endl;
		std::cout << std::endl;
		std::cin >> operation;
		switch (operation) {
		case '1':
			showChat();
			break;
		case '2':
			addMessenger();
			break;
		case '3':
			showAllUsersName();
			break;
		case'0':
			currentUser_ = nullptr;
			break;
		default:
			std::cout << "неизвестный выбор" << std::endl;
			break;
		}
	}
}
void Konsol_Chat::addMessenger() {
	setlocale(LC_ALL, "");
	std::string to, text;
	std::cout << "Кому (имя пользователя или все)";
	std::cin >> to;
	std::cout << "Текст: ";
	std::cin.ignore();
	getline(std::cin, text);
	if (!(to == "все" || getUserByName(to))) {//либо посылаем сообщения всем, либо посылаем пользователю, который уже есть в векторе
		std::cout << "сообщение об ошибке отправки: не удается найти" << to << std::endl;
		return;
	}
	if (to == "все")
		messageList_.push_back(Messenger{ currentUser_->GetUserLogin(),"все",text });//помещаем объект класса Message в констукторе которого указывем ссылку на текущего пользоваетеля, сообещение, и кому адресовано
	else
		messageList_.push_back(Messenger{ currentUser_->GetUserLogin(),getUserByName(to)->GetUserLogin(),text });

}
void Konsol_Chat::showAllUsersName()const {
	setlocale(LC_ALL, "");
	std::cout << "---Пользователи---" << std::endl;
	for (auto& user : userList_) {
		std::cout << user.GetUserName();
		if (currentUser_->GetUserLogin() == user.GetUserLogin())
			std::cout << "(мне)";
		std::cout << std::endl;

	}
	std::cout << "-----------" << std::endl;
}
