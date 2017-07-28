// Disable macOS Sierra tab bar (surfaced as: View > Show Tab Bar).
//
// Workaround provided by Morten Sorvig <Morten.Sorvig@qt.io>,
// http://lists.qt-project.org/pipermail/interest/2016-September/024488.html.

#import <AppKit/AppKit.h>

// Disables auto window tabbing where supported, otherwise a no-op.
void disableWindowTabbing()
{
    if ([NSWindow respondsToSelector:@selector(allowsAutomaticWindowTabbing)]) {
        NSWindow.allowsAutomaticWindowTabbing = NO;
    }
}
