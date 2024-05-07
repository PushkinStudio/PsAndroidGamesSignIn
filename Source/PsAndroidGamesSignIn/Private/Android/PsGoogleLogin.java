// Copyright 2015-2024 MY.GAMES. All Rights Reserved.

package com.pushkinstudio.PsAndroidGamesSignIn;

import android.app.Activity;
import android.content.Context;
import android.util.Log;

import com.google.android.gms.games.GamesSignInClient;
import com.google.android.gms.games.PlayGames;
import com.google.android.gms.games.PlayGamesSdk;

public class PsGoogleLogin
{
	private static final String LOGTAG = "UE-PS-GOOGLE";

	native static void nativeGoogleLoginCompleted(boolean Success, String ServerAuthCode);

	static public void Initialize(Context AppContext)
	{
		Log.d(LOGTAG, "Initialize");
		PlayGamesSdk.initialize(AppContext);
	}

	public static void Login(final Activity activity, String serverClientId)
	{
		Log.d(LOGTAG, "Login started");
		GamesSignInClient signInClient = PlayGames.getGamesSignInClient(activity);
		signInClient.isAuthenticated().addOnCompleteListener(isAuthenticatedTask ->
			{
				if (isAuthenticatedTask.isSuccessful())
				{
					if (isAuthenticatedTask.getResult().isAuthenticated())
					{
						Log.d(LOGTAG, "User already authenticated. Requesting player data");
						RequestPlayerData(activity, serverClientId);
					}
					else
					{
						Log.d(LOGTAG, "User not yet authenticated. Attempting sign in");
						signInClient.signIn().addOnCompleteListener(signInTask ->
							{
								if (signInTask.isSuccessful())
								{
									if (signInTask.getResult().isAuthenticated())
									{
										Log.d(LOGTAG, "Sign in succeeded. Requesting player data");
										RequestPlayerData(activity, serverClientId);
									}
									else
									{
										Log.e(LOGTAG, "Sign in failed");
										nativeGoogleLoginCompleted(false, new String());
									}
								}
								else
								{
									Log.e(LOGTAG, "GamesSignInClient.signIn task failed");
									nativeGoogleLoginCompleted(false, new String());
								}
							});
					}
				}
				else
				{
					Log.e(LOGTAG, "GamesSignInClient.isAuthenticated task failed");
					nativeGoogleLoginCompleted(false, new String());
				}
			});
	}

	private static void RequestPlayerData(Activity activity, String serverClientId)
	{
		Log.d(LOGTAG, "RequestPlayerData");
		PlayGames.getGamesSignInClient(activity).requestServerSideAccess(serverClientId, /* forceRefreshToken= */ false)
			.addOnCompleteListener(task -> {
				if (task.isSuccessful()) {
					String serverAuthToken = task.getResult();
					nativeGoogleLoginCompleted(true, serverAuthToken);
				}
				else {
					Log.e(LOGTAG, "Failed to retrieve authentication code.");
					nativeGoogleLoginCompleted(false, new String());
				}
			});
	}
}
