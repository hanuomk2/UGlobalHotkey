#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <uglobalhotkeys.h>
//#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	QString hotkey = "Ctrl+Shift+F12";
	ui->label->setText(QString("Press %1 key.").arg(hotkey));

	UGlobalHotkeys *hotkeyManager = new UGlobalHotkeys(this);
	hotkeyManager->registerHotkey(hotkey);

	connect(hotkeyManager, &UGlobalHotkeys::activated, [&](size_t id){
		value_++;
		QString text = QString::number(value_);
		ui->label_2->setText(text);
	});
}

MainWindow::~MainWindow()
{
	delete ui;
}
