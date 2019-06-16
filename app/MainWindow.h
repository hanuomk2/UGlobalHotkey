#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>                                          // add 2019.05.08
#include <windows.h>                                                // add 2019.05.31

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
    void show_systemtrayicon();

private:
	Ui::MainWindow *ui;
    ////////////////////////////////////////////////////////////////// add 2019.05.08
    QSystemTrayIcon tray_icon;
    QString icon_path;
    QMenu *menu;
    QAction *action_quit;
    QAction *action_qt;

// public slots:
    void screencapture(size_t id);
    QPixmap createcaptureDIB(HWND hwnd, int width, int height);     // add 2019.06.03
    QPixmap createcaptureDDB(HWND hwnd, int width, int height);
    ////////////////////////////////////////////////////////////////// end 2019.05.08
};

#endif // MAINWINDOW_H
