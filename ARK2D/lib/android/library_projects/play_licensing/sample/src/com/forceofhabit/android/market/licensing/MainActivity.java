/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */ 

package com.forceofhabit.android.market.licensing;

import com.forceofhabit.android.market.licensing.R;
import com.google.android.vending.licensing.AESObfuscator;
import com.google.android.vending.licensing.LicenseChecker;
import com.google.android.vending.licensing.LicenseCheckerCallback;
import com.google.android.vending.licensing.Policy;
import com.google.android.vending.licensing.ServerManagedPolicy;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.provider.Settings.Secure;
import android.view.View;
import android.view.Window;
import android.widget.Button;
import android.widget.TextView;

/**
 * Welcome to the world of Android Market licensing. We're so glad to have you
 * onboard!
 * <p>
 * The first thing you need to do is get your hands on your public key.
 * Update the BASE64_PUBLIC_KEY constant below with your encoded public key,
 * which you can find on the
 * <a href="http://market.android.com/publish/editProfile">Edit Profile</a>
 * page of the Market publisher site.
 * <p>
 * Log in with the same account on your Cupcake (1.5) or higher phone or
 * your FroYo (2.2) emulator with the Google add-ons installed. Change the
 * test response on the Edit Profile page, press Save, and see how this
 * application responds when you check your license.
 * <p>
 * After you get this sample running, peruse the
 * <a href="http://developer.android.com/guide/publishing/licensing.html">
 * licensing documentation.</a>
 */
public class MainActivity extends Activity {
    private static final String BASE64_PUBLIC_KEY = "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAidzgozUx+fVDGBCoHZvRJu4NOizl3ru3CG/fFlIqdRdseC5zVQYMB3tGkIyh9ScMm8gtrl3dLZYs3p8+ocrKN6O61S73+cUSmKYSrz18RzUrXLcuqqLV8prFcKsY58aDV8UChX8ctmF16OXtKEN8gt1WiKVUH/vkIfTgetpArRzLZD32Uoemx2PwME1ildPCNGphesQJMP9EgbaEU30dGPtH18NNnITad02LYMgfhuxVQii5I15Oj5e0dH1zNAobwBvRIGUi0SNMPbsnOQWnCRfzq1JsClcEI4EkFTAY1ODhkjJOLNxotv7l3tcdTayH8RoGb+arG9aI2KY8VgrFQwIDAQAB";

    // Generate your own 20 random bytes, and put them here.
    private static final byte[] SALT = new byte[] {
        -46, 65, 30, -128, -103, -57, 74, -64, 51, 88, -95, -45, 77, -117, -36, -113, -11, 32, -64,
        89
    };

    private TextView mStatusText;
    private Button mCheckLicenseButton;

    private LicenseCheckerCallback mLicenseCheckerCallback;
    private LicenseChecker mChecker;
    // A handler on the UI thread.
    private Handler mHandler;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_INDETERMINATE_PROGRESS);
        setContentView(R.layout.main);

        mStatusText = (TextView) findViewById(R.id.status_text);
        mCheckLicenseButton = (Button) findViewById(R.id.check_license_button);
        mCheckLicenseButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                doCheck();
            }
        });

        mHandler = new Handler();

        // Try to use more data here. ANDROID_ID is a single point of attack.
        String deviceId = Secure.getString(getContentResolver(), Secure.ANDROID_ID);

        // Library calls this when it's done.
        mLicenseCheckerCallback = new MyLicenseCheckerCallback();
        // Construct the LicenseChecker with a policy.
        mChecker = new LicenseChecker(
            this, new ServerManagedPolicy(this,
                new AESObfuscator(SALT, getPackageName(), deviceId)),
            BASE64_PUBLIC_KEY);
        doCheck();
    }

    protected Dialog onCreateDialog(int id) {
        final boolean bRetry = id == 1;
        return new AlertDialog.Builder(this)
            .setTitle(R.string.unlicensed_dialog_title)
            .setMessage(bRetry ? R.string.unlicensed_dialog_retry_body : R.string.unlicensed_dialog_body)
            .setPositiveButton(bRetry ? R.string.retry_button : R.string.buy_button, new DialogInterface.OnClickListener() {
                boolean mRetry = bRetry;
                public void onClick(DialogInterface dialog, int which) {
                    if ( mRetry ) {
                        doCheck();
                    } else {
                        Intent marketIntent = new Intent(Intent.ACTION_VIEW, Uri.parse(
                                "http://market.android.com/details?id=" + getPackageName()));
                            startActivity(marketIntent);                        
                    }
                }
            })
            .setNegativeButton(R.string.quit_button, new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dialog, int which) {
                    finish();
                }
            }).create();
    }

    private void doCheck() {
        mCheckLicenseButton.setEnabled(false);
        setProgressBarIndeterminateVisibility(true);
        mStatusText.setText(R.string.checking_license);
        mChecker.checkAccess(mLicenseCheckerCallback);
    }

    private void displayResult(final String result) {
        mHandler.post(new Runnable() {
            public void run() {
                mStatusText.setText(result);
                setProgressBarIndeterminateVisibility(false);
                mCheckLicenseButton.setEnabled(true);
            }
        });
    }
    
    private void displayDialog(final boolean showRetry) {
        mHandler.post(new Runnable() {
            public void run() {
                setProgressBarIndeterminateVisibility(false);
                showDialog(showRetry ? 1 : 0);
                mCheckLicenseButton.setEnabled(true);
            }
        });
    }    

    private class MyLicenseCheckerCallback implements LicenseCheckerCallback {
    	
    	 public String getPolicyReasonString(int policyReason) {
         	String result = "";
         	switch(policyReason) {
 	         	case Policy.LICENSED:
 	         		result += "LICENSED";
 	         		break;
 	         	case Policy.NOT_LICENSED:
 	         		result += "NOT LICENSED";
 	         		break;
 	         	case Policy.RETRY:
 	         		result += "RETRY / NETWORK ERROR";
 	         		break;
         	 }
         	 return result;
         }
    	 
    	 public String getErrorReasonString(int errorCode) {
    		 /*private static final int LICENSED = 0x0;
    		    private static final int NOT_LICENSED = 0x1;
    		    private static final int LICENSED_OLD_KEY = 0x2;
    		    private static final int ERROR_NOT_MARKET_MANAGED = 0x3;
    		    private static final int ERROR_SERVER_FAILURE = 0x4;
    		    private static final int ERROR_OVER_QUOTA = 0x5;

    		    private static final int ERROR_CONTACTING_SERVER = 0x101;
    		    private static final int ERROR_INVALID_PACKAGE_NAME = 0x102;
    		    private static final int ERROR_NON_MATCHING_UID = 0x103;*/
    		 String result = ""; 
    		 switch (errorCode) {
				case LicenseCheckerCallback.ERROR_INVALID_PACKAGE_NAME:
					result += "ERROR_INVALID_PACKAGE_NAME";
					break;
				case LicenseCheckerCallback.ERROR_NON_MATCHING_UID:
					result += "ERROR_NON_MATCHING_UID";
					break;
				case LicenseCheckerCallback.ERROR_NOT_MARKET_MANAGED:
					result += "ERROR_NOT_MARKET_MANAGED";
					break;
				case LicenseCheckerCallback.ERROR_CHECK_IN_PROGRESS:
					result += "ERROR_CHECK_IN_PROGRESS";
					break;
				case LicenseCheckerCallback.ERROR_INVALID_PUBLIC_KEY:
					result += "ERROR_INVALID_PUBLIC_KEY";
					break;
				case LicenseCheckerCallback.ERROR_MISSING_PERMISSION:
					result += "ERROR_MISSING_PERMISSION";
					break;
				}
    		    return result;
    	 }
    	 
        public void allow(int policyReason) {
            if (isFinishing()) {
                // Don't update UI if Activity is finishing.
                return;
            }
            // Should allow user access.
            displayResult(getString(R.string.allow) + ": " + getPolicyReasonString(policyReason));
        }
        
        public void dontAllow(int policyReason) {
            if (isFinishing()) {
                // Don't update UI if Activity is finishing.
                return;
            }
            
            displayResult(getString(R.string.dont_allow) + ": " + getPolicyReasonString(policyReason));
            // Should not allow access. In most cases, the app should assume
            // the user has access unless it encounters this. If it does,
            // the app should inform the user of their unlicensed ways
            // and then either shut down the app or limit the user to a
            // restricted set of features.
            // In this example, we show a dialog that takes the user to Market.
            // If the reason for the lack of license is that the service is
            // unavailable or there is another problem, we display a
            // retry button on the dialog and a different message.
            displayDialog(policyReason == Policy.RETRY);
        }

        public void applicationError(int errorCode) {
            if (isFinishing()) {
                // Don't update UI if Activity is finishing.
                return;
            }
            // This is a polite way of saying the developer made a mistake
            // while setting up or calling the license checker library.
            // Please examine the error code and fix the error.
            String result = String.format(getString(R.string.application_error), errorCode);
            		result += ": " + getErrorReasonString(errorCode);
            displayResult(result);
        }
    } 

    @Override
    protected void onDestroy() {
        super.onDestroy(); 
        mChecker.onDestroy();
    }

}
