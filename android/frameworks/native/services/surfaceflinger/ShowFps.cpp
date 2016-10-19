#include <cutils/memory.h>

//#include <gui/ISurface.h>
#include <gui/ISurfaceComposer.h>
#include <gui/ISurfaceComposerClient.h>
#include <utils/Log.h>
#include <binder/IBinder.h>
#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>

#include <gui/Surface.h>
#include <gui/SurfaceComposerClient.h>
#include <ui/Rect.h>
#include <SkCanvas.h>
#include <SkBitmap.h>
#include <android/native_window.h>

using namespace android;


int main(int argc, char** argv)
{
    // set up the thread-pool
    sp<ProcessState> proc(ProcessState::self());
    ProcessState::self()->startThreadPool();

    // create a client to surfaceflinger
    sp<SurfaceComposerClient> client = new SurfaceComposerClient();
		sp<SurfaceControl> surface;

		sp<IBinder> display(client->getBuiltInDisplay(ISurfaceComposer::eDisplayIdMain));

	  surface = client->createSurface(String8("guolianzhu"),320, 240, PIXEL_FORMAT_RGBA_8888);
		sp<Surface> sf = surface->getSurface();

		const String16 name("SurfaceFlinger");
		sp<ISurfaceComposer> mComposerService;
		while (getService(name, &mComposerService) != NO_ERROR) {
			usleep(250000);
		}


    SurfaceComposerClient::openGlobalTransaction();
    surface->setLayer(0x80000);
		surface->setPosition(0,0);
		surface->show();
    SurfaceComposerClient::closeGlobalTransaction();

		char fpsname[256] = {0};

		ANativeWindow_Buffer info;
		while(1)
		{
			sf->lock(&info,NULL);
			SkBitmap bm;
			bm.setConfig(SkBitmap::kARGB_8888_Config,info.width,info.height);
			memset(info.bits,0,4*info.width*info.height);
			bm.setPixels(info.bits);
			SkPaint paint;

			paint.setARGB(255,255,0,0);
			paint.setTextSize(48);

			SkCanvas canvas(bm);

			double fps = mComposerService->getFps(display);
			memset(fpsname,0,sizeof(fpsname));
			sprintf(fpsname,"fps=%lf",fps);
			canvas.drawText(fpsname,8,30,130,paint);
			//canvas.drawColor(SK_ColorWHITE);
			sf->unlockAndPost();
			usleep(500*1000);
		}

    IPCThreadState::self()->joinThreadPool();
    return 0;
}
