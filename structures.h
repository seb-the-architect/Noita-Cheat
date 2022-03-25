#pragma once
#include "pch.h"

struct Settings
{
	bool bLevitation = 1;
	bool bHealth = 1;
	bool bInstaKill = 1;
	bool bMana = 1;
	bool bOxygen = 1;
	bool bAmmo = 1;
	bool bRich = 1;

}settings;

struct Values
{
	float* Levitation = nullptr;
	float* Health = nullptr;
	float* MaxHealth = nullptr;
	float* AllHealth = nullptr;
	float* Mana = nullptr;
	float* MaxMana = nullptr;
	float* Oxygen = nullptr;
	int* Ammo = nullptr;
	int* Gold = nullptr;

}values;