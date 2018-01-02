# include "stdafx.h"
# include "MainForm.h"

// ����� ��������� �������� ������������� �� ���� ��� �����
// ���� � ���� ��� ����� �������� ��������, ��������� �� ������� (10000-100000),
// �� ��� ���������� � ��������� �������

int w3::MainForm::getDiscreetValue() {
	
	int discreet = 1000;
	try {
		discreet = System::Convert::ToInt32( meDiscreet->Text, 10 );
	} catch (...) {
		meDiscreet->Text = "1000";
	}

	if (discreet < 1000) {
		discreet = 1000;
		meDiscreet->Text = "1000";
	}

	if (discreet > 100000) {
		discreet = 100000;
		meDiscreet->Text = "100000";
	}

	return discreet;
}
//-------------------------------------------------------------------

// ����� ��������� �������� ��������� �������� �� ���� ��� �����
// ���� � ���� ��� ����� �������� ��������, ��������� �� ������� (10-500),
// �� ��� ���������� � ��������� �������

int w3::MainForm::getIntervalValue() {
	
	int interval = 500;
	try {
		interval = System::Convert::ToInt32( meInterval->Text, 10 );
	} catch (...) {
		meInterval->Text = "500";
	}

	if (interval < 10) {
		interval = 10;
		meInterval->Text = "10";
	}

	if (interval > 500) {
		interval = 500;
		meInterval->Text = "500";
	}

	return interval;
}
//-------------------------------------------------------------------

