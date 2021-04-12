#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <ShlObj_core.h>
#include "../../includes.h"
#include "../../imgui/imgui.h"
#include "../../imgui/imgui_internal.h"
#include "../../SDK/CBaseEntity.h"
#include "../configs/config.h"
#include "../../UTILS/variables.h"
static int selectedcfg = 0;
static std::string cfgname = " default";

std::vector<std::string> configs;
class Menu {
public:
	void AimbotTab();
	void AntiAimTab();
	void VisualsTab();
	void MiscTab();
	void SkinsTab();
	void RenderMenu();
};

extern Menu* pMenu;
void getconfig()
{
	//get all files on folder

	configs.clear();

	static char path[MAX_PATH];
	std::string szPath1;

	if (!SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_MYDOCUMENTS, NULL, 0, path)))
		return;

	szPath1 = std::string(path) + "\\Nexus\\*";


	WIN32_FIND_DATA FindFileData;
	HANDLE hf;

	configs.push_back(" choose config");

	hf = FindFirstFile(szPath1.c_str(), &FindFileData);
	if (hf != INVALID_HANDLE_VALUE) {
		do {
			std::string filename = FindFileData.cFileName;

			if (filename == ".")
				continue;

			if (filename == "..")
				continue;

			if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				if (filename.find(".cfg") != std::string::npos)
				{
					configs.push_back(std::string(filename));
				}
			}
		} while (FindNextFile(hf, &FindFileData) != 0);
		FindClose(hf);
	}
}

static int page = 0;

const char* knives[] =
{
	" default",
	" bayonet",
	" flip knife",
	" gut knife",
	" karambit",
	" m9 bayonet",
	" huntsman knife",
	" falchion knife",
	" bowie knife",
	" butterfly knife",
	" shadow daggers",
	" talon knife",
	" navaja knife",
	" stiletto knife",
	" ursus knife",
	" gold knife"
};

std::vector<std::pair<int, const char*>> weapon_skins =
{
{ 0,   "default" },
{ 692, "night riot" },
{ 693 , "flame test" },
{ 697 , "black sand" },
{ 700 , "urban hazard (p2000)" },
{ 701 , "grip" },
{ 702 , "aloha" },
{ 645 , "oxide blaze" },
{ 694 , "moonrise" },
{ 698 , "lionfish" },
{ 699 , "wild six" },
{ 703 , "swag - 7" },
{ 704 , "arctic wolf" },
{ 690 , "stymphalian" },
{ 691 , "mortis" },
{ 705 , "cortex" },
{ 695 , "neo - noir" },
{ 696, "bloodsport (mp7)" },
{ 361, "abyss" },
{ 253, "acid fade" },
{ 598, "aerial" },
{ 154, "afterimage" },
{ 609, "airlock" },
{ 455, "akihabara accept" },
{ 523, "amber fade (revolver)" },
{ 246, "amber fade" },
{ 210, "anodized gunmetal" },
{ 197, "anodized navy" },
{ 28,  "anodized navy" },
{ 286, "antique (nova)" },
{ 306, "antique (pp bizon)" },
{ 460, "aqua terrace" },
{ 474, "aquamarine revenge" },
{ 6,   "arctic camo" },
{ 583, "aristocrat" },
{ 423, "armor core" },
{ 242, "army mesh" },
{ 245, "army recon" },
{ 298, "army sheen" },
{ 234, "ash wood" },
{ 279, "asiimov (awp)" },
{ 255, "asiimov (m4a4)" },
{ 551, "asiimov (p250)" },
{ 359, "asiimov (p90)" },
{ 442, "asterion" },
{ 553, "atlas" },
{ 301, "atomic alloy" },
{ 573, "autotronic (bayonet)" },
{ 574, "autotronic (flip)" },
{ 575, "autotronic (gut)" },
{ 576, "autotronic (karambit)" },
{ 577, "autotronic (m9 bayonet)" },
{ 520, "avalanche" },
{ 229, "azure zebra" },
{ 174, "boom" },
{ 459, "bamboo forest" },
{ 457, "bamboo print" },
{ 458, "bamboo shadow" },
{ 383, "basilisk" },
{ 9,   "bengal tiger" },
{ 503, "big iron" },
{ 549, "bioleak" },
{ 172, "black laminate (ak-47)" },
{ 563, "black laminate (bayonet)" },
{ 564, "black laminate (flip)" },
{ 565, "black laminate (gut)" },
{ 566, "black laminate (karambit)" },
{ 567, "black laminate (m9 bayonet)" },
{ 190, "black limba" },
{ 629, "black sand" },
{ 557, "black tie" },
{ 166, "blaze orange" },
{ 37,  "blaze" },
{ 228, "blind spot" },
{ 75,  "blizzard marbleized" },
{ 217, "blood tiger" },
{ 222, "blood in the water" },
{ 597, "bloodsport" },
{ 62,  "bloomstick" },
{ 278, "blue fissure" },
{ 226, "blue laminate" },
{ 96,  "blue spruce" },
{ 42,  "blue steel" },
{ 13,  "blue streak" },
{ 216, "blue titanium" },
{ 370, "bone machine" },
{ 27,  "bone mask" },
{ 193, "bone pile" },
{ 77,  "boreal forest" },
{ 159, "brass" },
{ 317, "bratatat" },
{ 330, "briar" },
{ 615, "briefing" },
{ 578, "bright water (knife)" },
{ 579, "bright water (knife)" },
{ 189, "bright water (m4a1-s)" },
{ 425, "bronze deco" },
{ 39,  "bulldozer" },
{ 155, "bullet rain" },
{ 479, "bunsen burner" },
{ 364, "business class" },
{ 632, "buzz kill" },
{ 299, "caged steel" },
{ 339, "caiman" },
{ 240, "calicamo" },
{ 3,   "candy apple" },
{ 93,  "caramel" },
{ 70,  "carbon fiber" },
{ 391, "cardiac" },
{ 589, "carnivore" },
{ 388, "cartel" },
{ 394, "cartel" },
{ 528, "cartel" },
{ 44,  "case hardened" },
{ 399, "catacombs" },
{ 379, "cerberus" },
{ 327, "chainmail" },
{ 325, "chalice" },
{ 280, "chameleon" },
{ 548, "chantico's fire" },
{ 398, "chatterbox" },
{ 376, "chemical green" },
{ 593, "chopper" },
{ 438, "chronos" },
{ 627, "cirrus" },
{ 346, "coach class" },
{ 499, "cobalt core" },
{ 231, "cobalt disruption" },
{ 267, "cobalt halftone" },
{ 249, "cobalt quartz" },
{ 67,  "cold blooded" },
{ 47,  "colony" },
{ 343, "commuter" },
{ 110, "condemned" },
{ 351, "conspiracy" },
{ 373, "contamination" },
{ 46,  "contractor" },
{ 22,  "contrast spray" },
{ 274, "copper galaxy" },
{ 41,  "copper" },
{ 10,  "copperhead" },
{ 509, "corinthian" },
{ 281, "corporal" },
{ 181, "corticera (awp)" },
{ 184, "corticera (p2000)" },
{ 462, "counter terrace" },
{ 466, "crimson kimono" },
{ 12,  "crimson web (deagle)" },
{ 232, "crimson web" },
{ 310, "curse" },
{ 92,  "cyanospatter" },
{ 360, "cyrex (m4a1-s)" },
{ 312, "cyrex (scar-20)" },
{ 487, "cyrex (sg 553)" },
{ 637, "cyrex (usp)" },
{ 444, "daedalus" },
{ 411, "damascus steel (knife)" },
{ 410, "damascus steel (knife)" },
{ 247, "damascus steel (sg 553)" },
{ 329, "dark age" },
{ 60,  "dark water" },
{ 386, "dart" },
{ 471, "daybreak" },
{ 610, "dazzle" },
{ 403, "deadly poison" },
{ 293, "death rattle" },
{ 156, "death by kitty" },
{ 392, "delusion" },
{ 195, "demeter" },
{ 153, "demolition" },
{ 8,   "desert storm" },
{ 311, "desert warfare" },
{ 336, "desert-strike" },
{ 355, "desert-strike" },
{ 588, "desolate space" },
{ 319, "detour" },
{ 603, "directive" },
{ 429, "djinn" },
{ 178, "doomkitty" },
{ 417, "doppler (black pearl)" },
{ 418, "doppler (phase 1)" },
{ 419, "doppler (phase 2)" },
{ 420, "doppler (phase 3)" },
{ 421, "doppler (phase 4)" },
{ 415, "doppler (ruby)" },
{ 416, "doppler (sapphire)" },
{ 400, "?? dragon king" },
{ 344, "dragon lore" },
{ 48,  "dragon tattoo" },
{ 624, "dragonfire" },
{ 199, "dry season" },
{ 491, "dualing dragons" },
{ 447, "duelist" },
{ 428, "eco" },
{ 227, "electric hive" },
{ 422, "elite build (ak-47)" },
{ 525, "elite build (awp)" },
{ 486, "elite build (p90)" },
{ 182, "emerald dragon" },
{ 300, "emerald pinstripe" },
{ 196, "emerald" },
{ 453, "emerald" },
{ 480, "evil daimyo" },
{ 590, "exo" },
{ 207, "facets" },
{ 522, "fade (revolver)" },
{ 38,  "fade" },
{ 176, "faded zebra" },
{ 169, "fallout warning" },
{ 378, "fallout warning" },
{ 389, "fire elemental" },
{ 180, "fire serpent" },
{ 546, "firefight" },
{ 385, "firestarter" },
{ 341, "first class" },
{ 345, "first class" },
{ 631, "flashback" },
{ 541, "fleet flock" },
{ 493, "flux" },
{ 5,   "forest ddpat" },
{ 25,  "forest leaves" },
{ 78,  "forest night" },
{ 352, "fowl play" },
{ 295, "franklin" },
{ 580, "freehand" },
{ 581, "freehand" },
{ 582, "freehand" },
{ 490, "frontside misty" },
{ 552, "fubar" },
{ 524, "fuel injector (ak-47)" },
{ 614, "fuel injector (tec-9)" },
{ 508, "fuel rod" },
{ 250, "full stop" },
{ 568, "gamma doppler (emerald marble)" },
{ 569, "gamma doppler (phase 1)" },
{ 570, "gamma doppler (phase 2)" },
{ 571, "gamma doppler (phase 3)" },
{ 572, "gamma doppler (phase 4)" },
{ 243, "gator mesh" },
{ 225, "ghost camo" },
{ 554, "ghost crusader" },
{ 634, "gila" },
{ 111, "glacier mesh" },
{ 497, "golden coil" },
{ 185, "golden koi" },
{ 436, "grand prix" },
{ 21,  "granite marbleized" },
{ 212, "graphite" },
{ 214, "graphite" },
{ 104, "grassland leaves" },
{ 95,  "grassland" },
{ 188, "graven" },
{ 294, "green apple" },
{ 502, "green marine" },
{ 366, "green plaid" },
{ 384, "griffin" },
{ 611, "grim" },
{ 381, "grinder" },
{ 406, "grotto" },
{ 2,   "groundwater" },
{ 209, "groundwater" },
{ 257, "guardian" },
{ 290, "guardian" },
{ 15,  "gunsmoke" },
{ 439, "hades" },
{ 328, "hand cannon" },
{ 485, "handgun" },
{ 594, "harvester" },
{ 198, "hazard" },
{ 284, "heat" },
{ 431, "heat" },
{ 291, "heaven guard" },
{ 314, "heaven guard" },
{ 273, "heirloom" },
{ 220, "hemoglobin" },
{ 218, "hexane" },
{ 390, "highwayman" },
{ 219, "hive" },
{ 33,  "hot rod" },
{ 445, "hot rod" },
{ 377, "hot shot" },
{ 309, "howl" },
{ 241, "hunting blind" },
{ 456, "hydroponic" },
{ 475, "hyper beast (awp)" },
{ 430, "hyper beast (m4a1-s)" },
{ 537, "hyper beast (nova)" },
{ 61,  "hypnotic" },
{ 440, "icarus fell" },
{ 599, "ice cap" },
{ 472, "impact drill" },
{ 591, "imperial dragon" },
{ 515, "imperial" },
{ 536, "impire" },
{ 602, "imprint" },
{ 333, "indigo" },
{ 592, "iron clad" },
{ 623, "ironwork" },
{ 171, "irradiated alert" },
{ 303, "isaac" },
{ 357, "ivory" },
{ 316, "jaguar" },
{ 539, "jambiya" },
{ 340, "jet set" },
{ 542, "judgement of anubis" },
{ 202, "jungle ddpat" },
{ 147, "jungle dashed" },
{ 122, "jungle spray" },
{ 16,  "jungle tiger" },
{ 151, "jungle" },
{ 205, "jungle" },
{ 265, "kami (five-seven)" },
{ 308, "kami (galil-ar)" },
{ 504, "kill confirmed" },
{ 326, "knight" },
{ 356, "koi" },
{ 527, "kumicho dragon" },
{ 362, "labyrinth" },
{ 534, "lapis gator" },
{ 540, "lead conduit" },
{ 342, "leather" },
{ 26,  "lichen dashed" },
{ 51,  "lightning strike" },
{ 596, "limelight" },
{ 558, "lore (bayonet)" },
{ 559, "lore (flip)" },
{ 560, "lore (gut)" },
{ 561, "lore (karambit)" },
{ 562, "lore (m9 bayonet)" },
{ 483, "loudmouth" },
{ 266, "magma" },
{ 402, "malachite" },
{ 395, "man-o'-war (awp)" },
{ 432, "man-o'-war (negev)" },
{ 413, "marble fade" },
{ 261, "marina" },
{ 321, "master piece" },
{ 200, "mayan dreams" },
{ 587, "mecha industries (m4a1-s)" },
{ 626, "mecha industries (famas)" },
{ 446, "medusa" },
{ 258, "mehndi" },
{ 177, "memento" },
{ 34,  "metallic ddpat" },
{ 296, "meteorite" },
{ 468, "midnight storm" },
{ 441, "minotaur's labyrinth" },
{ 467, "mint kimono" },
{ 164, "modern hunter" },
{ 335, "module" },
{ 427, "monkey business" },
{ 450, "moon in libra" },
{ 204, "mosaico" },
{ 90,  "mudder" },
{ 404, "muertos" },
{ 382, "murky" },
{ 397, "naga" },
{ 496, "nebula crusader" },
{ 538, "necropos" },
{ 481, "nemesis" },
{ 464, "neon kimono" },
{ 600, "neon revolution" },
{ 433, "neon rider" },
{ 477, "neural net" },
{ 236, "night ops" },
{ 40,  "night" },
{ 223, "nightshade" },
{ 254, "nitro" },
{ 322, "nitro" },
{ 372, "nuclear garden" },
{ 168, "nuclear threat" },
{ 179, "nuclear threat" },
{ 369, "nuclear waste" },
{ 211, "ocean foam" },
{ 213, "ocean foam" },
{ 550, "oceanic" },
{ 365, "olive plaid" },
{ 545, "orange crash" },
{ 83,  "orange ddpat" },
{ 465, "orange kimono" },
{ 141, "orange peel" },
{ 434, "origami" },
{ 313, "orion" },
{ 349, "osiris" },
{ 36,  "ossified" },
{ 518, "outbreak" },
{ 183, "overgrowth" },
{ 157, "palm" },
{ 201, "palm" },
{ 448, "pandora's box" },
{ 276, "panther" },
{ 454, "para green" },
{ 443, "pathfinder" },
{ 608, "petroglyph" },
{ 584, "phobos" },
{ 526, "photic zone" },
{ 347, "pilot" },
{ 84,  "pink ddpat" },
{ 251, "pit viper" },
{ 506, "point disarray" },
{ 315, "poison dart" },
{ 74,  "polar camo" },
{ 107, "polar mesh" },
{ 435, "pole position" },
{ 622, "polymer" },
{ 449, "poseidon" },
{ 514, "power loader" },
{ 612, "powercore" },
{ 535, "praetorian" },
{ 170, "predator" },
{ 556, "primal saber" },
{ 260, "pulse" },
{ 287, "pulse" },
{ 338, "pulse" },
{ 407, "quicksilver" },
{ 167, "radiation hazard" },
{ 375, "radiation hazard" },
{ 498, "rangeen" },
{ 484, "ranger" },
{ 555, "re-entry" },
{ 367, "reactor" },
{ 595, "reboot" },
{ 543, "red astor" },
{ 275, "red fragcam" },
{ 14,  "red laminate" },
{ 348, "red leather" },
{ 320, "red python" },
{ 248, "red quartz" },
{ 259, "redline" },
{ 282, "redline" },
{ 307, "retribution" },
{ 510, "retrobution" },
{ 507, "ricochet" },
{ 488, "riot" },
{ 263, "rising skull" },
{ 318, "road rash" },
{ 478, "rocket pop" },
{ 604, "roll cage" },
{ 262, "rose iron" },
{ 332, "royal blue" },
{ 625, "royal consorts" },
{ 532, "royal legion" },
{ 512, "royal paladin" },
{ 482, "ruby poison dart" },
{ 203, "rust coat" },
{ 323, "rust coat" },
{ 414, "rust coat" },
{ 72,  "safari mesh" },
{ 119, "sage spray" },
{ 148, "sand dashed" },
{ 208, "sand dune" },
{ 99,  "sand dune" },
{ 116, "sand mesh" },
{ 630, "sand scale" },
{ 124, "sand spray" },
{ 264, "sandstorm" },
{ 289, "sandstorm" },
{ 175, "scorched" },
{ 71,  "scorpion" },
{ 605, "scumbria (five-seven)" },
{ 505, "scumbria (xm1014)" },
{ 473, "seabird" },
{ 405, "serenity" },
{ 288, "sergeant" },
{ 221, "serum" },
{ 368, "setting sun" },
{ 636, "shallow grave" },
{ 516, "shapewood" },
{ 192, "shattered" },
{ 452, "shipping forecast" },
{ 252, "silver quartz" },
{ 32,  "silver" },
{ 11,  "skulls" },
{ 304, "slashed" },
{ 59,  "slaughter" },
{ 616, "slipstream" },
{ 30,  "snake camo" },
{ 633, "sonar" },
{ 500, "special delivery" },
{ 547, "spectre" },
{ 194, "spitfire" },
{ 165, "splash jam" },
{ 162, "splash" },
{ 43, "stained" },
{ 277, "stainless" },
{ 230, "steel disruption" },
{ 628, "stinger" },
{ 494, "stone cold" },
{ 100, "storm" },
{ 371, "styx" },
{ 451, "sun in leo" },
{ 469, "sunset storm ?" },
{ 470, "sunset storm ?" },
{ 358, "supernova" },
{ 492, "survivor z" },
{ 601, "syd mead" },
{ 401, "system lock" },
{ 337, "tatter" },
{ 244, "teardown" },
{ 521, "teclu burner" },
{ 191, "tempest" },
{ 463, "terrace" },
{ 285, "terrain" },
{ 533, "the battlestar" },
{ 511, "the executioner" },
{ 269, "the fuschia is now" },
{ 256, "the kraken" },
{ 519, "tiger moth" },
{ 409, "tiger tooth" },
{ 350, "tigris" },
{ 272, "titanium bit" },
{ 101, "tornado" },
{ 206, "tornado" },
{ 305, "torque" },
{ 489, "torque" },
{ 374, "toxic" },
{ 393, "tranquility" },
{ 363, "traveler" },
{ 268, "tread plate" },
{ 613, "triarch" },
{ 283, "trigon" },
{ 530, "triumvirate" },
{ 233, "tropical storm" },
{ 635, "turf" },
{ 297, "tuxedo" },
{ 437, "twilight galaxy" },
{ 334, "twist" },
{ 98,  "ultraviolet" },
{ 271, "undertow" },
{ 17,  "urban ddpat" },
{ 149, "urban dashed" },
{ 354, "urban hazard" },
{ 387, "urban hazard" },
{ 143, "urban masked" },
{ 135, "urban perforated" },
{ 237, "urban rubble" },
{ 396, "urban shock" },
{ 426, "valence" },
{ 529, "valence" },
{ 238, "varicamo blue" },
{ 235, "varicamo" },
{ 606, "ventilator" },
{ 544, "ventilators" },
{ 270, "victoria" },
{ 585, "violent daimyo" },
{ 20,  "virus" },
{ 302, "vulcan" },
{ 158, "walnut" },
{ 638, "wasteland princess" },
{ 380, "wasteland rebel (ak-47)" },
{ 586, "wasteland rebel (glock)" },
{ 353, "water elemental" },
{ 224, "water sigil" },
{ 186, "wave spray" },
{ 136, "waves perforated" },
{ 607, "weasel" },
{ 102, "whiteout" },
{ 73,  "wings" },
{ 501, "wingshot" },
{ 76,  "winter forest" },
{ 424, "worm god" },
{ 495, "wraiths" },
{ 215, "x-ray" },
{ 476, "yellow jacket" },
{ 517, "yorick" },
{ 187, "zirka" },
};


std::vector<std::pair<int, const char*>> guns =
{
	{ SDK::ItemDefinitionIndex::WEAPON_AK47,			"ak-47" },
{ SDK::ItemDefinitionIndex::WEAPON_AUG,		  	    "aug" },
{ SDK::ItemDefinitionIndex::WEAPON_AWP,		     	"awp" },
{ SDK::ItemDefinitionIndex::WEAPON_CZ75A,		     	"cz75-a" },
{ SDK::ItemDefinitionIndex::WEAPON_DEAGLE,	    	"desert eagle" },
{ SDK::ItemDefinitionIndex::WEAPON_ELITE,			    "dual berettas" },
{ SDK::ItemDefinitionIndex::WEAPON_FAMAS,			    "famas" },
{ SDK::ItemDefinitionIndex::WEAPON_FIVESEVEN,     	"five-Seven" },
{ SDK::ItemDefinitionIndex::WEAPON_GALILAR,	    	"galil-ar" },
{ SDK::ItemDefinitionIndex::WEAPON_G3SG1,		    	"g3sg1" },
{ SDK::ItemDefinitionIndex::WEAPON_GLOCK,		    	"glock-18" },
{ SDK::ItemDefinitionIndex::WEAPON_M249,	    		"m249" },
{ SDK::ItemDefinitionIndex::WEAPON_M4A1_SILENCER,     "m4a1-s" },
{SDK::ItemDefinitionIndex::WEAPON_M4A1,	     		"m4a4" },
{ SDK::ItemDefinitionIndex::WEAPON_MAC10,		    	"mac-10" },
{ SDK::ItemDefinitionIndex::WEAPON_MAG7,		    	"mag-7" },
{ SDK::ItemDefinitionIndex::WEAPON_MP7,               "mp7" },
{ SDK::ItemDefinitionIndex::WEAPON_MP9,		    	"mp9" },
{ SDK::ItemDefinitionIndex::WEAPON_NEGEV,		    	"negev" },
{ SDK::ItemDefinitionIndex::WEAPON_NOVA,		    	"nova" },
{ SDK::ItemDefinitionIndex::WEAPON_HKP2000,	    	"p2000" },
{ SDK::ItemDefinitionIndex::WEAPON_P250,		    	"p250" },
{ SDK::ItemDefinitionIndex::WEAPON_P90,		    	"p90" },
{ SDK::ItemDefinitionIndex::WEAPON_BIZON,		    	"pp-bizon" },
{ SDK::ItemDefinitionIndex::WEAPON_REVOLVER,	    	"r8 revolver" },
{ SDK::ItemDefinitionIndex::WEAPON_SAWEDOFF,	    	"sawed-off" },
{ SDK::ItemDefinitionIndex::WEAPON_SCAR20,	    	"scar-20" },
{ SDK::ItemDefinitionIndex::WEAPON_SG556,		     	"sg-556" },
{ SDK::ItemDefinitionIndex::WEAPON_SSG08,		    	"scout" },
{ SDK::ItemDefinitionIndex::WEAPON_TEC9,		    	"tec-9" },
{ SDK::ItemDefinitionIndex::WEAPON_UMP45,		    	"ump-45" },
{ SDK::ItemDefinitionIndex::WEAPON_USP_SILENCER,   	"usp-s" },
{ SDK::ItemDefinitionIndex::WEAPON_XM1014,            "xm1014" },
{ SDK::ItemDefinitionIndex::WEAPON_KNIFE_BAYONET,  	"bayonet" },
{ SDK::ItemDefinitionIndex::WEAPON_KNIFE_FLIP,	    "flip knife" },
{ SDK::ItemDefinitionIndex::WEAPON_KNIFE_GUT,	    	"gut knife" },
{ SDK::ItemDefinitionIndex::WEAPON_KNIFE_KARAMBIT,    "karambit" },
{ SDK::ItemDefinitionIndex::WEAPON_KNIFE_M9_BAYONET,	"m9 bayonet" },
{ SDK::ItemDefinitionIndex::WEAPON_KNIFE_TACTICAL,	"hunstman lnife" },
{ SDK::ItemDefinitionIndex::WEAPON_KNIFE_FALCHION,	"falchion knife" },
{ SDK::ItemDefinitionIndex::WEAPON_KNIFE_BOWIE, "bowie knife" },
{ SDK::ItemDefinitionIndex::WEAPON_KNIFE_BUTTERFLY,	"butterfly knife" },
{ SDK::ItemDefinitionIndex::WEAPON_KNIFE_PUSH,        "shadow daggers" },
{ SDK::ItemDefinitionIndex::WEAPON_KNIFE_TALON,       "talon knife" },
{ SDK::ItemDefinitionIndex::WEAPON_KNIFE_NAVAJA,      "navaja knife" },
{ SDK::ItemDefinitionIndex::WEAPON_KNIFE_STILETTO,    "stiletto knife" },
{ SDK::ItemDefinitionIndex::WEAPON_KNIFE_URSUS,       "ursus knife" },
};

const char* weaponnames(int id)
{
	switch (id)
	{
	case 1:
		return "desert eagle";
	case 2:
		return "dual barettas";
	case 3:
		return "five-seven";
	case 4:
		return "glock-17";
	case 7:
		return "ak-47";
	case 8:
		return "aug";
	case 9:
		return "awp";
	case 10:
		return "famas";
	case 11:
		return "g3sg1";
	case 13:
		return "galil-ar";
	case 14:
		return "m249";
	case 60:
		return "m4a1-s";
	case 16:
		return "m4a4";
	case 17:
		return "mac-10";
	case 19:
		return "p90";
	case 24:
		return "ump-45";
	case 25:
		return "xm1014";
	case 26:
		return "pp-bizon";
	case 27:
		return "mag-7";
	case 28:
		return "negev";
	case 29:
		return "sawed-off";
	case 30:
		return "tec-9";
	case 32:
		return "p2000";
	case 33:
		return "mp7";
	case 34:
		return "mp9";
	case 35:
		return "nova";
	case 36:
		return "p250";
	case 38:
		return "scar-20";
	case 39:
		return "sg556";
	case 40:
		return "ssg08";
	case 61:
		return "usp-s";
	case 63:
		return "cz75-a";
	case 64:
		return "r8 revolver";
	case 508:
		return "m9 bayonet";
	case 500:
		return "bayonet";
	case 505:
		return "flip knife";
	case 506:
		return "gut knife";
	case 507:
		return "karambit";
	case 509:
		return "hunstman knife";
	case 512:
		return "falchion";
	case 514:
		return "bowie knife";
	case 515:
		return "butterfly";
	case 516:
		return "shadow daggers";
	case 523:
		return "talon knife";
	case 520:
		return "navaja knife";
	case 522:
		return "stiletto knife";
	case 519:
		return "ursus knife";

	default:
		return "";
	}
}