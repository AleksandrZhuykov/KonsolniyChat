#include<iostream>
#include <string>
#include "Konsol_Chat.h"
#include "Polzovatel.h"

void Konsol_Chat::start() {
	ChatWork_ = true;
}
std::shared_ptr<Polzovatel>Konsol_Chat::getUserByLogin(const std::string& login) const {//�������� login ������������ �� ��� �����
	for (auto& user : userList_) {//�������� ���� ������
		if (login == user.GetUserLogin())//���� ������������ ������ �� login ����� �������� ������������� �������� GetUserLogin
			return std::make_shared<Polzovatel>(user);// �� �� ���������� ��������� �� ����� ������������ 
	}
	return nullptr;
}
std::shared_ptr<Polzovatel>Konsol_Chat::getUserByName(const std::string& name)const {//�� �� �����, ��� � ����
	for (auto& user : userList_) {
		if (name == user.GetUserName())
			return std::make_shared<Polzovatel>(user);
	}
	return nullptr;
}
void Konsol_Chat::showLoginMenu() {
	currentUser_ = nullptr;//������������ �������� ������������ �������� nullptr
	char simvol;
	do {
		std::cout << "(1)�����" << std::endl;
		std::cout << "(2)������������������" << std::endl;
		std::cout << "(0)�������" << std::endl;
		std::cout << "\033[36m" << ">>" << "\033[0m";
		std::cin >> simvol;
		switch (simvol) {
		case '1':
			login();//������� ��� ������������ ������� � �������
			break;
		case '2':
			try {
				signUp();//��������������
			}
			catch (const std::exception& e) {//����� ����������, ��������� � ������� signup
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
	} while (!currentUser_ && ChatWork_);//���� ��������, ���� �� ��������������� ������������, � ���� ��� ��������
}
void Konsol_Chat::login() {
	std::string login, password;
	char simvol;
	do {
		std::cout << "�����:";
		std::cin >> login;
		std::cout << "������:";
		std::cin >> password;
		currentUser_ = getUserByLogin(login);//��������� �� ������������, ������� �����������������
		if (currentUser_ == nullptr || (password != currentUser_->GetUserPassword()))//���� ������� ������������ ��� � �������, �� ���������� ������ ����������, ��� ���� �������� ������
		{
			currentUser_ = nullptr;
			std::cout << "������ �����..." << std::endl;
			std::cout << "(0)����� ��� (����� �������) ���������: ";
			std::cin >> simvol;
			if (simvol == '0')
				break;
		}
	} while (!currentUser_);
}
void Konsol_Chat::showChat()const {
	std::string from;
	std::string to;
	std::cout << "---���---" << std::endl;
	for (auto& mess : messageList_) {
		if (currentUser_->GetUserLogin() == mess.GetFrom() || currentUser_->GetUserLogin() == mess.GetTo() || mess.GetTo() == "���") {//���� ������� ������������
			from = (currentUser_->GetUserLogin() == mess.GetFrom()) ? "���" : getUserByLogin(mess.GetFrom())->GetUserName();
			if (mess.GetTo() == "���") {//����, to ����� ����, �� �������� ��������� ���� �������������
				to = "���";
			}
			else {
				to = (currentUser_->GetUserLogin() == mess.GetTo()) ? "���" : getUserByLogin(mess.GetTo())->GetUserName();
				//���� ������� ��� ������������ ����� to, �� ���������� ��������� ������ ����, ���� ���, �� �������� ��� ������������ � ����������� ��� �������� ���� to
			}
			std::cout << "��������� �� " << from << " ���� " << to << std::endl;
			std::cout << " �����:" << mess.GetText() << std::endl;
		}
	}
	std::cout << "-----------" << std::endl;
}
void Konsol_Chat::signUp() {
	std::string login, password, name;
	double time;
	std::cout << "�����: ";
	std::cin >> login;
	std::cout << "������:";
	std::cin >> password;
	std::cout << "���:";
	std::cin >> name;
	if (getUserByLogin(login) || login == "���") //���� login �����, �� ������
	{
		throw UserLoginExp();
	}
	Polzovatel user = Polzovatel(login, password, name, time);// ������� ������ ������ Polzovatel
	userList_.push_back(user);//��������� ������������ 
	currentUser_ = std::make_shared<Polzovatel>(user);//������� ��������� �� �������� ������������
}
void Konsol_Chat::showUserMenu() {
	char operation;
	std::cout << "������, " << currentUser_->GetUserName() << std::endl;
	while (currentUser_) {
		std::cout << "����: (1)�����������| (2)�������� ���������|(3)������������|(0)�����" << std::endl;
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
			std::cout << "����������� �����" << std::endl;
			break;
		}
	}
}
void Konsol_Chat::addMessenger() {
	setlocale(LC_ALL, "");
	std::string to, text;
	std::cout << "���� (��� ������������ ��� ���)";
	std::cin >> to;
	std::cout << "�����: ";
	std::cin.ignore();
	getline(std::cin, text);
	if (!(to == "���" || getUserByName(to))) {//���� �������� ��������� ����, ���� �������� ������������, ������� ��� ���� � �������
		std::cout << "��������� �� ������ ��������: �� ������� �����" << to << std::endl;
		return;
	}
	if (to == "���")
		messageList_.push_back(Messenger{ currentUser_->GetUserLogin(),"���",text });//�������� ������ ������ Message � ����������� �������� �������� ������ �� �������� �������������, ����������, � ���� ����������
	else
		messageList_.push_back(Messenger{ currentUser_->GetUserLogin(),getUserByName(to)->GetUserLogin(),text });

}
void Konsol_Chat::showAllUsersName()const {
	setlocale(LC_ALL, "");
	std::cout << "---������������---" << std::endl;
	for (auto& user : userList_) {
		std::cout << user.GetUserName();
		if (currentUser_->GetUserLogin() == user.GetUserLogin())
			std::cout << "(���)";
		std::cout << std::endl;

	}
	std::cout << "-----------" << std::endl;
}
