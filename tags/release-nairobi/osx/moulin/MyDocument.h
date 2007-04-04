//
//  MyDocument.h
//  moulin
//
//  Created by reg on 1/12/07.
//  Copyright __MyCompanyName__ 2007 . All rights reserved.
//


#import <Cocoa/Cocoa.h>

@class WebView;

@interface MyDocument : NSDocument
{
    IBOutlet WebView *webView;
    IBOutlet NSWindow *window;
    IBOutlet NSTextField	*searchFieldOutlet;
    IBOutlet NSToolbarItem	*activeSearchItem;

    NSString	*docTitle;
    NSString	*frameStatus;
    NSString	*resourceStatus;
    NSString	*url;
	
	NSURL		*URLToLoad;
    
    int resourceCount;
    int resourceFailedCount;
    int resourceCompletedCount;
	
	IBOutlet id myWindow;
	
}
- (void)setupToolbar;
- (NSToolbarItem *)toolbar:(NSToolbar *)toolbar itemForItemIdentifier:(NSString *)itemIdentifier willBeInsertedIntoToolbar:(BOOL)flag;
- (NSArray *)toolbarAllowedItemIdentifiers:(NSToolbar *)toolbar;
- (NSArray *)toolbarDefaultItemIdentifiers:(NSToolbar *)toolbar;

- (IBAction)connectURL:(id)sender;
- (void)setFrameStatus: (NSString *)s;
- (void)setResourceStatus: (NSString *)s;
- (NSString *)stringByEncodingURLEscapes:(NSString *)source;
@end
