#pragma once

# include "Threads.h"

namespace w3 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// ������ ��� MainForm
	///
	/// ��������! ��� ��������� ����� ����� ������ ���������� ����� ��������
	///          �������� ����� ����� �������� ("Resource File Name") ��� �������� ���������� ������������ �������,
	///          ���������� �� ����� ������� � ����������� .resx, �� ������� ������� ������ �����. � ��������� ������,
	///          ������������ �� ������ ��������� �������� � ���������������
	///          ���������, ��������������� ������ �����.
	/// </summary>
	public ref class MainForm : public Form
	{

	public:
		MainForm(void)
		{
			InitializeComponent();
			//
			//TODO: �������� ��� ������������

			// �������� ������ �����������, ��������� ������, �� 5 ������
			TransmittingBuffer = gcnew TTransmittingBuffer( 500000 );

			// �������� �����������, ��������� ��������� ����� �����
			Transmitter = gcnew TTransmitterThread( TransmittingBuffer );
			// ����� �����������

			// �������� ����������, ��������� ��������� ����� �����
			Generator = gcnew TGeneratorThread( TransmittingBuffer ); 
		}

		// ����� ��������� �������� ������������� �� ���� ��� �����
		// ���� � ���� ��� ����� �������� ��������, ��������� �� ������� (10000-100000),
		// �� ��� ���������� � ��������� �������
		int getDiscreetValue();

		// ����� ��������� �������� ��������� �������� �� ���� ��� �����
		// ���� � ���� ��� ����� �������� ��������, ��������� �� ������� (10-500),
		// �� ��� ���������� � ��������� �������
		int getIntervalValue();

		// ������ ���� ��������������� VS ���, ��� � �� ������

	protected:
		/// <summary>
		/// ���������� ��� ������������ �������.
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}

			// �������� �����������, ���������� � ������ �����������
			delete Transmitter;
			delete Generator;
			delete TransmittingBuffer;

		}
	private: System::Windows::Forms::Label^  label1;
	protected: 
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  textBox1;

	private: System::Windows::Forms::TextBox^  textBox2;
	private: System::Windows::Forms::Button^  btnStart;
	private: System::Windows::Forms::Button^  btn_Exit;
	private: System::Windows::Forms::MaskedTextBox^  meDiscreet;
	private: System::Windows::Forms::MaskedTextBox^  meInterval;

	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  lbOutBufferFill;

	private: System::Windows::Forms::Label^  lbInBufferFill;

	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::Timer^  timerFormRefresh;
	private: System::Windows::Forms::Button^  btnPause;
	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// ��������� ���������� ������������.
		/// </summary>

#pragma region Windows Form Designer generated code
		/// <summary>
		/// ������������ ����� ��� ��������� ������������ - �� ���������
		/// ���������� ������� ������ ��� ������ ��������� ����.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->btnStart = (gcnew System::Windows::Forms::Button());
			this->btn_Exit = (gcnew System::Windows::Forms::Button());
			this->meDiscreet = (gcnew System::Windows::Forms::MaskedTextBox());
			this->meInterval = (gcnew System::Windows::Forms::MaskedTextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->lbOutBufferFill = (gcnew System::Windows::Forms::Label());
			this->lbInBufferFill = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->timerFormRefresh = (gcnew System::Windows::Forms::Timer(this->components));
			this->btnPause = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(21, 19);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(129, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"������� �������������";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(21, 51);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(173, 13);
			this->label2->TabIndex = 1;
			this->label2->Text = L"�������� ����� ���������, ��";
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(65, 75);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(129, 20);
			this->textBox1->TabIndex = 2;
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(241, 75);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(65, 20);
			this->textBox2->TabIndex = 6;
			// 
			// btnStart
			// 
			this->btnStart->Location = System::Drawing::Point(24, 113);
			this->btnStart->Name = L"btnStart";
			this->btnStart->Size = System::Drawing::Size(75, 23);
			this->btnStart->TabIndex = 7;
			this->btnStart->Text = L"�����";
			this->btnStart->UseVisualStyleBackColor = true;
			this->btnStart->Click += gcnew System::EventHandler(this, &MainForm::btnStart_Click);
			// 
			// btn_Exit
			// 
			this->btn_Exit->Location = System::Drawing::Point(24, 216);
			this->btn_Exit->Name = L"btn_Exit";
			this->btn_Exit->Size = System::Drawing::Size(75, 23);
			this->btn_Exit->TabIndex = 10;
			this->btn_Exit->Text = L"�����";
			this->btn_Exit->UseVisualStyleBackColor = true;
			this->btn_Exit->Click += gcnew System::EventHandler(this, &MainForm::btn_Exit_Click);
			// 
			// meDiscreet
			// 
			this->meDiscreet->Location = System::Drawing::Point(207, 12);
			this->meDiscreet->Mask = L"0000000";
			this->meDiscreet->Name = L"meDiscreet";
			this->meDiscreet->Size = System::Drawing::Size(100, 20);
			this->meDiscreet->TabIndex = 11;
			this->meDiscreet->Text = L"1000";
			// 
			// meInterval
			// 
			this->meInterval->Location = System::Drawing::Point(206, 44);
			this->meInterval->Mask = L"000";
			this->meInterval->Name = L"meInterval";
			this->meInterval->Size = System::Drawing::Size(100, 20);
			this->meInterval->TabIndex = 12;
			this->meInterval->Text = L"500";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(21, 82);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(38, 13);
			this->label3->TabIndex = 13;
			this->label3->Text = L"�����";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(203, 82);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(32, 13);
			this->label4->TabIndex = 14;
			this->label4->Text = L"����";
			// 
			// lbOutBufferFill
			// 
			this->lbOutBufferFill->AutoSize = true;
			this->lbOutBufferFill->Location = System::Drawing::Point(21, 182);
			this->lbOutBufferFill->Name = L"lbOutBufferFill";
			this->lbOutBufferFill->Size = System::Drawing::Size(189, 13);
			this->lbOutBufferFill->TabIndex = 15;
			this->lbOutBufferFill->Text = L"������������� ������ ��������, %";
			// 
			// lbInBufferFill
			// 
			this->lbInBufferFill->AutoSize = true;
			this->lbInBufferFill->Location = System::Drawing::Point(21, 153);
			this->lbInBufferFill->Name = L"lbInBufferFill";
			this->lbInBufferFill->Size = System::Drawing::Size(180, 13);
			this->lbInBufferFill->TabIndex = 16;
			this->lbInBufferFill->Text = L"������������� ������ ������, %";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(312, 19);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(105, 13);
			this->label7->TabIndex = 17;
			this->label7->Text = L"(�� 1000 �� 100000)";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(312, 51);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(75, 13);
			this->label8->TabIndex = 18;
			this->label8->Text = L"(�� 10 �� 500)";
			// 
			// timerFormRefresh
			// 
			this->timerFormRefresh->Interval = 50;
			this->timerFormRefresh->Tick += gcnew System::EventHandler(this, &MainForm::timerFormRefresh_Tick);
			// 
			// btnPause
			// 
			this->btnPause->Location = System::Drawing::Point(105, 113);
			this->btnPause->Name = L"btnPause";
			this->btnPause->Size = System::Drawing::Size(75, 23);
			this->btnPause->TabIndex = 19;
			this->btnPause->Text = L"�����";
			this->btnPause->UseVisualStyleBackColor = true;
			this->btnPause->Visible = false;
			this->btnPause->Click += gcnew System::EventHandler(this, &MainForm::btnPause_Click);
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(446, 265);
			this->Controls->Add(this->btnPause);
			this->Controls->Add(this->label8);
			this->Controls->Add(this->label7);
			this->Controls->Add(this->lbInBufferFill);
			this->Controls->Add(this->lbOutBufferFill);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->meInterval);
			this->Controls->Add(this->meDiscreet);
			this->Controls->Add(this->btn_Exit);
			this->Controls->Add(this->btnStart);
			this->Controls->Add(this->textBox2);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Name = L"MainForm";
			this->Text = L"������";
			this->Load += gcnew System::EventHandler(this, &MainForm::MainForm_Load);
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &MainForm::MainForm_FormClosing);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: System::Void MainForm_Load(System::Object^  sender, System::EventArgs^  e) {
			 }
	private:
		// ����������� ������� �����

		// ���������� ������� ������ "�����/����"
		System::Void btnStart_Click(System::Object^  sender, System::EventArgs^  e) {
			
			// ������ ���������
			if (btnStart->Text == "�����") {

				btnStart->Text = "����";
				
				// ������ �������
				Generator->start( getDiscreetValue() );
				Transmitter->start( getIntervalValue() );

				// ������ ������� ���������� ��������� �� �����
				timerFormRefresh->Start();

				// ��������� �������������� ���������� ��������
				meDiscreet->Enabled = false;
				meInterval->Enabled = false;

				// ���������� ������ "�����"
				btnPause->Visible = true;
			}

			// ��������� ���������
			else { 
				btnStart->Text = "�����";
			
				// ������������� ������ ���������� ��������� �� �����
				timerFormRefresh->Stop();

				// ������������� ������
				Transmitter->stop();
				Generator->stop();

				// ��������� �������������� ���������� ��������
				meDiscreet->Enabled = true;
				meInterval->Enabled = true;

				// ������ ������ "�����"
				btnPause->Visible = false;
			}
		}
		// ------------------------------------------------------------------
			 
		// ���������� ������� ������ ������
		System::Void btn_Exit_Click(System::Object^  sender, System::EventArgs^  e) {
			Close();
		}

		// ���������� �������, ������������ ����� ��������� �����
		System::Void MainForm_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {

			// ���� ���� �������� ���������, �� �� �������������, ������ ��� ������� �����.
			if (btnStart->Text != "�����") {

				// ��������� ������
				Generator->stop();
				Transmitter->stop();
			}
		}
		// ------------------------------------------------------------------

		// ���������� �������, ������������ �������� ��� � 100 ��, 
		// ������ ��� ���������� ��������� �� �����
		System::Void timerFormRefresh_Tick(System::Object^  sender, System::EventArgs^  e) {
			
			// �������� �� ������ ����������� ���������� ��������, ������� ����� ������� ��������
			int quantity = Transmitter->getQuantity();

			// ������� ��� �� �����
			lbInBufferFill->Text = "������������� ������ ������, %: " + 
				String::Format("{0:N2}", (100.0 - (float)quantity/1000.0));

			// �������� �� ������ ����������� ��� ������������
			lbOutBufferFill->Text = "������������� ������ ��������, %: " + 
				String::Format("{0:N2}", TransmittingBuffer->fillingInPercent() );
		}
		//-------------------------------------------------------------------

		// ���������� ������� ������ "�����/�����������"
		System::Void btnPause_Click(System::Object^  sender, System::EventArgs^  e) {

			if (btnPause->Text == "�����") {

				btnPause->Text = "�����������";

				// ���������������� ������
				Generator->pause();
				Transmitter->pause();

				// ������������� ������ ���������� ��������� �� �����
				timerFormRefresh->Stop();
			} else {

				btnPause->Text = "�����";

				// ������������ ������ �������
				Generator->resume();
				Transmitter->resume();

				// ������ ������� ���������� ��������� �� �����
				timerFormRefresh->Start();
			}
		}
		//-------------------------------------------------------------------

		// ����� �����������
		TTransmittingBuffer ^TransmittingBuffer;

		// ����������
		TTransmitterThread ^Transmitter;

		// ���������
		TGeneratorThread ^Generator;

};
// ------------------------------------------------------------------


}

