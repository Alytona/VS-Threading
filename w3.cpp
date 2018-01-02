// w3.cpp: главный файл проекта.

#include "stdafx.h"
#include "MainForm.h"

using namespace w3;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Включение визуальных эффектов Windows XP до создания каких-либо элементов управления
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Создание главного окна и его запуск
	Application::Run(gcnew MainForm());
	return 0;
}
