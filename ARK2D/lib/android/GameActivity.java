package org.%COMPANY_NAME%.%GAME_SHORT_NAME%;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.UnsupportedEncodingException;
import java.net.HttpURLConnection;
import java.net.URL;
import java.security.GeneralSecurityException;
import java.security.InvalidAlgorithmParameterException;
import java.security.InvalidKeyException;
import java.security.KeyFactory;
import java.security.NoSuchAlgorithmException;
import java.security.NoSuchProviderException;
import java.security.PublicKey;
import java.security.SecureRandom;
import java.security.spec.X509EncodedKeySpec;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.EnumSet;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.crypto.BadPaddingException;
import javax.crypto.Cipher;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.SecretKey;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;
import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.opengles.GL10;

%INAPPBILLING_BLOCKSTART%
import org.ashleygwinnell.nines.iab_util.IabHelper;
import org.ashleygwinnell.nines.iab_util.IabResult;
import org.ashleygwinnell.nines.iab_util.Inventory;
import org.ashleygwinnell.nines.iab_util.Purchase;
%INAPPBILLING_BLOCKEND%

import org.json.JSONException;
import org.json.JSONObject;

import android.app.Activity;
import android.app.AlertDialog;
%ANDROIDSDK16_LINECOMMENT% import android.app.NativeActivity;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.graphics.PixelFormat;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.opengl.GLSurfaceView;
import android.opengl.GLU;
import android.os.Bundle;
import android.os.Environment;
import android.os.Looper;
import android.os.Vibrator;
import android.util.Base64;
import android.util.Log;
%ANDROIDSDK16_LINECOMMENT% import android.view.InputDevice;
%ANDROIDSDK16_LINECOMMENT% import android.view.InputDevice.MotionRange;
import android.view.KeyEvent;
import android.view.MotionEvent;
%ANDROIDSDK16_LINECOMMENT% import android.view.InputEvent;
import android.view.View;
import android.view.WindowManager;
import android.view.inputmethod.InputMethodManager;
import android.webkit.MimeTypeMap;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.Toast;

import com.google.android.gms.games.Games;

%OUYA_BLOCKSTART%

import tv.ouya.console.api.*;

%OUYA_BLOCKEND%


%FIRETV_BLOCKSTART%

import android.hardware.input.InputManager;
import android.hardware.input.InputManager.InputDeviceListener;

import com.amazon.device.*;
import com.amazon.device.gamecontroller.*;

import com.amazon.ags.api.AGResponseCallback;
import com.amazon.ags.api.AGResponseHandle;
import com.amazon.ags.api.AmazonGamesCallback;
import com.amazon.ags.api.AmazonGamesClient;
import com.amazon.ags.api.AmazonGamesFeature;
import com.amazon.ags.api.AmazonGamesStatus;
import com.amazon.ags.api.achievements.AchievementsClient;
import com.amazon.ags.api.achievements.UpdateProgressResponse;
import com.amazon.ags.api.leaderboards.GetPlayerScoreResponse;
import com.amazon.ags.api.leaderboards.LeaderboardsClient;
import com.amazon.ags.api.leaderboards.SubmitScoreResponse;
import com.amazon.ags.api.overlay.PopUpLocation;
import com.amazon.ags.constants.LeaderboardFilter;

%FIRETV_BLOCKEND%


public class %GAME_CLASS_NAME%Activity extends BaseGameActivity

%FIRETV_BLOCKSTART%
	implements InputDeviceListener
%FIRETV_BLOCKEND%

{

	// const variables copied from C++ Util/Callbacks class.
	public static final int CALLBACK_ANDROID_LICENSING_ALLOW = 1;
	public static final int CALLBACK_ANDROID_LICENSING_DISALLOW = 2;
	public static final int CALLBACK_ANDROID_SIGNIN_SUCCESSFUL = 11;
	public static final int CALLBACK_ANDROID_SIGNIN_UNSUCCESSFUL = 12;
	public static final int CALLBACK_ANDROID_THREAD_START = 21;

	public static final int CALLBACK_GAMECENTER_SIGNIN_SUCCESSFUL = 31;

	public static final int CALLBACK_OUYA_LICENSING_ALLOW = 41;
	public static final int CALLBACK_OUYA_LICENSING_DISALLOW = 42;

	public static final int CALLBACK_STEAM_OPENURLINBROWSER_LINUX = 51;

	public static final int CALLBACK_GAMECIRCLE_INIT_SUCCESS = 61;
	public static final int CALLBACK_GAMECIRCLE_INIT_FAIL = 62;
	public static final int CALLBACK_GAMECIRCLE_SUBMITHIGHSCORE_SUCCESS = 63;
	public static final int CALLBACK_GAMECIRCLE_SUBMITHIGHSCORE_FAIL = 64;
	public static final int CALLBACK_GAMECIRCLE_UNLOCKACHIEVEMENT_SUCCESS = 65;
	public static final int CALLBACK_GAMECIRCLE_UNLOCKACHIEVEMENT_FAIL = 66;

	%INAPPBILLING_BLOCKSTART%

		public IabHelper mHelper;
		public IabHelper.OnIabPurchaseFinishedListener mPurchaseFinishedListener;
		public IabHelper.QueryInventoryFinishedListener mReceivedInventoryListener;
		public IabHelper.OnConsumeFinishedListener mConsumeFinishedListener;
		public boolean _iabIsSetup = false;

	%INAPPBILLING_BLOCKEND%

	public static %GAME_CLASS_NAME%Activity s_activity;
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {

		super.onCreate(savedInstanceState);
        s_activity = this;

        String gameOrientation = "%GAME_ORIENTATION%";
        if (gameOrientation.equals("landscape")) {
        	setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
        } else if (gameOrientation.equals("portrait")) {
        	setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
        } else {

        }

        mGLView = new %GAME_CLASS_NAME%View(this);
        setContentView(mGLView);

        %FIRETV_BLOCKSTART%
        	InputManager im = (InputManager) getSystemService(Context.INPUT_SERVICE);
        	im.registerInputDeviceListener(this, null);
        	im.getInputDevice(0);
        %FIRETV_BLOCKEND%

        %OUYA_BLOCKSTART%
    	OuyaFacade.getInstance().init(this, ouyaDeveloperId);
    	OuyaInputMapper.init(this);
    	OuyaController.showCursor(false);

    	ouya_init();
    	%OUYA_BLOCKEND%

    	%INAPPBILLING_BLOCKSTART%
	    	String base64EncodedPublicKey = "%INAPPBILLING_PUBLICKEY%";

			mHelper = new IabHelper(this, base64EncodedPublicKey);

			mHelper.startSetup(new IabHelper.OnIabSetupFinishedListener() {
				public void onIabSetupFinished(IabResult result) {
				   if (!result.isSuccess()) {
					   Log.d("game", "In-app Billing setup failed: " + result);
				   } else {
					   Log.d("game", "In-app Billing is set up OK");
					   _iabIsSetup = true;

					   //mHelper.launchPurchaseFlow(s_activity, "tipjar.medium", 10001, mPurchaseFinishedListener, "mypurchasetoken");

				   }
				}
			});

			mPurchaseFinishedListener = new IabHelper.OnIabPurchaseFinishedListener() {
				public void onIabPurchaseFinished(IabResult result, Purchase purchase) {
					if (result.isFailure()) { // Handle error
						return;
					} //else if (purchase.getSku().equals("ITEM_SKU")) {
						//iab_consumeItem(); //buyButton.setEnabled(false);
					//}
				}
			};

			mReceivedInventoryListener = new IabHelper.QueryInventoryFinishedListener() {
				public void onQueryInventoryFinished(IabResult result, Inventory inventory) {
					if (result.isFailure()) {
						// Handle failure
					} else {
						// mHelper.consumeAsync(inventory.getPurchase("ITEM_SKU"), mConsumeFinishedListener);
					}
				}
			};
			mConsumeFinishedListener = new IabHelper.OnConsumeFinishedListener() {
				public void onConsumeFinished(Purchase purchase, IabResult result) {
					if (result.isSuccess()) { //clickButton.setEnabled(true);

					} else {
						// handle error
					}
				}
			};


    	%INAPPBILLING_BLOCKEND%
    }

    @Override
    protected void onActivityResult(int request, int response, Intent data) {
        super.onActivityResult(request, response, data);

		%INAPPBILLING_BLOCKSTART%
		if (mHelper != null) {
			mHelper.handleActivityResult(request, response, data);
		}
		%INAPPBILLING_BLOCKEND%


    }

    %INAPPBILLING_BLOCKSTART%


    	@Override
		public void onDestroy() {
			super.onDestroy();
			if (mHelper != null) mHelper.dispose();
			mHelper = null;
		}
    	public void iab_consumeItem() {
			mHelper.queryInventoryAsync(mReceivedInventoryListener);
		}


		public static boolean googleplaybilling_isSetup() {
			return s_activity._iabIsSetup;
		}

		public static void googleplaybilling_startPurchase(String item, int referenceNumber, String extraThing) {
			s_activity.mHelper.launchPurchaseFlow(s_activity, item, referenceNumber, s_activity.mPurchaseFinishedListener, extraThing);
		}



	%INAPPBILLING_BLOCKEND%

    %OUYA_BLOCKSTART%
	    @Override
	    protected void onDestroy() {
	    	OuyaInputMapper.shutdown(this);
	    	OuyaFacade.getInstance().shutdown();
	    	super.onDestroy();
	    }
    %OUYA_BLOCKEND%

    // --------------------------------
    //
    // GOOGLE PLAY GAME SERVICES
    //
    // --------------------------------
    %GAME_SERVICES_BLOCKSTART%

    	public static void googleplaygameservices_signIn() {
    		%GAME_CLASS_NAME%Activity.createLooper();
	        s_activity.beginUserInitiatedSignIn();
	    }
	    public static void googleplaygameservices_signOut() {
	        s_activity.signOut();
	    }
	    public static boolean googleplaygameservices_isSignedIn() {
	        return s_activity.isSignedIn();
	    }
	    //public static boolean googleplaygameservices_isSigningIn() {
	    //    return (s_activity.mHelper.mState == GameHelper.STATE_CONNECTING); //ARK2DGooglePlayServices.isConnecting();
	    //}

	    public static void googleplaygameservices_viewAchievements()
	    {
	        if (googleplaygameservices_isSignedIn()) {
	            s_activity.startActivityForResult( Games.Achievements.getAchievementsIntent( s_activity.getApiClient() ), 1000); // REQUEST_ACHIEVEMENTS
	        } else {
	            googleplaygameservices_signIn();
	        }
	    }
	    public static void googleplaygameservices_unlockAchievement(String id)
	    {
	        if (googleplaygameservices_isSignedIn()) {
	            Games.Achievements.unlock(s_activity.getApiClient(), id);
	        }
	    }
	    public static void googleplaygameservices_viewScores(String id)
	    {
	        if (googleplaygameservices_isSignedIn()) {
	            if (id.length() == 0) {
	                s_activity.startActivityForResult( Games.Leaderboards.getAllLeaderboardsIntent(s_activity.getApiClient()), 1002); //REQUEST_LEADERBOARD);
	            } else {
	                s_activity.startActivityForResult( Games.Leaderboards.getLeaderboardIntent( s_activity.getApiClient(),  id), 1001); //REQUEST_LEADERBOARD);
	            }
	        } else {
	            googleplaygameservices_signIn();
	        }
	    }
	    public static void googleplaygameservices_submitScore(String id, int score)
	    {
	        if (googleplaygameservices_isSignedIn()) {
	            Games.Leaderboards.submitScore(s_activity.getApiClient(), id, score);
	        }
	    }

	    @Override
	    public void onSignInSucceeded() {
	        Log.i("playservices", "sign-in succeeded");
	        //if (mHelper.mUserInitiatedSignIn) {
	            %GAME_CLASS_NAME%Renderer.nativeCallbackById(%GAME_CLASS_NAME%Activity.CALLBACK_ANDROID_SIGNIN_SUCCESSFUL);
	        //}
	    }

	    @Override
	    public void onSignInFailed() {
	        Log.i("playservices", "sign-in failed");
	        %GAME_CLASS_NAME%Renderer.nativeCallbackById(%GAME_CLASS_NAME%Activity.CALLBACK_ANDROID_SIGNIN_UNSUCCESSFUL);
	    }

	%GAME_SERVICES_BLOCKEND%

	public static void createLooper() {
		try {
			Looper.prepare();
		} catch(RuntimeException e) {
			// ...
		}
	}


	// --------------------------------
	%FIRETV_BLOCKSTART%
		// ---------
		// DOCS:
		// https://developer.amazon.com/appsandservices/apis/engage/gamecircle/docs/initialize-android
		// ---------
		AmazonGamesClient agsClient;
		static boolean agsConnected = false;
		AmazonGamesCallback callback = new AmazonGamesCallback() {
			@Override
			public void onServiceNotReady(AmazonGamesStatus status) {
				if (status == AmazonGamesStatus.CANNOT_INITIALIZE) {
					Log.i("game", "AmazonGamesStatus.CANNOT_INITIALIZE");
				}
				%GAME_CLASS_NAME%Renderer.nativeCallbackById(%GAME_CLASS_NAME%Activity.CALLBACK_GAMECIRCLE_INIT_FAIL);
			}
			@Override
			public void onServiceReady(AmazonGamesClient amazonGamesClient) {
				agsClient = amazonGamesClient;
				agsConnected = true;
				agsClient.setPopUpLocation(PopUpLocation.BOTTOM_CENTER);
				%GAME_CLASS_NAME%Renderer.nativeCallbackById(%GAME_CLASS_NAME%Activity.CALLBACK_GAMECIRCLE_INIT_SUCCESS);
			}
		};

		//list of features your game uses (in this example, achievements and leaderboards)
		EnumSet<AmazonGamesFeature> myGameFeatures = EnumSet.of(
			AmazonGamesFeature.Achievements,
			AmazonGamesFeature.Leaderboards
		);

		public static void firetv_connect() {
			//Looper.prepare();
			AmazonGamesClient.initialize(s_activity, s_activity.callback, s_activity.myGameFeatures);
		}


		// achievements
		public static void firetv_viewAchievements() {
			Log.d("game", "firetv_viewAchievements");
			createLooper();
			if (!agsConnected) { firetv_connect(); }
			if (!isNetworkAvailable()) {
				String s = "No Internet Connection. Achievements not available. ";
				openErrorDialog(s);
				Log.d("game", s);
				return;
			}


			AchievementsClient acClient = s_activity.agsClient.getAchievementsClient();
			acClient.showAchievementsOverlay((Object[]) null);
		}
		public static void firetv_unlockAchievement(String achievementId) {
			final String achievementId2 = achievementId;
			Log.d("game", "firetv_unlockAchievement: " + achievementId);
			createLooper();
			if (!agsConnected) { firetv_connect(); }

			AchievementsClient acClient = s_activity.agsClient.getAchievementsClient();
			AGResponseHandle<UpdateProgressResponse> handle = acClient.updateProgress(achievementId, 100.0f);
			handle.setCallback(new AGResponseCallback<UpdateProgressResponse>() {

				@Override
				public void onComplete(UpdateProgressResponse result) {
					if (result.isError()) {
						// Add optional error handling here. Not strictly required
						// since retries and on-device request caching are automatic.
						Log.d("game", "firetv_unlockAchievement: " + achievementId2 + ". FAIL.");
						%GAME_CLASS_NAME%Renderer.nativeCallbackById(%GAME_CLASS_NAME%Activity.CALLBACK_GAMECIRCLE_UNLOCKACHIEVEMENT_FAIL);
					} else {
						// Continue game flow.
						Log.d("game", "firetv_unlockAchievement: " + achievementId2 + ". SUCCESS.");
						%GAME_CLASS_NAME%Renderer.nativeCallbackById(%GAME_CLASS_NAME%Activity.CALLBACK_GAMECIRCLE_UNLOCKACHIEVEMENT_SUCCESS);
					}
				}
			});
		}

		public static void firetv_viewScores() {
			Log.d("game", "firetv_viewScores");
			createLooper();
			if (!agsConnected) { firetv_connect(); }
			if (!isNetworkAvailable()) {
				String s = "No Internet Connection. Highscores not available. ";
				openErrorDialog(s);
				Log.d("game", s);
				return;
			}

			LeaderboardsClient lbClient = s_activity.agsClient.getLeaderboardsClient();
			lbClient.showLeaderboardsOverlay();
		}
		public static void firetv_viewScores(String id) {
			Log.d("game", "firetv_viewScores");
			createLooper();
			if (!agsConnected) { firetv_connect(); }
			if (!isNetworkAvailable()) {
				String s = "No Internet Connection. Highscores not available. ";
				openErrorDialog(s);
				Log.d("game", s);
				return;
			}

			LeaderboardsClient lbClient = s_activity.agsClient.getLeaderboardsClient();
			lbClient.showLeaderboardOverlay(id);
		}

		public static void firetv_submitScore(String id, int score) {
			final String id2 = id;
			final int score2 = score;
			Log.d("game", "firetv_submitScore: " + id + ", " + score);
			createLooper();
			if (!agsConnected) { firetv_connect(); }


			// Replace YOUR_LEADERBOARD_ID with an actual leaderboard ID from your game.
			final LeaderboardsClient lbClient = s_activity.agsClient.getLeaderboardsClient();
			AGResponseHandle<SubmitScoreResponse> handle = lbClient.submitScore(id, (long) score);

			// Optional callback to receive notification of success/failure.
			handle.setCallback(new AGResponseCallback<SubmitScoreResponse>() {

				@Override
				public void onComplete(SubmitScoreResponse result) {
					if (result.isError()) {

						// Add optional error handling here. Not strictly required
						// since retries and on-device request caching are automatic.
						Log.d("game", "firetv_submitScore result: " + id2 + ", score: " + score2 + ". FAIL.");
						%GAME_CLASS_NAME%Renderer.nativeCallbackById(%GAME_CLASS_NAME%Activity.CALLBACK_GAMECIRCLE_SUBMITHIGHSCORE_FAIL);
					} else {

						// Continue game flow.
						// Map<LeaderboardFilter, Integer> map = result.getNewRank();
						// int newRank = map.get(LeaderboardFilter.GLOBAL_ALL_TIME).intValue();

						// Log.d("game", "firetv_submitScore result: " + id2 + ", score: " + score2 + ". rank: " + newRank + " . SUCCESS.");
						// %GAME_CLASS_NAME%Renderer.nativeCallbackByIdIntParam(%GAME_CLASS_NAME%Activity.CALLBACK_GAMECIRCLE_SUBMITHIGHSCORE_SUCCESS, newRank);


						lbClient.getLocalPlayerScore(id2, LeaderboardFilter.GLOBAL_ALL_TIME, null).setCallback(new AGResponseCallback<GetPlayerScoreResponse>() {
							@Override
							public void onComplete(GetPlayerScoreResponse result) {
								if (result.isError()) {
									// Handle getLocalPlayerScore error
									Log.d("game", "firetv_submitScore result: " + id2 + ", score: " + score2 + ". FAIL.");
									%GAME_CLASS_NAME%Renderer.nativeCallbackById(%GAME_CLASS_NAME%Activity.CALLBACK_GAMECIRCLE_SUBMITHIGHSCORE_FAIL);
								} else {
									int rank = result.getRank();
									//long score = result.getScoreValue();
									// Do work with weekly high score

									Log.d("game", "firetv_submitScore result: " + id2 + ", score: " + score2 + ". rank: " + rank + " . SUCCESS.");
									%GAME_CLASS_NAME%Renderer.nativeCallbackByIdIntParam(%GAME_CLASS_NAME%Activity.CALLBACK_GAMECIRCLE_SUBMITHIGHSCORE_SUCCESS, rank);
								}
							}
						});

					}
				}
			});

			if (!isNetworkAvailable()) {
				String s = "No Internet Connection. Highscores not available. ";
				Log.d("game", s);
				openErrorDialog(s);
				%GAME_CLASS_NAME%Renderer.nativeCallbackById(%GAME_CLASS_NAME%Activity.CALLBACK_GAMECIRCLE_SUBMITHIGHSCORE_FAIL);
				return;
			}
		}

	%FIRETV_BLOCKEND%

	%!FIRETV_BLOCKSTART%
		public static void firetv_viewAchievements() { Log.d("game", "firetv_viewAchievements called in non-Amazon build."); }
		public static void firetv_viewScores() { Log.d("game", "firetv_viewScores called in non-Amazon build."); }
		public static void firetv_unlockAchievement(String achievementId) { Log.d("game", "firetv_unlockAchievement called in non-Amazon build."); }
	%!FIRETV_BLOCKEND%


    @Override
    public void onBackPressed() {
    	//super.onBackPressed();
    	%GAME_CLASS_NAME%Renderer.nativeBackPressed();
    }

    @Override
    protected void onPause() {
    	Log.i("game", "Activity Pause");
    	super.onPause();
    	mGLView.onPause();

    	%FIRETV_BLOCKSTART%
			if (agsClient != null) {
				agsClient.release();
			}
		%FIRETV_BLOCKEND%
    }

    @Override
    protected void onStart() {
    	Log.i("game", "Activity Resume");
    	super.onStart();
    	//mGLView.onStart();
    }

    @Override
    protected void onResume() {
    	Log.i("game", "Activity Resume");
    	super.onResume();
    	mGLView.onResume();

    	%FIRETV_BLOCKSTART%
			firetv_connect();
		%FIRETV_BLOCKEND%
    }

   	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
	    //Log.i("game", "Key Down: " + keyCode);

		%ANDROIDSDK16_BLOCKSTART%
		    int deviceId = %GAME_CLASS_NAME%View.getInputDeviceState(event);
		    if (deviceId != -1) {
				//final int keyCode = event.getKeyCode();
				if (%GAME_CLASS_NAME%View.isGameKey(keyCode)) {
					if (event.getRepeatCount() == 0) {
						//final String symbolicName = KeyEvent.keyCodeToString(keyCode);
						%GAME_CLASS_NAME%Renderer.nativeGamepadKeyDown(deviceId, keyCode);
					}
					return true;
				}
		    }
		%ANDROIDSDK16_BLOCKEND%

	    /*switch (keyCode) {
	    	case KeyEvent.KEYCODE_BACK:
				// Sample for handling the Menu button globally
				return true;
	    }*/
	    %GAME_CLASS_NAME%Renderer.nativeKeyDown(keyCode, new String(Character.toString((char)event.getUnicodeChar())));
	    return false;
	}
	@Override
	public boolean onKeyUp(int keyCode, KeyEvent event) {
 		//Log.i("game", "Key Up: " + keyCode);

		%ANDROIDSDK16_BLOCKSTART%
	 		int deviceId = %GAME_CLASS_NAME%View.getInputDeviceState(event);
		    if (deviceId != -1) {
				//final int keyCode = event.getKeyCode();
				if (%GAME_CLASS_NAME%View.isGameKey(keyCode)) {
					if (event.getRepeatCount() == 0) {
						//final String symbolicName = KeyEvent.keyCodeToString(keyCode);
						%GAME_CLASS_NAME%Renderer.nativeGamepadKeyUp(deviceId, keyCode);
					}
					return true;
				}
		    }
	    %ANDROIDSDK16_BLOCKEND%

 		%GAME_CLASS_NAME%Renderer.nativeKeyUp(keyCode, new String(Character.toString((char)event.getUnicodeChar())));
 		return false;
	}

	%ANDROIDSDK16_BLOCKSTART%
		@Override
		public boolean dispatchGenericMotionEvent(MotionEvent event) {
			// Check that the event came from a joystick since a generic motion event could be almost anything.
			if ((event.getSource() & InputDevice.SOURCE_CLASS_JOYSTICK) != 0
				&& event.getAction() == MotionEvent.ACTION_MOVE)
			{
				// Update device state for visualization and logging.
				int deviceId = %GAME_CLASS_NAME%View.getInputDeviceState(event);
				if (deviceId != -1) {
					//Log.i("game", "joystick motion");
					int numAxes = %GAME_CLASS_NAME%Renderer.nativeGamepadNumAxes(deviceId);
					for (int i = 0; i < numAxes; i++) {
						int axis = %GAME_CLASS_NAME%Renderer.nativeGamepadAxisAtIndex(deviceId, i);

						float value = event.getAxisValue(axis);
	%ANDROIDSDK16_BLOCKEND%

						%OUYA_BLOCKSTART%
							// Modify values for Ouya (again)
							if (axis == OuyaController.AXIS_L2 || axis == OuyaController.AXIS_R2) {
								value = (value - 0.5f)*2;
							}
						%OUYA_BLOCKEND%

						%FIRETV_BLOCKSTART%
							// Modify values for Fire TV (just in case?)
							if (axis == GameController.AXIS_TRIGGER_LEFT || axis == GameController.AXIS_TRIGGER_RIGHT) {
								value = (value - 0.5f)*2;
							}
						%FIRETV_BLOCKEND%

	%ANDROIDSDK16_BLOCKSTART%

						%GAME_CLASS_NAME%Renderer.nativeGamepadAxisChanged(deviceId, axis, value);
					}
				}
			}
			return true; // super.dispatchGenericMotionEvent(event);
		}
	%ANDROIDSDK16_BLOCKEND%

	public static int dips(int i) {
		// Get the screen's density scale
		final float scale = s_activity.getResources().getDisplayMetrics().density;
		final int padd = (int) (i * scale + 0.5f);
		return padd;
	}

	public static String s_inputDialogText = new String("");
	public static void openInputDialog(int jniCallbackId, String title, String defaultText) {
		final int f_jniCallbackId = jniCallbackId;
		final String f_defaultText = defaultText;

		final AlertDialog.Builder alert2 = new AlertDialog.Builder(s_activity);
		final EditText input = new EditText(s_activity);

		alert2.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
			public void onClick(DialogInterface dialog, int whichButton) {
				s_inputDialogText = f_defaultText;
				dialog.cancel();
			}
        });
        alert2.setPositiveButton("Ok", new DialogInterface.OnClickListener() {
        	public void onClick(DialogInterface dialog, int whichButton) {
        		String value = input.getText().toString().trim();

        		System.out.println("OK pressed" + value);
        		s_inputDialogText = value;
        		%GAME_CLASS_NAME%Renderer.nativeCallbackById(f_jniCallbackId);
        	}
        });

		final AlertDialog alert = alert2.create();
		final FrameLayout fl = new FrameLayout(s_activity);
		fl.setPadding(dips(10), 0, dips(10), 0);

		input.setOnFocusChangeListener(new View.OnFocusChangeListener() {
			@Override
			public void onFocusChange(View v, boolean hasFocus) {
				if (hasFocus) {
					alert.getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_STATE_ALWAYS_VISIBLE);
					input.setSelection(input.getText().length());
				}
			}
		});
		input.setText(defaultText);
        input.setSingleLine(true);
        alert.setTitle(title);

        fl.addView(input);
        alert.setView(fl);

        alert.show();
	}
	public static String getInputDialogText() {
		return s_inputDialogText;
	}

	public static void container_close() {
		Log.i("game", "finishing activity");
		s_activity.finish();
	}

	public static void openSoftwareKeyboard() {
		Log.i("game", "keyboard open java");
		InputMethodManager imm = (InputMethodManager) s_activity.getSystemService(Context.INPUT_METHOD_SERVICE);
		imm.toggleSoftInputFromWindow(s_activity.mGLView.getApplicationWindowToken(), InputMethodManager.SHOW_FORCED, 0);
	}
	public static void closeSoftwareKeyboard() {
		Log.i("game", "keyboard close java");
		InputMethodManager imm = (InputMethodManager) s_activity.getSystemService(Context.INPUT_METHOD_SERVICE);
		imm.hideSoftInputFromWindow(s_activity.mGLView.getApplicationWindowToken(), 0);
	}
	public static String urlRequest(String urlString) {
		Log.i("game", "url request java: " + urlString);
		String response = new String("");
		try {
			URL url = new URL(urlString);
			HttpURLConnection connection = (HttpURLConnection) url.openConnection();
			connection.connect();
			InputStream stream = connection.getInputStream();
			BufferedInputStream buff = new BufferedInputStream(stream);
			int character = -1;
			while ((character = buff.read()) != -1) {
				response += (char) character;
			}
			stream.close();
			return response;
		} catch (IOException e) {
			Log.i("game", "IO Exception: " + e.getMessage());
			Log.i("game", "Have you added \"INTERNET\" to {'android':{'permissions':[]}} to the ARK2D configuration?");

			s_activity.runOnUiThread(new Runnable() {
				public void run() {
					//Toast.makeText(s_activity, "No Internet Connection", Toast.LENGTH_SHORT).show();
				}
			});
		}
		return response;
	}

	public static boolean isNetworkAvailable()
	{
	    boolean haveConnectedWifi = false;
		boolean haveConnectedMobile = false;
		boolean haveConnectedEthernet = false;

		ConnectivityManager cm = (ConnectivityManager) s_activity.getSystemService(Context.CONNECTIVITY_SERVICE);
		NetworkInfo[] netInfo = cm.getAllNetworkInfo();
		for (NetworkInfo ni : netInfo) {
			if (ni.getTypeName().equalsIgnoreCase("WIFI")) {
				if (ni.isConnected()) {
					haveConnectedWifi = true;
				}
			}
			if (ni.getTypeName().equalsIgnoreCase("MOBILE")) {
				if (ni.isConnected()) {
					haveConnectedMobile = true;
				}
			}
			if (ni.getTypeName().equalsIgnoreCase("ETHERNET")) {
				if (ni.isConnected()) {
					haveConnectedEthernet = true;
				}
			}
		}
		return (haveConnectedWifi || haveConnectedMobile || haveConnectedEthernet);
	}

	public static void openBrowserToUrl(String url)
	{
		if (!url.startsWith("http://") && !url.startsWith("https://")) {
			url = "http://" + url;
		}

		Intent browserIntent = new Intent(Intent.ACTION_VIEW, Uri.parse( url ));
		s_activity.startActivity(browserIntent);
	}

	public static void openGooglePlayStore(String packageName) {
		try {
		    s_activity.startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse("market://details?id=" + packageName)));
		} catch (android.content.ActivityNotFoundException anfe) {
		    s_activity.startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse("http://play.google.com/store/apps/details?id=" + packageName)));
		}
	}
	public static void openErrorDialog(String text)
	{
		if (s_activity != null) {
			final String text2 = text;
			s_activity.runOnUiThread(new Runnable() {

				public void run() {
					AlertDialog alertDialog = new AlertDialog.Builder(s_activity).create();

					// Setting Dialog Title
					alertDialog.setTitle("Error:");

					// Setting Dialog Message
					alertDialog.setMessage(text2);

					// Setting Icon to Dialog
					//alertDialog.setIcon(R.drawable.tick);

					// Setting OK Button
					alertDialog.setButton(AlertDialog.BUTTON_NEUTRAL, "OK", new DialogInterface.OnClickListener() {
						public void onClick(DialogInterface dialog, int which) {

						}
					});

			        // Showing Alert Message
			        alertDialog.show();
			    }
			});
	    }
	}

	public static void openGalleryToImageUrl(String url)
	{
		try {

			if (!new File(url).exists()) {
				System.err.println("File (" + url + ") does not exist");
				return;
			}

			if (url.startsWith("/")) {
				url = url.substring(1, url.length());
			}
			//s_activity.startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse("content://" + url)));

			Uri uri =  Uri.fromFile(new File(url));
			Intent intent = new Intent(android.content.Intent.ACTION_VIEW);
			String mime = "*/*";
			MimeTypeMap mimeTypeMap = MimeTypeMap.getSingleton();
			if (mimeTypeMap.hasExtension(
					MimeTypeMap.getFileExtensionFromUrl(uri.toString())))
			    mime = mimeTypeMap.getMimeTypeFromExtension(
			    		MimeTypeMap.getFileExtensionFromUrl(uri.toString()));
			intent.setDataAndType(uri,mime);
			s_activity.startActivity(intent);

		} catch (Exception e) {
			e.printStackTrace();
		}
	}



    // THREAD?!
    public static void thread_start(final int callback_id, final int thread_id)
    {
        Log.i("game", "Java: Thread Start Called");
        new Thread(new Runnable() {
            public void run() {
                Log.i("game", "Java: Thread Started");
                %GAME_CLASS_NAME%Renderer.nativeCallbackByIdIntParam(callback_id, thread_id);
                 Log.i("game", "Java: Thread Finished");
            }
        }).start();
    }



    // ----------------
    // OUYA
    // ----------------
    %OUYA_BLOCKSTART%

    	public static final String ouyaDeveloperId = "%OUYA_DEVELOPERID%";
		public static String ouyaUsername = new String("");
		public static PublicKey ouyaPK = null;
		public static boolean ouyaPurchasedEntitlement = false;

		public static final Map<String, Product> s_productsByName = new HashMap<String, Product>();
		public static final Map<String, Product> s_outstandingPurchaseRequests = new HashMap<String, Product>();
		public static final List<Purchasable> PRODUCT_ID_LIST = Arrays.asList(new Purchasable("%OUYA_ENTITLEMENT_ID%"));
		public static CancelIgnoringOuyaResponseListener<String> s_purchaseListener = null;
		public static CancelIgnoringOuyaResponseListener<ArrayList<Product>> s_productListListener = null;
		public static CancelIgnoringOuyaResponseListener<String> s_receiptsListener = null;
		public static CancelIgnoringOuyaResponseListener<GamerInfo> s_gamerInfoListener = null;

		public static void ouya_init() {
			Log.w("game", "ouya_init");

		    // Create a PublicKey object from the key data downloaded from the developer portal.
	        try {
	            // Read in the key.der file (downloaded from the developer portal)
	            InputStream inputStream = s_activity.getResources().openRawResource(R.raw.key);
	            byte[] applicationKey = new byte[inputStream.available()];
	            inputStream.read(applicationKey);
	            inputStream.close();

	            // Create a public key
	            X509EncodedKeySpec keySpec = new X509EncodedKeySpec(applicationKey);
	            KeyFactory keyFactory = KeyFactory.getInstance("RSA");
	            ouyaPK = keyFactory.generatePublic(keySpec);
	        } catch (Exception e) {
	            Log.e("game", "Unable to create encryption key", e);
	        }

	        s_gamerInfoListener = new CancelIgnoringOuyaResponseListener<GamerInfo>() {
		    	public void onSuccess(GamerInfo gi) {
					Log.w("game", "requestGamerInfo success!");
					Log.w("game", "username: " + gi.getUsername());
					ouyaUsername = gi.getUsername();
				}
				public void onFailure(int errorCode, String errorMessage, Bundle optionalData) {
					Log.w("game", "requestGamerInfo failure...");
					Log.w("game", "errorCode: " + errorCode);
					Log.w("game", "errorMessage: " + errorMessage);
				}
		    };

	        s_receiptsListener = new CancelIgnoringOuyaResponseListener<String>() {
	            @Override
	            public void onSuccess(String receiptResponse) {
	                OuyaEncryptionHelper helper = new OuyaEncryptionHelper();
	                List<Receipt> receipts = null;
	                try {
	                    JSONObject response = new JSONObject(receiptResponse);
	                    receipts = helper.decryptReceiptResponse(response, ouyaPK);
	                } catch (Exception e) {
	                    throw new RuntimeException(e);
	                }

	                for (Receipt r : receipts) {
	                    Log.d("Receipt", "You have purchased: " + r.getIdentifier() + " for " + r.getFormattedPrice());
	                    if (r.getIdentifier().equals("%OUYA_ENTITLEMENT_ID%")) {
	                    	ouyaPurchasedEntitlement = true;
	                    }
	                }

	                if (ouyaPurchasedEntitlement) {
	                	%GAME_CLASS_NAME%Renderer.nativeCallbackById(CALLBACK_OUYA_LICENSING_ALLOW);
	                }
	            }

	            @Override
	            public void onFailure(int errorCode, String errorMessage, Bundle errorBundle) {
	                Log.d("Error", errorMessage);
	                ouyaPurchasedEntitlement = false;
	                %GAME_CLASS_NAME%Renderer.nativeCallbackById(CALLBACK_OUYA_LICENSING_DISALLOW);
	            }
	        };

	        s_purchaseListener = new CancelIgnoringOuyaResponseListener<String>() {
	            @Override
	            public void onSuccess(String result) {
	                try {
	                    OuyaEncryptionHelper helper = new OuyaEncryptionHelper();

	                    JSONObject response = new JSONObject(result);

	                    String id = helper.decryptPurchaseResponse(response, ouyaPK);
	                    Product storedProduct;
	                    synchronized (s_outstandingPurchaseRequests) {
	                        storedProduct = s_outstandingPurchaseRequests.remove(id);
	                    }
	                    if(storedProduct == null) {
	                        onFailure(
	                            OuyaErrorCodes.THROW_DURING_ON_SUCCESS,
	                            "No purchase outstanding for the given purchase request",
	                            Bundle.EMPTY);
	                        return;
	                    }

	                    Log.d("Purchase", "Congrats you bought: " + storedProduct.getName());

	                    ouyaPurchasedEntitlement = true;
	                    %GAME_CLASS_NAME%Renderer.nativeCallbackById(CALLBACK_OUYA_LICENSING_ALLOW);
	                } catch (Exception e) {
	                    Log.e("Purchase", "Your purchase failed.", e);
	                }
	            }

	            @Override
	            public void onFailure(int errorCode, String errorMessage, Bundle errorBundle) {
	                Log.d("Error", errorMessage);
	            }
	        };

	        s_productListListener = new CancelIgnoringOuyaResponseListener<ArrayList<Product>>() {
	            @Override
	            public void onSuccess(ArrayList<Product> products) {
	                for(Product p : products) {
	                    Log.d("Product", p.getName() + " costs " + p.getFormattedPrice() + ". " + p.getIdentifier() + ". " + p.getDeveloperName());
	                    s_productsByName.put(p.getIdentifier(), p);
	                }
	            }

	            @Override
	            public void onFailure(int errorCode, String errorMessage, Bundle errorBundle) {
	                Log.d("Error", errorMessage);
	            }
	        };

	        OuyaFacade.getInstance().requestGamerInfo(s_gamerInfoListener);
	        OuyaFacade.getInstance().requestReceipts(s_receiptsListener);
	        OuyaFacade.getInstance().requestProductList(PRODUCT_ID_LIST, s_productListListener);


		}





	    public static boolean ouya_isOuya() {
	    	//if (android.os.Build.MANUFACTURER.toLowerCase().contains("ouya") ||
	    	//	android.os.Build.MODEL.toLowerCase().contains("ouya") ||
	    	//	android.os.Build.PRODUCT.toLowerCase().contains("ouya")) {
	    	//	return true;
	    	//}
	    	//return false;
	    	return OuyaFacade.getInstance().isRunningOnOUYASupportedHardware();
	    }
	    public static String ouya_getUsername() {
	    	Log.w("game", "ouya_getUsername: " + ouyaUsername);
	    	return ouyaUsername;
	    }

	    public static void ouya_requestPurchaseByString(String productId)
	    {
			Log.d("game", "ouya_requestPurchaseByString: " + productId);
			ouya_requestPurchase(s_productsByName.get(productId));
	    }

	    public static void ouya_requestPurchase(Product product)
	    {

			try {
				SecureRandom sr = SecureRandom.getInstance("SHA1PRNG");


		        // This is an ID that allows you to associate a successful purchase with
		        // it's original request. The server does nothing with this string except
		        // pass it back to you, so it only needs to be unique within this instance
		        // of your app to allow you to pair responses with requests.
		        String uniqueId = Long.toHexString(sr.nextLong());

		        JSONObject purchaseRequest = new JSONObject();
		        purchaseRequest.put("uuid", uniqueId);
		        purchaseRequest.put("identifier", product.getIdentifier());
		        // This value is only needed for testing, not setting it results in a live purchase
		        purchaseRequest.put("testing", "true");
		        String purchaseRequestJson = purchaseRequest.toString();

		        byte[] keyBytes = new byte[16];
		        sr.nextBytes(keyBytes);
		        SecretKey key = new SecretKeySpec(keyBytes, "AES");

		        byte[] ivBytes = new byte[16];
		        sr.nextBytes(ivBytes);
		        IvParameterSpec iv = new IvParameterSpec(ivBytes);

		        Cipher cipher = Cipher.getInstance("AES/CBC/PKCS5Padding", "BC");
		        cipher.init(Cipher.ENCRYPT_MODE, key, iv);
		        byte[] payload = cipher.doFinal(purchaseRequestJson.getBytes("UTF-8"));

		        cipher = Cipher.getInstance("RSA/ECB/PKCS1Padding", "BC");
		        cipher.init(Cipher.ENCRYPT_MODE, ouyaPK);
		        byte[] encryptedKey = cipher.doFinal(keyBytes);

		        Purchasable purchasable = new Purchasable(
		        	product.getIdentifier(),
		        	Base64.encodeToString(encryptedKey, Base64.NO_WRAP),
		        	Base64.encodeToString(ivBytes, Base64.NO_WRAP),
		        	Base64.encodeToString(payload, Base64.NO_WRAP)
		        );

		        synchronized (s_outstandingPurchaseRequests) {
		            s_outstandingPurchaseRequests.put(uniqueId, product);
		        }
		        OuyaFacade.getInstance().requestPurchase(purchasable, s_purchaseListener);

			} catch (NoSuchAlgorithmException e) {
				e.printStackTrace();
			} catch (InvalidKeyException e) {
				e.printStackTrace();
			} catch (InvalidAlgorithmParameterException e) {
				e.printStackTrace();
			} catch (NoSuchProviderException e) {
				e.printStackTrace();
			} catch (NoSuchPaddingException e) {
				e.printStackTrace();
			} catch (IllegalBlockSizeException e) {
				e.printStackTrace();
			} catch (BadPaddingException e) {
				e.printStackTrace();
			} catch (UnsupportedEncodingException e) {
				e.printStackTrace();
			} catch (JSONException e) {
				e.printStackTrace();
			}
	    }

    %OUYA_BLOCKEND%
    // ----------------

    %!OUYA_BLOCKSTART%
    	public static boolean ouya_isOuya() {
		    	return false;
	    }
	    public static String ouya_getUsername() {
	    	Log.w("game", "ouya_getUsername called on a non-ouya platform.");
	    	return "Username";
	    }
    %!OUYA_BLOCKEND%

    public static boolean firetv_isAmazonFireTV() {
    	if (android.os.Build.MANUFACTURER.equals("Amazon") &&
    		android.os.Build.MODEL.startsWith("AFT")) {
    		return true;
    	}
    	return false;
    }

    // VIBRATOR
    public static boolean util_vibrator_hasVibrator() {
        Vibrator v = (Vibrator) s_activity.getSystemService(Context.VIBRATOR_SERVICE);
        if (v == null) { return false; }
        //return v.hasVibrator();
        return true;
    }
    public static void util_vibrator_vibrate(int millis) {
        Vibrator v = (Vibrator) s_activity.getSystemService(Context.VIBRATOR_SERVICE);
        if (v == null) { return; }

        //if (v.hasVibrator()) {
        v.vibrate((long) millis);
        //}
    }
    public static void util_vibrator_cancel() {
        Vibrator v = (Vibrator) s_activity.getSystemService(Context.VIBRATOR_SERVICE);
        if (v == null) { return; }

        //if (v.hasVibrator()) {
        v.cancel();
        //}
    }



    private %GAME_CLASS_NAME%View mGLView;

    static {
    	try {
    		//System.load("data/data/org.%COMPANY_NAME%.%GAME_SHORT_NAME%/lib/libark2d.so");
    		System.loadLibrary("openal");
    		System.loadLibrary("zip");
    		System.loadLibrary("ark2d");
    		%GAME_SHARED_LIBRARIES%
    		System.loadLibrary("%GAME_SHORT_NAME%");
    		Log.i("game", "Loading libraries");
    	} catch(UnsatisfiedLinkError e) {
    		throw new RuntimeException("Could not load native libraries");
    	}
    }


	%ANDROIDSDK16_LINECOMMENT% @Override
	public void onInputDeviceAdded(int deviceId) {
		Log.i("game", "Input Device added: " + deviceId);
		%GAME_CLASS_NAME%View.addGamepad(deviceId);
	}


	%ANDROIDSDK16_LINECOMMENT% @Override
	public void onInputDeviceChanged(int deviceId) {
		Log.i("game", "Input Device changed: " + deviceId);
	}


	%ANDROIDSDK16_LINECOMMENT% @Override
	public void onInputDeviceRemoved(int deviceId) {
		Log.i("game", "Input Device removed: " + deviceId);
		%GAME_CLASS_NAME%Renderer.nativeGamepadRemove(deviceId);
	}



}

class %GAME_CLASS_NAME%View extends GLSurfaceView {
	private static class ContextFactory implements GLSurfaceView.EGLContextFactory {
	    private static int EGL_CONTEXT_CLIENT_VERSION = 0x3098;
	    public EGLContext createContext(EGL10 egl, EGLDisplay display, EGLConfig eglConfig) {
	        Log.w("game", "creating OpenGL ES 2.0 context");
	        checkEglError("Before eglCreateContext", egl);
	        int[] attrib_list = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL10.EGL_NONE };
	        EGLContext context = egl.eglCreateContext(display, eglConfig, EGL10.EGL_NO_CONTEXT, attrib_list);
	        checkEglError("After eglCreateContext", egl);
	        return context;
	    }

	    public void destroyContext(EGL10 egl, EGLDisplay display, EGLContext context) {
	        egl.eglDestroyContext(display, context);
	    }
	}

	private static void checkEglError(String prompt, EGL10 egl) {
	    int error;
	    while ((error = egl.eglGetError()) != EGL10.EGL_SUCCESS) {
	        Log.e("game", String.format("%s: EGL error: 0x%x", prompt, error));
	    }
	}

	private static class ConfigChooser implements GLSurfaceView.EGLConfigChooser {

        public ConfigChooser(int r, int g, int b, int a, int depth, int stencil) {
            mRedSize = r;
            mGreenSize = g;
            mBlueSize = b;
            mAlphaSize = a;
            mDepthSize = depth;
            mStencilSize = stencil;
        }

        /* This EGL config specification is used to specify 2.0 rendering.
         * We use a minimum size of 4 bits for red/green/blue, but will
         * perform actual matching in chooseConfig() below.
         */
        private static int EGL_OPENGL_ES2_BIT = 4;
        private static int[] s_configAttribs2 =
        {
            EGL10.EGL_RED_SIZE, 4,
            EGL10.EGL_GREEN_SIZE, 4,
            EGL10.EGL_BLUE_SIZE, 4,
            EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL10.EGL_NONE
        };

        public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display) {

            /* Get the number of minimally matching EGL configurations
             */
            int[] num_config = new int[1];
            egl.eglChooseConfig(display, s_configAttribs2, null, 0, num_config);

            int numConfigs = num_config[0];

            if (numConfigs <= 0) {
                throw new IllegalArgumentException("No configs match configSpec");
            }

            /* Allocate then read the array of minimally matching EGL configs
             */
            EGLConfig[] configs = new EGLConfig[numConfigs];
            egl.eglChooseConfig(display, s_configAttribs2, configs, numConfigs, num_config);

            //if (DEBUG) {
             //    printConfigs(egl, display, configs);
            //}
            /* Now return the "best" one
             */
            return chooseConfig(egl, display, configs);
        }

        public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display,
                EGLConfig[] configs) {
            for(EGLConfig config : configs) {
                int d = findConfigAttrib(egl, display, config,
                        EGL10.EGL_DEPTH_SIZE, 0);
                int s = findConfigAttrib(egl, display, config,
                        EGL10.EGL_STENCIL_SIZE, 0);

                // We need at least mDepthSize and mStencilSize bits
                if (d < mDepthSize || s < mStencilSize)
                    continue;

                // We want an *exact* match for red/green/blue/alpha
                int r = findConfigAttrib(egl, display, config,
                        EGL10.EGL_RED_SIZE, 0);
                int g = findConfigAttrib(egl, display, config,
                            EGL10.EGL_GREEN_SIZE, 0);
                int b = findConfigAttrib(egl, display, config,
                            EGL10.EGL_BLUE_SIZE, 0);
                int a = findConfigAttrib(egl, display, config,
                        EGL10.EGL_ALPHA_SIZE, 0);

                if (r == mRedSize && g == mGreenSize && b == mBlueSize && a == mAlphaSize)
                    return config;
            }
            return null;
        }

        private int findConfigAttrib(EGL10 egl, EGLDisplay display,
                EGLConfig config, int attribute, int defaultValue) {

            if (egl.eglGetConfigAttrib(display, config, attribute, mValue)) {
                return mValue[0];
            }
            return defaultValue;
        }

        private void printConfigs(EGL10 egl, EGLDisplay display,
            EGLConfig[] configs) {
            int numConfigs = configs.length;
            Log.w("game", String.format("%d configurations", numConfigs));
            for (int i = 0; i < numConfigs; i++) {
                Log.w("game", String.format("Configuration %d:\n", i));
                printConfig(egl, display, configs[i]);
            }
        }

        private void printConfig(EGL10 egl, EGLDisplay display,
                EGLConfig config) {
            int[] attributes = {
                    EGL10.EGL_BUFFER_SIZE,
                    EGL10.EGL_ALPHA_SIZE,
                    EGL10.EGL_BLUE_SIZE,
                    EGL10.EGL_GREEN_SIZE,
                    EGL10.EGL_RED_SIZE,
                    EGL10.EGL_DEPTH_SIZE,
                    EGL10.EGL_STENCIL_SIZE,
                    EGL10.EGL_CONFIG_CAVEAT,
                    EGL10.EGL_CONFIG_ID,
                    EGL10.EGL_LEVEL,
                    EGL10.EGL_MAX_PBUFFER_HEIGHT,
                    EGL10.EGL_MAX_PBUFFER_PIXELS,
                    EGL10.EGL_MAX_PBUFFER_WIDTH,
                    EGL10.EGL_NATIVE_RENDERABLE,
                    EGL10.EGL_NATIVE_VISUAL_ID,
                    EGL10.EGL_NATIVE_VISUAL_TYPE,
                    0x3030, // EGL10.EGL_PRESERVED_RESOURCES,
                    EGL10.EGL_SAMPLES,
                    EGL10.EGL_SAMPLE_BUFFERS,
                    EGL10.EGL_SURFACE_TYPE,
                    EGL10.EGL_TRANSPARENT_TYPE,
                    EGL10.EGL_TRANSPARENT_RED_VALUE,
                    EGL10.EGL_TRANSPARENT_GREEN_VALUE,
                    EGL10.EGL_TRANSPARENT_BLUE_VALUE,
                    0x3039, // EGL10.EGL_BIND_TO_TEXTURE_RGB,
                    0x303A, // EGL10.EGL_BIND_TO_TEXTURE_RGBA,
                    0x303B, // EGL10.EGL_MIN_SWAP_INTERVAL,
                    0x303C, // EGL10.EGL_MAX_SWAP_INTERVAL,
                    EGL10.EGL_LUMINANCE_SIZE,
                    EGL10.EGL_ALPHA_MASK_SIZE,
                    EGL10.EGL_COLOR_BUFFER_TYPE,
                    EGL10.EGL_RENDERABLE_TYPE,
                    0x3042 // EGL10.EGL_CONFORMANT
            };
            String[] names = {
                    "EGL_BUFFER_SIZE",
                    "EGL_ALPHA_SIZE",
                    "EGL_BLUE_SIZE",
                    "EGL_GREEN_SIZE",
                    "EGL_RED_SIZE",
                    "EGL_DEPTH_SIZE",
                    "EGL_STENCIL_SIZE",
                    "EGL_CONFIG_CAVEAT",
                    "EGL_CONFIG_ID",
                    "EGL_LEVEL",
                    "EGL_MAX_PBUFFER_HEIGHT",
                    "EGL_MAX_PBUFFER_PIXELS",
                    "EGL_MAX_PBUFFER_WIDTH",
                    "EGL_NATIVE_RENDERABLE",
                    "EGL_NATIVE_VISUAL_ID",
                    "EGL_NATIVE_VISUAL_TYPE",
                    "EGL_PRESERVED_RESOURCES",
                    "EGL_SAMPLES",
                    "EGL_SAMPLE_BUFFERS",
                    "EGL_SURFACE_TYPE",
                    "EGL_TRANSPARENT_TYPE",
                    "EGL_TRANSPARENT_RED_VALUE",
                    "EGL_TRANSPARENT_GREEN_VALUE",
                    "EGL_TRANSPARENT_BLUE_VALUE",
                    "EGL_BIND_TO_TEXTURE_RGB",
                    "EGL_BIND_TO_TEXTURE_RGBA",
                    "EGL_MIN_SWAP_INTERVAL",
                    "EGL_MAX_SWAP_INTERVAL",
                    "EGL_LUMINANCE_SIZE",
                    "EGL_ALPHA_MASK_SIZE",
                    "EGL_COLOR_BUFFER_TYPE",
                    "EGL_RENDERABLE_TYPE",
                    "EGL_CONFORMANT"
            };
            int[] value = new int[1];
            for (int i = 0; i < attributes.length; i++) {
                int attribute = attributes[i];
                String name = names[i];
                if ( egl.eglGetConfigAttrib(display, config, attribute, value)) {
                    Log.w("game", String.format("  %s: %d\n", name, value[0]));
                } else {
                    // Log.w(TAG, String.format("  %s: failed\n", name));
                    while (egl.eglGetError() != EGL10.EGL_SUCCESS);
                }
            }
        }

        // Subclasses can adjust these values:
        protected int mRedSize;
        protected int mGreenSize;
        protected int mBlueSize;
        protected int mAlphaSize;
        protected int mDepthSize;
        protected int mStencilSize;
        private int[] mValue = new int[1];
    }

	public %GAME_CLASS_NAME%View(Context context) {
		super(context);

		this.getHolder().setFormat(PixelFormat.TRANSLUCENT);
		setEGLContextFactory(new ContextFactory());
		setEGLConfigChooser(new ConfigChooser(8, 8, 8, 8, 0, 0));

		mRenderer = new %GAME_CLASS_NAME%Renderer(context);
        setRenderer(mRenderer);
	}

	@Override
	public void onPause() {
		Log.i("game", "View Pause");
		super.onPause();
		%GAME_CLASS_NAME%Renderer.nativePause();
	}
	/*@Override
	public void onStart() {
		Log.i("game", "View Start");
		super.onStart();
		%GAME_CLASS_NAME%Renderer.nativeStart();
	}*/
	@Override
	public void onResume() {
		Log.i("game", "View Resume");
		super.onResume();
		%GAME_CLASS_NAME%Renderer.nativeResume();
	}



	@Override
	public boolean onTouchEvent(MotionEvent event) {
		Log.i("game", "View Touch Event");
		if (event.getAction() == MotionEvent.ACTION_DOWN) {
			//int pointerIndex = event.getPointerId(0);
			int thisx = (int) event.getX(); //pointerIndex);
			int thisy = (int) event.getY(); //pointerIndex);

			//Log.e("jni", "touch-down: " + thisx + "," + thisy);
			%GAME_CLASS_NAME%Renderer.nativeTouchMove(thisx, thisy);
			%GAME_CLASS_NAME%Renderer.nativeTouchDown(thisx, thisy);
		} else if (event.getAction() == MotionEvent.ACTION_MOVE) {
			int thisx = (int) event.getX(); //pointerIndex);
			int thisy = (int) event.getY(); //pointerIndex);
			//Log.e("jni", "touch-move: " + thisx + "," + thisy);
			%GAME_CLASS_NAME%Renderer.nativeTouchMove(thisx, thisy);
		} else if (event.getAction() == MotionEvent.ACTION_UP) {
			int thisx = (int) event.getX(); //pointerIndex);
			int thisy = (int) event.getY(); //pointerIndex);
			//Log.e("jni", "touch-up: " + thisx + "," + thisy);
			%GAME_CLASS_NAME%Renderer.nativeTouchUp(thisx, thisy);
		}
		return true;
		//return super.onTouchEvent(event);
	}

	%ANDROIDSDK16_BLOCKSTART%
		// The Left Joystick
		// If you do not consume motion events from the left joystick, the system will send your app dpad KeyEvents.
		// If you don't want your app to get these key events, return true from onGenericMotionEvent. Otherwise, you can differentiate these dpad events using the source field as such:
		@Override
		public boolean onGenericMotionEvent(MotionEvent event) {
			//if (event.isFromSource(InputDevice.SOURCE_CLASS_JOYSTICK)) {
			if ((event.getSource() & InputDevice.SOURCE_CLASS_JOYSTICK) != 0) {
				if (event.getAction() == MotionEvent.ACTION_MOVE) {
					// process the joystick movement...
					return true;
				}
			}
			return true; // false;
		}
	%ANDROIDSDK16_BLOCKEND%



	// ------
	// init gamepads
	public static void initGamepads()
	{
		Log.i("game", "Initialising Gamepads (Java)");
		Log.i("game", android.os.Build.MANUFACTURER + " : " + android.os.Build.MODEL + " : " + android.os.Build.PRODUCT);

 		%ANDROIDSDK16_BLOCKSTART%
			int[] deviceIds = InputDevice.getDeviceIds();
			for(int i = 0; i < deviceIds.length; i++)
			{
				int deviceId = deviceIds[i];
				addGamepad(deviceId);
			}
		%ANDROIDSDK16_BLOCKEND%
		%!ANDROIDSDK16_LINECOMMENT% Log.e("game", "Gamepads not available on Android 2.3.");
	}

	public static void addGamepad(int deviceId) {
		%ANDROIDSDK16_BLOCKSTART%
		InputDevice device = InputDevice.getDevice(deviceId);

		if ((device.getSources() & InputDevice.SOURCE_GAMEPAD) != 0 &&
			(device.getSources() & InputDevice.SOURCE_JOYSTICK) != 0) {

			if (!%GAME_CLASS_NAME%Renderer.nativeGamepadExists(deviceId))
			{
				%GAME_CLASS_NAME%Renderer.nativeGamepadAdd(deviceId, device.getName());

				// do axises.
				int j = 0;
				//int k = 0;
				final List<MotionRange> ranges = device.getMotionRanges();
				for (MotionRange range : ranges) {
					if ((range.getSource() & InputDevice.SOURCE_CLASS_JOYSTICK) != 0) {
						%GAME_CLASS_NAME%Renderer.nativeGamepadAddAxis(deviceId, j, range.getAxis());
						j++;
					}
				}

				// do buttons (ouya controller)
				// ...done natively
				//%GAME_CLASS_NAME%Renderer.nativeGamepadAddButton(deviceId, 0);
	        }
	    }
	    %ANDROIDSDK16_BLOCKEND%
	}

	// ------
	// TODO:
	// http://grepcode.com/file/repository.grepcode.com/java/ext/com.google.android/android-apps/4.0.3_r1/com/example/android/apis/view/GameControllerInput.java
	// ------
	%ANDROIDSDK16_BLOCKSTART%
	public static int getInputDeviceState(InputEvent event) {
		return event.getDeviceId();
	}
	%ANDROIDSDK16_BLOCKEND%



	// Check whether this is a key we care about.
	// In a real game, we would probably let the user configure which keys to use
	// instead of hardcoding the keys like this.
	public static boolean isGameKey(int keyCode) {
		switch (keyCode) {
			case KeyEvent.KEYCODE_DPAD_UP:
			case KeyEvent.KEYCODE_DPAD_DOWN:
			case KeyEvent.KEYCODE_DPAD_LEFT:
			case KeyEvent.KEYCODE_DPAD_RIGHT:
			case KeyEvent.KEYCODE_DPAD_CENTER:
			case KeyEvent.KEYCODE_SPACE:
			%OUYA_BLOCKSTART%
				case OuyaController.BUTTON_O:
				case OuyaController.BUTTON_U:
				case OuyaController.BUTTON_Y:
				case OuyaController.BUTTON_A:
				case OuyaController.BUTTON_MENU:
			%OUYA_BLOCKEND%
			%FIRETV_BLOCKSTART%
				case GameController.BUTTON_A:
				case GameController.BUTTON_B:
				case GameController.BUTTON_X:
				case GameController.BUTTON_Y:
				case GameController.BUTTON_STICK_LEFT:
				case GameController.BUTTON_STICK_RIGHT:
				case GameController.BUTTON_SHOULDER_LEFT:
				case GameController.BUTTON_SHOULDER_RIGHT:
			%FIRETV_BLOCKEND%
				return true;
			default:
				%ANDROIDSDK16_LINECOMMENT% return KeyEvent.isGamepadButton(keyCode);
				%!ANDROIDSDK16_LINECOMMENT% return false;
		}
	}

	%GAME_CLASS_NAME%Renderer mRenderer;


}

class %GAME_CLASS_NAME%Renderer implements GLSurfaceView.Renderer {
	public static boolean s_initted = false;

	public Context context;
	public %GAME_CLASS_NAME%Renderer(Context context) {
		this.context = context;
	}

	public static void createDir(String path) {
		Log.i("game", "making path; " + path);
		File extf = new File(path);
		if (!extf.exists()) {
	        if (!extf.mkdirs()) {
	            Log.e("game", "Problem creating folder: " + path);
	        }
	    }
	}

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
	//	nativeInit();
		if (s_initted) {
			Log.i("game", "Surface Already Created");
		} else {
			Log.i("game", "Surface Created");

			//%OUYA_BLOCKSTART%
			//	// Ouya init.
			//	%GAME_CLASS_NAME%Activity.ouya_init();
			//%OUYA_BLOCKEND%

			String apkFilePath = null;
			ApplicationInfo appInfo = null;
			String externalDataPath = null;
			PackageManager packMgmr = context.getPackageManager();
			try {
	            appInfo = packMgmr.getApplicationInfo("%PACKAGE_DOT_NOTATION%", 0);
	            externalDataPath = Environment.getExternalStorageDirectory().getAbsolutePath() + "/Android/data/%PACKAGE_DOT_NOTATION%/files/"; // does not include trailing slash.
	        } catch (NameNotFoundException e) {
	            e.printStackTrace();
	            throw new RuntimeException("Unable to locate assets, aborting...");
	        }
			apkFilePath = appInfo.sourceDir;
			Log.i("game", "APK File Path:" + apkFilePath);
			Log.i("game", "Save File Path:" + externalDataPath);

			createDir(externalDataPath);
			createDir(externalDataPath+"assets/");

			// TODO: create other resource directories for project.

			%FIRETV_BLOCKSTART%
				%GAME_CLASS_NAME%Activity.firetv_connect();
			%FIRETV_BLOCKEND%

			nativeInit(apkFilePath, externalDataPath);

			s_initted = true;
		}
	}
	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height) {
		Log.i("game", "Surface Changed");
		/*int w = width;//480;
		int h = height;//320;
		gl.glViewport(0, 0, w, h);

		gl.glMatrixMode(GL10.GL_PROJECTION);
		gl.glLoadIdentity();

		//gl.glOrthox(0, w, h, 0,  -1, 1);//(0, 1,  1, 0, -1, 1);
		GLU.gluOrtho2D(gl, 0.0f, w, h, 0.0f);

		gl.glMatrixMode(GL10.GL_MODELVIEW);
		gl.glLoadIdentity();
		*/
		nativeResize(width, height);
	}
	@Override
	public void onDrawFrame(GL10 gl) {
		//Log.i("game", "Render");
		nativeRender();
	}

	public static native void nativeInit(String apkPath, String externalDataPath);
	public static native void nativeResize(int width, int height);
	public static native void nativeRender();
	public static native void nativeDone();

	public static native void nativeTouchDown(int x, int y);
	public static native void nativeTouchMove(int x, int y);
	public static native void nativeTouchUp(int x, int y);

	public static native void nativeKeyDown(int key, String keyChar);
	public static native void nativeKeyUp(int key, String keyChar);

	public static native void nativeStart();
	public static native void nativePause();
	public static native void nativeResume();
	public static native void nativeBackPressed();

	public static native void nativeCallbackById(int id);
	public static native void nativeCallbackByIdIntParam(int id, int param);

	public static native void nativeGamepadAdd(int id, String name);
	public static native boolean nativeGamepadExists(int id);

	public static native void nativeGamepadAddAxis(int id, int index, int axis);
	public static native int nativeGamepadAxisAtIndex(int id, int index);
	public static native void nativeGamepadAxisChanged(int id, int axis, float value);
	public static native int nativeGamepadNumAxes(int id);

	public static native void nativeGamepadKeyDown(int id, int buttonindex);
	public static native void nativeGamepadKeyUp(int id, int buttonindex);

	public static native void nativeGamepadRemove(int id);
}




