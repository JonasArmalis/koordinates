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
		System::Windows::Forms::Panel^ panel1;

	public:
		ChartDisplayForm(List<System::Drawing::Point>^ points)
		{
			InitializeComponent();
			this->points = points;
			this->DoubleBuffered = true; // Helps prevent flickering

			// Force a repaint
			//this->panel1->Invalidate();
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

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->panel1 = (gcnew System::Windows::Forms::Panel());
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
			// ChartDisplayForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1584, 761);
			this->Controls->Add(this->panel1);
			this->Name = L"ChartDisplayForm";
			this->Text = L"ChartDisplayForm";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private:
		System::Void panel1_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
			Graphics^ g = e->Graphics;

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

			int padding = 10;
			minX -= padding;
			maxX += padding;
			minY -= padding;
			maxY += padding;

			double scaleX = (double)panelWidth / (maxX - minX);
			double scaleY = (double)panelHeight / (maxY - minY);

			Pen^ axisPen = gcnew Pen(Color::Black, 2);
			SolidBrush^ labelBrush = gcnew SolidBrush(Color::Black);
			System::Drawing::Font^ labelFont = gcnew System::Drawing::Font("Arial", 8);

			// Y-axis
			int yAxisX = (int)(-minX * scaleX);
			g->DrawLine(axisPen, yAxisX, 0, yAxisX, panelHeight);

			// X-axis
			int xAxisY = (int)(panelHeight - (-minY * scaleY));
			g->DrawLine(axisPen, 0, xAxisY, panelWidth, xAxisY);

			// Draw points
			SolidBrush^ pointBrush = gcnew SolidBrush(Color::Red);
			for each (Point point in points) {
				int mappedX = (int)((point.X - minX) * scaleX);
				int mappedY = (int)(panelHeight - (point.Y - minY) * scaleY);

				g->FillEllipse(pointBrush, mappedX - 3, mappedY - 3, 6, 6);
			}

			// Add axis labels at intervals
			int xInterval = (maxX - minX) / 10;
			int yInterval = (maxY - minY) / 10;

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

	};
}
