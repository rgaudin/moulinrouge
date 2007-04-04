//
//  MyDocument.m
//  moulin
//
//  Created by reg on 1/12/07.
//  Copyright __MyCompanyName__ 2007 . All rights reserved.
//

#import "MyDocument.h"
#import <WebKit/WebKit.h>

static NSString* 	BackButtonIdentifier 		= @"moulinBackButtonIdentifier";
static NSString* 	ForwardButtonIdentifier 	= @"moulinForwardButtonIdentifier";
static NSString* 	HomeButtonIdentifier 		= @"moulinHomeButtonIdentifier";
static NSString* 	BiggerButtonIdentifier 		= @"moulinBiggerButtonIdentifier";
static NSString* 	SmallerButtonIdentifier 	= @"moulinSmallerButtonIdentifier";
static NSString* 	SearchPageIdentifier		= @"moulinSearchPageIdentifier";
static NSString* 	defaultHomePage				= @"http://127.0.0.1:2111/";
static NSString* 	defaultSearchURL			= @"http://127.0.0.1:2111/_WikiSearch:";

@implementation MyDocument

- (void)dealloc
{
    [docTitle release];
    [frameStatus release];
    [resourceStatus release];
	[URLToLoad release];
    [url release];
	[activeSearchItem release];
    activeSearchItem = nil;
    [searchFieldOutlet release];
    searchFieldOutlet = nil;

    [window release];
    [super dealloc];
}

- (id)webView
{
    return webView;
}

- (NSString *)windowNibName
{
    return @"MyDocument";
}

- (void)loadURL:(NSURL *)URL
{
	[[webView mainFrame] loadRequest:[NSURLRequest requestWithURL:URL]];
}

- (void)setURLToLoad:(NSURL *)URL
{
	[URL retain];
	[URLToLoad release];
	URLToLoad = URL;
}

- (void)windowControllerDidLoadNib:(NSWindowController *) aController
{
    [super windowControllerDidLoadNib:aController];

	[searchFieldOutlet retain];
    [searchFieldOutlet removeFromSuperview];
	
    // Set the WebView delegates
    [webView setFrameLoadDelegate:self];
    [webView setUIDelegate:self];
    [webView setResourceLoadDelegate:self];
    
    // Load a default URL
	NSURL *URL = URLToLoad != nil ? URLToLoad : [NSURL URLWithString:defaultHomePage];
	[self setURLToLoad:nil];
	[self setupToolbar];
	[self loadURL:URL];
}

- (NSData *)dataRepresentationOfType:(NSString *)aType
{
    return [[[webView mainFrame] dataSource] data];
}

- (void)setupToolbar
{
    NSToolbar *toolbar = [[NSToolbar alloc] initWithIdentifier:@"moulinToolbar"];
    [toolbar setAllowsUserCustomization: YES];
    [toolbar setAutosavesConfiguration: NO];
    [toolbar setDelegate: self];
    [myWindow setToolbar:[toolbar autorelease]];
}

- (BOOL) validateToolbarItem: (NSToolbarItem *) toolbarItem {
    return YES;
}


- (NSToolbarItem *)toolbar:(NSToolbar *)toolbar itemForItemIdentifier:(NSString *)itemIdentifier willBeInsertedIntoToolbar:(BOOL)flag
{
	NSToolbarItem *item = nil;
	
	if ([itemIdentifier isEqual: BackButtonIdentifier]) {
		item = [[[NSToolbarItem alloc] initWithItemIdentifier: itemIdentifier] autorelease];
		[item setLabel: NSLocalizedString(@"Back")];
		[item setPaletteLabel: NSLocalizedString(@"Back")];
		[item setToolTip: NSLocalizedString(@"Go Back")];
		[item setImage: [NSImage imageNamed: @"back"]];
		[item setTarget: webView];
		[item setAction: @selector(goBack:)];
    } else if ([itemIdentifier isEqual: ForwardButtonIdentifier]) {
		item = [[[NSToolbarItem alloc] initWithItemIdentifier: itemIdentifier] autorelease];
		[item setLabel: @"Forward"];
		[item setPaletteLabel: @"Forward"];
		[item setToolTip: @"Go Forward"];
		[item setImage: [NSImage imageNamed: @"forward"]];
		[item setTarget: self];
		[item setAction: @selector(goForward:)];
    }  else if ([itemIdentifier isEqual: HomeButtonIdentifier]) {
		item = [[[NSToolbarItem alloc] initWithItemIdentifier: itemIdentifier] autorelease];
		[item setLabel: @"Home"];
		[item setPaletteLabel: @"Home"];
		[item setToolTip: @"Go Home"];
		[item setImage: [NSImage imageNamed: @"home"]];
		[item setTarget: self];
		[item setAction: @selector(goHome:)];
    } else if ([itemIdentifier isEqual: BiggerButtonIdentifier]) {
		item = [[[NSToolbarItem alloc] initWithItemIdentifier: itemIdentifier] autorelease];
		[item setLabel: @"Bigger"];
		[item setPaletteLabel: @"Bigger"];
		[item setToolTip: @"Make Text Bigger"];
		[item setImage: [NSImage imageNamed: @"textBigger"]];
		[item setTarget: webView];
		[item setAction: @selector(makeTextLarger:)];
    } else if ([itemIdentifier isEqual: SmallerButtonIdentifier]) {
		item = [[[NSToolbarItem alloc] initWithItemIdentifier: itemIdentifier] autorelease];
		[item setLabel: @"Smaller"];
		[item setPaletteLabel: @"Smaller"];
		[item setToolTip: @"Make Text Smaller"];
		[item setImage: [NSImage imageNamed: @"textSmaller"]];
		[item setTarget: webView];
		[item setAction: @selector(makeTextSmaller:)];
    } else if ([itemIdentifier isEqual: NSToolbarPrintItemIdentifier]) {
		item = [[[NSToolbarItem alloc] initWithItemIdentifier: itemIdentifier] autorelease];
		[item setTarget: self];
		[item setAction: @selector(printShowingPrintPanel:)];
    } else if ([itemIdentifier isEqual: SearchPageIdentifier]) {
		item = [[[NSToolbarItem alloc] initWithItemIdentifier: itemIdentifier] autorelease];
		NSMenu *submenu = nil;
		NSMenuItem *submenuItem = nil, *menuFormRep = nil;
	
		// Set up the standard properties 
		[item setLabel: @"Search"];
		[item setPaletteLabel: @"Search"];
		[item setToolTip: @"Search a page on moulin"];
	
        searchFieldOutlet = [[NSSearchField alloc] initWithFrame:[searchFieldOutlet frame]];
		// Use a custom view, a text field, for the search item 
		[item setView: searchFieldOutlet];
		[item setMinSize:NSMakeSize(30, NSHeight([searchFieldOutlet frame]))];
		[item setMaxSize:NSMakeSize(400,NSHeight([searchFieldOutlet frame]))];

		// By default, in text only mode, a custom items label will be shown as disabled text, but you can provide a 
		// custom menu of your own by using <item> setMenuFormRepresentation] 
		submenu = [[[NSMenu alloc] init] autorelease];
		submenuItem = [[[NSMenuItem alloc] initWithTitle: @"Search Panel" action: @selector(searchUsingSearchPanel:) keyEquivalent: @""] autorelease];
		menuFormRep = [[[NSMenuItem alloc] init] autorelease];

		[submenu addItem: submenuItem];
		[submenuItem setTarget: self];
		[menuFormRep setSubmenu: submenu];
		[menuFormRep setTitle: [item label]];

        // Normally, a menuFormRep with a submenu should just act like a pull down.  However, in 10.4 and later, the menuFormRep can have its own target / action.  If it does, on click and hold (or if the user clicks and drags down), the submenu will appear.  However, on just a click, the menuFormRep will fire its own action.
        [menuFormRep setTarget: self];
        [menuFormRep setAction: @selector(searchMenuFormRepresentationClicked:)];

        // Please note, from a user experience perspective, you wouldn't set up your search field and menuFormRep like we do here.  This is simply an example which shows you all of the features you could use.
		[item setMenuFormRepresentation: menuFormRep];
    }
	
    return item;
}

- (void) toolbarWillAddItem: (NSNotification *) notif {
    // Optional delegate method:  Before an new item is added to the toolbar, this notification is posted.
    // This is the best place to notice a new item is going into the toolbar.  For instance, if you need to 
    // cache a reference to the toolbar item or need to set up some initial state, this is the best place 
    // to do it.  The notification object is the toolbar to which the item is being added.  The item being 
    // added is found by referencing the @"item" key in the userInfo 
    NSToolbarItem *addedItem = [[notif userInfo] objectForKey: @"item"];
    if([[addedItem itemIdentifier] isEqual: SearchPageIdentifier]) {
		activeSearchItem = [addedItem retain];
		[activeSearchItem setTarget: self];
		[activeSearchItem setAction: @selector(searchUsingToolbarSearchField:)];
    }

}  

- (void)goHome
{
	[self loadURL:[NSURL URLWithString:defaultHomePage]];
}

- (void)printShowingPrintPanel:(BOOL)showPanels {
 
	NSView *printView = [[[webView mainFrame] frameView] documentView];
 
    NSPrintOperation *op = [NSPrintOperation
                printOperationWithView:printView
                printInfo:[self printInfo]];
    [op setShowPanels:showPanels];
    if (showPanels) {
        // Add accessory view, if needed
    }
 
    [self runModalPrintOperation:op
                delegate:nil
                didRunSelector:NULL
                contextInfo:NULL];
}

- (NSArray *)toolbarAllowedItemIdentifiers:(NSToolbar *)toolbar
{
    return [NSArray arrayWithObjects:NSToolbarSeparatorItemIdentifier,
                                     NSToolbarSpaceItemIdentifier,
                                     NSToolbarFlexibleSpaceItemIdentifier,
									 NSToolbarSeparatorItemIdentifier,
									 NSToolbarPrintItemIdentifier,
									 BackButtonIdentifier, 
									 ForwardButtonIdentifier,
									 HomeButtonIdentifier, 
									 BiggerButtonIdentifier, 
									 SearchPageIdentifier, 
									 SmallerButtonIdentifier, nil];
}

- (NSArray *)toolbarDefaultItemIdentifiers:(NSToolbar *)toolbar
{
    return [NSArray arrayWithObjects:HomeButtonIdentifier, 
									 BackButtonIdentifier, 
									 ForwardButtonIdentifier, 
									 NSToolbarSeparatorItemIdentifier,
									 BiggerButtonIdentifier, 
									 SmallerButtonIdentifier,
									 NSToolbarSeparatorItemIdentifier, 
									 NSToolbarPrintItemIdentifier, 
									 NSToolbarFlexibleSpaceItemIdentifier,
									 SearchPageIdentifier, nil];
}

- (void) searchUsingToolbarSearchField:(id) sender {
    // This message is sent when the user strikes return in the search field in the toolbar 
    NSString *searchString = [(NSTextField *)[activeSearchItem view] stringValue];
	if ([searchString length] != 0)
		[self loadURL:[NSURL URLWithString: [defaultSearchURL stringByAppendingString: searchString]]];
}

- (void) searchMenuFormRepresentationClicked:(id) sender {

    //[[documentWindow toolbar] setDisplayMode: NSToolbarDisplayModeIconOnly];
//    [documentWindow makeFirstResponder:[activeSearchItem view]];
}

- (void) searchUsingSearchPanel:(id) sender {
    // This message is sent from the search items custom menu representation 
  //  NSBeginInformationalAlertSheet ( @"searchUsingSearchPanel is not implemented (left as an exercise to the reader   )",@"",@"",@"",documentWindow,nil,nil,nil,nil,@"");
}

- (BOOL)readFromURL:(NSURL *)URL ofType:(NSString *)type error:(NSError **)error
{
	if (webView != nil) {
		[self loadURL:URL];
	} else {
		[self setURLToLoad:URL];
	}
	*error = nil;
	return YES;
}

- (BOOL)readFromFile:(NSString *)path ofType:(NSString *)type
{
	// This method is called on Panther and is deprecated on Tiger. 
	// On Tiger, readFromURL:ofType:error is called instead.
	NSError *error;
	return [self readFromURL:[NSURL fileURLWithPath:path] ofType:type error:&error];
}

- (IBAction)connectURL:(id)sender
{
	[self loadURL:[NSURL URLWithString:[sender stringValue]]];
}

// Methods used to update the load status

// Updates the status and error messages
- (void)updateWindow
{
//    if (resourceStatus != nil)
//        [[webView window] setTitle: [NSString stringWithFormat: @"%@:  %@", docTitle, resourceStatus]];
//    else if (frameStatus != nil)
//        [[webView window] setTitle: [NSString stringWithFormat: @"%@:  %@", docTitle, frameStatus]];
//    else if (docTitle != nil)
        [[webView window] setTitle: [NSString stringWithFormat: @"%@", docTitle]];
//   else
//        [[webView window] setTitle: @""];
//    if (url != nil)
//        [textField setStringValue:url];
}

// Updates the resource status and error messages
- (void)updateResourceStatus
{
    if (resourceFailedCount)
        [self setResourceStatus: [NSString stringWithFormat: @"Loaded %d of %d, %d resource errors", resourceCompletedCount, resourceCount - resourceFailedCount, resourceFailedCount]];
    else
        [self setResourceStatus: [NSString stringWithFormat: @"Loaded %d of %d", resourceCompletedCount, resourceCount]];
    [[webView window] setTitle: [NSString stringWithFormat: @"%@", docTitle, resourceStatus]];
}


// Accessor methods for instance variables

- (NSString *)docTitle
{
    return docTitle;
}

- (void)setDocTitle: (NSString *)t
{
    [docTitle release];
    docTitle = [t retain];
}

- (NSString *)frameStatus
{
    return frameStatus;
}

- (void)setFrameStatus: (NSString *)s
{
    [frameStatus release];
    frameStatus = [s retain];
}

- (NSString *)resourceStatus
{
    return resourceStatus;
}

- (void)setResourceStatus: (NSString *)s
{
    [resourceStatus release];
    resourceStatus = [s retain];
}

- (NSString *)url
{
    return url;
}

- (void)setURL: (NSString *)u
{
    [url release];
    url = [u retain];
}


// WebFrameLoadDelegate Methods

- (void)webView:(WebView *)sender didStartProvisionalLoadForFrame:(WebFrame *)frame
{
    // Only report feedback for the main frame.
    if (frame == [sender mainFrame]) {
        // Reset resource status variables
        resourceCount = 0;    
        resourceCompletedCount = 0;
        resourceFailedCount = 0;
    

        [self setFrameStatus:@"Loading..."];
        [self setURL:[[[[frame provisionalDataSource] request] URL] absoluteString]];
        [self updateWindow];
    }
}

- (void)webView:(WebView *)sender didReceiveTitle:(NSString *)title forFrame:(WebFrame *)frame
{
    // Only report feedback for the main frame.
    if (frame == [sender mainFrame]) {
        [self setDocTitle:title];
        [self updateWindow];
    }
}

- (void)webView:(WebView *)sender didFinishLoadForFrame:(WebFrame *)frame
{
    // Only report feedback for the main frame.
    if (frame == [sender mainFrame]) {
        [self setFrameStatus: nil];
        [self updateWindow];
    }
}

- (void)webView:(WebView *)sender didFailProvisionalLoadWithError:(NSError *)error forFrame:(WebFrame *)frame
{
    // Only report feedback for the main frame.
    if (frame == [sender mainFrame]) {
        [self setDocTitle: @""];
        [self setFrameStatus:[error description]];
        [self updateWindow];
    }
}


// WebUIDelegate Methods

- (WebView *)webView:(WebView *)sender createWebViewWithRequest:(NSURLRequest *)request
{
    MyDocument *myDocument = [[NSDocumentController sharedDocumentController] openUntitledDocumentOfType:@"HTML Document" display:YES];
    [[[myDocument webView] mainFrame] loadRequest:request];
    
    return [myDocument webView];
}

- (void)webViewShow:(WebView *)sender
{
    id myDocument = [[NSDocumentController sharedDocumentController] documentForWindow:[sender window]];
    [myDocument showWindows];
}


// WebResourceLoadDelegate Methods

- (id)webView:(WebView *)sender identifierForInitialRequest:(NSURLRequest *)request fromDataSource:(WebDataSource *)dataSource
{
    // Return some object that can be used to identify this resource
    return [NSNumber numberWithInt: resourceCount++];    
}

-(NSURLRequest *)webView:(WebView *)sender resource:(id)identifier willSendRequest:(NSURLRequest *)request redirectResponse:(NSURLResponse *)redirectResponsefromDataSource:(WebDataSource *)dataSource
{
    // Update the status message
    [self updateResourceStatus];
    return request;
}

-(void)webView:(WebView *)sender resource:(id)identifier didFailLoadingWithError:(NSError *)error fromDataSource:(WebDataSource *)dataSource
{
    // Increment the failed count and update the status message
    resourceFailedCount++;
    [self updateResourceStatus];
}

-(void)webView:(WebView *)sender resource:(id)identifier didFinishLoadingFromDataSource:(WebDataSource *)dataSource
{
    // Increment the success count and pdate the status message
    resourceCompletedCount++;
    [self updateResourceStatus];    
}

@end
