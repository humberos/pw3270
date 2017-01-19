#
# spec file for package pw3270
#
# Copyright (c) 2015 SUSE LINUX GmbH, Nuernberg, Germany.
# Copyright (C) <2008> <Banco do Brasil S.A.>
#
# All modifications and additions to the file contributed by third parties
# remain the property of their copyright owners, unless otherwise agreed
# upon. The license for this file, and modifications and additions to the
# file, is the same license as for the pristine package itself (unless the
# license for the pristine package is not an Open Source License, in which
# case the license is the MIT License). An "Open Source License" is a
# license that conforms to the Open Source Definition (Version 1.9)
# published by the Open Source Initiative.

# Please submit bugfixes or comments via http://bugs.opensuse.org/
#

#---[ Versions ]------------------------------------------------------------------------------------------------------

%define MAJOR_VERSION 5
%define MINOR_VERSION 1

#---[ Selected modules ]----------------------------------------------------------------------------------------------

%define _libvrs		%{MAJOR_VERSION}_%{MINOR_VERSION}
%define _dbus     	1

#---[ Packaging ]-----------------------------------------------------------------------------------------------------

Name:           pw3270
Version:        5.1 
Release:        0
Summary:        IBM 3270 Terminal emulator for GTK
License:        GPL-2.0
Group:          System/X11/Terminals
Url:            https://portal.softwarepublico.gov.br/social/pw3270/

Source:         pw3270-%{version}.tar.bz2
#Source1:        %{name}.rpmlintrc

BuildRoot:      %{_tmppath}/%{name}-%{version}-build

Requires:       lib3270-%{_libvrs} = %{version}
Requires:       shared-mime-info

Requires:       libpw3270-%{MAJOR_VERSION}_%{MINOR_VERSION}

#--[ Setup by distribution ]------------------------------------------------------------------------------------------
# 
# References:
#
# https://en.opensuse.org/openSUSE:Build_Service_cross_distribution_howto#Detect_a_distribution_flavor_for_special_code
#

%define _distro		linux


#--[ Red HAT ]--------------------------------------------------------------------------------------------------------

%if 0%{?rhel_version}

%define _distro rhel%{rhel_version}

BuildRequires:  dbus-devel
BuildRequires:  dbus-glib-devel
BuildRequires:  openssl-devel
BuildRequires:  gtk3-devel

%endif

#--[ CentOS ]---------------------------------------------------------------------------------------------------------

%if 0%{?centos_version}

%define _distro centos%{centos_version}

BuildRequires:  pkgconfig(dbus-1)
BuildRequires:  pkgconfig(dbus-glib-1)
BuildRequires:  pkgconfig(openssl)
BuildRequires:  pkgconfig(gtk+-3.0)

%endif

#--[ Fedora ]---------------------------------------------------------------------------------------------------------

%if 0%{?fedora}

%define _distro fedora%{fedora}

BuildRequires:  pkgconfig(dbus-1)
BuildRequires:  pkgconfig(dbus-glib-1)
BuildRequires:  pkgconfig(openssl)
BuildRequires:  pkgconfig(gtk+-3.0)

%endif

#--[ SuSE ]-----------------------------------------------------------------------------------------------------------

%if 0%{?suse_version}

# https://en.opensuse.org/openSUSE:Packaging_Conventions_RPM_Macros#.25sles_version
%if 0%{?is_opensuse}
	%define _distro opensuse%{suse_version}
%else
	%define _distro suse%{suse_version}
%endif

BuildRequires:  libopenssl-devel
BuildRequires:  rsvg-view

BuildRequires:  pkgconfig(dbus-1)
BuildRequires:  pkgconfig(dbus-glib-1)
BuildRequires:  pkgconfig(gtk+-3.0)

%endif

#---------------------------------------------------------------------------------------------------------------------

BuildRequires:  autoconf >= 2.61
BuildRequires:  automake
BuildRequires:  binutils
BuildRequires:  coreutils
BuildRequires:  desktop-file-utils
BuildRequires:  findutils
BuildRequires:  gcc-c++
BuildRequires:  gettext-devel
BuildRequires:  m4
BuildRequires:  pkgconfig
BuildRequires:  sed
BuildRequires:	optipng
BuildRequires:	fdupes

%description
Open-source GTK-based IBM 3270 terminal emulator with many advanced features. It can be used to communicate with any IBM host that supports 3270-style connections over TELNET.
Based on the original x3270 code, pw3270 was originally created for Banco do Brasil, and is now used worldwide. 

#--[ lib3270 ]--------------------------------------------------------------------------------------------------------

%package -n lib3270-%{_libvrs}
Summary:        3270 Communication library for %{name}
Group:          Development/Libraries/C and C++
Requires:       openssl
Provides:       lib3270 = %{version}

%description -n lib3270-%{_libvrs}
Open-source GTK-based IBM 3270 terminal emulator with many advanced features. It can be used to communicate with any IBM host that supports 3270-style connections over TELNET.

This package contains the tn3270 protocol library for %{name}

#--[ Devel ]----------------------------------------------------------------------------------------------------------

%package -n lib3270-devel
Summary:        Devel for 3270 Communication library for %{name}
Group:          Development/Libraries/C and C++
Provides:       lib3270-devel-%{MAJOR_VERSION}_%{MINOR_VERSION}
Requires:       lib3270-%{MAJOR_VERSION}_%{MINOR_VERSION}

%description -n lib3270-devel
Open-source GTK-based IBM 3270 terminal emulator with many advanced features. It can be used to communicate with any IBM host that supports 3270-style connections over TELNET.
This package contains the development files for tn3270 protocol library for %{name}

%package -n %{name}-devel
Summary:        Files required for development of %{name} plugins
Group:          Development/Libraries/C and C++
Requires:       lib3270-devel-%{MAJOR_VERSION}_%{MINOR_VERSION}
Requires:       libpw3270-%{MAJOR_VERSION}_%{MINOR_VERSION}

%description -n %{name}-devel
Open-source GTK-based IBM 3270 terminal emulator with many advanced features. It can be used to communicate with any IBM host that supports 3270-style connections over TELNET.

This package contains the development files for %{name}

#--[ Plugins ]--------------------------------------------------------------------------------------------------------

%if 0%{?_dbus}
%package plugin-dbus
Summary:        DBUS object for %{name}
Group:          System/X11/Terminals
Requires:       %{name} = %{version}
Requires:       dbus-1

%description plugin-dbus
Plugin exporting a DBUS object from every %{name} open session.
%endif

#---[ Build & Install ]-----------------------------------------------------------------------------------------------

%prep

%setup -q -n pw3270-%{version}

echo "m4_define([SVN_REVISION], %{release})" > revision.m4
echo "m4_define([SVN_URL], http://softwarepublico.gov.br/gitlab/pw3270/principal.git)" >> revision.m4
echo "m4_define([APP_LEVEL], 0)" >> revision.m4

aclocal
autoconf

export CFLAGS="$RPM_OPT_FLAGS"
export CXXFLAGS="$RPM_OPT_FLAGS"
export FFLAGS="$RPM_OPT_FLAGS"

%configure

%build
make clean
make all

%install
%makeinstall

%find_lang pw3270 langfiles

cat > pw3270.desktop << EOF
[Desktop Entry]
X-SuSE-translate=true
GenericName=pw3270
Name=3270 Terminal
Comment=IBM 3270 Terminal emulator
Exec=pw3270
Icon=%{_datadir}/pw3270/pw3270.png
Terminal=false
Type=Application
StartupNotify=true
EOF
chmod 644 pw3270.desktop

desktop-file-install	--mode 644 \
			--dir $RPM_BUILD_ROOT/%{_datadir}/applications \
			--add-category System \
			--add-category TerminalEmulator \
			pw3270.desktop

# Java now lives in another package
rm ${RPM_BUILD_ROOT}/%{_datadir}/pw3270/ui/*java*.xml

# ooRexx now lives in another package
rm ${RPM_BUILD_ROOT}/%{_datadir}/pw3270/ui/*rexx*.xml

%fdupes $RPM_BUILD_ROOT

%clean
rm -rf %{buildroot}

#---[ Files ]---------------------------------------------------------------------------------------------------------

%files -f langfiles
%defattr(-,root,root)
%doc AUTHORS LICENSE 
# %{_mandir}/*/*

# Main application
%dir %{_datadir}/pw3270
%dir %{_datadir}/pw3270/ui
%{_bindir}/pw3270
%{_libdir}/libpw3270.so.%{MAJOR_VERSION}
%{_libdir}/libpw3270.so.%{MAJOR_VERSION}.%{MINOR_VERSION}

%{_datadir}/applications/pw3270.desktop

%{_datadir}/pw3270/ui/00default.xml
%{_datadir}/pw3270/ui/10functions.xml
%{_datadir}/pw3270/ui/10keypad.xml
%{_datadir}/pw3270/colors.conf
%{_datadir}/pw3270/pw3270.png
%{_datadir}/pw3270/pw3270-logo.png
%dir %{_libdir}/pw3270-plugins

%files -n lib3270-%{_libvrs}
%defattr(-,root,root)
%{_libdir}/lib3270.so.%{MAJOR_VERSION}.%{MINOR_VERSION}
%{_libdir}/lib3270.so.%{MAJOR_VERSION}

%files -n lib3270-devel
%defattr(-,root,root)
%{_includedir}/lib3270
%{_includedir}/lib3270.h
%{_libdir}/pkgconfig/lib3270.pc
%{_libdir}/lib3270.so
%{_datadir}/pw3270/locale

%files -n %{name}-devel
%defattr(-,root,root)
%{_includedir}/pw3270
%{_includedir}/pw3270.h
%{_datadir}/pw3270/ui/98trace.xml
%{_datadir}/pw3270/ui/99debug.xml
%{_libdir}/libpw3270.so
%{_libdir}/pkgconfig/pw3270.pc

%{_libdir}/libpw3270cpp.a
%{_includedir}/pw3270cpp.h

%if 0%{?_dbus}
%files plugin-dbus
%defattr(-,root,root)
%{_libdir}/pw3270-plugins/dbus3270.so
%endif

#---[ Scripts ]-------------------------------------------------------------------------------------------------------

%post
/sbin/ldconfig
exit 0

%postun
/sbin/ldconfig
exit 0

%post -n lib3270-%{_libvrs}
/sbin/ldconfig
exit 0

%postun -n lib3270-%{_libvrs}
/sbin/ldconfig
exit 0

%changelog
