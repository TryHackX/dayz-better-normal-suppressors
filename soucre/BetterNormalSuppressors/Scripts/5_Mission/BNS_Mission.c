modded class MissionServer
{
	override void OnInit()
	{
		super.OnInit();
		BNS_Config.Load();
	}
}

modded class MissionGameplay
{
	override void OnInit()
	{
		super.OnInit();

		// offline / listen server
		if (GetGame().IsServer())
			BNS_Config.Load();
	}
}
