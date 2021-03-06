#!/bin/bash

APPLEVEL="0"

test -n "$srcdir" || srcdir=`dirname "$0"`
test -n "$srcdir" || srcdir=.

olddir=`pwd`
cd "$srcdir"

if test -e revision ; then
	. revision
fi

touch ChangeLog

# Inicia com os defaults
TEMPFILE=autogen.tmp

if [ -d .svn ]; then

	# Tenta detectar a versão
	SVNVERSION=$(which svnversion 2> /dev/null)
	if test -x "${SVNVERSION}" ; then
		svnversion | cut -d: -f2 | sed 's@[M|m]@@g' > ${TEMPFILE} 2> /dev/null
		if [ "$?" == "0" ]; then
			PACKAGE_REVISION=$(cat ${TEMPFILE})
		fi
	fi

	# Tenta detectar a URL
	SVN=$(which svn 2> /dev/null)
	if test -x "${SVN}" ; then

		LANG="EN_US" "${SVN}" info > ${TEMPFILE} 2>&1
		if [ "$?" == "0" ]; then
			PACKAGE_SOURCE=$(cat ${TEMPFILE} | grep "^URL: " | cut -d" " -f2)
		fi

	fi

elif [ -d .git ]; then

	# Obtém revisão via git
	# Referência: http://stackoverflow.com/questions/4120001/what-is-the-git-equivalent-for-revision-number

	# Obtém URL via git
	PACKAGE_SOURCE=$(git config --get remote.origin.url)

	# Obtém número total de commits
	PACKAGE_REVISION=$(git rev-list HEAD --count)

fi

if test -z $PACKAGE_REVISION ; then
	echo "Can´t detect package revision, using current date"
	PACKAGE_REVISION=`date +%y%m%d`
fi

if test -z $PACKAGE_SOURCE ; then
	echo "Can´t detect package source, using default one"
	PACKAGE_SOURCE="https://portal.softwarepublico.gov.br/social/pw3270/"
fi

echo "PACKAGE_REVISION=$PACKAGE_REVISION" > $srcdir/revision
echo "PACKAGE_SOURCE=$PACKAGE_SOURCE" >> $srcdir/revision
echo "PACKAGE_LEVEL=$APPLEVEL" >> $srcdir/revision

echo "m4_define([SVN_REVISION], $PACKAGE_REVISION)" > $srcdir/revision.m4
echo "m4_define([SVN_URL], $PACKAGE_SOURCE)" >> $srcdir/revision.m4
echo "m4_define([APP_LEVEL], $APPLEVEL)" >> $srcdir/revision.m4

aclocal
if test $? != 0 ; then
	echo "aclocal failed."
	exit -1
fi

autoconf
if test $? != 0 ; then
	echo "autoconf failed."
	exit -1
fi

echo "Package set to revision $PACKAGE_REVISION and source $PACKAGE_SOURCE"

cd "$olddir"
test -n "$NOCONFIGURE" || "$srcdir/configure" "$@"

