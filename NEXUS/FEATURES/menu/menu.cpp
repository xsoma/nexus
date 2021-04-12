#include "menu.h"

void Menu::AimbotTab()
{
	static int weapons = 0;
	if (ImGui::Button("Auto", ImVec2(75, 15))) weapons = 0;
	ImGui::SameLine();
	if (ImGui::Button("Scout", ImVec2(75, 15))) weapons = 1;
	ImGui::SameLine();
	if (ImGui::Button("AWP", ImVec2(75, 15))) weapons = 2;
	ImGui::SameLine();
	if (ImGui::Button("Heavy Pistols", ImVec2(75, 15))) weapons = 3;
	ImGui::SameLine();
	if (ImGui::Button("Pistols", ImVec2(75, 15))) weapons = 4;
	ImGui::SameLine();
	if (ImGui::Button("Rifle", ImVec2(75, 15))) weapons = 5;
	ImGui::SameLine();
	if (ImGui::Button("Other", ImVec2(75, 15))) weapons = 6;
	ImGui::Columns(2, nullptr, false);

	const char* hitboxes[] = { "head", "upper chest", "chest", "body", "arms", "stomach", "pelvis", "foots" };
	static bool hitbox[8];

	ImGui::Checkbox("enable aimbot", &SETTINGS::settings.aim_bool);

	std::vector<std::string> vec;
	static  std::string preview = "";
	if (ImGui::BeginCombo("hitboxes", preview.c_str()))
	{
		preview = "";
		for (size_t i = SETTINGS::settings.hitboxes; i < IM_ARRAYSIZE(hitboxes); i++) {
			ImGui::Selectable(hitboxes[i], &hitbox[i], ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
			if (hitbox[i])
				vec.push_back(hitboxes[i]);
		}
		for (size_t i = SETTINGS::settings.hitboxes; i < vec.size(); i++) {
			if (vec.size() == 1)
				preview += vec.at(i);
			else if (!(i == vec.size() - 1))
				preview += vec.at(i) + ", ";
			else
				preview += vec.at(i);
		}
		ImGui::EndCombo();
	}
	switch (weapons)
	{
	case 0: 
		ImGui::SliderInt("hit-chance", &SETTINGS::settings.auto_hitchance, 0, 100);
		ImGui::SliderInt("min-damage", &SETTINGS::settings.auto_mindamage, 1, 100);
		break;
	
	case 1: 
		ImGui::SliderInt("hit-chance", &SETTINGS::settings.scout_hitchance, 0, 100);
		ImGui::SliderInt("min-damage", &SETTINGS::settings.scout_mindamage, 1, 100);
		break;
	
	case 2: 
		ImGui::SliderInt("hit-chance", &SETTINGS::settings.awp_hitchance, 0, 100);
		ImGui::SliderInt("min-damage", &SETTINGS::settings.awp_mindamage, 1, 120);
		break;
	
	case 3: 
		ImGui::SliderInt("hit-chance", &SETTINGS::settings.heavy_pistol_hitchance, 0, 100);
		ImGui::SliderInt("min-damage", &SETTINGS::settings.heavy_pistol_mindamage, 1, 100);
		break;
	
	case 4: 
		ImGui::SliderInt("hit-chance", &SETTINGS::settings.pistol_hitchance, 0, 100);
		ImGui::SliderInt("min-damage", &SETTINGS::settings.pistol_mindamage, 1, 100);
		break;
	
	case 5: 
		ImGui::SliderInt("hit-chance", &SETTINGS::settings.rifle_hitchance, 0, 100);
		ImGui::SliderInt("min-damage", &SETTINGS::settings.rifle_mindamage, 1, 100);
		break;
	
	case 6: 
		ImGui::SliderInt("hit-chance", &SETTINGS::settings.other_hitchance, 0, 100);
		ImGui::SliderInt("min-damage", &SETTINGS::settings.other_mindamage, 1, 100);
		break;
	
	}
	ImGui::Checkbox(("multipoint"), &SETTINGS::settings.multi_bool);
	if (SETTINGS::settings.multi_bool) {
		ImGui::SliderFloat("head-scale", &SETTINGS::settings.point_val, 0, 1);
		ImGui::SliderFloat("body-scale", &SETTINGS::settings.body_val, 0, 1);
	}
	ImGui::Checkbox(("auto-stop"), &SETTINGS::settings.auto_stop);
	ImGui::Checkbox(("fakelag prediction"), &SETTINGS::settings.fake_bool);

	ImGui::NextColumn();

	ImGui::Checkbox(("slow-walk"), &SETTINGS::settings.slow_walk);
	if (SETTINGS::settings.slow_walk)
		ImGui::Hotkey(("##slow_walk_bind"), &SETTINGS::settings.slow_walk_key);
	ImGui::Checkbox(("fast-duck"), &SETTINGS::settings.remove_duck_cooldown);
	ImGui::Checkbox(("auto-zeus"), &SETTINGS::settings.autozeus);
	ImGui::Checkbox(("auto-scope"), &SETTINGS::settings.auto_scope);
	ImGui::Checkbox(("auto-revolver"), &SETTINGS::settings.auto_revolver);
	ImGui::Checkbox(("resolver"), &SETTINGS::settings.resolver);
}

void Menu::AntiAimTab()
{
	const char* pitch[] = { " none", " emotion", " minimal", " fake down" };
	const char* yaw[] = { " none", " backwards", " backwards jitter", " 180z", " keybased", " freestanding desync" };

	ImGui::Columns(2, nullptr, false);

	ImGui::Checkbox(("enable anti-aim"), &SETTINGS::settings.aa_bool);
	ImGui::Combo(("pitch"), &SETTINGS::settings.aa_pitch_type, pitch, ARRAYSIZE(pitch));
	ImGui::Combo("yaw", &SETTINGS::settings.aa_real_type, yaw, ARRAYSIZE(yaw));

	ImGui::Spacing();

	ImGui::Checkbox("desync anti-aim", &SETTINGS::settings.desync_aa);
	ImGui::SliderFloat("", &SETTINGS::settings.desync_range, 0, 58);

	ImGui::Spacing();

	ImGui::Checkbox(("disable thirdperson on nade"), &SETTINGS::settings.disable_tp_on_nade);

	ImGui::NextColumn();

	if (SETTINGS::settings.aa_real_type == 4)
	{
		ImGui::Hotkey(("left"), &SETTINGS::settings.aa_left);
		ImGui::Hotkey(("right"), &SETTINGS::settings.aa_right);
		ImGui::Hotkey(("back"), &SETTINGS::settings.aa_back);
	}
}

void Menu::VisualsTab()
{
	const char* chams[] = { " none", " metallic", " normal" };

	ImGui::Columns(2, nullptr, false);

	ImGui::Checkbox(("enable visuals"), &SETTINGS::settings.esp_bool);
	ImGui::Checkbox(("draw box"), &SETTINGS::settings.box_bool);
	ImGui::Checkbox(("draw player name"), &SETTINGS::settings.name_bool);
	ImGui::Checkbox(("draw player weapon"), &SETTINGS::settings.weap_bool);
	ImGui::Checkbox(("draw player health"), &SETTINGS::settings.health_bool);
	ImGui::Checkbox(("draw event logs"), &SETTINGS::settings.info_bool);
	ImGui::Checkbox(("draw ammo"), &SETTINGS::settings.ammo_bool);
	ImGui::Checkbox(("draw fov arrows"), &SETTINGS::settings.fov_bool);
	ImGui::SameLine();
	ImGui::ColorEdit4(("##fov_arrows"), SETTINGS::settings.arrows_col, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar);
	ImGui::Checkbox(("draw radar"), &SETTINGS::settings.radar);
	ImGui::Combo("chams", &SETTINGS::settings.chams_type, chams, IM_ARRAYSIZE(chams), 3);
	ImGui::SameLine();
	ImGui::ColorEdit4(("##chams3"), SETTINGS::settings.invisible_col, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar);
	ImGui::ColorEdit4(("##chams1"), SETTINGS::settings.vmodel_col, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar);
	ImGui::Checkbox(("desync chams"), &SETTINGS::settings.desync_chams);
	ImGui::SameLine();
	ImGui::ColorEdit4(("##desync"), SETTINGS::settings.desync_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar);
	ImGui::Checkbox(("enemy glow"), &SETTINGS::settings.glowenable);
	ImGui::SameLine();
	ImGui::ColorEdit4(("##glow"), SETTINGS::settings.glow_col, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar);

	ImGui::NextColumn();

	ImGui::Checkbox(("bullet tracers"), &SETTINGS::settings.beam_bool);
	ImGui::SameLine();
	ImGui::ColorEdit4(("##tracer"), SETTINGS::settings.bulletlocal_col, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar);
	ImGui::Checkbox(("bullet impact"), &SETTINGS::settings.bullet_impact);
	ImGui::Checkbox(("thirdperson"), &SETTINGS::settings.tp_bool);
	if (SETTINGS::settings.tp_bool)
		ImGui::Hotkey("##tp_key", &SETTINGS::settings.thirdperson_int);
	ImGui::Checkbox(("remove smoke"), &SETTINGS::settings.smoke_bool);
	ImGui::Checkbox(("remove flash"), &SETTINGS::settings.no_flash);
	ImGui::Checkbox(("remove scope"), &SETTINGS::settings.scope_bool);
	ImGui::Checkbox(("remove zoom"), &SETTINGS::settings.removescoping);
	ImGui::Checkbox(("fix sensitivity"), &SETTINGS::settings.fixscopesens);
	ImGui::Checkbox(("static crosshair"), &SETTINGS::settings.static_crosshair);
	ImGui::Checkbox(("force radgoll"), &SETTINGS::settings.force_ragdoll);
	ImGui::SliderFloat("render fov", &SETTINGS::settings.fov_val, 0, 179);
	ImGui::SliderFloat("viewmodel fov", &SETTINGS::settings.viewfov_val, 0, 179);
	ImGui::Checkbox(("lag comp hitbox"), &SETTINGS::settings.lag_comp_hitbox);
	ImGui::Checkbox(("asus walls"), &SETTINGS::settings.asus_bool);
	ImGui::Checkbox(("nightmode"), &SETTINGS::settings.night_bool);
}

void Menu::MiscTab()
{
	const char* fakelag_mode[] = { "factor", "adaptive" };
	const char* hit_sound[] = { "none", "metallic", "click", "bell", "bubble", "stapler", "custom" };

	ImGui::Columns(2, nullptr, false);

	ImGui::Checkbox(("anti-untrusted"), &SETTINGS::settings.anti_untrusted);

	ImGui::Spacing();

	ImGui::Checkbox(("bunnyhop"), &SETTINGS::settings.bhop_bool);
	ImGui::Checkbox(("auto-strafe"), &SETTINGS::settings.strafe_bool);
	ImGui::Checkbox(("chat-spam"), &SETTINGS::settings.misc_chat_spam);
	ImGui::Checkbox(("clan-tag"), &SETTINGS::settings.misc_clantag);
	ImGui::Combo(("hitsound"), &SETTINGS::settings.hit_sound, hit_sound, ARRAYSIZE(hit_sound));
	ImGui::Checkbox(("enable fakelag"), &SETTINGS::settings.lag_bool);
	ImGui::SliderInt("choke amount", &SETTINGS::settings.fake_lag_choke, 1, 16);

	ImGui::NextColumn();

	getconfig();
	ImGui::Spacing();
	if (ImGui::Combo("cfg", &selectedcfg, [](void* data, int idx, const char** out_text)
		{
			*out_text = configs[idx].c_str();
			return true;
		}, nullptr, configs.size(), 10))
	{
		cfgname = configs[selectedcfg];
		cfgname.erase(cfgname.length() - 4, 4);
		strcpy(SETTINGS::settings.config_name, cfgname.c_str());
	}

		ImGui::Spacing();

		if (ImGui::Button("save"))
		{
			Config->Save();
		}
		ImGui::SameLine();
		if (ImGui::Button("load"))
		{
			Config->Load();

		}

		ImGui::Spacing();
		ImGui::InputText("name", SETTINGS::settings.config_name, 52);
		if (ImGui::Button("create", ImVec2(-1, 15)))
			Config->Save();
		if (ImGui::Button("reset settings", ImVec2(-1, 15))) 
			Config->setup();
}

void Menu::SkinsTab()
{
	static int tab1 = 1;
	ImGui::Columns(2, nullptr, false);

	ImGui::Checkbox("enable", &SETTINGS::settings.skinenabled);

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	static int currWeap = SETTINGS::settings.currentWeapon;
	int pk = SETTINGS::settings.W[SETTINGS::settings.currentWeapon].ChangerSkin;
	static int currWeapSkin = SETTINGS::settings.W[SETTINGS::settings.currentWeapon].ChangerSkin;
	static char weapName[18];
	static char filterSkins[32];
	static char filterGuns[32];

	ImGui::InputText(("search"), filterGuns, IM_ARRAYSIZE(filterGuns));
	ImGui::BeginChild(5, ImVec2(-1, 250), true, 0);
	for (auto it : guns)
	{
		if (strcmp(it.second, " default") == 0)
			continue;

		if (!(std::string(it.second).find(std::string(filterGuns)) != std::string::npos))
			continue;

		const bool item_selected = ((int)it.first == (int)SETTINGS::settings.currentWeapon);

		if (ImGui::Selectable(it.second, item_selected))
		{
			SETTINGS::settings.currentWeapon = (int)it.first;
		}
	}
	ImGui::EndChild();
	//ImGui::ListBoxCombo("##knifes", ImVec2(150, 250), &SETTINGS::settings.knifemodel, (void*)knives, IM_ARRAYSIZE(knives), false);

	ImGui::NextColumn();
	std::string skinName = weaponnames(SETTINGS::settings.currentWeapon);
	ImGui::InputText(("search##1"), filterSkins, IM_ARRAYSIZE(filterSkins));
	ImGui::InputInt(("custom id"), &SETTINGS::settings.W[SETTINGS::settings.currentWeapon].ChangerSkin);
	ImGui::BeginChild(("##skins"), ImVec2(-1, 233), true, 0);
	std::string skinStr = "";
	int curItem = -1;
	int s = 0;
	for (auto it : weapon_skins)
	{
		if (!(std::string(it.second).find(std::string(filterSkins)) != std::string::npos))
			continue;

		bool selected = ((int)it.first == (int)SETTINGS::settings.W[SETTINGS::settings.currentWeapon].ChangerSkin);

		if (ImGui::Selectable(it.second, selected))
		{
			SETTINGS::settings.W[SETTINGS::settings.currentWeapon].ChangerSkin = (int)it.first;

		}
	}
	ImGui::EndChild();
}

void Menu::RenderMenu() {
	ImGui::SetNextWindowSize(ImVec2(606, 355), ImGuiCond_FirstUseEver);
	ImGui::Begin("Nexus", nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
	{
		ImVec2 p = ImGui::GetCursorScreenPos();
		ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(p.x - 5, p.y - 8), ImVec2(p.x + ImGui::GetWindowWidth() / 2 - 5, p.y - 5), ImColor(177, 0, 0), ImColor(177, 0, 0), ImColor(177, 0, 0), ImColor(177, 0, 0));
		ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(p.x + ImGui::GetWindowWidth() / 2 - 5, p.y - 8), ImVec2(p.x + ImGui::GetWindowWidth() - 5, p.y - 5), ImColor(177, 0, 0), ImColor(177, 0, 0), ImColor(177, 0, 0), ImColor(177, 0, 0));

		ImGui::Text("Nexus");


		ImGui::BeginChild("tabs", ImVec2(0, 20), false);
		{
			if (ImGui::Button("Aimbot", ImVec2(ImGui::GetWindowWidth() / 5 - 7, 20))) page = 0;
			ImGui::SameLine();
			if (ImGui::Button("Anti-Aim", ImVec2(ImGui::GetWindowWidth() / 5 - 7, 20))) page = 1;
			ImGui::SameLine();
			if (ImGui::Button("Visuals", ImVec2(ImGui::GetWindowWidth() / 5 - 7, 20))) page = 2;
			ImGui::SameLine();
			if (ImGui::Button("Misc", ImVec2(ImGui::GetWindowWidth() / 5 - 7, 20))) page = 3;
			ImGui::SameLine();
			if (ImGui::Button("Skins", ImVec2(ImGui::GetWindowWidth() / 5 - 7, 20))) page = 4;
		}
		ImGui::EndChild();

		ImGui::BeginChild("main", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar);
		{
			ImGui::PushItemWidth(175);
			switch (page) {
			case 0:
				AimbotTab();
				break;
			case 1:
				AntiAimTab();
				break;
			case 2:
				VisualsTab();
				break;
			case 3:
				MiscTab();
				break;
			case 4:
				SkinsTab();
				break;
			}
			ImGui::PopItemWidth();
		}
		ImGui::EndChild();
	}
	ImGui::End();
}
Menu* pMenu = new Menu();