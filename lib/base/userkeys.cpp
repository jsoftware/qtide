/* userkeys */

#include "state.h"

// ---------------------------------------------------------------------
// this has the list of keys that may be user defined
QStringList Config::getuserkeys()
{
  QStringList r;
  r
      << "Ctrl+Shift+A"
      << "Ctrl+Shift+C"
      << "Ctrl+Shift+D"
      << "Ctrl+Shift+G"
      << "Ctrl+H"
      << "Ctrl+Shift+H"
      << "Ctrl+I"
      << "Ctrl+Shift+I"
      << "Ctrl+Shift+J"
      << "Ctrl+K"
      << "Ctrl+P"
      << "Ctrl+Shift+Q"
      << "Ctrl+U"
      << "Ctrl+Shift+U"
      << "Ctrl+Shift+V"
      << "Ctrl+Shift+X"
      << "Ctrl+Shift+Y"
      << "F2"
      << "Ctrl+F2"
      << "Ctrl+Shift+F2"
      << "Shift+F2"
      << "F3"
      << "Ctrl+F3"
      << "Ctrl+Shift+F3"
      << "Shift+F3"
      << "F4"
      << "Ctrl+F4"
      << "Ctrl+Shift+F4"
      << "Shift+F4"
      << "F6"
      << "Ctrl+F6"
      << "Ctrl+Shift+F6"
      << "Shift+F6"
      << "F7"
      << "Ctrl+F7"
      << "Ctrl+Shift+F7"
      << "Shift+F7"
      << "F11"
      << "Ctrl+F11"
      << "Ctrl+Shift+F11"
      << "Shift+F11"
      << "F12"
      << "Ctrl+F12"
      << "Ctrl+Shift+F12"
      << "Shift+F12";
  return r;
}
