// w3.cpp: ������� ���� �������.

#include "stdafx.h"
#include "MainForm.h"

using namespace w3;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// ��������� ���������� �������� Windows XP �� �������� �����-���� ��������� ����������
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// �������� �������� ���� � ��� ������
	Application::Run(gcnew MainForm());
	return 0;
}
