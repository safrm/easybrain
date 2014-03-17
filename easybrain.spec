Name:       easybrain
Summary:    Offline notes editor and manager
Version:    0.0.1
Release:    1
Group:      Applications/Editors
License:    LGPL v2.1
URL:        http://easybrain.safrm.net/
Vendor:     Miroslav Safr <miroslav.safr@gmail.com>
Source0:    %{name}-%{version}.tar.bz2
Autoreq: on
Autoreqprov: on
BuildRequires:  pkgconfig(QtCore)
BuildRequires:  pkgconfig(QtGui)
BuildRequires:  pkgconfig(QtNetwork)
BuildRequires:  pkgconfig(QtSvg)
BuildRequires:  pkgconfig(QtXml)
BuildRequires:  qt-devel  
BuildRequires:  gcc-c++

%description
Offline notes editor and manager which uses own ftp server and password protected encryption.

%prep
%setup -c -n ./%{name}-%{version}

%build
%if 0%{?fedora} || 0%{?rhel_version} || 0%{?centos_version}
 /usr/bin/qmake-qt4 install_prefix=/usr
%else
 qmake install_prefix=/usr
%endif
make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
export INSTALL_ROOT=%{buildroot}
make install

%clean
rm -fr %{buildroot}

%files
%defattr(-,root,root,-)
%{_bindir}/easybrain
%{_datadir}/applications/easybrain.desktop
%{_datadir}/icons/hicolor/scalable/apps/easybrain.svg
%dir %{_datadir}/doc/easybrain
%{_datadir}/doc/easybrain/readme.txt 


