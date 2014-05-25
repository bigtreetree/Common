#ifndef NOTIFICATION_H
#define NOTIFICATION_H
#include <iostream>

class TNotification
{
public:
	TNotification(){}

private:
	TNotification(const TNotification& rhs);
	TNotification& operator= (const TNotification& rhs);

};
typedef std::shared_ptr<TNotification> TypeNoticationPtr;
#endif