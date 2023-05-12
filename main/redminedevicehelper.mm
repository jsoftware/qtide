#include "redminedevicehelper.h"
#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>

// Device Type
bool isPhone = ([UIDevice currentDevice].userInterfaceIdiom == UIUserInterfaceIdiomPhone); 
bool isPad = ([UIDevice currentDevice].userInterfaceIdiom == UIUserInterfaceIdiomPad); 
bool isAppleTV = ([UIDevice currentDevice].userInterfaceIdiom == UIUserInterfaceIdiomTV);

//Device scale factor 
float ios_scale = (float)([UIScreen mainScreen].scale);

//Type of device display 
bool isRetina = ([[UIScreen mainScreen] respondsToSelector:@selector(displayLinkWithTarget:selector:)] && ([UIScreen mainScreen].scale == 2.0)) ? true : false;

QString ios_model = QString::fromNSString([[UIDevice currentDevice] model]);
