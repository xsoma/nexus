#pragma once
#include <xstring>
#include <vector>
#include "../../UTILS/color.h"
#include "../../SDK/CGlobalVars.h"
#include "../../SDK/CInput.h"
#include "../../SDK/IEngine.h"
#include "../../UTILS/interfaces.h"
#include "../../UTILS/render.h"
#include "../../SDK/ConVar.h"
int showtime = 5.f;
std::string prefix = "[Nexus] ";

class CMessage
{
public:
	CMessage(std::string str, float time, CColor clr)
	{
		this->str = str;
		this->time = gat::get_absolute_time();
		this->clr = clr;
		alpha = 255;
	}
	std::string str;
	float time;
	CColor clr;
	int alpha;
};

class CEventlog
{
public:
	std::vector <CMessage> messages;
	void Draw()
	{
		if (!INTERFACES::Engine->IsConnected() || !INTERFACES::Engine->IsInGame()) {
			messages.clear();
			return;
		}

		if (!SETTINGS::settings.info_bool)
			return;
		if (messages.empty())
			return;
		while (messages.size() > 10)
			messages.pop_back();
		for (int i = messages.size() - 1; i >= 0; i--)
		{
			if (messages[i].time + showtime <= gat::get_absolute_time())
				messages[i].alpha -= 1;

			RENDER::Text(7 - (255 - messages[i].alpha), 7 + (15 * i), CColor(255, 50, 50), FONTS::font_logs, messages[i].str.c_str());

			if (messages[i].alpha <= 0)
				messages.erase(messages.begin() + i);
		}

	}
};
extern CEventlog* EventLog;