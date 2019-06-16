#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
    // w.show();                        // comment out 2019.05.08
    w.show_systemtrayicon();            // システムトレイアイコン化 2019.05.08

	return a.exec();
}
