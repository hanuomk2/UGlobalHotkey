#include "uexception.h"

UException::UException(QString const &message) throw()
	: Message(message.toLocal8Bit())
{
}

UException::~UException() throw ()
{
}

char const *UException::what() const throw ()
{
	return Message.data();
}
