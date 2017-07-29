// Disable macOS Sierra tab bar (surfaced as: View > Show Tab Bar).
//
// Workaround provided by Morten Sorvig <Morten.Sorvig@qt.io>,
// http://lists.qt-project.org/pipermail/interest/2016-September/024488.html.

#import <AppKit/AppKit.h>

#if !defined(MAC_OS_X_VERSION_10_12) || \
    MAC_OS_X_VERSION_MIN_REQUIRED < MAC_OS_X_VERSION_10_12

@interface NSWindow (SierraSDK)
+ (BOOL) allowsAutomaticWindowTabbing;
+ (BOOL) setAllowsAutomaticWindowTabbing: (BOOL) allowsAutomaticWindowTabbing;
@end

#endif

// Disables auto window tabbing where supported, otherwise a no-op.
void disableWindowTabbing()
{
    if ([NSWindow respondsToSelector:@selector(allowsAutomaticWindowTabbing)]) {
        NSWindow.allowsAutomaticWindowTabbing = NO;
    }
}
