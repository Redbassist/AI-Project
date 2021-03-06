#include "FactoryManager.h"

static bool instanceFlag = false;
static FactoryManager* instance = NULL;

FactoryManager::FactoryManager()
{
}

FactoryManager * FactoryManager::GetInstance()
{
	if (!instanceFlag) {
		instance = new FactoryManager();
		instanceFlag = true;
	}
	return instance;
}

void FactoryManager::AddFactory(Factory * factory)
{
	factories.push_back(factory);
}

void FactoryManager::Update()
{
	for (int i = 0; i < factories.size(); i++) {
		factories[i]->Update(factories);
		if (factories[i]->LifeCheck()) {
			factories.erase(factories.begin() + i);
			i--;
		}
	}
}

void FactoryManager::Draw()
{
	for (int i = 0; i < factories.size(); i++) {
		factories[i]->Draw();
	}
}

void FactoryManager::DrawUI()
{
	for (int i = 0; i < factories.size(); i++) {
		factories[i]->drawui();
	}
}