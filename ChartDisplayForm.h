#pragma once

#include <vector>

namespace koordinates {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Collections::Generic;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for ChartDisplayForm
	/// </summary>
	public ref class ChartDisplayForm : public System::Windows::Forms::Form
	{
	private:
		List<System::Drawing::Point>^ points;
		Color chartColor = Color::Red;
		bool isBarChart;
		System::Windows::Forms::Button^ button1;
		System::Windows::Forms::Panel^ panel1;
		System::Windows::Forms::ColorDialog^ colorDialog1;
		System::Windows::Forms::Button^ colorButton;

		System::ComponentModel::Container^ components;

	public:
		ChartDisplayForm(List<System::Drawing::Point>^ points)
		{
			InitializeComponent();
			this->points = points;
			this->DoubleBuffered = true;
			this->isBarChart = false;

		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~ChartDisplayForm()
		{
			if (components)
			{
				delete components;
			}
		}

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->colorDialog1 = (gcnew System::Windows::Forms::ColorDialog());
			this->colorButton = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// panel1
			// 
			this->panel1->AutoSize = true;
			this->panel1->BackColor = System::Drawing::Color::Transparent;
			this->panel1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->panel1->ForeColor = System::Drawing::Color::Red;
			this->panel1->Location = System::Drawing::Point(12, 12);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(1400, 700);
			this->panel1->TabIndex = 0;
			this->panel1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &ChartDisplayForm::panel1_Paint);
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(1437, 62);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(118, 46);
			this->button1->TabIndex = 1;
			this->button1->Text = L"Pakeisti į stulpelinį";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &ChartDisplayForm::button1_Click);
			// 
			// colorButton
			// 
			this->colorButton->Location = System::Drawing::Point(1440, 142);
			this->colorButton->Name = L"colorButton";
			this->colorButton->Size = System::Drawing::Size(114, 42);
			this->colorButton->TabIndex = 2;
			this->colorButton->Text = L"Pasirinkti spalvą";
			this->colorButton->UseVisualStyleBackColor = true;
			this->colorButton->Click += gcnew System::EventHandler(this, &ChartDisplayForm::colorButton_Click);
			// 
			// ChartDisplayForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1584, 761);
			this->Controls->Add(this->colorButton);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->panel1);
			this->Name = L"ChartDisplayForm";
			this->Text = L"Koordinačių plokštuma";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private:
		System::Void panel1_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e)
		{
			Graphics^ g = e->Graphics;
			Pen^ axisPen = gcnew Pen(Color::Black, 2);
			Pen^ linePen = gcnew Pen(chartColor, 1);
			Pen^ barPen = gcnew Pen(chartColor, 10);
			SolidBrush^ labelBrush = gcnew SolidBrush(Color::Black);
			SolidBrush^ pointBrush = gcnew SolidBrush(chartColor);

			int barWidth = 10;
			int padding = 20;

			int panelWidth = panel1->ClientSize.Width;
			int panelHeight = panel1->ClientSize.Height;

			int minX = points[0].X;
			int maxX = points[0].X;
			int minY = points[0].Y;
			int maxY = points[0].Y;

			for each (Point point in points) {
				if (point.X < minX) minX = point.X;
				if (point.X > maxX) maxX = point.X;
				if (point.Y < minY) minY = point.Y;
				if (point.Y > maxY) maxY = point.Y;
			}

			minX -= padding;
			maxX += padding;
			minY -= padding;
			maxY += padding;

			double scaleX = (double)panelWidth / (maxX - minX);
			double scaleY = (double)panelHeight / (maxY - minY);

			System::Drawing::Font^ labelFont = gcnew System::Drawing::Font("Arial", 8);

			// Y-axis
			int yAxisX = (int)(-minX * scaleX);
			g->DrawLine(axisPen, yAxisX, 0, yAxisX, panelHeight);

			// X-axis
			int xAxisY = (int)(panelHeight - (-minY * scaleY));
			g->DrawLine(axisPen, 0, xAxisY, panelWidth, xAxisY);

			if (isBarChart) {
				for each (Point point in points) {
					int mappedX = (int)((point.X - minX) * scaleX);
					int mappedY = (int)(panelHeight - (point.Y - minY) * scaleY);

					// Ensure bar height is calculated correctly
					int barHeight = xAxisY - mappedY;  // Height from the point to the x-axis

					// Fill rectangle representing the bar
					g->DrawLine(barPen, mappedX, xAxisY, mappedX, mappedY);
				}
			}
			else {
				// Line chart: connect points with lines
				Point prevMappedPoint;
				bool firstPoint = true;

				for each (Point point in points) {
					int mappedX = (int)((point.X - minX) * scaleX);
					int mappedY = (int)(panelHeight - (point.Y - minY) * scaleY);

					// Draw the point as a filled ellipse
					g->FillEllipse(pointBrush, mappedX - 3, mappedY - 3, 6, 6);

					// Connect the points with lines
					if (!firstPoint) {
						g->DrawLine(linePen, prevMappedPoint.X, prevMappedPoint.Y, mappedX, mappedY);
					}
					prevMappedPoint = Point(mappedX, mappedY);
					firstPoint = false;
				}
			}

			// Add axis labels at intervals
			int xInterval = (maxX - minX) / 15;
			int yInterval = (maxY - minY) / 15;

			for (int x = minX + padding; x <= maxX - padding; x += xInterval) {
				int mappedX = (int)((x - minX) * scaleX);
				g->DrawLine(axisPen, mappedX, xAxisY - 5, mappedX, xAxisY + 5);
				g->DrawString(x.ToString(), labelFont, labelBrush, mappedX - 10, xAxisY + 10);
			}

			for (int y = minY + padding; y <= maxY - padding; y += yInterval) {
				int mappedY = (int)(panelHeight - (y - minY) * scaleY);
				g->DrawLine(axisPen, yAxisX - 5, mappedY, yAxisX + 5, mappedY);
				g->DrawString(y.ToString(), labelFont, labelBrush, yAxisX - 30, mappedY - 6);
			}

		
		}

			private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {

				isBarChart = !isBarChart;
				if (isBarChart) this->button1->Text = L"Pakeisti į linijinį";
				else this->button1->Text = L"Pakeisti į stulpelinį";

				this->panel1->Invalidate();
			}	


	private: System::Void colorButton_Click(System::Object^ sender, System::EventArgs^ e) {
		if (colorDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
			chartColor = colorDialog1->Color;
			panel1->Invalidate();
		}
	}
};
}