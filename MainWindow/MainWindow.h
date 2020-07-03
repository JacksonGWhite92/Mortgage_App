#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include <qdebug.h>
#include <iostream>
#include <iomanip>
#include <QFileInfo>
#include <vector>
#include <map>
#include <fstream>
#include <string>
#include <sstream>
#include <QPainter.h>
#include <QtCharts>

using namespace std;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);


private slots:
	void Home_Value_Field_Changes_Submitted();
	void Down_Payment_Field_Changes_Submitted();
	void Loan_Term_Field_Changes_Submitted();
	void Payments_Per_Year_Field_Changes_Submitted();
	void Interest_Rate_Field_Changes_Submitted();
	void Extra_Mortgage_Field_Changes_Submitted();
	void Stock_Growth_Rate_Field_Changes_Submitted();
	void Stock_Contributions_Field_Changes_Submitted();
	void Save_New_Default_Values();


private:
    Ui::MainWindowClass ui;

	void Load_Default_Values();
	void On_Startup();
	void Generate_Tables();
	void Calculate_Monthly_Payment();
	void Clear_Vectors_and_Resize();
	void Changes_Submitted();
	double Home_Value, Down_Payment, Loan_Value, Loan_Term, Payments_Per_Year, Interest_Rate, Adjusted_Stock_Contribution;
	double Extra_Mortgage, Monthly_Payment, Monthly_Stock_Contribution, Monthly_Interest_Rate, Stock_Growth_Rate;
	int Number_Of_Payments;
	std::vector<double> Starting_Balance;
	std::vector<double> Remaining_Balance;
	std::vector<double> Monthly_Interest;
	std::vector<double> Monthly_Principal;
	std::vector<double> Total_Interest;
	std::vector<double> Total_Principal;
	std::vector<double> Stock_Value;
	std::vector<double> Interest_Difference;
	std::vector<double> Stock_Contributed;
	std::vector<float>   Payment_Number;
	QChart *chart = new QChart();
	QSplineSeries *Starting_Balance_Series = new QSplineSeries;
	QSplineSeries *Principal_Paid_Series = new QSplineSeries;
	QSplineSeries *Interest_Paid_Series = new QSplineSeries;
	QSplineSeries *Stock_Value_Series = new QSplineSeries;
	QLineSeries *House_Paid_Off_Series = new QLineSeries;
	QValueAxis *yAxis = new QValueAxis;
	QValueAxis *xAxis = new QValueAxis;
};
