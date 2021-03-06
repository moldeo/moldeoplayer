#!/bin/sh

DIE=0

echo "deb directory..."
(mkdir deb ) > /dev/null || {
	echo "cleaning..."

	(rm -Rf deb/*) > /dev/null || {
	  echo
	  echo "**Error**: removing error ."
	  echo ""
	  exit 1
	}
}



echo "making distribution source file..."
(make dist) > /dev/null || {
  echo
  echo "**Error**: make dist ."
  echo ""
  exit 1
}


echo "copying..."

(mv moldeoplayerglut-*.tar.gz ./deb/) > /dev/null || {
  echo
  echo "**Error**: moving file ."
  echo ""
  exit 1
}


echo "renaming..."
( rename 's/moldeoplayerglut/moldeoplayerglut/' deb/*.tar.gz ) > /dev/null || {
  echo
  echo "**Error**: renaming to moldeoplayerglut."
  echo ""
  exit 1
}
( rename 's/\.tar.gz/\.orig.tar.gz/' deb/*.tar.gz ) > /dev/null || {
  echo
  echo "**Error**: renaming to orig."
  echo ""
  exit 1
}
( rename 's/\-/\_/' deb/*.tar.gz ) > /dev/null || {
  echo
  echo "**Error**: renaming to underscore."
  echo ""
  exit 1
}
( rename 's/A-Z/a-z/' deb/*.tar.gz ) > /dev/null || {
  echo
  echo "**Error**: renaming to lowercase."
  echo ""
  exit 1
}

echo "extracting..."

(tar -C deb -zxvf ./deb/*.orig.tar.gz ) > /dev/null || {
  echo
  echo "**Error**: extracting ."
  echo ""
  exit 1
}

( rename 's/moldeoplayerglut/moldeoplayerglut/' deb/wxMoldeoControlsTest-* ) > /dev/null || {
  echo
  echo "**Error**: renaming to moldeoplayerglut."
  echo ""
  exit 1
}

echo " 
Now execute in deb folder:
 cd deb/moldeoplayerglut-*
 dh_make -s -e info@moldeointeractive.com.ar -p moldeoplayerglut
 cp ../../control.amd64 debian/control
 vi debian/changelog
 dpkg-buildpackage -us -uc -rfakeroot 2>&1 | tee ../../buildpkg_logs.txt
"


echo "Success: extraction"
exit 0
