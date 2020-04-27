#include "registry.h"
#include "winutils.h"

Registry::Registry() {}

QString Registry :: getW3dir() {
    CRegKey reg;
    if (reg.Open(HKEY_CURRENT_USER, _T("Software\\Eurobattle.net"), KEY_READ | KEY_WOW64_64KEY)==ERROR_SUCCESS) {
        QString s=this->getRegString(&reg, "w3dir");
        reg.Close();
        return s;
    }
    else return "Registry error";
}

QString Registry :: getEuroPath() {
    CRegKey reg;
    if (reg.Open(HKEY_CURRENT_USER, _T("Software\\Eurobattle.net"), KEY_READ | KEY_WOW64_64KEY)==ERROR_SUCCESS) {
        QString s=this->getRegString(&reg, "europath");
        reg.Close();
        return s;
    }
    else return "Registry error";
}

QString Registry :: getRegString(CRegKey * reg, QString name) {
    wchar_t * szBuffer = new wchar_t[MAX_PATH];
    ULONG bufsize=MAX_PATH;
    std::wstring w=name.toStdWString();

    DWORD s = reg->QueryStringValue(w.c_str(), szBuffer, &bufsize);
    if (s == ERROR_SUCCESS) {
        QString s=QString::fromWCharArray(szBuffer);
        delete[] szBuffer;
        return s;
    }
    else {
        return "";
    }
}

DWORD Registry :: setBnetGateways() {
    CRegKey reg;
    if (reg.Open(HKEY_CURRENT_USER, _T("Software\\Blizzard Entertainment\\Warcraft III"), KEY_WRITE | KEY_WOW64_64KEY)==ERROR_SUCCESS){
        wchar_t const *lines[15]= {
            _T("1008"),
            _T("05"),
            _T("uswest.battle.net"),
            _T("8"),
            _T("Lordaeron (U.S. West)"),
            _T("useast.battle.net"),
            _T("6"),
            _T("Azeroth (U.S. East)"),
            _T("asia.battle.net"),
            _T("-9"),
            _T("Kalimdor (Asia)"),
            _T("europe.battle.net"),
            _T("-1"),
            _T("Northrend (Europe)"),
            _T("")
        };
        size_t totalSize=0;
        for (int i=0; i<15; i++) {
            totalSize+=(wcslen(lines[i])+1)*sizeof(wchar_t);
        }

        LPBYTE pString;
        pString = static_cast<LPBYTE>(malloc(totalSize));

        size_t offset=0;
        for (int i=0; i<15; i++) {
            size_t len=(wcslen(lines[i])+1)*sizeof(wchar_t);
            memcpy(pString+offset, lines[i], len);
            offset+=len;
        }

        DWORD r = RegSetValueEx(reg, _T("Battle.net Gateways"), 0, REG_MULTI_SZ, pString, totalSize);

        reg.Close();
        delete pString;

        return r;
    }
    else {
        return GetLastError();
    }
    return GetLastError();
}

bool Registry::removeInstallationEntries() {
    CRegKey reg;

    if (Winutils::IsWow64()) {
        if (reg.Open(HKEY_LOCAL_MACHINE, _T("Software\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall"), KEY_ALL_ACCESS | KEY_WOW64_64KEY)==ERROR_SUCCESS) {
            DWORD ret = reg.RecurseDeleteKey(_T("Eurobattle.net"));
            reg.Close();
            if (ret==ERROR_SUCCESS) return true;
            else return false;

        }
        return false;
    }
    else {
        if (reg.Open(HKEY_LOCAL_MACHINE, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall"), KEY_ALL_ACCESS | KEY_WOW64_64KEY)==ERROR_SUCCESS) {
            DWORD ret = reg.RecurseDeleteKey(_T("Eurobattle.net"));
            reg.Close();
            if (ret==ERROR_SUCCESS) return true;
            else return false;

        }
        return false;
    }
}
