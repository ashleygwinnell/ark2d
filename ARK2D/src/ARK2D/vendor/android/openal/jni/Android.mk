TARGET_PLATFORM := android-3

ROOT_PATH := $(call my-dir)

########################################################################################################

include $(CLEAR_VARS)

LOCAL_MODULE     := openal
LOCAL_ARM_MODE   := arm
LOCAL_PATH       := $(ROOT_PATH)
LOCAL_C_INCLUDES := $(LOCAL_PATH)/src $(LOCAL_PATH)/include $(LOCAL_PATH)/src/OpenAL32/Include
LOCAL_SRC_FILES  := src/OpenAL32/alAuxEffectSlot.c \
                    src/OpenAL32/alBuffer.c        \
                    src/OpenAL32/alDatabuffer.c    \
                    src/OpenAL32/alEffect.c        \
                    src/OpenAL32/alError.c         \
                    src/OpenAL32/alExtension.c     \
                    src/OpenAL32/alFilter.c        \
                    src/OpenAL32/alListener.c      \
                    src/OpenAL32/alSource.c        \
                    src/OpenAL32/alState.c         \
                    src/OpenAL32/alThunk.c         \
                    src/Alc/ALc.c                  \
                    src/Alc/alcConfig.c            \
                    src/Alc/alcEcho.c              \
                    src/Alc/alcModulator.c         \
                    src/Alc/alcReverb.c            \
                    src/Alc/alcRing.c              \
                    src/Alc/alcThread.c            \
                    src/Alc/ALu.c                  \
                    src/Alc/android.c              \
                    src/Alc/bs2b.c                 \
                    src/Alc/null.c                 \

LOCAL_CFLAGS     := -DAL_BUILD_LIBRARY -DAL_ALEXT_PROTOTYPES
LOCAL_LDLIBS     := -llog -Wl,-s

include $(BUILD_SHARED_LIBRARY)

########################################################################################################

