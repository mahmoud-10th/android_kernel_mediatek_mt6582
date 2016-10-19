#include "CalcFps.h"

#ifdef CALC_FPS_SF

namespace android {

ANDROID_SINGLETON_STATIC_INSTANCE( CalcFps );

CalcFps::CalcFps() {
    init();
}

CalcFps::~CalcFps() {
}

void CalcFps::init() {
    mTimePeriod = 1000;
    mPeriod = 10;
    mIgnoreThresh_us = 500000;
    mFrameArrivalSteps = (mIgnoreThresh_us/ 16666);
    mMargin_us = 2000;
		mFrameArrivalsIndex = 0;
    for (unsigned int i = 0; i < 50; i++)
		{
        mAccumFrameArrivals[i] = 0;
		}
}

float CalcFps::fps() {
    static nsecs_t oldtime_us = 0;
		nsecs_t currtime_us = ns2us(systemTime());

    nsecs_t diff = currtime_us - oldtime_us;

    oldtime_us = currtime_us;

    if (diff > mIgnoreThresh_us)
		{
        return 0;
    }

    if (mFrameArrivalsIndex< 128) {
        mFrameArrivals[mFrameArrivalsIndex] = diff;
				mFrameArrivalsIndex++;
    }
		else
		{
			//TODO
			for (unsigned int i = 0;i < 128 ;i++)
			{
				ALOGE("guolianzhu0304 %lld",mFrameArrivals[i]);
			}
			ALOGE("\n\n");
			mFrameArrivalsIndex = 0;
		}

    mCurrentFrame++;

		unsigned int currstep = (diff + mMargin_us) / 16666;

		if (currstep < mFrameArrivalSteps)
		{
			mAccumFrameArrivals[currstep-1]++;
		}

		mTimeElapsed += ((float)diff/1000.0);

		if (mTimeElapsed >= mTimePeriod)
		{
			float fps = (1000.0 * mCurrentFrame)/(float)mTimeElapsed;
			mCurrentFrame = 0;
			mTimeElapsed = 0.0;
			for (unsigned int i = 0; i < ((mFrameArrivalSteps / 6) + 1); i++)
			{
				ALOGE("%lld %lld %lld %lld %lld %lld",
						mAccumFrameArrivals[i*6],
						mAccumFrameArrivals[i*6+1],
						mAccumFrameArrivals[i*6+2],
						mAccumFrameArrivals[i*6+3],
						mAccumFrameArrivals[i*6+4],
						mAccumFrameArrivals[i*6+5]);
			}
			ALOGE("\n\n");

			for (unsigned int i = 0; i < mFrameArrivalSteps; i++)
			{
				mAccumFrameArrivals[i] = 0;
			}

			mOldFps = mNewFps = fps;
			return mNewFps;
		}

		return mOldFps;
}

};//namespace android
#endif
