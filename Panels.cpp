#include "SDK.h"
#include "Panels.h"
#include "CDrawManager.h"

CScreenSize gScreenSize;
//===================================================================================
void __fastcall Hooked_PaintTraverse(PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{
	try
	{
		VMTManager & hook = VMTManager::GetHook(pPanels);
		//Get a pointer to the instance of your VMTManager with the function GetHook.
		hook.GetMethod<void(__thiscall*)(PVOID, unsigned int, bool, bool)>(gOffsets.iPaintTraverseOffset)(
			pPanels, vguiPanel, forceRepaint, allowForce); //Call the original.

		static unsigned int vguiMatSystemTopPanel;

		if (vguiMatSystemTopPanel == NULL)
		{
			const auto sz_name = gInts.Panels->GetName(vguiPanel);
			if (sz_name[0] == 'M' && sz_name[3] == 'S')
				//Look for MatSystemTopPanel without using slow operations like strcmp or strstr.
			{
				vguiMatSystemTopPanel = vguiPanel;
				Intro();
			}
		}

		if (vguiMatSystemTopPanel == vguiPanel) //If we're on MatSystemTopPanel, call our drawing code.
		{
			//This section will be called when the player is not at the menu game and can see the screen or not taking a screenshot.
			gDrawManager.DrawString((gScreenSize.iScreenWidth / 2), 200, gDrawManager.dwGetTeamColor(3),
			                        "Welcome to Darkstorm"); //Remove this if you want.

			//Test ESP code.

			auto pBaseLocalEnt = gInts.EntList->GetClientEntity(me); //Grab the local player's entity.

			if (pBaseLocalEnt == NULL) //Always check for null pointers.
				return;

			Vector vecWorld;
			Vector vecScreen; //Setup the Vectors.

			pBaseLocalEnt->GetWorldSpaceCenter(vecWorld); //Get the center of the player.

			if (gDrawManager.WorldToScreen(vecWorld, vecScreen)) //If the player is visble.
			{
				gDrawManager.DrawString(vecScreen.x, vecScreen.y, 0xFFFFFFFF, "You"); //Draw on the player.
			}
		}
	}
	catch (...)
	{
		Log::Fatal("Failed PaintTraverse");
	}
}

//===================================================================================
auto Intro() -> void
{
	try
	{
#define DUMP 0
		gDrawManager.Initialize(); //Initalize the drawing class.
#if DUMP
  		gOffsets.DumpAllNetvars(); 
#endif
		gOffsets.Initialize(); // Initialize the netvars
		Log::Msg("Injection Successful"); //If the module got here without crashing, it is good day.
	}
	catch (...)
	{
		Log::Fatal("Failed Intro");
	}
}
