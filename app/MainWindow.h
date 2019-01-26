#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
	Q_OBJECT
private:
	unsigned int value_ = 0;
public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();
private:
	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
