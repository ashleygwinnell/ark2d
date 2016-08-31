package org.%COMPANY_NAME%.%GAME_SHORT_NAME%;


import android.app.Application;

%ONESIGNAL_BLOCKSTART%
import com.onesignal.OneSignal;
%ONESIGNAL_BLOCKEND%


public class %GAME_CLASS_NAME%Application extends Application {

   @Override
   public void onCreate() {
      super.onCreate();
      %ONESIGNAL_BLOCKSTART%
      OneSignal.startInit(this).init();
      %ONESIGNAL_BLOCKEND%
   }
}

