


#include "Tasks/CP_AttributeChangeTask.h"
#include "AbilitySystemComponent.h"


UCP_AttributeChangeTask* UCP_AttributeChangeTask::ListenForAttributeChange(UAbilitySystemComponent* AbilitySystemComponent, FGameplayAttribute Attribute)
{
	UCP_AttributeChangeTask* WaitForAttributeChangeTask = NewObject<UCP_AttributeChangeTask>();
	WaitForAttributeChangeTask->ASC = AbilitySystemComponent;
	WaitForAttributeChangeTask->AttributeToListenFor = Attribute;

	if (!IsValid(AbilitySystemComponent))
	{
		WaitForAttributeChangeTask->RemoveFromRoot();
		return nullptr;
	}
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(WaitForAttributeChangeTask, &UCP_AttributeChangeTask::AttributeChanged);

	return WaitForAttributeChangeTask;
}

void UCP_AttributeChangeTask::EndTask()
{
	if (ASC.IsValid())
	{
		ASC->GetGameplayAttributeValueChangeDelegate(AttributeToListenFor).RemoveAll(this);
	}

	SetReadyToDestroy();
	MarkAsGarbage();
}

void UCP_AttributeChangeTask::AttributeChanged(const FOnAttributeChangeData& Data)
{
	OnAttributeChanged.Broadcast(Data.Attribute, Data.NewValue, Data.OldValue);
}