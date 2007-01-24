/* AppController */

#import <Cocoa/Cocoa.h>
#import <WebKit/WebKit.h>

@interface AppController : NSObject
{
	NSTask *httpd;
}
- (void)dealloc;
- (void)applicationDidFinishLaunching:(NSNotification *)aNotification;
@end
