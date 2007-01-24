
#import "AppController.h"

static NSString * documentRoot = @"/Volumes/MOULINCD/datas";

@implementation AppController

- (void)dealloc
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    [super dealloc];
}

- (id)init
{
	[super init];
	
	NSFileManager *fileManager = [NSFileManager defaultManager];

	NSAlert *alert = [[NSAlert alloc] init];
	[alert addButtonWithTitle:@"Retry"];
	[alert addButtonWithTitle:@"Quit"];
	[alert setMessageText:@"Couldn't read CDROM"];
	[alert setInformativeText:@"The CDROM datas couldn't be found."];
	[alert setAlertStyle:NSWarningAlertStyle];
	
	while ([fileManager fileExistsAtPath: [documentRoot stringByAppendingString:@"/index.db"]] == nil)
	{
		if ([alert runModal] == NSAlertFirstButtonReturn) {

		} else {

			[[NSApplication sharedApplication] terminate:self];
		}
	}
	
	[alert release];
	
	if (![fileManager fileExistsAtPath:@"/tmp/.moulin_index.db"])
		[fileManager copyPath:[documentRoot stringByAppendingString:@"/index.db"] toPath:@"/tmp/.moulin_index.db" handler:nil];
	if (![fileManager fileExistsAtPath:@"/tmp/.moulin_math_index.db"])
		[fileManager copyPath:[documentRoot stringByAppendingString:@"/math_index.db"] toPath:@"/tmp/.moulin_math_index.db" handler:nil];

	// httpd starts
	httpd = [[NSTask alloc] init];
	NSString *path = [[[NSBundle mainBundle] bundlePath] stringByAppendingString: @"/Contents/Resources/moulin_httpd"];
	NSLog(@"path: %@\n", path);
    [httpd setLaunchPath: path];

    NSArray *arguments;
    arguments = [NSArray arrayWithObjects: @"-p", @"2111", @"-d", documentRoot, @"-T", @"utf-8", @"-D", nil];
    [httpd setArguments: arguments];

	[httpd launch];
	
	while(![httpd isRunning])
	{
		NSLog(@"waiting 1 second for moulin_httpd to start");
		system("/bin/sleep 1");	
	}


	return self;
}

- (void)applicationWillTerminate:(NSNotification *)notification {
	[httpd terminate];
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{

}

@end

