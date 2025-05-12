#include "nav.h"

NavigationManager& NavigationManager::instance()
{
	static NavigationManager instance;
	return instance;
}

NavigationManager::NavigationManager(QObject* parent)
	: QObject(parent)
{}
