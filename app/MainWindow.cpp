#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QDebug>
#include <uglobalhotkeys.h>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	UGlobalHotkeys *hotkeyManager = new UGlobalHotkeys(this);
	hotkeyManager->registerHotkey("Ctrl+Shift+F12");
	connect(hotkeyManager, &UGlobalHotkeys::activated, [=](size_t id){
		qDebug() << "Activated: " << QString::number(id);
	});
}

MainWindow::~MainWindow()
{
	delete ui;
}
