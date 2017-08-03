
#if defined(ARK2D_IPHONE) || defined(ARK2D_MACINTOSH)

	#import <QuartzCore/QuartzCore.h>
	#import <Foundation/Foundation.h>


    #import <GameKit/GameKit.h>


	//#import "../../vendor/iphone/ga/GAITrackedViewController.h"

	// @interface MyDICDController : UIViewController <UIDocumentInteractionControllerDelegate>

	// @end


	@protocol GameCenterManagerDelegate <NSObject>
	@optional
	- (void) processGameCenterAuth: (NSError*) error;
	- (void) scoreReported: (NSError*) error;
	- (void) reloadScoresComplete: (GKLeaderboard*) leaderBoard error: (NSError*) error;
	- (void) achievementSubmitted: (GKAchievement*) ach error:(NSError*) error;
	- (void) achievementResetResult: (NSError*) error;
	- (void) mappedPlayerIDToPlayer: (GKPlayer*) player error: (NSError*) error;
	@end




	// GameCenter?
	#import <Foundation/Foundation.h>
	//#import <UIKit/UIKit.h>


	@interface GameCenterManager : NSObject
	{
	    NSMutableDictionary* earnedAchievementCache;
	    
	    id <GameCenterManagerDelegate, NSObject> delegate;
	}

	//This property must be attomic to ensure that the cache is always in a viable state...
	@property (retain) NSMutableDictionary* earnedAchievementCache;

	@property (nonatomic, assign)  id <GameCenterManagerDelegate> delegate;

	+ (BOOL) isGameCenterAvailable;

	- (void) authenticateLocalUser;
	- (BOOL) isAuthenticatedLocalUser;

	- (void) reportScore: (int64_t) score forCategory: (NSString*) category;
	- (void) challengeScore: (int64_t) score forCategory: (NSString*) category;
	- (void) reloadHighScoresForCategory: (NSString*) category;

	- (void) submitAchievement: (NSString*) identifier percentComplete: (double) percentComplete;
	- (void) resetAchievements;
	- (void) challengeAchievement: (NSString*) identifier percentComplete: (double) percentComplete;

	- (void) openGameCenter;

	- (void) mapPlayerIDtoPlayer: (NSString*) playerID;
	@end


	@interface GameICloudManager : NSObject 
	{
		@public NSMutableDictionary* iclouddocuments;
		@public NSMetadataQuery* query;
	}

	-(void)loadData:(NSMetadataQuery *)query;
	-(void)loadDocuments;

	@property (strong) NSMetadataQuery* query;
	@property (strong, nonatomic) NSMutableDictionary* iclouddocuments; // = [NSDictionary dictionary];

	@end

#endif
