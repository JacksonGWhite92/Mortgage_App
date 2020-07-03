#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	Load_Default_Values();
	On_Startup();
	Update();
}

void MainWindow::On_Startup()
{
	ui.Home_Value_Field->setValue(Home_Value);
	ui.Down_Payment_Field->setValue(Down_Payment);
	Loan_Value = Home_Value - Down_Payment;
	ui.Loan_Value_Field->setPlainText("$ " + QString::number(Loan_Value));
	ui.Loan_Term_Field->setValue(Loan_Term);
	ui.Payments_Per_Year_Field->setValue(Payments_Per_Year);
	ui.Interest_Rate_Field->setValue(Interest_Rate);
	ui.Extra_Mortgage_Field->setValue(Extra_Mortgage);
	Number_Of_Payments = Loan_Term * Payments_Per_Year + 1;
	ui.Stock_Contributions_Field->setValue(Monthly_Stock_Contribution);
	ui.Stock_Growth_Rate_Field->setValue(Stock_Growth_Rate);

	Starting_Balance_Series->setName("Starting Balance");
	Starting_Balance_Series->setColor(QColorConstants::Svg::chartreuse);

	Principal_Paid_Series->setName("Principal Paid");
	Principal_Paid_Series->setColor(QColorConstants::Svg::magenta);

	Interest_Paid_Series->setName("Interest Paid");
	Interest_Paid_Series->setColor(QColorConstants::Svg::aqua);

	Stock_Value_Series->setName("Stock Interest Gained");
	Stock_Value_Series->setColor(QColorConstants::Svg::yellow);

	House_Paid_Off_Series->setName("House Paid Off");
	QPen *House_Paid_Off_Pen = new QPen;
	House_Paid_Off_Pen->setStyle(Qt::DashLine);
	House_Paid_Off_Pen->setColor(QColorConstants::Svg::black);
	House_Paid_Off_Series->setPen(*House_Paid_Off_Pen);

	ui.tableWidget->setColumnCount(8);
	ui.tableWidget->verticalHeader()->setVisible(false);
	chart->setBackgroundBrush(QColor(120, 120, 130));
	chart->legend()->setLabelColor(QColorConstants::Svg::white);
	ui.Chart->setChart(chart);
	ui.Chart->setRenderHint(QPainter::HighQualityAntialiasing);
	Clear_Vectors_and_Resize();

}

void MainWindow::Load_Default_Values()
{
	QFile Default_Values_File("Default_Values.txt");

	string line;
	// test for files not existing. 
	if (!Default_Values_File.open(QIODevice::ReadOnly))
	{
		qDebug() << "\n\n\nError! Could not open file\n";
		exit(-1); // must include stdlib.h 
	}
	else
	{
		ui.Message_Box->setText("Default Values Loaded.");
		map<string, double> Defaults_Map;
		QTextStream in(&Default_Values_File);
		while (!in.atEnd())
		{
			QString line = in.readLine();
			istringstream is_line(qPrintable(line));
			string key;
			if (getline(is_line, key, '='))
			{
				string value;
				if (getline(is_line, value))
				{
					Defaults_Map[key] = stof(value);
				}

			}
		}
		Default_Values_File.close();

		try
		{
			Home_Value = Defaults_Map.at("Home_Value");
			Payments_Per_Year = Defaults_Map.at("Payments_Per_Year");
			Loan_Term = Defaults_Map.at("Loan_Term_Years");
			Down_Payment = Defaults_Map.at("Down_Payment");
			Interest_Rate = Defaults_Map.at("Interest_Rate_Percent");
			Extra_Mortgage = Defaults_Map.at("Extra_Mortgage");
			Monthly_Stock_Contribution = Defaults_Map.at("Stock_Contributions");
			Stock_Growth_Rate = Defaults_Map.at("Stock_Growth_Rate");


		}
		catch (const out_of_range)
		{
			qDebug() << endl << endl << "One or more variables was not found in Default_Values.txt. Please Correct." << endl;
			exit(-1);
		}
	}
}

void MainWindow::Reload_Default_Values()
{
	QFile Default_Values_File("Default_Values.txt");

	string line;
	// test for files not existing. 
	if (!Default_Values_File.open(QIODevice::ReadOnly))
	{
		qDebug() << "\n\n\nError! Could not open file\n";
		exit(-1); // must include stdlib.h 
	}
	else
	{
		ui.Message_Box->setText("Default Values Reloaded.");
		map<string, double> Defaults_Map;
		QTextStream in(&Default_Values_File);
		while (!in.atEnd())
		{
			QString line = in.readLine();
			istringstream is_line(qPrintable(line));
			string key;
			if (getline(is_line, key, '='))
			{
				string value;
				if (getline(is_line, value))
				{
					Defaults_Map[key] = stof(value);
				}

			}
		}
		Default_Values_File.close();

		try
		{
			Home_Value = Defaults_Map.at("Home_Value");
			Payments_Per_Year = Defaults_Map.at("Payments_Per_Year");
			Loan_Term = Defaults_Map.at("Loan_Term_Years");
			Down_Payment = Defaults_Map.at("Down_Payment");
			Interest_Rate = Defaults_Map.at("Interest_Rate_Percent");
			Extra_Mortgage = Defaults_Map.at("Extra_Mortgage");
			Monthly_Stock_Contribution = Defaults_Map.at("Stock_Contributions");
			Stock_Growth_Rate = Defaults_Map.at("Stock_Growth_Rate");

			ui.Home_Value_Field->setValue(Home_Value);
			ui.Down_Payment_Field->setValue(Down_Payment);
			Loan_Value = Home_Value - Down_Payment;
			ui.Loan_Value_Field->setPlainText("$ " + QString::number(Loan_Value));
			ui.Loan_Term_Field->setValue(Loan_Term);
			ui.Payments_Per_Year_Field->setValue(Payments_Per_Year);
			ui.Interest_Rate_Field->setValue(Interest_Rate);
			ui.Extra_Mortgage_Field->setValue(Extra_Mortgage);
			Number_Of_Payments = Loan_Term * Payments_Per_Year + 1;
			ui.Stock_Contributions_Field->setValue(Monthly_Stock_Contribution);
			ui.Stock_Growth_Rate_Field->setValue(Stock_Growth_Rate);

			Update();
		}
		catch (const out_of_range)
		{
			qDebug() << endl << endl << "One or more variables was not found in Default_Values.txt. Please Correct." << endl;
			exit(-1);
		}
	}
}

void MainWindow::Save_New_Default_Values()
{
	QFile Default_Values_File("Default_Values.txt");
	string line;
	// test for files not existing. 
	if (!Default_Values_File.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
	{
		qDebug() << "\n\n\nError! Could not open file\n";
		exit(-1); // must include stdlib.h 
	}
	else
	{
		QTextStream in(&Default_Values_File);
		in << "Home_Value=" << Home_Value << endl;
		in << "Down_Payment=" << Down_Payment << endl;
		in << "Interest_Rate_Percent=" << Interest_Rate << endl;
		in << "Loan_Term_Years=" << Loan_Term << endl;
		in << "Payments_Per_Year=" << Payments_Per_Year << endl;
		in << "Extra_Mortgage=" << Extra_Mortgage << endl;
		in << "Stock_Contributions=" << Monthly_Stock_Contribution << endl;
		in << "Stock_Growth_Rate=" << Stock_Growth_Rate << endl;
		ui.Message_Box->setText("The default values have been updated.");
		Default_Values_File.flush();
		Default_Values_File.close();

	}
}

void MainWindow::Field_Changes_Submitted()
{

	QObject::sender()->blockSignals(true);
	QDoubleSpinBox* Field = qobject_cast<QDoubleSpinBox*>(QObject::sender());
	Field->clearFocus();
	double New_Value = 0;
	QString Prefix = "";
	QString Suffix = "";
	if (Field->objectName() == "Home_Value_Field")
	{
		Home_Value = Field->value();
		New_Value = Home_Value;
		Prefix = "$";
	}
	else if (Field->objectName() == "Down_Payment_Field")
	{
		Down_Payment = Field->value();
		New_Value = Down_Payment;
		Prefix = "$";
	}
	else if (Field->objectName() == "Loan_Term_Field")
	{
		Loan_Term = Field->value();
		New_Value = Loan_Term;
		Suffix = " years";
	}
	else if (Field->objectName() == "Payments_Per_Year_Field")
	{
		Payments_Per_Year = Field->value();
		New_Value = Payments_Per_Year;
	}
	else if (Field->objectName() == "Interest_Rate_Field")
	{
		Interest_Rate = Field->value();
		New_Value = Interest_Rate;
		Suffix = "%";
	}
	else if (Field->objectName() == "Extra_Mortgage_Field")
	{
		Extra_Mortgage = Field->value();
		New_Value = Extra_Mortgage;
		Prefix = "$";
	}
	else if (Field->objectName() == "Stock_Contributions_Field")
	{
		Monthly_Stock_Contribution = Field->value();
		New_Value = Monthly_Stock_Contribution;
		Prefix = "$";
	}
	else if (Field->objectName() == "Stock_Growth_Rate_Field")
	{
		Stock_Growth_Rate = Field->value();
		New_Value = Stock_Growth_Rate;
		Suffix = "%";
	}
	ui.Message_Box->setText(QObject::sender()->objectName() + " has been changed to " + Prefix + QString::number(New_Value) + Suffix + ".");
	Update();
	QObject::sender()->blockSignals(false);

}

void MainWindow::Update()
{
	Calculate_Monthly_Payment();
	Clear_Vectors_and_Resize();
	Do_The_Math();
	Fill_The_Table();
	Make_The_Chart();
}

void MainWindow::Calculate_Monthly_Payment()
{
	Loan_Value = Home_Value - Down_Payment;
	Monthly_Interest_Rate = Interest_Rate / 100 / Payments_Per_Year;
	Monthly_Payment = round(Monthly_Interest_Rate * Loan_Value / (1 - pow((1 + Monthly_Interest_Rate), -Payments_Per_Year * Loan_Term)) * 100) / 100;
	ui.Monthly_Payment_Field->setPlainText("$ " + QString::number(Monthly_Payment));
}

void MainWindow::Clear_Vectors_and_Resize()
{
	Number_Of_Payments = Loan_Term * Payments_Per_Year + 1;
	ui.tableWidget->clearContents();
	ui.tableWidget->setRowCount(Number_Of_Payments);

	Starting_Balance.clear();
	Starting_Balance.resize(Number_Of_Payments);

	Remaining_Balance.clear();
	Remaining_Balance.resize(Number_Of_Payments);

	Monthly_Interest.clear();
	Monthly_Interest.resize(Number_Of_Payments);

	Monthly_Principal.clear();
	Monthly_Principal.resize(Number_Of_Payments);

	Total_Interest.clear();
	Total_Interest.resize(Number_Of_Payments);

	Total_Principal.clear();
	Total_Principal.resize(Number_Of_Payments);

	Stock_Value.clear();
	Stock_Value.resize(Number_Of_Payments);

	Interest_Difference.clear();
	Interest_Difference.resize(Number_Of_Payments);

	Stock_Contributed.clear();
	Stock_Contributed.resize(Number_Of_Payments);

	Payment_Number.clear();
	Payment_Number.resize(Number_Of_Payments);

	Starting_Balance_Series->clear();
	Principal_Paid_Series->clear();
	Interest_Paid_Series->clear();
	Stock_Value_Series->clear();
	House_Paid_Off_Series->clear();

	ui.Chart->repaint();
}

void MainWindow::Do_The_Math()
{
	Starting_Balance[0] = Loan_Value;
	Remaining_Balance[0] = Starting_Balance[0];

	Starting_Balance[1] = Remaining_Balance[0];
	Total_Interest[0] = 0;
	Total_Principal[0] = 0;
	Stock_Value[0] = 0;
	Interest_Difference[0] = Stock_Value[0] - Total_Principal[0];
	Payment_Number[0] = 0;
	int i = 1;
	while (i < Number_Of_Payments)
	{
		Payment_Number[i] = i;

		Monthly_Interest[i] = round(Starting_Balance[i] * Monthly_Interest_Rate * 100) / 100;
		if (Starting_Balance[i] > 0)
		{
			Monthly_Principal[i] = Monthly_Payment - Monthly_Interest[i] + Extra_Mortgage;
			Adjusted_Stock_Contribution = Monthly_Stock_Contribution;
		}
		else if (Starting_Balance[i] < 0)
		{
			i = i - 1;
			Monthly_Principal[i] = Starting_Balance[i];
			Adjusted_Stock_Contribution = Monthly_Payment + Extra_Mortgage - Monthly_Interest[i] - Monthly_Principal[i] + Monthly_Stock_Contribution;
			House_Paid_Off = Payment_Number[i];

		}
		else
		{
			Monthly_Principal[i] = 0;
			Adjusted_Stock_Contribution = Monthly_Payment + Extra_Mortgage + Monthly_Stock_Contribution;
		}

		Stock_Contributed[i] = Stock_Contributed[i - 1] + Adjusted_Stock_Contribution;
		Stock_Value[i] = (Stock_Value[i - 1] + Adjusted_Stock_Contribution)*(1 + Stock_Growth_Rate / 100 / Payments_Per_Year);
		Total_Interest[i] = Total_Interest[i - 1] + Monthly_Interest[i];
		Total_Principal[i] = Total_Principal[i - 1] + Monthly_Principal[i];
		Interest_Difference[i] = Stock_Value[i] - Stock_Contributed[i] - Total_Interest[i];
		Remaining_Balance[i] = Starting_Balance[i] - Monthly_Principal[i];

		i++;

		if (i < Number_Of_Payments) Starting_Balance[i] = Remaining_Balance[i - 1];
	}
}

void MainWindow::Fill_The_Table()
{
	for (int i = 0; i < Number_Of_Payments; i++)
	{
		Starting_Balance_Series->append(Payment_Number[i] / Payments_Per_Year, Starting_Balance[i]);
		Principal_Paid_Series->append(Payment_Number[i] / Payments_Per_Year, Total_Principal[i]);
		Interest_Paid_Series->append(Payment_Number[i] / Payments_Per_Year, Total_Interest[i]);
		Stock_Value_Series->append(Payment_Number[i] / Payments_Per_Year, Stock_Value[i] - Stock_Contributed[i]);
		ui.tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(Payment_Number[i], 'f', 0)));
		ui.tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(Starting_Balance[i], 'f', 2)));
		ui.tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(Total_Principal[i], 'f', 2)));
		ui.tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(Total_Interest[i], 'f', 2)));
		ui.tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(Stock_Value[i], 'f', 2)));
		ui.tableWidget->setItem(i, 5, new QTableWidgetItem(QString::number(Stock_Contributed[i], 'f', 2)));
		ui.tableWidget->setItem(i, 6, new QTableWidgetItem(QString::number((Stock_Value[i] - Stock_Contributed[i]), 'f', 2)));
		ui.tableWidget->setItem(i, 7, new QTableWidgetItem(QString::number(Interest_Difference[i], 'f', 2)));
	}

	ui.tableWidget->setHorizontalHeaderLabels({ "Payments Made", "Beginning Balance", "Principal Paid",
		"Interest Paid", "Stock Value", "Total Stock Contributions", "Stock Interest Gained", "Interest Difference" });
	ui.tableWidget->horizontalHeader()->setVisible(true);
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::Make_The_Chart()
{
	chart->removeSeries(Starting_Balance_Series);
	chart->removeSeries(Principal_Paid_Series);
	chart->removeSeries(Interest_Paid_Series);
	chart->removeSeries(Stock_Value_Series);
	chart->removeSeries(House_Paid_Off_Series);

	std::vector<double> Max_Values{ Total_Principal.back(), Total_Interest.back(), Starting_Balance.front(), Stock_Value.back() - Stock_Contributed.back() };
	double Chart_Max = *max_element(Max_Values.begin(), Max_Values.end());
	int NumDigits = floor(log10(Chart_Max));

	Chart_Max = ceil(Chart_Max / pow(10, floor(log10(Chart_Max)))) * pow(10, NumDigits);
	House_Paid_Off_Series->append(House_Paid_Off / Payments_Per_Year, 0);
	House_Paid_Off_Series->append((House_Paid_Off / Payments_Per_Year) + 0.000001, Chart_Max);

	xAxis->setRange(0, Loan_Term);
	xAxis->setTickType(QValueAxis::TicksDynamic);
	xAxis->setTickAnchor(qreal(0));
	xAxis->setTickInterval(qreal(5));
	xAxis->setLabelsColor(QColorConstants::White);
	xAxis->setTitleText("Years");
	xAxis->setTitleBrush(Qt::white);
	xAxis->setLabelFormat("%.0f");

	yAxis->setRange(0, Chart_Max);
	yAxis->setLabelsColor(QColorConstants::White);
	yAxis->setTitleText("Value");
	yAxis->setTitleBrush(Qt::white);
	yAxis->setLabelFormat("%.0f");

	chart->addSeries(Starting_Balance_Series);
	chart->addSeries(Principal_Paid_Series);
	chart->addSeries(Interest_Paid_Series);
	chart->addSeries(Stock_Value_Series);
	chart->addSeries(House_Paid_Off_Series);
	chart->setAxisX(xAxis, Starting_Balance_Series);
	chart->setAxisX(xAxis, Principal_Paid_Series);
	chart->setAxisX(xAxis, Interest_Paid_Series);
	chart->setAxisX(xAxis, Stock_Value_Series);
	chart->setAxisX(xAxis, House_Paid_Off_Series);
	chart->setAxisY(yAxis, Starting_Balance_Series);
	chart->setAxisY(yAxis, Principal_Paid_Series);
	chart->setAxisY(yAxis, Interest_Paid_Series);
	chart->setAxisY(yAxis, Stock_Value_Series);
	chart->setAxisY(yAxis, House_Paid_Off_Series);

	ui.Chart->repaint();
}


