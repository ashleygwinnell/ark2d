package org.%COMPANY_NAME%.%GAME_SHORT_NAME%;


import android.app.Application;

%ONESIGNAL_BLOCKSTART%
import com.onesignal.OneSignal;
%ONESIGNAL_BLOCKEND%


%GOOGLEANALYTICS_BLOCKSTART%
	import com.google.android.gms.analytics.GoogleAnalytics;
	import com.google.android.gms.analytics.Tracker;
%GOOGLEANALYTICS_BLOCKEND%

public class %GAME_CLASS_NAME%Application extends Application {

   @Override
   public void onCreate() {
      super.onCreate();
      %ONESIGNAL_BLOCKSTART%
      OneSignal.startInit(this).init();
      %ONESIGNAL_BLOCKEND%
   }

   %GOOGLEANALYTICS_BLOCKSTART%
	/**
	* Gets the default {@link Tracker} for this {@link Application}.
	* @return tracker
	*/
	synchronized public Tracker getDefaultTracker() {
		if (%GAME_CLASS_NAME%Activity.s_tracker == null) {
			GoogleAnalytics analytics = GoogleAnalytics.getInstance(this);
			// To enable debug logging use: adb shell setprop log.tag.GAv4 DEBUG
			%GAME_CLASS_NAME%Activity.s_tracker = analytics.newTracker(R.xml.analytics);
		}
		return %GAME_CLASS_NAME%Activity.s_tracker;
	}
	%GOOGLEANALYTICS_BLOCKEND%
}

