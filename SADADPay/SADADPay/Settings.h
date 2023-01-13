// Settings.h

#pragma once

#include "Definitions.h"

void	SetDefaultSettings(SETTINGS_INFO* pSettings);
void	LoadSettings(SETTINGS_INFO* pSettings);
void	SaveSettings(SETTINGS_INFO* pSettings);

BOOL	LoadParameters(PARAMETERS_INFO* pParameters);
void	SaveParameters(PARAMETERS_INFO* pParameters);

BOOL	LoadConfigXML(PARAMETERS_INFO* pParameters);
