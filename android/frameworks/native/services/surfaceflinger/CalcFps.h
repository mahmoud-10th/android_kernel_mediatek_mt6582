/*
 * Add for calculate FPS,by guolz1@lenovo.com.
 * Version 1.0
 */

#include <stdio.h>
#include <utils/Singleton.h>
#include <cutils/properties.h>
#include <cutils/log.h>

namespace android {

class CalcFps : public Singleton<CalcFps> {
    public:
    CalcFps();
    ~CalcFps();

    void init();
    float fps();

    private:
        unsigned long mTimePeriod;
        float mTimeElapsed;
        unsigned int mPeriod;
        unsigned int mCurrentFrame;
        unsigned int mFrameArrivalSteps;
        nsecs_t      mIgnoreThresh_us;
        unsigned int mMargin_us;
				unsigned int mFrameArrivalsIndex;
        nsecs_t      mFrameArrivals[128];
        nsecs_t      mAccumFrameArrivals[50];
				float mNewFps;
				float mOldFps;
};

}; // namespace android

