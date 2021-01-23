#!/bin/bash

#make the project
make macos

APP_NAME=SwampSurveyor
APP_DIR=${APP_NAME}.app
rm -r $APP_DIR
mkdir $APP_DIR
mkdir ${APP_DIR}/Contents/
mkdir ${APP_DIR}/Contents/MacOS/
mkdir ${APP_DIR}/Contents/Resources/

#copy dylibs for sdl, supporting
cp /usr/local/opt/sdl2/lib/libSDL2-2.0.0.dylib ${APP_DIR}/Contents/MacOS/
cp /usr/local/opt/sdl2_image/lib/libSDL2_image-2.0.0.dylib ${APP_DIR}/Contents/MacOS/
cp /usr/local/opt/sdl2_ttf/lib/libSDL2_ttf-2.0.0.dylib ${APP_DIR}/Contents/MacOS/
cp /usr/local/opt/libpng/lib/libpng16.16.dylib ${APP_DIR}/Contents/MacOS/
cp /usr/local/opt/libtiff/lib/libtiff.5.dylib ${APP_DIR}/Contents/MacOS
cp /usr/local/opt/jpeg/lib/libjpeg.9.dylib ${APP_DIR}/Contents/Macos
cp /usr/lib/libz.1.dylib ${APP_DIR}/Contents/Macos
cp /usr/local/opt/webp/lib/libwebp.7.dylib ${APP_DIR}/Contents/Macos
cp /usr/local/opt/freetype/lib/libfreetype.6.dylib ${APP_DIR}/Contents/Macos

#copy the binary
cp $APP_NAME ${APP_DIR}/Contents/MacOS/

#make the iconset
mkdir ${APP_NAME}.iconset
sips -z 16 16   resources/dist/icon.png --out ${APP_NAME}.iconset/icon_16x16.png
sips -z 32 32   resources/dist/icon.png --out ${APP_NAME}.iconset/icon_16x16@2x.png
sips -z 32 32   resources/dist/icon.png --out ${APP_NAME}.iconset/icon_32x32.png
sips -z 64 64   resources/dist/icon.png --out ${APP_NAME}.iconset/icon_32x32@2x.png
sips -z 128 128 resources/dist/icon.png --out ${APP_NAME}.iconset/icon_128x128.png
sips -z 256 256 resources/dist/icon.png --out ${APP_NAME}.iconset/icon_128x128@2x.png
sips -z 256 256 resources/dist/icon.png --out ${APP_NAME}.iconset/icon_256x256.png
sips -z 512 512 resources/dist/icon.png --out ${APP_NAME}.iconset/icon_256x256@2x.png
sips -z 512 512 resources/dist/icon.png --out ${APP_NAME}.iconset/icon_512x512.png
cp resources/dist/icon.png ${APP_NAME}.iconset/icon_512x512@2x.png
iconutil -c icns ${APP_NAME}.iconset
rm -R ${APP_NAME}.iconset

#add the app icon and metadata
cp resources/dist/Info.plist ${APP_DIR}/Contents/
mv ${APP_NAME}.icns ${APP_DIR}/Contents/Resources

#change dylib paths in executable
install_name_tool -change /usr/local/opt/sdl2/lib/libSDL2-2.0.0.dylib \
                  @executable_path/libSDL2-2.0.0.dylib \
                  ${APP_DIR}/Contents/MacOS/${APP_NAME}
install_name_tool -change /usr/local/opt/sdl2_image/lib/libSDL2_image-2.0.0.dylib \
                  @executable_path/libSDL2_image-2.0.0.dylib \
                  ${APP_DIR}/Contents/MacOS/${APP_NAME}
install_name_tool -change /usr/local/opt/sdl2_ttf/lib/libSDL2_ttf-2.0.0.dylib \
                  @executable_path/libSDL2_ttf-2.0.0.dylib \
                  ${APP_DIR}/Contents/MacOS/${APP_NAME}

#change locations in sdl_image
install_name_tool -change /usr/local/opt/libpng/lib/libpng16.16.dylib \
                  @executable_path/libpng16.16.dylib \
                  ${APP_DIR}/Contents/MacOS/libSDL2_image-2.0.0.dylib

install_name_tool -change /usr/local/opt/libtiff/lib/libtiff.5.dylib \
                  @executable_path/libtiff.5.dylib \
                  ${APP_DIR}/Contents/MacOS/libSDL2_image-2.0.0.dylib

install_name_tool -change /usr/local/opt/jpeg/lib/libjpeg.9.dylib \
                  @executable_path/libjpeg.9.dylib \
                  ${APP_DIR}/Contents/MacOS/libSDL2_image-2.0.0.dylib

install_name_tool -change /usr/lib/libz.1.dylib \
                  @executable_path/libz.1.dylib \
                  ${APP_DIR}/Contents/MacOS/libSDL2_image-2.0.0.dylib

install_name_tool -change /usr/local/opt/webp/lib/libwebp.7.dylib \
                  @executable_path/libwebp.7.dylib \
                  ${APP_DIR}/Contents/MacOS/libSDL2_image-2.0.0.dylib

install_name_tool -change /usr/local/opt/sdl2/lib/libSDL2-2.0.0.dylib \
                  @executable_path/libSDL2-2.0.0.dylib \
                  ${APP_DIR}/Contents/MacOS/libSDL2_image-2.0.0.dylib

#change libtiff
install_name_tool -change /usr/local/opt/jpeg/lib/libjpeg.9.dylib \
                  @executable_path/libjpeg.9.dylib \
                  ${APP_DIR}/Contents/MacOS/libtiff.5.dylib

install_name_tool -change /usr/lib/libz.1.dylib \
                  @executable_path/libz.1.dylib \
                  ${APP_DIR}/Contents/MacOS/libtiff.5.dylib

#change sdl ttf
install_name_tool -change /usr/local/opt/sdl2/lib/libSDL2-2.0.0.dylib \
                  @executable_path/libSDL2-2.0.0.dylib \
                  ${APP_DIR}/Contents/MacOS/libSDL2_ttf-2.0.0.dylib

install_name_tool -change /usr/local/opt/freetype/lib/libfreetype.6.dylib \
                  @executable_path/libfreetype.6.dylib \
                  ${APP_DIR}/Contents/MacOS/libSDL2_ttf-2.0.0.dylib

#change freetype
install_name_tool -change /usr/local/opt/libpng/lib/libpng16.16.dylib \
                  @executable_path/libpng16.16.dylib \
                  ${APP_DIR}/Contents/MacOS/libfreetype.6.dylib

#check dylib location rename
#otool -L ${APP_DIR}/Contents/MacOS/${APP_NAME}

#publish resources
#zip -r resources-1.0.zip resources

#zip
rm ${APP_NAME}.zip
zip -r ${APP_NAME}.zip resources $APP_DIR macos_installer.sh > /dev/null 2>&1
