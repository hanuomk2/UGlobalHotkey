#pragma once

#include <QWidget>
#include <QAbstractNativeEventFilter>
#include <QSet>

#if defined(Q_OS_LINUX)
#include "xcb/xcb.h"
#include "xcb/xcb_keysyms.h"
#elif defined(Q_OS_MAC)
#include <Carbon/Carbon.h>
#endif

#include "ukeysequence.h"
#include "uexception.h"
#include "uglobal.h"

#if defined(Q_OS_LINUX)
struct UHotkeyData {
	xcb_keycode_t keyCode;
	int mods;
	bool operator ==(const UHotkeyData& data) const {
		return data.keyCode == this->keyCode && data.mods == this->mods;
	}
};
#endif

class UGLOBALHOTKEY_EXPORT UGlobalHotkeys : public QWidget
		#if defined(Q_OS_LINUX)
		, public QAbstractNativeEventFilter
		#endif
{
	Q_OBJECT
public:
	explicit UGlobalHotkeys(QWidget *parent = 0);
	void registerHotkey(QString const &keySeq, size_t id = 1);
	void registerHotkey(UKeySequence const &keySeq, size_t id = 1);
    void registerHotkey(size_t winMod, size_t key, size_t id = 1); ///////////////////////////// 2019.05.08 add Virtual KeyCode対応
	void unregisterHotkey(size_t id = 1);
	void unregisterAllHotkeys();
	~UGlobalHotkeys();
protected:
#if defined(Q_OS_WIN)
	bool winEvent (MSG *message, long *result);
	bool nativeEvent(QByteArray const &eventType, void *message, long *result);
#elif defined(Q_OS_LINUX)
	bool nativeEventFilter(QByteArray const &eventType, void *message, long *result);
	bool linuxEvent(xcb_generic_event_t *message);
	void regLinuxHotkey(UKeySequence const &keySeq, size_t id);
	void unregLinuxHotkey(size_t id);
#endif
public:
#if defined (Q_OS_MAC)
	void onHotkeyPressed(size_t id);
#endif
signals:
	void activated(size_t id);
private:
#if defined(Q_OS_WIN)
	QSet<size_t> Registered;
#elif defined(Q_OS_LINUX)
	QHash<size_t, UHotkeyData> Registered;
	xcb_connection_t *X11Connection;
	xcb_window_t X11Wid;
	xcb_key_symbols_t *X11KeySymbs;
#elif defined(Q_OS_MAC)
	QHash<size_t, EventHotKeyRef> HotkeyRefs;
#endif
};
