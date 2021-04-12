#pragma once

/*
global variables and other stuff that is probably too small to put in another
file (and i'm too lazy to make files for all of them) would go in here.
Stuff like fonts and shit
*/

namespace GLOBAL
{
	HWND csgo_hwnd = FindWindow(0, "Counter-Strike: Global Offensive");

	bool should_send_packet;
	bool is_fakewalking;
	int choke_amount;

	Vector real_angles;
	Vector fake_angles;
	Vector strafe_angle;

	int randomnumber;
	float flHurtTime;
	bool DisableAA;
	bool Aimbotting;
	Vector finaleye;
	Vector freestanding;
	Vector freestanding2;
	Vector freestanding3;
	Vector freestanding4;
	Vector recoilframe;

	Vector FakePosition;
	int ground_tickz;
	bool CircleStraferActive;
	SDK::CUserCmd* originalCMD;
	SDK::CUserCmd* cmd;

	bool disable_fakelag;

	bool           Shot[65];
	bool		   MissedShot;
	bool           Hit[65];
	int            MissedShots[65];

}
namespace FONTS
{
	unsigned int font_esp_flags;
	unsigned int font_indicators;
	unsigned int font_watermark;
	unsigned int font_logs;
	unsigned int visuals_esp_font;
	unsigned int font_esp_name;
	unsigned int visuals_lby_font;
	unsigned int font_weapon_icon;
	bool ShouldReloadFonts();
	void InitFonts();
}
namespace SETTINGS
{
	class Sticker_t
	{
	public:
		int   iID = 0;
		float flWear = 0.f;
		float flScale = 1.f;
		int   iRotation = 0;
	};
	class Weapon_tTT
	{
	public:
		int SkinsWeapon;
		int SkinsKnife;
		int VremennyiWeapon;
		int VremennyiKnife;
		int Stikers1;
		int Stikers2;
		int Stikers3;
		int Stikers4;
		float ChangerWear = 0;
		int ChangerStatTrak = 0;
		char ChangerName[32] = "";
		bool ChangerEnabled;
	};

	class CSettings
	{
	public:
		// returns true/false whether the function succeeds, usually returns false if file doesn't exist
		bool Save(std::string file_name);
		bool Load(std::string file_name);

		bool Remove(std::string file_name);

		void CreateConfig(std::string name); // creates a blank config

		std::vector<std::string> GetConfigs();

		bool skinenabled;
		int currentWeapon;
		int knifemodel;
		int Knife;

		bool menu_open = false;
		float menucolor[3] = { 51 / 255, 0 / 255, 204 / 255 };

		struct
		{
			bool ChangerEnabled = false;
			int ChangerSkin = 0;
			char ChangerName[32] = "";
			bool stat;
			int statamount;
			bool customstat;
			int customstatamount;
			char Name;
		} W[519];

		Weapon_tTT weapons[520];

		struct wpnz
		{
			bool  EnabledStickers;
			bool  EnabledSkins;
			int   ChangerSkin;
			char  ChangerName[32] = "";
			int   ChangerStatTrak;
			int   ChangerSeed;
			float ChangerWear;
			Sticker_t Stickers[5];
		}Weapons[519];

		bool fix;
		bool backaimhead;
		bool baim;
		int rage_lagcompensation_type = 0;
		bool rage_lagcompensation;
		bool force_ragdoll;
		bool CUSTOMMODEL;
		int customct;
		int customtt;

		int gloves;
		int skingloves;
		float glovewear;
		bool glovesenabled;
		bool rankchanger;
		int KnifeSkin;
		int AK47Skin;
		int GalilSkin;
		int M4A1SSkin;
		int M4A4Skin;
		int AUGSkin;
		int FAMASSkin;
		int AWPSkin;
		int SSG08Skin;
		bool fakelatency_enabled;
		bool fixshit;
		float fakelatency_amount;
		int SCAR20Skin;
		int P90Skin;
		int Mp7Skin;
		int NovaSkin;
		int UMP45Skin;
		int GlockSkin;
		int SawedSkin;
		int USPSkin;
		int MagSkin;
		int XmSkin;
		int DeagleSkin;
		int DualSkin;
		int FiveSkin;
		int RevolverSkin;
		int Mac10Skin;
		int tec9Skin;
		int Cz75Skin;
		int NegevSkin;
		int M249Skin;
		int Mp9Skin;
		int P2000Skin;
		int BizonSkin;
		int Sg553Skin;
		int P250Skin;
		int G3sg1Skin;


		struct Visualsxd
		{
			bool Enabled = true;
			bool Visuals_BoxEnabled = true;
			int Visuals_BoxType = 1;
			int Visuals_HealthBarType = 1;
			bool Visuals_Scoped = false;
			bool Visuals_AmmoESP = true;
			int Visuals_AmmoESPType = 1;

			bool Visuals_EspTeam = false;
			bool Visuals_VisableOnly = false;

			float BoxColorPickCTVIS[4] = { 1.0f,0.0f,0.0f,1.0f };
			float BoxColorPickCTINVIS[4] = { 1.0f,1.0f,0.0f,1.0f };


			float BoxColorPickTVIS[4] = { 0.0f,1.0f,0.0f,1.0f };
			float BoxColorPickTINVIS[4] = { 0.0f,1.0f,1.0f,1.0f };


			float LagCompHitboxes[4] = { 0.0f,1.0f,0.0f,1.0f };



			float SkeletonCTVIS[4] = { 1.0f,0.0f,0.0f,1.0f };
			float SkeletonCTINVIS[4] = { 1.0f,1.0f,0.0f,1.0f };

			float SkeletonTVIS[4] = { 0.0f,1.0f,0.0f,1.0f };
			float SkeletonTINVIS[4] = { 0.0f,1.0f,1.0f,1.0f };




			float CTChamsVisable[4] = { 1.0f,0.0f,0.0f,1.0f };

			float TChamsVisable[4] = { 1.0f,1.0f,0.0f,1.0f };


			float CTChamsInVisable[4] = { 0.0f,1.0f,0.0f,1.0f };
			float TChamsInVisable[4] = { 1.0f,1.0f,0.0f,1.0f };

			float beamtrace[4] = { 1.0f,0.0f,0.0f,1.0f };
			int thirdkey;
			int Hitsound;
			bool tp_bool;
			bool Monitor;
			bool Visuals_HealthBar = true;
			bool Visuals_Name = true;
			bool Visuals_ArmorBar = false;
			bool Visuals_Flashed = false;
			bool Visuals_Defuser = false;
			bool Visuals_Weapons = false;
			int Visuals_WeaponsType = 0;
			bool Visuals_AimLines = false;
			bool Visuals_Skeltal = false;
			bool Visuals_EngineRadar = false;
			bool Visuals_DrawLinesAA = false;
			bool Visuals_DroppedWeapons = false;
			bool Visuals_NoRecoil = false;
			bool Visuals_NoFlash = false;
			bool Visuals_NoSmoke = false;
			bool Visuals_NoSmokeVar = false;
			bool Visuals_NoScope = false;
			bool Visuals_NoZoom = false;
			bool Visuals_NoPostProcess = false;


			bool Visuals_Chams = true;
			bool Visuals_ChamsTeam = false;
			bool Visuals_ChamsXQZ = false;
			int  Visuals_ChamsMaterial = 1;
			bool Visuals_GhostAngle = false;
			bool Visuals_ChamsGuns = false;
			bool Visuals_Crosshair = false;
			bool Visuals_CrosshairDynamic = false;


			float CTGlowVisable[4] = { 1.f,1.0f,1.0f,1.0f };
			float TGlowVisable[4] = { 1.f,1.0f,1.0f,1.0f };
			float CTGlowInVisable[4] = { 1.f,1.0f,1.0f,1.0f };
			float TGlowInVisable[4] = { 1.f,1.0f,1.0f,1.0f };

			bool Visuals_Hitmarker = false;
			bool Visuals_Spread = false;
			bool Visuals_GrenadePrediction = false;
			bool Visuals_DamageESP = false;
			bool Visuals_C4 = false;
			bool eventlog_enabled = false;
			float SpreadColor[4] = { 1.0f,0.0f,0.0f,1.0f };

			float DamageESPColor[4] = { 1.0f,0.0f,0.0f,1.0f };
			bool Vis_Glow = true;
			bool Vis_Glow_Team = false;
			bool Vis_Glow_Vis = false;


			bool Visuals_NightMode = false;
			bool Visuals_Asus = false;

			bool Visuals_DrawBeams = false;
			float Visuals_DrawBeamsDuration = 3.f;
			float Visuals_DrawBeamsSize = 3.f;

			bool Visuals_DrawCapsules = false;
			bool Visuals_DrawEventLog = false;
			float capsulecolor[4] = { 1.0,0.0f,0.0f,1.0f };
			float Visuals_DrawCapsulesDuration = 5.f;
			int Visuals_Spread_Type = 0;
			struct
			{
				bool Enabled = false;
				float Distance = 800.f;
				int type = 0;
				float Radius = 10.f;
				bool Animated = false;
				bool visonly = true;
			}SoundESP;
			struct
			{
				struct
				{
					bool	ExternalRadar;
					int		RadarStyle;
					float	RadarDistance;
					bool	RadarVisibleOnly;
					bool	RadarSmoke;
				}Radar;
				bool Monitor = false;
			}Panels;

			bool lbyIndicator = false;
			bool strelkiIndicator = false;
		}Visuals;

		int auto_mindamage = 0;
		int auto_hitchance = 0;

		int scout_mindamage = 0;
		int scout_hitchance = 0;

		int awp_mindamage = 0;
		int awp_hitchance = 0;

		int heavy_pistol_mindamage = 0;
		int heavy_pistol_hitchance = 0;

		int pistol_mindamage = 0;
		int pistol_hitchance = 0;

		int rifle_mindamage = 0;
		int rifle_hitchance = 0;

		int other_mindamage = 0;
		int other_hitchance = 0;

		bool friendfire;
		bool bhop_bool;
		bool strafe_bool;
		bool esp_bool = false;
		bool desync_chams = false;
		int chams_type;
		int xhair_type;
		bool tp_bool;
		bool aim_bool;
		bool limbsvel = false;
		float foot, head, neck, body, calf, arm, hand, legs, chest, stomach, pelvis, arms;
		bool mfoot, mhead, mneck, mbody, mcalf, marm, mhand = false;
		bool shoot_bool;
		bool bhead, bneck, bchest, bstomach, bpelvis, barms, blegs;
		int hitbox = 0;
		bool head_hitbox;
		bool hitscan_head;
		bool hitscan_neck;
		bool hitscan_chest;
		bool hitscan_pelvis;
		bool hitscan_arms;
		bool hitscan_legs;
		bool no_flash;
		bool mmoving = false;
		bool limbsifwalking;
		float pointlimit;
		int hitscan = 0;
		bool autopoint;
		int hit_sound;
		char hit_sound_custom[52];
		int aim_type;
		bool aa_bool;
		bool aa_jitter;
		bool aa_jitter1;
		bool aa_jitter2;
		float jitter_range;
		float jitter_range1;
		float jitter_range2;
		int aa_left;
		char config_name[52];
		int aa_right;
		int aa_back;
		int aa_auto;
		bool disable_tp_on_nade;
		bool auto_scope;
		int aa_pitch;
		bool preserve_killfeed;
		bool anti_untrusted = true;
		int aa_type;
		int hitboxes;
		bool up_bool;
		bool misc_bool;
		int config_sel;
		bool beam_bool;
		bool stop_bool;
		bool night_bool;
		bool box_bool;
		bool name_bool;
		bool weap_bool;
		int weapon_esp_type;
		bool health_bool;
		bool info_bool;
		bool back_bool;
		bool lag_bool;
		bool bomb_bool;
		bool inaccuracy_bool;
		bool lag_comp_hitbox;
		bool multi_bool = true;
		bool fakefix_bool;
		bool angle_bool;
		bool autoslow;
		bool tp_angle_bool;
		bool glow_bool;
		bool dist_bool;
		bool fov_bool;
		bool smoke_bool;
		bool scope_bool;
		bool predict_bool;
		bool fake_bool;
		bool asus_bool;
		bool radar;
		int media_type;
		bool novis_bool;
		bool localglow_bool;
		bool duck_bool;
		bool autozeus;
		bool autoknife;
		bool fake_crouch;
		bool crouch_peak;
		bool auto_revolver;
		bool money_bool;
		bool delay_shot;
		int lag_type = 2;
		bool chams_bool;
		bool xqz;
		bool resolver;
		bool ammo_bool;
		int spread_bool;


		float spreadcirclecol[4] = { 1.0f,0.0f,0.0f,0.3f };

		int fake_lag_choke;
		float fake_walk_speed = 45;

		bool debug_bool;


		float vmodel_col[3] = { 1.f, 1.f, 1.f };
		float invisible_col[3] = { 1.f, 1.f, 1.f };;
		float visiblemodel_col[3];

		float imodel_col[4] = { 0.0f,1.0f,0.0f,0.5f };

		float desync_color[3] = { 1.f, 0.f, 0.f };

		CColor box_col;
		CColor barrel_col;
		CColor name_col;
		CColor weapon_col;
		CColor distance_col;


		float localchams_col[4] = { 1.0f,0.0f,1.0f,0.5f };
		float grenadepredline_col[4] = { 1.0f,0.0f,1.0f,0.5f };

		float bulletlocal_col[3] = { 1.f, 1.f, 1.f };
		float bulletenemy_col[4] = { 0.0f,1.0f,1.0f,1.0f };
		float bulletteam_col[4]{ 1.0f,1.0f,1.0f,1.0f };

		float bulletlife = 3.0f;
		float bulletsize = 2.0f;

		CColor menu_col = CColor(60, 60, 60);
		CColor checkbox_col = CColor(5, 135, 5);
		CColor slider_col = CColor(5, 135, 5);
		CColor tab_col = CColor(91, 91, 91);

		float glow_col[3] = { 1.f, 1.f, 1.f };

		float glowlocal_col[4] = { 0.3f,0.3f,0.9f,0.7f };

		float arrows_col[3] = { 1.f, 1.f, 1.f };

		CColor fov_col;

		int chance_val;
		int damage_val;
		float delta_val;
		float point_val;
		float desync_range;
		float body_val;
		bool misc_clantag;
		bool misc_chat_spam;
		char custom_chat_spam[128];
		bool localschams;
		bool localesp;
		int localchams;
		float fov_val = 90;
		float viewfov_val = 90;

		bool fakechams;
		float fakechamscol[4] = { 1.0f,1.0f,1.0f,1.0f };


		bool flip_bool;
		int aa_side;

		bool legit_bool;
		int legit_key;
		bool rcs_bool;
		float legitfov_val;
		int legitbone_int;
		float rcsamount_min;
		float rcsamount_max;
		float legitaim_val;
		bool legittrigger_bool;
		int legittrigger_key;

		int thirdperson_int;
		int flip_int;

		bool glowenable;
		int glowstyle;
		bool glowlocal;
		int glowstylelocal;
		int hitmarker_val;
		bool aa_auto_bool;
		bool desync_aa;
		int aa_mode;

		int aa_real_type;
		int aa_real1_type;
		int aa_real2_type;

		int aa_fake_type;
		int aa_fake1_type;
		int aa_fake2_type;

		int aa_pitch_type;
		int aa_pitch1_type;
		int aa_pitch2_type;

		float aa_realadditive_val;
		float aa_fakeadditive_val;

		float aa_realadditive1_val;
		float aa_fakeadditive1_val;
		float delta1_val;

		float aa_realadditive2_val;
		float aa_fakeadditive2_val;
		float delta2_val;

		float spinangle;
		float spinspeed;

		float spinangle1;
		float spinspeed1;

		float spinangle2;
		float spinspeed2;

		float spinanglefake;
		float spinspeedfake;

		float spinanglefake1;
		float spinspeedfake1;

		float spinanglefake2;
		float spinspeedfake2;

		bool lbyflickup;
		bool lbyflickup1;
		bool lbyflickup2;

		bool aa_fakeangchams_bool;

		int chamstype;
		float fov_time;
		bool rifk_arrow;

		int glowteamselection;
		bool glowteam;

		int chamsteamselection;
		int chamsteam;

		int espteamselection;
		int espteamcolourselection;
		bool boxteam;
		bool nameteam;
		bool weaponteam;
		bool flagsteam;
		bool healthteam;
		bool moneyteam;
		bool ammoteam;
		bool arrowteam;
		CColor boxteam_col;
		CColor nameteam_col;
		CColor weaponteam_col;
		CColor arrowteam_col;


		float teamvis_color[4] = { 1.0f,0.5f,0.5f,0.5f };
		float teaminvis_color[4] = { 1.0f,1.0f,0.5f,0.5f };






		float teamglow_color[4] = { 1.0f,1.0f,0.5f,0.5f };

		bool matpostprocessenable;
		bool removescoping;
		bool fixscopesens;
		bool static_crosshair;

		int quickstopkey;
		bool stop_flip;
		bool chamsmetallic;
		int flashlightkey;
		int overridekey;
		int autostopmethod;
		bool override;
		bool lbyenable;
		int circlestrafekey;
		float circlstraferetract;
		bool slow_walk;
		bool auto_stop;
		int slow_walk_key;
		int auto_stop_key;
		bool remove_duck_cooldown;
		bool bullet_impact;
		float daytimevalue = 98;
		float beam_time;
		CColor impact_colour;
		CColor nonhit_beam_clour;
		float circlemin;
		float circlemax;
		float circlevel;
		float circlenormalizemultiplier;

		bool skinchangerenable;

		int knifeToUse;
		int bayonetID, karambitID, flipID, gutID, m9ID, huntsmanID;
		int gloveToUse;
		int bloodhoundID, driverID, handwrapsID, motoID, specialistID, sportID, hydraID;

		int uspID, p2000ID, glockID, dualberettaID, p250ID, fivesevenID, tech9ID, r8ID, deagleID;
		int novaID, xm1014ID, mag7ID, sawedoffID, m249ID, negevID;
		int mp9ID, mac10ID, mp7ID, ump45ID, p90ID, ppbizonID;
		int famasID, galilID, ak47ID, m4a4ID, m4a1sID, ssg08ID, augID, sg553ID, awpID, scar20ID, g3sg1ID;


	private:
	}; CSettings settings;
}

bool using_fake_angles[65];
bool full_choke;
bool is_shooting;

bool in_tp;
bool fake_walk;

int resolve_type[65];

int target;
int shots_fired[65];
int shots_hit[65];
int shots_missed[65];
int backtrack_missed[65];

bool didShot;
bool didMiss;

float tick_to_back[65];
float lby_to_back[65];
bool backtrack_tick[65];

float lby_delta;
float update_time[65];
float walking_time[65];

float local_update;

int hitmarker_time;
int random_number;

bool menu_hide;

int oldest_tick[65];
float compensate[65][12];
Vector backtrack_hitbox[65][20][12];
float backtrack_simtime[65][12];
namespace gat {
	float get_absolute_time();
}