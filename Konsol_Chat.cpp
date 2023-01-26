#include<iostream>
#include <string>
#include "Konsol_Chat.h"
#include "Polzovatel.h"


void Konsol_Chat::start() {
	ChatWork_ = true;
}
std::shared_ptr<Polzovatel>Konsol_Chat::getUserByLogin(const std::string& login) const {//получаем login пользовател€ по его имени
	for (auto& user : userList_) {//проходим весь вектор
		if (login == user.GetUserLogin())//если передаваема€ ссылка на login равна значению возвращаемому функцией GetUserLogin
			return std::make_shared<Polzovatel>(user);// то мы возвращаем указатель на этого пользовател€ 
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
	setlocale(LC_ALL, "");
	currentUser_ = nullptr;//присываиваем текущему пользователю значение nullptr
	char simvol;
	do {
		std::cout << "\033[33m" << "(1)Ћогин" << std::endl;
		std::cout << "(2)«арегистрироватьс€" << std::endl;
		std::cout << "(0)«акрыть" << std::endl;
		std::cout << "\033[36m" << ">>" << "\033[0m";
		std::cin >> simvol;
		switch (simvol) {
		case '1':
			login();//заходим под существующим логином и паролем
			break;
		case '2':
			try {
				signUp();//регистрируемс€
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
	setlocale(LC_ALL, "");
	std::string login, password;
	char simvol;

	do {
		std::cout << "Ћогин:";
		std::cin >> login;
		std::cout << "ѕароль:";
		std::cin >> password;

		currentUser_ = getUserByLogin(login);//указатель на пользовател€, который зарегистрировалс€
		if (currentUser_ == nullptr || (password != currentUser_->GetUserPassword()))//если данного пользовател€ нет в векторе, то возвращаем пустой указаетель, или если неверный пароль
		{
			currentUser_ = nullptr;
			std::cout << "ќшибка входа..." << std::endl;
			std::cout << "(0)выход или (люба€ клавиша) повторить: ";
			std::cin >> simvol;
			if (simvol == '0')
				break;
		}
	} while (!currentUser_);
}

void Konsol_Chat::showChat()const {
	setlocale(LC_ALL, "");
	std::string from;
	std::string to;
	std::cout << "---Chat---" << std::endl;
	for (auto& mess : messageList_) {
		if (currentUser_->GetUserLogin() == mess.GetFrom() || currentUser_->GetUserLogin() == mess.GetTo() || mess.GetTo() == "all") {//если текущий пользователь
			from = (currentUser_->GetUserLogin() == mess.GetFrom()) ? "me" : getUserByLogin(mess.GetFrom())->GetUserName();
			if (mess.GetTo() == "all") {//если, to равно all, то адресуем сообщение всем пользовател€м
				to = "all";
			}
			else {
				to = (currentUser_->GetUserLogin() == mess.GetTo()) ? "me" : getUserByLogin(mess.GetTo())->GetUserName();
				//если мы текущее им€ пользовател€ равно to, то отправл€ем сообщение самому себе, если нет, то получаем им€ пользовател€ и присваиваем его значение полю to
			}
			std::cout << "—ообщение от " << from << " to " << to << std::endl;
			std::cout << " “екст:" << mess.GetText() << std::endl;
		}
	}
	std::cout << "-----------" << std::endl;

}
void Konsol_Chat::signUp() {
	setlocale(LC_ALL, "");
	std::string login, password, name;
	double time;
	std::cout << "Ћогин: ";
	std::cin >> login;
	std::cout << "ѕароль:";
	std::cin >> password;
	std::cout << "»м€:";
	std::cin >> name;
	if (getUserByLogin(login) || login == "all") //если login зан€т, то ошибка
	{
		throw UserLoginExp();
	}
	Polzovatel user = Polzovatel(login, password, name, time);// создаем объект класса Polzovatel
	userList_.push_back(user);//добавл€ем пользовател€ 
	currentUser_ = std::make_shared<Polzovatel>(user);//создаем указатель на текущего пользовател€
}
void Konsol_Chat::showUserMenu() {
	setlocale(LC_ALL, "");
	char operation;
	std::cout << "ѕривет, " << currentUser_->GetUserName() << std::endl;
	while (currentUser_) {
		std::cout << "ћеню: (1)ѕоказать„ат| (2)ƒобавить сообщение|(3)ѕользователи|(0)¬ыйти" << std::endl;
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

	std::cout << " ому (им€ пользовател€ или все)";
	std::cin >> to;
	std::cout << "“екст: ";
	std::cin.ignore();
	getline(std::cin, text);
	if (!(to == "all" || getUserByName(to))) {//либо посылаем сообщени€ всем, либо посылаем пользователю, который уже есть в векторе
		std::cout << "сообщение об ошибке отправки: не удаетс€ найти" << to << std::endl;
		return;
	}
	if (to == "all")
		messageList_.push_back(Messenger{ currentUser_->GetUserLogin(),"all",text });//помещаем объект класса Message в констукторе которого указывем ссылку на текущего пользоваетел€, сообещение, и кому адресовано
	else
		messageList_.push_back(Messenger{ currentUser_->GetUserLogin(),getUserByName(to)->GetUserLogin(),text });

}
void Konsol_Chat::showAllUsersName()const {
	std::cout << "---Users---" << std::endl;
	for (auto& user : userList_) {
		std::cout << user.GetUserName();
		if (currentUser_->GetUserLogin() == user.GetUserLogin())
			std::cout << "(me)";
		std::cout << std::endl;

	}
	std::cout << "-----------" << std::endl;
}
