/*==============================================================================
Simple Event Example
Copyright (c), Firelight Technologies Pty, Ltd 2012-2014.

This example demonstrates the various ways of playing an event. Controlling event
playback using a persistent instance versus fire and forget.
==============================================================================*/
#include "fmod_studio.hpp"
#include "fmod.hpp"
#include "common.h"

int FMOD_Main()
{
    void *extraDriverData = NULL;
    Common_Init(&extraDriverData);

    FMOD::Studio::System* system = NULL;
    ERRCHECK( FMOD::Studio::System::create(&system) );
    ERRCHECK( system->initialize(32, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, extraDriverData) );
    
    FMOD::Studio::Bank* masterBank = NULL;
    ERRCHECK( system->loadBankFile(Common_MediaPath("Master Bank.bank"), FMOD_STUDIO_LOAD_BANK_NORMAL, &masterBank) );
    
    FMOD::Studio::Bank* stringsBank = NULL;
    ERRCHECK( system->loadBankFile(Common_MediaPath("Master Bank.strings.bank"), FMOD_STUDIO_LOAD_BANK_NORMAL, &stringsBank) );
    
    FMOD::Studio::Bank* ambienceBank = NULL;
    ERRCHECK( system->loadBankFile(Common_MediaPath("Surround_Ambience.bank"), FMOD_STUDIO_LOAD_BANK_NORMAL, &ambienceBank) );
    
    FMOD::Studio::Bank* menuBank = NULL;
    ERRCHECK( system->loadBankFile(Common_MediaPath("UI_Menu.bank"), FMOD_STUDIO_LOAD_BANK_NORMAL, &menuBank) );
    
    FMOD::Studio::Bank* weaponsBank = NULL;
    ERRCHECK( system->loadBankFile(Common_MediaPath("Weapons.bank"), FMOD_STUDIO_LOAD_BANK_NORMAL, &weaponsBank) );

    // Get the Looping Ambience event
    FMOD::Studio::ID loopingAmbienceID = {0};
    ERRCHECK( system->lookupID("event:/Ambience/Country", &loopingAmbienceID) );

    FMOD::Studio::EventDescription* loopingAmbienceDescription = NULL;
    ERRCHECK( system->getEvent(&loopingAmbienceID, FMOD_STUDIO_LOAD_BEGIN_NOW, &loopingAmbienceDescription) );
    
    FMOD::Studio::EventInstance* loopingAmbienceInstance = NULL;
    ERRCHECK( loopingAmbienceDescription->createInstance(&loopingAmbienceInstance) );
    
    // Get the 4 Second Surge event
    FMOD::Studio::ID cancelID = {0};
    ERRCHECK( system->lookupID("event:/UI/Cancel", &cancelID) );

    FMOD::Studio::EventDescription* cancelDescription = NULL;
    ERRCHECK( system->getEvent(&cancelID, FMOD_STUDIO_LOAD_BEGIN_NOW, &cancelDescription) );
    
    FMOD::Studio::EventInstance* cancelInstance = NULL;
    ERRCHECK( cancelDescription->createInstance(&cancelInstance) );
    
    // Get the Single Explosion event
    FMOD::Studio::ID explosionID = {0};
    ERRCHECK( system->lookupID("event:/Explosions/Single Explosion", &explosionID) );

    FMOD::Studio::EventDescription* explosionDescription = NULL;
    ERRCHECK( system->getEvent(&explosionID, FMOD_STUDIO_LOAD_BEGIN_NOW, &explosionDescription) );

    // Start loading explosion sample data and keep it in memory
    ERRCHECK( explosionDescription->loadSampleData() );

    do
    {
        Common_Update();
        
        if (Common_BtnPress(BTN_ACTION1))
        {
            // One-shot event
            FMOD::Studio::EventInstance* eventInstance = NULL;
            ERRCHECK( explosionDescription->createInstance(&eventInstance) );

            ERRCHECK( eventInstance->start() );

            // Release will clean up the instance when it completes
            ERRCHECK( eventInstance->release() );
        }
    
        if (Common_BtnPress(BTN_ACTION2))
        {
            ERRCHECK( loopingAmbienceInstance->start() );
        }

        if (Common_BtnPress(BTN_ACTION3))
        {
            ERRCHECK( loopingAmbienceInstance->stop(FMOD_STUDIO_STOP_IMMEDIATE) );
        }

        if (Common_BtnPress(BTN_ACTION4))
        {
            // Calling start on an instance will cause it to restart if it's already playing
            ERRCHECK( cancelInstance->start() );
        }

        ERRCHECK( system->update() );

        Common_Draw("==================================================");
        Common_Draw("Simple Event Example.");
        Common_Draw("Copyright (c) Firelight Technologies 2014-2014.");
        Common_Draw("==================================================");
        Common_Draw("");
        Common_Draw("Press %s to fire and forget the explosion", Common_BtnStr(BTN_ACTION1));
        Common_Draw("Press %s to start the looping ambience", Common_BtnStr(BTN_ACTION2));
        Common_Draw("Press %s to stop the looping ambience", Common_BtnStr(BTN_ACTION3));
        Common_Draw("Press %s to start/restart the cancel sound", Common_BtnStr(BTN_ACTION4));
        Common_Draw("Press %s to quit", Common_BtnStr(BTN_QUIT));

        Common_Sleep(50);
    } while (!Common_BtnPress(BTN_QUIT));
    
    ERRCHECK( weaponsBank->unload() );
    ERRCHECK( menuBank->unload() );
    ERRCHECK( ambienceBank->unload() );
    ERRCHECK( stringsBank->unload() );
    ERRCHECK( masterBank->unload() );

    ERRCHECK( system->release() );

    Common_Close();

    return 0;
}
