#pragma once
#ifndef FACTORYMANANAGER_H
#define FACTORYMANANAGER_H

#include <SFML/Graphics.hpp>
#include "stdafx.h"
#include "Factory.h"
#include "Globals.h"

class FactoryManager {
private:
public:
	FactoryManager();
	static FactoryManager* GetInstance();
	void AddFactory(Factory*);
	void Update();
	void Draw();
	vector<Factory*> factories;
};

#endif 
