/*==============================================================================
Event Parameter Example
Copyright (c), Firelight Technologies Pty, Ltd 2012-2014.

This example demonstrates how to control event playback using game parameters.
==============================================================================*/
#include "fmod_studio.hpp"
#include "fmod.hpp"
#include "common.h"


struct ProgrammerSoundContext
{
    FMOD::System* system;
    const char* soundName;
};

FMOD_RESULT F_CALLBACK programmerSoundCallback(FMOD_STUDIO_EVENT_CALLBACK_TYPE type, void *parameters);

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
    ERRCHECK( system->loadBankFile(Common_MediaPath("Character.bank"), FMOD_STUDIO_LOAD_BANK_NORMAL, &ambienceBank) );
    
    FMOD::Studio::ID eventID = {0};
    ERRCHECK( system->lookupID("event:/Character/Radio/Command", &eventID) );

    FMOD::Studio::EventDescription* eventDescription = NULL;
    ERRCHECK( system->getEvent(&eventID, FMOD_STUDIO_LOAD_BEGIN_NOW, &eventDescription) );

    FMOD::Studio::EventInstance* eventInstance = NULL;
    ERRCHECK( eventDescription->createInstance(&eventInstance) );
    
    ProgrammerSoundContext programmerSoundContext;
    ERRCHECK( system->getLowLevelSystem(&programmerSoundContext.system) );

    ERRCHECK( eventInstance->setUserData(&programmerSoundContext) );
    ERRCHECK( eventInstance->setCallback(programmerSoundCallback) );
    do
    {
        Common_Update();

        if (Common_BtnPress(BTN_ACTION1))
        {
            programmerSoundContext.soundName = Common_MediaPath("sequence-one.ogg");
            ERRCHECK( eventInstance->start() );
        }

        if (Common_BtnPress(BTN_ACTION2))
        {
            programmerSoundContext.soundName = Common_MediaPath("sequence-two.ogg");
            ERRCHECK( eventInstance->start() );
        }

        if (Common_BtnPress(BTN_ACTION3))
        {
            programmerSoundContext.soundName = Common_MediaPath("sequence-three.ogg");
            ERRCHECK( eventInstance->start() );
        }

        if (Common_BtnPress(BTN_ACTION4))
        {
            programmerSoundContext.soundName = Common_MediaPath("sequence-four.ogg");
            ERRCHECK( eventInstance->start() );
        }

        ERRCHECK( system->update() );

        Common_Draw("==================================================");
        Common_Draw("Event Parameter Example.");
        Common_Draw("Copyright (c) Firelight Technologies 2014-2014.");
        Common_Draw("==================================================");
        Common_Draw("");
        Common_Draw("Press %s to play event with sound 1", Common_BtnStr(BTN_ACTION1));
        Common_Draw("Press %s to play event with sound 2", Common_BtnStr(BTN_ACTION2));
        Common_Draw("Press %s to play event with sound 3", Common_BtnStr(BTN_ACTION3));
        Common_Draw("Press %s to play event with sound 4", Common_BtnStr(BTN_ACTION4));
        Common_Draw("");
        Common_Draw("Press %s to quit", Common_BtnStr(BTN_QUIT));

        Common_Sleep(50);
    } while (!Common_BtnPress(BTN_QUIT));

    ERRCHECK( system->release() );

    Common_Close();

    return 0;
}

#define CHECK_RESULT(op) \
    do{ \
        FMOD_RESULT res = op; \
        if (res != FMOD_OK) \
        { \
            return res; \
        } \
    } while(false)

FMOD_RESULT F_CALLBACK programmerSoundCallback(FMOD_STUDIO_EVENT_CALLBACK_TYPE type, void *parameters)
{
    switch (type)
    {
    case FMOD_STUDIO_EVENT_CALLBACK_CREATE_PROGRAMMER_SOUND:
        {
            FMOD_STUDIO_PROGRAMMER_SOUND_PROPERTIES* props = (FMOD_STUDIO_PROGRAMMER_SOUND_PROPERTIES*)parameters;
    
            // Get our context from the event instance user data
            ProgrammerSoundContext* context = NULL;
            CHECK_RESULT( ((FMOD::Studio::EventInstance*)props->event)->getUserData((void**)&context) );

            // Create the sound
            CHECK_RESULT( context->system->createSound(context->soundName, FMOD_CREATECOMPRESSEDSAMPLE | FMOD_NONBLOCKING, NULL, (FMOD::Sound**)&props->sound) );
        }
        break;
    case FMOD_STUDIO_EVENT_CALLBACK_DESTROY_PROGRAMMER_SOUND:
        {
            FMOD_STUDIO_PROGRAMMER_SOUND_PROPERTIES* props = (FMOD_STUDIO_PROGRAMMER_SOUND_PROPERTIES*)parameters;

            CHECK_RESULT( ((FMOD::Sound*)props->sound)->release() );
        }
        break;
    }

    return FMOD_OK;
}
