#ifndef NOTIFICATION_H
#define NOTIFICATION_H
#include <iostream>
#include <string>

class TNotification
{
public:
	TNotification(){}

private:
	TNotification(const TNotification& rhs);
	TNotification& operator= (const TNotification& rhs);
	virtual std::string name() const;

};
typedef std::shared_ptr<TNotification> TypeNoticationPtr;
#endif