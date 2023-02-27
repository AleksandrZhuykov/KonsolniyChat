// Консольный чат.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <iostream>
#include <windows.h>
#include "Konsol_Chat.h"

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
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


