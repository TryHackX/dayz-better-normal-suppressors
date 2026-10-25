# Better Normal Suppressors

Suppressors last longer - exactly as much longer as the server sets it, for any suppressor (also from other mods). For DayZ 1.30 (Badlands).

[Steam Workshop](https://steamcommunity.com/sharedfiles/filedetails/?id=3331857686) · DayZ 1.30 (Badlands) · author: TryHackX

## Support

If the mod is useful for you or your server, you can support further development:

| Currency | Network | Address |
|---|---|---|
| Monero (XMR) | Monero | `45hvee4Jv7qeAm6SrBzXb9YVjb8DkHtFtFh7qkDMxS9zYX3NRi1dV27MtSdVC5X8T1YVoiG8XFiJkh4p9UncqWGxHi4tiwk` |
| Bitcoin (BTC) | Bitcoin (on-chain) | `bc1qncavcek4kknpvykedxas8kxash9kdng990qed2` |
| Ethereum (ETH) | Ethereum (mainnet) | `0xa3d38d5Cf202598dd782C611e9F43f342C967cF5` |

## Default lifetime

| Suppressor | Class | Default | Shots (vanilla -> mod) |
|---|---|---|---|
| Standardized Suppressor | `M4_Suppressor` | x1.5 | 144 -> 216 (5.56) |
| Normalized Suppressor | `AK_Suppressor` | x2 | 72 -> 144 (7.62x39) |
| Pistol Suppressor | `PistolSuppressor` | x2 | 90 -> 180 (9x19) |
| Plastic Bottle Suppressor | `ImprovisedSuppressor` | x2 | 6 -> 12 (5.56) |
| PB Suppressor | `MakarovPBSuppressor` | x1 | 450 (9x19) |
| Groza suppressor barrel | `Groza_Barrel_Suppressor` | x1 | vanilla |

## Configuration

`<server profile>/BetterNormalSuppressors/config.json` (created on the first start):

```json
{
    "ConfigVersion": 1,
    "Suppressors": [
        { "ClassName": "M4_Suppressor", "Multiplier": 1.5 },
        { "ClassName": "AK_Suppressor", "Multiplier": 2.0 }
    ]
}
```

- `ClassName` - any suppressor class, also from other mods; it also covers suppressors based on it.
- `Multiplier` - how many times longer than in vanilla it lasts (0.1 - 100); below 1 it wears faster.
- Suppressors that are not in the list wear like in vanilla.

## How it works

The game wears a suppressor by `damageBarrel / (barrelArmor × hitpoints)` per shot. The mod does not change any suppressor in the config
(hitpoints and barrelArmor stay vanilla, so repairs and damage levels work like in vanilla) - it scales the wear of every shot on the server.

## Installation

- Server-side mod: add it to the `-serverMod=` parameter of the server - players do not need to download it.
- `-mod=` on the server and the client works too; then copy `keys/TryHackX_New.bikey` to the `keys` folder of the server.

## Building from source

1. DayZ Tools → **AddonBuilder**: source directory `soucre/BetterNormalSuppressors`, addon prefix `BetterNormalSuppressors`, output `addons/`.
2. Sign the PBO with your own key (DayZ Tools → DSUtils / DSCreateKey + DSSignFile) and give servers your `.bikey`.
   The release on Steam Workshop is signed with `TryHackX_New`.

## Compatibility

- DayZ 1.30 (Badlands), tested on a dedicated server with real shots.
- No other mods required.

## License

© TryHackX. You are free to use this mod on your server. Please do not repack or reupload it without permission.
