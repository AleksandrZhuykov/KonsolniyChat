// Консольный чат.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <iostream>
#include "Konsol_Chat.h"

int main()
{
    setlocale(LC_ALL, "");
    Konsol_Chat chat;
    chat.start();
    while (chat.ChatWork())
    {
        chat.showLoginMenu();
        while (chat.getCurrentUser()) {
            chat.showUserMenu();
        }
    }
    return 0;

}


