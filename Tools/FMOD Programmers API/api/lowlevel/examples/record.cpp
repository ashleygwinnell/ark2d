/*==============================================================================
Record example
Copyright (c), Firelight Technologies Pty, Ltd 2004-2014.

This example shows how to record continuously, and play back the same data as
closely to the record cursor as possible without stuttering.
==============================================================================*/
#include "fmod.hpp"
#include "common.h"

#define LATENCY_MS      (50) /* Some devices will require higher latency to avoid glitches */
int FMOD_Main()
{
    FMOD::System   *system = 0;
    FMOD::Sound    *sound = 0;
    FMOD::Channel  *channel = 0;
    FMOD_RESULT     result = FMOD_OK;
    unsigned int    version = 0;
    unsigned int    soundlength = 0;
    bool            dspenabled = false;
    void           *extradriverdata = 0;
    unsigned int    recordpos = 0;
    unsigned int    recorddelta = 0;
    unsigned int    minrecorddelta = (unsigned int)-1;
    unsigned int    lastrecordpos = 0;
    unsigned int    samplesrecorded = 0;
    unsigned int    playpos = 0;
    float           smootheddelta = 0;
    FMOD_CREATESOUNDEXINFO exinfo;
    
    Common_Init(&extradriverdata);

    /*
        Create a System object and initialize.
    */
    result = FMOD::System_Create(&system);
    ERRCHECK(result);

    result = system->getVersion(&version);
    ERRCHECK(result);

    if (version < FMOD_VERSION)
    {
        Common_Fatal("FMOD lib version %08x doesn't match header version %08x", version, FMOD_VERSION);
    }

    result = system->init(100, FMOD_INIT_NORMAL, extradriverdata);
    ERRCHECK(result);


    int recordrate;
    int recordchannels;
    system->getRecordDriverInfo(0, NULL, NULL, 0, 0, &recordrate, 0, &recordchannels);

    unsigned int adjustedlatency = (recordrate * LATENCY_MS) / 1000;
    unsigned int driftthreshold =  adjustedlatency / 2;


    /*
        Create user sound to record into.
    */
    memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
    exinfo.cbsize           = sizeof(FMOD_CREATESOUNDEXINFO);
    exinfo.numchannels      = recordchannels;
    exinfo.format           = FMOD_SOUND_FORMAT_PCM16;
    exinfo.defaultfrequency = recordrate;
    exinfo.length           = exinfo.defaultfrequency * sizeof(short) * exinfo.numchannels * 5; /* 5 second buffer, doesnt really matter how big this is, but not too small of course. */
    
    result = system->createSound(0, FMOD_LOOP_NORMAL | FMOD_OPENUSER, &exinfo, &sound);
    ERRCHECK(result);

    result = system->recordStart(0, sound, true);
    ERRCHECK(result);

    result = sound->getLength(&soundlength, FMOD_TIMEUNIT_PCM);
    ERRCHECK(result);

    /*
        Main loop
    */
    do
    {
        Common_Update();

        if (Common_BtnPress(BTN_ACTION1))
        {
            FMOD_REVERB_PROPERTIES propon = FMOD_PRESET_CONCERTHALL;
            FMOD_REVERB_PROPERTIES propoff = FMOD_PRESET_OFF;

            dspenabled = !dspenabled;

            result = system->setReverbProperties(0, dspenabled ? &propon : &propoff);
            ERRCHECK(result);
        }

        result = system->update();
        ERRCHECK(result);
       
        system->getRecordPosition(0, &recordpos);
        ERRCHECK(result);

        recorddelta = (recordpos >= lastrecordpos) ? (recordpos - lastrecordpos) : (recordpos + soundlength - lastrecordpos);
        lastrecordpos = recordpos;
        
        samplesrecorded += recorddelta;
        if (samplesrecorded >= adjustedlatency && !channel)
        {
            result = system->playSound(sound, 0, false, &channel);
            ERRCHECK(result);
        }

        if (channel && recorddelta)
        {
            /*
                If the record driver steps the position of the record cursor in larger increments than the user
                defined latency value, then we should increase our latency value to match.
            */
            if (recorddelta < minrecorddelta)
            {
                minrecorddelta = recorddelta;
                if (adjustedlatency < recorddelta)
                {
                    adjustedlatency = recorddelta;
                }
            }

            result = channel->getPosition(&playpos, FMOD_TIMEUNIT_PCM);
            ERRCHECK(result);
            
	        /*
                Smooth total
            */
            {
                const float dampratio = 0.97f;
                static float total = 0;

                total *= dampratio;
                total += (recordpos >= playpos) ? (recordpos - playpos) : (recordpos + soundlength - playpos);
                smootheddelta = total * (1.0f - dampratio);
            }
           
            if (smootheddelta < (adjustedlatency - driftthreshold) || smootheddelta > soundlength / 2) /* If play cursor is catching up to record (or passed it), slow playback down */
            {
                channel->setFrequency(recordrate - (recordrate / 50)); /* Decrease speed by 2% */
            }
            else if (smootheddelta > (adjustedlatency + driftthreshold)) /* If play cursor is falling too far behind record, speed playback up */
            {
                channel->setFrequency(recordrate + (recordrate / 50)); /* Increase speed by 2% */
            }
            else
            {
                channel->setFrequency(recordrate); /* Otherwise set to normal rate */
            }
        }
       
        Common_Draw("==================================================");
        Common_Draw("Record Example.");
        Common_Draw("Copyright (c) Firelight Technologies 2004-2014.");
        Common_Draw("==================================================");
        Common_Draw("");
        Common_Draw("Adjust LATENCY define to compensate for stuttering");
        Common_Draw("");
        Common_Draw("Press %s to %s DSP effect", Common_BtnStr(BTN_ACTION1), dspenabled ? "disable" : "enable");
        Common_Draw("Press %s to quit", Common_BtnStr(BTN_QUIT));
        Common_Draw("");
        Common_Draw("Default playback latency: %4d (%dms)", (recordrate * LATENCY_MS) / 1000, LATENCY_MS);
        Common_Draw("Current playback latency: %4d (%dms)", (int)smootheddelta, (int)smootheddelta * 1000 / recordrate);
        Common_Draw("Record position: %6d", recordpos);
        Common_Draw("Play Position:   %6d", playpos);

        Common_Sleep(10);
    } while (!Common_BtnPress(BTN_QUIT));

    /*
        Shut down
    */
    result = sound->release();
    ERRCHECK(result);
    result = system->release();
    ERRCHECK(result);

    Common_Close();

    return 0;
}
