#pragma once

#include <exception>
#include <QString>
#include <QByteArray>
#include "uglobal.h"

class UGLOBALHOTKEY_EXPORT UException : public std::exception {
public:
	UException(QString const &message) throw();
	char const *what() const throw();
	~UException() throw ();
private:
	QByteArray Message;
};
