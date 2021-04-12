#include "../includes.h"

#include "interfaces.h"

#include "../SDK/ISurface.h"
#include "render.h"
#include "../SDK/IEngine.h"
#include "../SDK/CUserCmd.h"
float gat::get_absolute_time() {
	return (float)(clock() / (float)1000.f);
}
namespace FONTS
{
	bool ShouldReloadFonts()
	{
		static int old_width, old_height;
		int width, height;
		INTERFACES::Engine->GetScreenSize(width, height);

		if (width != old_width || height != old_height)
		{
			old_width = width;
			old_height = height;
			return true;
		}
		return false;
	}
	void InitFonts()
	{
		font_esp_flags = RENDER::CreateF("Smallest Pixel-7", 11, 400, 0, 0, SDK::FONTFLAG_ANTIALIAS | SDK::FONTFLAG_DROPSHADOW);
		font_indicators = RENDER::CreateF("Tahoma", 16, 500, 0, 0, SDK::FONTFLAG_ANTIALIAS);
		font_watermark = RENDER::CreateF("Tahoma", 12, 500, 0, 0, SDK::FONTFLAG_DROPSHADOW);
		font_logs = RENDER::CreateF("Arial", 14, 700, 0, 0, SDK::FONTFLAG_DROPSHADOW);
		visuals_esp_font = RENDER::CreateF("Smallest Pixel-7", 10, 100, 0, 0, SDK::FONTFLAG_OUTLINE);
		font_esp_name = RENDER::CreateF("Verdana", 12, 400, 0, 0, SDK::FONTFLAG_ANTIALIAS | SDK::FONTFLAG_DROPSHADOW);
		visuals_lby_font = RENDER::CreateF("Verdana", 25, 500, 0, 0, SDK::FONTFLAG_ANTIALIAS | SDK::FONTFLAG_DROPSHADOW);
		font_weapon_icon = RENDER::CreateF("astriumwep", 16, 400, 0, 0, SDK::FONTFLAG_DROPSHADOW | SDK::FONTFLAG_ANTIALIAS);
	}
}