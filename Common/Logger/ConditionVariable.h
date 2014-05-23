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

//Flags = 0��ʾ��ռ��Դ��= CONDITION_VARIABLE_LOCKMODE_SHARED ��ʾ�������Դ�ķ���
BOOL SleepConditionVariableSRW( _Inout_ PCONDITION_VARIABLE ConditionVariable, 
								_Inout_ PSRWLOCK SRWLock,
								_In_ DWORD dwMilliseconds,
								_In_ ULONG Flags);

VOID WakeConditionVariable( _Inout_ PCONDITION_VARIABLE ConditionVariable );

VOID WakeAllConditionVariable( _Inout_ PCONDITION_VARIABLE ConditionVariable );

//�������� wake��������������sleep��ʼ��ȡ�ٽ���Դ���д���������ȡ���˾ͷ��أ�һ��wakeall����
//�����Ѷ���߳��������sleep