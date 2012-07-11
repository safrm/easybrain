Name:       easybrain
Summary:    Offline notes editor and manager
Version:    0.0.1
Release:    1
Group:      System/Libraries
License:    LGPL v2.1
URL:        http://easybrain.safrm.net
Vendor:     Miroslav Safr <miroslav.safr@gmail.com>
Source0:    %{name}-%{version}.tar.bz2
Autoreq: on
Autoreqprov: on

%description
Offline notes editor and manager which uses own ftp server and password protected encryption.


%prep
%setup -c -n ./%{name}-%{version}
# >> setup
# << setup

%build
# >> build pre
qmake install_prefix=/usr
# << build pre
make %{?jobs:-j%jobs}

# >> build post
# << build post

%install
rm -rf %{buildroot}
# >> install pre
export INSTALL_ROOT=%{buildroot}
# << install pre 
make install

# >> install post
# << install post






%files
%defattr(-,root,root,-)
# >> files
%{_bindir}/easybrain
%{_datadir}/applications/easybrain.desktop
%{_datadir}/icons/hicolor/scalable/apps/easybrain.svg
%{_datadir}/doc/easybrain/readme.txt 
%{_datadir}/doc/easybrain/todo.txt 
%{_datadir}/doc/easybrain/LICENSE.LGPL
# << files


