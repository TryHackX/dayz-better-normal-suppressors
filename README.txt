Better Normal Suppressors 1.1 - server setup (DayZ 1.30 Badlands)

1. Server-side mod: add it to the server -serverMod parameter - players do not need to download it.
   (-mod on the server and the client works too.)
2. Only with -mod: copy keys\TryHackX_New.bikey to your server's "keys" folder.
   (Older versions were signed with TryHackX.bikey - that key is no longer used.)
3. Start the server once - the config is created automatically:
   <server profile>\BetterNormalSuppressors\config.json
   Restart the server after editing it.

How long a suppressor lasts - "Multiplier" x the vanilla lifetime (0.1 - 100):
   M4_Suppressor           Standardized Suppressor     default x1.5   vanilla 144 shots of 5.56     -> 216
   AK_Suppressor           Normalized Suppressor        default x2     vanilla 72 shots of 7.62x39   -> 144
   PistolSuppressor        Pistol Suppressor            default x2     vanilla 90 shots of 9x19      -> 180
   ImprovisedSuppressor    Plastic Bottle Suppressor    default x2     vanilla 6 shots of 5.56       -> 12
   MakarovPBSuppressor     PB Suppressor                default x1     vanilla 450 shots of 9x19
   Groza_Barrel_Suppressor Groza suppressor barrel      default x1     (vanilla)
   Stronger ammo wears a suppressor faster - the multiplier works the same for every ammo.

config.json - list any suppressor class you want (also suppressors of other mods):
{
    "ConfigVersion": 1,
    "Suppressors": [
        { "ClassName": "M4_Suppressor", "Multiplier": 1.5 },
        { "ClassName": "AK_Suppressor", "Multiplier": 2.0 },
        { "ClassName": "PistolSuppressor", "Multiplier": 2.0 },
        { "ClassName": "ImprovisedSuppressor", "Multiplier": 2.0 },
        { "ClassName": "MakarovPBSuppressor", "Multiplier": 1.0 },
        { "ClassName": "Groza_Barrel_Suppressor", "Multiplier": 1.0 }
    ]
}
   - A class from the list also covers the suppressors based on it (e.g. coloured versions from other mods),
     unless they have their own entry.
   - A suppressor that is not in the list wears like in vanilla.
   - Values below 1 make a suppressor wear faster (0.5 = half the vanilla lifetime).
   - A wrong class name or a value out of range is reported in the server script log.

The mod does not change any suppressor in config.cpp (hitpoints and barrelArmor stay vanilla), it scales
the wear of every shot on the server. So repairs and damage levels work like in vanilla.

Note about version 1.0: it raised the hitpoints of the suppressors. The game wears a suppressor by
damageBarrel / (barrelArmor x hitpoints) per shot, so the lifetime grows with hitpoints squared and the
suppressors really lasted x2.25 (instead of x1.5) and x4 (instead of x2). For the lifetime of version 1.0
set 2.25 for M4_Suppressor and 4 for AK_Suppressor, PistolSuppressor and ImprovisedSuppressor.
