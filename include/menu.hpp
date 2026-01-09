#pragma once
#include <iostream>
#include "../include/database.hpp"

extern DatabaseConnection* db;

void printMenu();
void menuAdmin();
void menuManager();
void menuUser();