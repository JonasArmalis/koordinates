#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <msclr/marshal_cppstd.h>
#include "ChartDisplayForm.h"


namespace koordinates
{
	using namespace std;
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Collections::Generic;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();
		}

	protected:
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		System::Windows::Forms::Button^ btnFileUpload;
		List<System::Drawing::Point>^ points;
	private: System::Windows::Forms::Label^ label1;

	protected:
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->btnFileUpload = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// btnFileUpload
			// 
			this->btnFileUpload->Location = System::Drawing::Point(210, 81);
			this->btnFileUpload->Name = L"btnFileUpload";
			this->btnFileUpload->Size = System::Drawing::Size(192, 47);
			this->btnFileUpload->TabIndex = 0;
			this->btnFileUpload->Text = L"Įkelti CSV failą";
			this->btnFileUpload->UseVisualStyleBackColor = true;
			this->btnFileUpload->Click += gcnew System::EventHandler(this, &MainForm::btnFileUpload_Click);
			// 
			// label1
			// 
			this->label1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(59, 31);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(510, 13);
			this->label1->TabIndex = 1;
			this->label1->Text = L"Sveiki atvykę į koordinačių grafikų sistemą, paspaudę mygtuką pasirinkite CSV fai"
				L"lą su taškų koordinatėmis";
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(642, 180);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->btnFileUpload);
			this->Name = L"MainForm";
			this->Text = L"Koordinatės";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private:

		System::Void btnFileUpload_Click(System::Object^ sender, System::EventArgs^ e)
		{
			OpenFileDialog^ fileDialog = gcnew OpenFileDialog;

			fileDialog->InitialDirectory = "c:\\";
			fileDialog->Filter = "CSV files (*.csv)|*.csv";
			fileDialog->FilterIndex = 1;
			fileDialog->RestoreDirectory = true;

			if (fileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {

				string filePath = msclr::interop::marshal_as<string>(fileDialog->FileName);
				//Read the points from the selected file
				points = LoadPointsFromFile(filePath);

				if (points != nullptr) {
					ChartDisplayForm^ displayForm = gcnew ChartDisplayForm(points);
					displayForm->Show();  // Show the new form
				}
			}
		}

		List<System::Drawing::Point>^ LoadPointsFromFile(string filePath)
		{
			List<System::Drawing::Point>^ pointList = gcnew List<System::Drawing::Point>();
			ifstream fileStream(filePath);

			if (fileStream.is_open()) {
				try {
					string line;

					// Skip the header
					getline(fileStream, line);

					// Read the file line by line
					while (getline(fileStream, line)) {
						istringstream iss(line);
						string xString, yString;

						getline(iss, xString, ',');
						getline(iss, yString, ',');

						int x = stoi(xString);
						int y = stoi(yString);

						pointList->Add(System::Drawing::Point(x, y));
					}
					fileStream.close();
				}
				catch (const exception& ex) {
					MessageBox::Show("Error reading the file, please make sure to use the correct format, here's an example:  \n x, y \n 5, 7 \n 3, 8 \n (Don't leave an empty new line at the end)");
					return nullptr; 
				}
			}
			else {
				MessageBox::Show("Unable to open the file.");
				return nullptr; 
			}

			return pointList; 
		}



	}; 
}
