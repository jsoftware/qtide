0. Install android sdk

  download from http://developer.android.com/sdk/index.html
  
  set related  environment variables.

1. edit the following files:

AndroidManifest.xml
  package name

res/values/strings.xml
  app_name

2. install folder

  tar all files under your ~install folder to a file named jqtdata.tgz, 

  executables and shared objects (jconsole, libj.so, libjpcre.so etc) should not be included.

  copy and replace jqtdata.tgz in the template.

3. welcome.ijs

  edit welcome.ijs that will start your application.

4. build apk

  build using command (ant is a command line tool)

    ant release

5. (optional) sign the apk

  followig instructions at http://developer.android.com/tools/publishing/app-signing.html
