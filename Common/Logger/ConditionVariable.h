#ifndef CONDITIONVARIABLE_H
#define CONDITIONVARIABLE_H
class TConditionVariable
{
public:
	TConditionVariable();
	~TConditionVariable();
};
#endif

BOOL SleepConditionVariableCS( _Inout_ PCONDITION_VARIABLE ConditionVariable, 
							  _Inout_ PCRITICAL_SECTION CriticalSection,
							  _In_ DWORD dwMilliseconds )

//Flags = 0表示独占资源；= CONDITION_VARIABLE_LOCKMODE_SHARED 表示共享对资源的访问
BOOL SleepConditionVariableSRW( _Inout_ PCONDITION_VARIABLE ConditionVariable, 
								_Inout_ PSRWLOCK SRWLock,
								_In_ DWORD dwMilliseconds,
								_In_ ULONG Flags);

VOID WakeConditionVariable( _Inout_ PCONDITION_VARIABLE ConditionVariable );

VOID WakeAllConditionVariable( _Inout_ PCONDITION_VARIABLE ConditionVariable );

//工作过程 wake唤醒条件变量，sleep开始获取临界资源或读写锁，如果获取到了就返回，一般wakeall是用
//来唤醒多个线程因读锁而sleep