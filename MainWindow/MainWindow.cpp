#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	Load_Default_Values();
	On_Startup();
	Calculate_Monthly_Payment();
	Generate_Tables();
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
			Home_Value = Defaults_Map.at("House_Value");
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
		in << "House_Value=" << Home_Value << endl;
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
	QPen *newPen = new QPen;
	newPen->setColor(QColorConstants::Green);
	newPen->setJoinStyle(Qt::RoundJoin);
	Starting_Balance_Series->setPen(*newPen);
	//Starting_Balance_Series->setColor(QColor(0, 255, 0));

	Principal_Paid_Series->setName("Principal Paid");
	Principal_Paid_Series->setColor(QColor(255, 20, 147));
	Interest_Paid_Series->setName("Interest Paid");
	Interest_Paid_Series->setColor(QColor(0, 191, 255));
	Stock_Value_Series->setName("Stock Interest Gained");
	Stock_Value_Series->setColor(QColor(255, 255, 0));

	House_Paid_Off_Series->setName("House Paid Off");
	House_Paid_Off_Series->setColor(QColor(255, 255, 255));
	ui.tableWidget->setColumnCount(8);
	ui.tableWidget->verticalHeader()->setVisible(false);
	chart->setBackgroundBrush(QColor(120, 120, 130));
	chart->legend()->setLabelColor(QColor(255, 255, 255));
	ui.Chart->setChart(chart);
	ui.Chart->setRenderHint(QPainter::HighQualityAntialiasing);
	Clear_Vectors_and_Resize();

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

void MainWindow::Changes_Submitted()
{
	Calculate_Monthly_Payment();
	Clear_Vectors_and_Resize();
	Generate_Tables();
}

void MainWindow::Generate_Tables()
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
	int House_Paid_Off;
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

	for (i = 0; i < Number_Of_Payments; i++)
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
		"Interest Paid", "Stock Value", "Total Stock Contributions", "Stock Interest Gained", "Interest Difference"});
	ui.tableWidget->horizontalHeader()->setVisible(true);
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	

	chart->removeSeries(Starting_Balance_Series);
	chart->removeSeries(Principal_Paid_Series);
	chart->removeSeries(Interest_Paid_Series);
	chart->removeSeries(Stock_Value_Series);
	chart->removeSeries(House_Paid_Off_Series);

	std::vector<double> Max_Values{ Total_Principal.back(), Total_Interest.back(), Starting_Balance.front(), Stock_Value.back() - Stock_Contributed.back() };
	double Chart_Max = *max_element(Max_Values.begin(), Max_Values.end());
	int NumDigits = floor(log10(Chart_Max));
	
	Chart_Max = ceil(Chart_Max / pow(10, floor(log10(Chart_Max)))) * pow(10, NumDigits);
	House_Paid_Off_Series->append(House_Paid_Off, 0);
	House_Paid_Off_Series->append(House_Paid_Off+0.0001, Chart_Max);
	QPen *House_Paid_Off_Pen = new QPen;
	House_Paid_Off_Pen->setStyle(Qt::DashLine);
	House_Paid_Off_Pen->setColor(QColorConstants::White);
	House_Paid_Off_Series->setPen(*House_Paid_Off_Pen);
	

	xAxis->setRange(0, Loan_Term);
	qDebug() << endl << endl << xAxis->tickInterval() << endl << endl;
	xAxis->setLabelsColor(QColor(255, 255, 255));
	xAxis->setTitleText("Years");
	xAxis->setTitleBrush(Qt::white);
	xAxis->setLabelFormat("%.0f");


	yAxis->setRange(0, Chart_Max);
	yAxis->setLabelsColor(QColor(255, 255, 255));
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

void MainWindow::Calculate_Monthly_Payment()
{
	Monthly_Interest_Rate = Interest_Rate / 100 / Payments_Per_Year;
	Monthly_Payment = round(Monthly_Interest_Rate * Loan_Value / (1 - pow((1 + Monthly_Interest_Rate), -Payments_Per_Year * Loan_Term)) * 100) / 100;
	ui.Monthly_Payment_Field->setPlainText("$ " + QString::number(Monthly_Payment));
}

void MainWindow::Home_Value_Field_Changes_Submitted()
{
	ui.Home_Value_Field->blockSignals(true);
	Home_Value = ui.Home_Value_Field->value();
	ui.Home_Value_Field->clearFocus();
	ui.Message_Box->setText("Home Value has been changed to $" + QString::number(Home_Value) + ".");
	Changes_Submitted();
	ui.Home_Value_Field->blockSignals(false);
}

void MainWindow::Down_Payment_Field_Changes_Submitted()
{
	ui.Down_Payment_Field->blockSignals(true);
	Down_Payment = ui.Down_Payment_Field->value();
	ui.Down_Payment_Field->clearFocus();
	ui.Message_Box->setText("Down Payment has been changed to $" + QString::number(Down_Payment) + ".");
	Changes_Submitted();
	ui.Down_Payment_Field->blockSignals(false);
}

void MainWindow::Loan_Term_Field_Changes_Submitted()
{
	ui.Loan_Term_Field->blockSignals(true);
	Loan_Term = ui.Loan_Term_Field->value();
	ui.Loan_Term_Field->clearFocus();
	ui.Message_Box->setText("Loan Term has been changed to " + QString::number(Loan_Term) + " years.");
	Changes_Submitted();
	ui.Loan_Term_Field->blockSignals(false);
}

void MainWindow::Payments_Per_Year_Field_Changes_Submitted()
{
	ui.Payments_Per_Year_Field->blockSignals(true);
	Payments_Per_Year = ui.Payments_Per_Year_Field->value();
	ui.Payments_Per_Year_Field->clearFocus();
	ui.Message_Box->setText("Payments Per Year has been changed to " + QString::number(Payments_Per_Year) + ".");
	Changes_Submitted();
	ui.Payments_Per_Year_Field->blockSignals(false);
}

void MainWindow::Interest_Rate_Field_Changes_Submitted()
{
	ui.Interest_Rate_Field->blockSignals(true);
	Interest_Rate = ui.Interest_Rate_Field->value();
	ui.Interest_Rate_Field->clearFocus();
	ui.Message_Box->setText("Interest Rate has been changed to " + QString::number(Interest_Rate) + "%.");
	Changes_Submitted();
	ui.Interest_Rate_Field->blockSignals(false);
}

void MainWindow::Extra_Mortgage_Field_Changes_Submitted()
{
	ui.Extra_Mortgage_Field->blockSignals(true);
	Extra_Mortgage = ui.Extra_Mortgage_Field->value();
	ui.Extra_Mortgage_Field->clearFocus();
	ui.Message_Box->setText("Extra Mortgage has been changed to $" + QString::number(Extra_Mortgage) + ".");
	Changes_Submitted();
	ui.Extra_Mortgage_Field->blockSignals(false);
}

void MainWindow::Stock_Contributions_Field_Changes_Submitted()
{
	ui.Stock_Contributions_Field->blockSignals(true);
	Monthly_Stock_Contribution = ui.Stock_Contributions_Field->value();
	ui.Stock_Contributions_Field->clearFocus();
	ui.Message_Box->setText("Monthly Stock Contributions has been changed to $" + QString::number(Monthly_Stock_Contribution) + ".");
	Changes_Submitted();
	ui.Stock_Contributions_Field->blockSignals(false);
}

void MainWindow::Stock_Growth_Rate_Field_Changes_Submitted()
{
	ui.Stock_Growth_Rate_Field->blockSignals(true);
	Stock_Growth_Rate = ui.Stock_Growth_Rate_Field->value();
	ui.Stock_Growth_Rate_Field->clearFocus();
	ui.Message_Box->setText("Yearly Stock Growth Rate has been changed to " + QString::number(Stock_Growth_Rate) + "%.");
	Changes_Submitted();
	ui.Stock_Growth_Rate_Field->blockSignals(false);
}