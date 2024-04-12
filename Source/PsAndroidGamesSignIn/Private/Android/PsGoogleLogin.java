// Copyright 2015-2024 MY.GAMES. All Rights Reserved.

package com.pushkinstudio.PsAndroidGamesSignIn;

import com.epicgames.ue4.GameActivity;
import com.google.android.gms.auth.api.signin.GoogleSignInOptions;
import com.google.android.gms.auth.api.signin.GoogleSignInClient;
import com.google.android.gms.auth.api.signin.GoogleSignIn;
import com.google.android.gms.auth.api.signin.GoogleSignInResult;
import com.google.android.gms.auth.api.signin.GoogleSignInAccount;
import com.google.android.gms.auth.api.signin.GoogleSignInApi;
import com.google.android.gms.auth.api.Auth;
import com.google.android.gms.games.Games;
import com.google.android.gms.games.GamesClient;
import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import android.app.Activity;
import android.content.Intent;
import android.util.Log;

public class PsGoogleLogin
{
	native void nativeGoogleLoginCompleted(boolean Success, String ServerAuthCode);

	private GameActivity mActivity;
	private GoogleSignInClient mGoogleSignInClient;

	private static final String LOGTAG = "UE4-PS-GOOGLE";

	private static final int REQUEST_SIGN_IN = 13481;

	public PsGoogleLogin(GameActivity activity) 
	{
		mActivity = activity;
	}

	public void Init(String serverClientId)
	{
		Log.d(LOGTAG, "Init");

		GoogleSignInOptions gso = new GoogleSignInOptions.Builder(GoogleSignInOptions.DEFAULT_GAMES_SIGN_IN)
									.requestServerAuthCode(serverClientId)
									.build();
		mGoogleSignInClient = GoogleSignIn.getClient(mActivity, gso);
	}

	public void SignInSilently()
	{
		Log.d(LOGTAG, "SignInSilently");

		GoogleSignInOptions signInOptions = GoogleSignInOptions.DEFAULT_GAMES_SIGN_IN;
		GoogleSignInAccount account = GoogleSignIn.getLastSignedInAccount(mActivity);

		Log.d(LOGTAG, "SignInSilently trying sign in");
		mGoogleSignInClient
			.silentSignIn()
			.addOnCompleteListener(
				mActivity,
				new OnCompleteListener<GoogleSignInAccount>()
				{
					@Override
					public void onComplete(Task<GoogleSignInAccount> task)
					{
						Log.d(LOGTAG, "SignInSilently onComplete");

						GoogleSignInAccount account = GoogleSignIn.getLastSignedInAccount(mActivity);
						if (account != null)
						{
							GamesClient gamesClient = Games.getGamesClient(mActivity, account);
							if (gamesClient != null)
							{
								gamesClient.setViewForPopups(mActivity.findViewById(android.R.id.content));
							}
						}

						if (task.isSuccessful())
						{
							Log.d(LOGTAG, "SignInSilently onComplete success");
							GoogleSignInAccount signedInAccount = task.getResult();
							nativeGoogleLoginCompleted(true, signedInAccount.getServerAuthCode());
						}
						else
						{
							Log.d(LOGTAG, "SignInSilently onComplete fail");
							nativeGoogleLoginCompleted(false, new String());
						}
					}
				});
	}

	public void SignInInteractively()
	{
		Log.d(LOGTAG, "SignInInteractively");

		Intent signInIntent = mGoogleSignInClient.getSignInIntent();
		if (signInIntent != null)
		{
			Log.d(LOGTAG, "SignInInteractively starting activity");
			mActivity.startActivityForResult(signInIntent, REQUEST_SIGN_IN);
		}
		else
		{
			Log.d(LOGTAG, "SignInInteractively fail");
			nativeGoogleLoginCompleted(false, new String());
		}
	}

	public void onActivityResult(int requestCode, int resultCode, Intent data) 
	{
		if (requestCode == REQUEST_SIGN_IN) 
		{
			if (resultCode == Activity.RESULT_OK)
			{
				Log.d(LOGTAG, "SignInInteractively onActivityResult resultCode OK");
				GoogleSignInResult result = Auth.GoogleSignInApi.getSignInResultFromIntent(data);
				if (result.isSuccess())
				{
					Log.d(LOGTAG, "SignInInteractively onActivityResult SignInResult success");

					GamesClient gamesClient = Games.getGamesClient(mActivity, GoogleSignIn.getLastSignedInAccount(mActivity));
					gamesClient.setViewForPopups(mActivity.findViewById(android.R.id.content));
						
					GoogleSignInAccount signedInAccount = result.getSignInAccount();
					nativeGoogleLoginCompleted(true, signedInAccount.getServerAuthCode());
				}
				else
				{
					String errorMessage = result.getStatus().getStatusMessage();
					Log.d(LOGTAG, String.format("SignInInteractively onActivityResult SignInResult fail, error \"%s\"", errorMessage));
					nativeGoogleLoginCompleted(false, new String());
				}
			}
			else
			{
				Log.d(LOGTAG, String.format("SignInInteractively onActivityResult resultCode fail %d", resultCode));
				nativeGoogleLoginCompleted(false, new String());
			}
		}
	}
}
