Instructions
==================

Build on OS X 10.9.2 Mavericks with XCode 5.0.2

* Use `mysql-connector-c-6.0.2.tar.gz` (also available from [here](http://www.mysql.com/downloads/connector/c/#downloads))
* [Download](http://www.cmake.org/cmake/resources/software.html) and install CMake for Mac OS X Universal
* Generate XCode project file running 
```bash
cd path/to/mysql-connector-c-6.0.2
cmake -G Xcode
```
* In file `include/my_net.h`, comment out line 47 and 49 
```c++
//#include <netinet/in_systm.h>
#include <netinet/in.h>
//#include <netinet/ip.h>
```
* In file `include/my_global.h`, change line 129 to 
```c++
if defined(__i386__) || defined(__ppc__) || defined(__arm__)
```
* In file `include/my_config.h`, change line 63 to 
```c++
/* #define HAVE_FDATASYNC 1 */
```
* Now build with `xcodebuild` command for various architectures 
```bash
xcodebuild -project libmysql.xcodeproj -target mysqlclient -configuration Release -sdk iphonesimulator7.0 ONLY_ACTIVE_ARCH=NO ARCHS=i386 PRODUCT_NAME=mysqlclient_simulator
xcodebuild -project libmysql.xcodeproj -target mysqlclient -configuration Release -sdk iphoneos7.0 ONLY_ACTIVE_ARCH=NO ARCHS="armv6 armv7 armv7s" PRODUCT_NAME=mysqlclient_device
```
* Combine them into a single `libmysqlclient.a` file 
```bash
lipo libmysql/Release-iphoneos/libmysqlclient_device.a libmysql/Release-iphonesimulator/libmysqlclient_simulator.a -create -output libmysqlclient.a
```
* You've done! New ios library is located in `./libmysqlclient.a`
* Now, to build the `libmysqlclient.dylib` for Mac Os X, run (not tested yet) 
```bash
xcodebuild -project libmysql.xcodeproj -target libmysql -configuration Release -sdk macosx10.9 -mmacosx-version-min=10.9 ONLY_ACTIVE_ARCH=NO ARCHS="x86_64"
```