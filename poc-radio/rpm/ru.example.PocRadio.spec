Name:       ru.example.PocRadio
Summary:    Учебная PTT-рация для ОС Аврора
Version:    0.1.0
Release:    1
License:    BSD-3-Clause
Packager:   Student <student@example.com>
URL:        https://example.com/poc-radio
Source0:    %{name}-%{version}.tar.bz2

Requires:   sailfishsilica-qt5 >= 0.10.9
BuildRequires:  pkgconfig(auroraapp)
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Network)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)

%description
Каркас учебного приложения Push-to-Talk: QML, C++ и состояния рации.

%prep
%autosetup

%build
%qmake5
%make_build

%install
%qmake5_install

%files
%defattr(-,root,root,-)
%{_bindir}/%{name}
%defattr(644,root,root,-)
%{_datadir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/*/apps/%{name}.png
