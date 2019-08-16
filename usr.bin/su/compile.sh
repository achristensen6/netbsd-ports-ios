clang -isysroot /Users/coolstar/jb/bad-iOS12.4-SDK/iPhoneOS12.4.sdk -arch arm64 -miphoneos-version-min=9.0 su.c suutil.c -o $(basename $(pwd)) -DBSD4_4
ldid -S/Users/coolstar/Downloads/Ent.plist $(basename $(pwd))
