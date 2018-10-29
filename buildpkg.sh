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

(mv moldeoplayer-*.tar.gz ./deb/) > /dev/null || {
  echo
  echo "**Error**: moving file ."
  echo ""
  exit 1
}


echo "renaming..."
( rename 's/\.tar.gz/\.orig.tar.gz/' deb/*.tar.gz ) > /dev/null || {
  echo
  echo "**Error**: renaming ."
  echo ""
  exit 1
}
( rename 's/\-/\_/' deb/*.tar.gz ) > /dev/null || {
  echo
  echo "**Error**: renaming ."
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

cd deb/moldeoplayer-*
dh_make -s -e info@moldeo.org -p moldeoplayer -c gpl3

echo "Success: Extraction"

sed -i -e 's/Architecture: any/Architecture: amd64/g' debian/control
sed -i -e 's/Section: unknown/Section: graphics/g' debian/control
sed -i -e 's/Maintainer: fabricio /Maintainer: Moldeo Interactive /g' debian/control
sed -i -e 's/<insert the upstream URL, if relevant>/http:\\\\www.moldeo.org/g' debian/control
#sed -i -e 's/libmoldeoBROKEN/libmoldeo/g' debian/control
sed -i -e 's/,autotools-dev/, autotools-dev, libmoldeo-dev, libsdl2-dev, libsdl1.2-dev, freeglut3-dev/g' debian/control

#development
#sed -i -e '0,/, ${misc:Depends}/{s/, ${misc:Depends}/,${misc:Depends}, liblua5.1-0-dev, libgstreamer1.0-dev, libgstreamer-plugins-base1.0-dev, libglew-dev, libalut-dev/g}' debian/control
#sed -i -e '0,/<insert up to 60 chars description>/{s/<insert up to 60 chars description>/Moldeo Core Library, Dev Package/}'  debian/control

#binary
sed -i -e 's/, ${misc:Depends}/,${misc:Depends}, libmoldeo, libsdl2-2.0-0, libsdl1.2debian, freeglut3/g' debian/control
sed -i -e 's/<insert up to 60 chars description>/Moldeo Player/'  debian/control
sed -i -e 's/<insert long description, indented with spaces>/Moldeo Player for Moldeo Software Platform/g'  debian/control

sed -i -e 's/unstable/experimental/g' debian/changelog
sed -i -e 's/fabricio/Moldeo Interactive/g' debian/changelog
sed -i -e 's/Initial release (Closes: #nnnn)  <nnnn is the bug number of your ITP>/Initial release/g' debian/changelog
#sed -i -e 's///g'  debian/control
#
#sed -i -e 's///g' debian/control


echo "debian/moldeoplayer/usr/bin/*" > debian/moldeoplayer.install

echo "usr/bin" > debian/moldeoplayer.dirs

#echo "usr/lib" > debian/moldeoplayer.dirs

#echo "usr/include/*" > debian/moldeoplayer-dev.install

xed ../../control.amd64.11.10 debian/control ../../moldeoplayer.dirs debian/moldeoplayer.dirs ../../moldeoplayer.install debian/moldeoplayer.install ../../moldeoplayer-dev.dirs debian/moldeoplayer-dev.dirs ../../moldeoplayer-dev.install debian/moldeoplayer-dev.install debian/changelog

dpkg-buildpackage -us -uc -rfakeroot 2>&1 | tee ../../buildpkg_logs.txt



echo "Success: packaging"
exit 0
