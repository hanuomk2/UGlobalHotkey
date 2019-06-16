#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <uglobalhotkeys.h>
//#include <QDebug>
//#include <windows.h>                                                // includeファイル追加(VK_SNAPSHOT用) 2019.05.08
#include <QScreen>                                                  // includeファイル追加(スクリーンキャプチャ用) 2019.05.09
#include <QDesktopWidget>                                           // includeファイル追加(スクリーンキャプチャ用) 2019.05.09
#include <QClipboard>                                               // includeファイル追加(クリックボード用) 2019.05.08
#include <QWindow>
#include <QtWinExtras/QtWin>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

    // QString hotkey = "Ctrl+Shift+F12";                           // comment out 2019.05.08
    QString hotkey = "PrintScreen or Alt+PrintScreen";              // テキストの変更 2019.05.08
	ui->label->setText(QString("Press %1 key.").arg(hotkey));

	UGlobalHotkeys *hotkeyManager = new UGlobalHotkeys(this);
    // hotkeyManager->registerHotkey(hotkey);                       // comment out 2019.05.08
    hotkeyManager->registerHotkey(0, VK_SNAPSHOT, 0);               // ホットキーの登録id=0(PrintScreen) 2019.05.08
    hotkeyManager->registerHotkey(1, VK_SNAPSHOT, 1);               // ホットキーの登録id=1(Alt+PrintScreen) 2019.05.08

    /***************************************************************** comment out 2019.05.08
	connect(hotkeyManager, &UGlobalHotkeys::activated, [&](size_t id){
		value_++;
		QString text = QString::number(value_);
		ui->label_2->setText(text);
	});
    *****************************************************************/

    ////////////////////////////////////////////////////////////////// システムトレイアイコン化 add 2019.05.08
    // 最大化ボタン・最小化ボタン・閉じるボタン・メニューを非表示
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);

    // メニュー・アクションの生成
    menu = new QMenu(this);
    action_qt = new QAction(tr("Open"), this);
    action_quit = new QAction(tr("Quit"), this);

    // メニューにアクションを追加
    menu->addAction(action_qt);
    menu->addSeparator();
    menu->addAction(action_quit);

    // トレイアイコンにメニューを追加
    tray_icon.setContextMenu(menu);

    // シグナル・スロットの設定
    connect(action_qt, &QAction::triggered, this, &MainWindow::show);
    connect(action_quit, &QAction::triggered, this, &QApplication::quit);
    connect(hotkeyManager, &UGlobalHotkeys::activated, [&](size_t id){
        value_++;
        QString text = QString::number(value_);
        ui->label_2->setText(text);
        qDebug("counter: %d", value_);
        screencapture(id);
    });
    ////////////////////////////////////////////////////////////////// システムトレイアイコン化 end 2019.05.08
}

MainWindow::~MainWindow()
{
	delete ui;
}

////////////////////////////////////////////////////////////////////// システムトレイアイコン化 add 2019.05.08
void MainWindow::show_systemtrayicon()
{
    // tray_icon.hide();
    tray_icon.setIcon(QPixmap(":/images/Capture.bmp"));
    tray_icon.show();
}
////////////////////////////////////////////////////////////////////// システムトレイアイコン化 end 2019.05.08

////////////////////////////////////////////////////////////////////// VK_SNAPSHOTでスクリーンキャプチャ add 2019.05.09
void MainWindow::screencapture(size_t id)
{
    const int buflen = 24;
    const char screenTitle[buflen] = "Pressed PrintScreen";
    const char screenMessage[buflen] = "Screen Capture";
    const char windowTitle[buflen] = "Pressed Alt+PrintScreen";
    const char windowMessage[buflen] = "Active Window Capture";
    QPixmap pixmap;
    const char* title = screenTitle;
    const char* message = screenMessage;

    if (!id)
    {
        ////////////////////////////////////////////////////////////// ホットキーid=0(PrintScreen)押下の場合→スクリーンキャプチャ
        pixmap = QPixmap::grabWindow(0);
        title = screenTitle;
        message = screenMessage;
    }
    else
    {
        ////////////////////////////////////////////////////////////// ホットキーid=1(Alt+PrintScreen)押下の場合→アクティブウィンドウキャプチャ
#if defined(Q_OS_WIN)
        HWND hWnd = GetForegroundWindow();                          // アクティブウィンドウのHWND取得(※windows API)

        // grabWindow()だと、クライアント領域だけをキャプチャ(Aeroも非対応)
        // pixmap = QPixmap::grabWindow((WId)hWnd);

        RECT Rect;
        GetWindowRect(hWnd, &Rect);                                  // アクティブウィンドウの各座標取得(※windows API)
        // DDBだと何故か上手くキャプチャできない by windows 10(他のOSはテストしていない)
        // pixmap = createcaptureDDB(hWnd, Rect.right - Rect.left, Rect.bottom - Rect.top);
        // アクティブブウィンドウのキャプチャ画像をDIB BITMAPで取得しQPixmapに変換する(Aero非対応)
        pixmap = createcaptureDIB(hWnd, Rect.right - Rect.left, Rect.bottom - Rect.top);
        title = windowTitle;
        message = windowMessage;
#endif
    }

    if (!pixmap.isNull())                                           // pixmapが空でない場合、メッセージ表示&クリップボードにセット
    {
        tray_icon.showMessage(tr(title), tr(message),QIcon(pixmap), 1000/*msec*/);// メッセージ表示

        QClipboard *clipboard = QApplication::clipboard();          // クリップボードにQPixmapをセット
        clipboard->setPixmap(pixmap);
    }
}
////////////////////////////////////////////////////////////////////// VK_SNAPSHOTでスクリーンキャプチャ end 2019.05.09

QPixmap MainWindow::createcaptureDIB(HWND hwnd, int width, int height)
{
    HDC hdc;
    static BITMAPINFO bmpInfo;
    static LPDWORD lpPixel;
    static HBITMAP hBitmap;
    static HDC hMemDC;

    //DIBの情報を設定する
    bmpInfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
    bmpInfo.bmiHeader.biWidth=width;
    bmpInfo.bmiHeader.biHeight=height;
    bmpInfo.bmiHeader.biPlanes=1;
    bmpInfo.bmiHeader.biBitCount=24;
    bmpInfo.bmiHeader.biCompression=BI_RGB;

    //DIBSection作成
    hdc = CreateCompatibleDC(nullptr);
    hBitmap=CreateDIBSection(hdc, &bmpInfo, DIB_RGB_COLORS, (void**)&lpPixel, NULL, 0);
    hMemDC=CreateCompatibleDC(hdc);
    SelectObject(hMemDC, hBitmap);

    //スクリーンをDIBSectionにコピー
    BitBlt(hMemDC, 0, 0, width, height, GetWindowDC(hwnd), 0, 0, SRCCOPY);

    //DIBSectionをpixmapに変換
    // pixmap = QtWin::fromHBITMAP(hBitmap);                // これだと何故かエラー
    QImage image = QtWin::imageFromHBITMAP(hdc, hBitmap, width, height);

    ReleaseDC(nullptr,hdc);
    DeleteDC(hMemDC);
    DeleteObject(hBitmap);

    return QPixmap::fromImage(image);
}

QPixmap MainWindow::createcaptureDDB(HWND hwnd, int width, int height)
{
    HDC hdc;
    static HBITMAP hBitmap;
    static HDC hMemDC;

    //メモリデバイスコンテキストを作る
    hdc = CreateCompatibleDC(nullptr);
    hBitmap=CreateCompatibleBitmap(hdc, width, height);
    hMemDC=CreateCompatibleDC(hdc);
    SelectObject(hMemDC, hBitmap);

    BitBlt(hMemDC, 0 , 0, width, height, GetWindowDC(hwnd), 0, 0, SRCCOPY);
    QImage image = QtWin::imageFromHBITMAP(hdc, hBitmap, width, height);

    ReleaseDC(nullptr, hdc);
    DeleteDC(hMemDC);
    DeleteObject(hBitmap);

    return QPixmap::fromImage(image);
}
