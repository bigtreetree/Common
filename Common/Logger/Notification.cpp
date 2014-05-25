#include "Notification.h"

std::string TNotification::name() const
{
	return typeid(this).name();
}