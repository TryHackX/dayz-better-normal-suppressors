//! one suppressor in the config: how many times longer than in vanilla it lasts
class BNS_SuppressorSetting
{
	string ClassName;
	float Multiplier;
}

//! Server settings ($profile:BetterNormalSuppressors/config.json)
class BNS_Settings
{
	int ConfigVersion = 1;
	ref array<ref BNS_SuppressorSetting> Suppressors = new array<ref BNS_SuppressorSetting>;
}

class BNS_Config
{
	static const float MIN_MULTIPLIER = 0.1;
	static const float MAX_MULTIPLIER = 100.0;
	static const string FOLDER = "$profile:BetterNormalSuppressors";
	static const string FILE = "$profile:BetterNormalSuppressors/config.json";

	protected static ref BNS_Settings s_Settings;
	//! class name -> multiplier, in the order of the config
	protected static ref map<string, float> s_Multipliers;
	protected static ref array<string> s_Order;
	//! multiplier found for each suppressor type (also for subclasses of the configured classes)
	protected static ref map<string, float> s_TypeCache;

	//! defaults: the 4 suppressors of the mod description, the other vanilla suppressors unchanged (x1)
	static BNS_Settings CreateDefault()
	{
		BNS_Settings settings = new BNS_Settings();
		AddDefault(settings, "M4_Suppressor", 1.5);
		AddDefault(settings, "AK_Suppressor", 2.0);
		AddDefault(settings, "PistolSuppressor", 2.0);
		AddDefault(settings, "ImprovisedSuppressor", 2.0);
		AddDefault(settings, "MakarovPBSuppressor", 1.0);
		AddDefault(settings, "Groza_Barrel_Suppressor", 1.0);
		return settings;
	}

	protected static void AddDefault(BNS_Settings settings, string className, float multiplier)
	{
		BNS_SuppressorSetting entry = new BNS_SuppressorSetting();
		settings.Suppressors.Insert(entry);
		entry.ClassName = className;
		entry.Multiplier = multiplier;
	}

	static BNS_Settings Get()
	{
		if (!s_Settings)
		{
			s_Settings = CreateDefault();
			BuildMultipliers(s_Settings);
		}
		return s_Settings;
	}

	static void Load()
	{
		if (!GetGame().IsServer())
			return;

		if (!FileExist(FOLDER))
			MakeDirectory(FOLDER);

		BNS_Settings settings;
		string error;
		if (FileExist(FILE))
		{
			settings = new BNS_Settings();
			if (JsonFileLoader<BNS_Settings>.LoadFile(FILE, settings, error))
			{
				Print("[BetterNormalSuppressors] Config loaded from " + FILE);
			}
			else
			{
				Print("[BetterNormalSuppressors] ERROR: " + error);
				Print("[BetterNormalSuppressors] Using the default settings, the file was not changed.");
				settings = CreateDefault();
			}
		}
		else
		{
			settings = CreateDefault();
			if (JsonFileLoader<BNS_Settings>.SaveFile(FILE, settings, error))
				Print("[BetterNormalSuppressors] Default config created at " + FILE);
			else
				Print("[BetterNormalSuppressors] ERROR: " + error);
		}

		s_Settings = settings;
		BuildMultipliers(settings);
	}

	//! every class the admin lists is used; clamped values, empty names skipped
	protected static void BuildMultipliers(BNS_Settings settings)
	{
		s_Multipliers = new map<string, float>;
		s_Order = new array<string>;
		s_TypeCache = new map<string, float>;

		if (!settings.Suppressors)
			return;

		foreach (BNS_SuppressorSetting entry : settings.Suppressors)
		{
			if (!entry || entry.ClassName == "")
				continue;

			float clamped = Math.Clamp(entry.Multiplier, MIN_MULTIPLIER, MAX_MULTIPLIER);
			if (clamped != entry.Multiplier)
				Print("[BetterNormalSuppressors] WARNING: " + entry.ClassName + " multiplier " + entry.Multiplier.ToString() + " is out of range (0.1 - 100), using " + clamped.ToString());

			if (!GetGame().ConfigIsExisting("CfgVehicles " + entry.ClassName))
				Print("[BetterNormalSuppressors] WARNING: " + entry.ClassName + " does not exist (wrong name or its mod is not loaded)");

			if (s_Order.Find(entry.ClassName) == -1)
				s_Order.Insert(entry.ClassName);
			s_Multipliers.Set(entry.ClassName, clamped);
			Print("[BetterNormalSuppressors] " + entry.ClassName + " lasts x" + clamped.ToString());
		}
	}

	//! multiplier for this suppressor: its own class, else the first configured class it is based on; 0 = not handled
	static float GetMultiplier(EntityAI suppressor)
	{
		Get();
		string type = suppressor.GetType();
		float multiplier;
		if (s_TypeCache.Find(type, multiplier))
			return multiplier;

		multiplier = 0;
		if (!s_Multipliers.Find(type, multiplier))
		{
			foreach (string name : s_Order)
			{
				if (suppressor.IsKindOf(name))
				{
					multiplier = s_Multipliers.Get(name);
					break;
				}
			}
		}

		s_TypeCache.Set(type, multiplier);
		return multiplier;
	}

	//! server, every shot. EEFired comes right before the engine wears the suppressor by
	//! damageBarrel / (barrelArmor * hitpoints); the wear is scaled to 1 / multiplier of that.
	static void OnFired(Weapon_Base weapon, string ammoType)
	{
		ItemBase suppressor = weapon.GetAttachedSuppressor();
		if (!suppressor || suppressor.IsRuined())
			return;

		float multiplier = GetMultiplier(suppressor);
		if (multiplier <= 0 || multiplier == 1)
			return;

		float armor = suppressor.ConfigGetFloat("barrelArmor");
		float hitpoints = suppressor.GetMaxHealth("", "");
		float damageBarrel = GetGame().ConfigGetFloat("CfgAmmo " + ammoType + " damageBarrel");
		if (armor <= 0 || hitpoints <= 0 || damageBarrel <= 0)
			return;

		float engineWear = damageBarrel / (armor * hitpoints);
		float wantedWear = engineWear / multiplier;

		// shorter lifetime: add the extra wear (negative health change)
		if (multiplier < 1)
		{
			suppressor.AddHealth("", "", engineWear - wantedWear);
			return;
		}

		// longer lifetime: give back part of the wear - before the engine wear as far as the maximum
		// allows (so the shot cannot ruin a suppressor that should survive it), the rest right after it
		float giveBack = engineWear - wantedWear;
		float room = hitpoints - suppressor.GetHealth("", "");
		float now = Math.Min(giveBack, room);
		if (now > 0)
			suppressor.AddHealth("", "", now);

		float later = giveBack - now;
		if (later > 0)
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(GiveBackLater, suppressor, later);
	}

	static void GiveBackLater(ItemBase suppressor, float amount)
	{
		if (suppressor && !suppressor.IsRuined())
			suppressor.AddHealth("", "", amount);
	}
}

modded class Weapon_Base
{
	override void EEFired(int muzzleType, int mode, string ammoType)
	{
		super.EEFired(muzzleType, mode, ammoType);

		if (GetGame().IsServer())
			BNS_Config.OnFired(this, ammoType);
	}
}
