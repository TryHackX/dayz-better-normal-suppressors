class CfgPatches
{
    class BetterNormalSuppressors
    {
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data",
			"DZ_Scripts",
			"DZ_Weapons_Muzzles"
		};
    };
};

class CfgMods
{
	class BetterNormalSuppressors
	{
		dir="BetterNormalSuppressors";
		name="BetterNormalSuppressors";
		credits="TryHackX";
		author="TryHackX";
		authorID="76561199846115789";
		version="1.1";
		type="mod";
		dependencies[]={"World","Mission"};

		class defs
		{
			class worldScriptModule
			{
				value="";
				files[]={"BetterNormalSuppressors/Scripts/4_World"};
			};
			class missionScriptModule
			{
				value="";
				files[]={"BetterNormalSuppressors/Scripts/5_Mission"};
			};
		};
	};
};

// No suppressor class is changed here - the vanilla hitpoints and barrelArmor stay as they are.
// The engine wears a suppressor by damageBarrel / (barrelArmor * hitpoints) per shot, so the lifetime grows
// with hitpoints squared (version 1.0 raised hitpoints: x1.5 / x2 became x2.25 / x4). The script scales the
// wear of every shot instead, so a suppressor lasts exactly "Multiplier" times longer - for any suppressor
// class the server lists in BetterNormalSuppressors/config.json.
