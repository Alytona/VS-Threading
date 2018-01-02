# include "stdafx.h"
# include "MainForm.h"

// Метод получения значения дискретизации из поля для ввода
// Если в поле для ввода вводится значение, выходящее за границы (10000-100000),
// то оно приводится к ближайшей границе

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

// Метод получения значения интервала отправки из поля для ввода
// Если в поле для ввода вводится значение, выходящее за границы (10-500),
// то оно приводится к ближайшей границе

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

