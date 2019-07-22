#include <iostream>
using namespace std;

#include <windows.h>
#include <shlobj.h>
#include <winnls.h>
#include <shobjidl.h>
#include <objbase.h>
#include <objidl.h>
#include <shlguid.h>
#include <winbase.h >


HRESULT CreateLink(LPCSTR lpszPathObj, LPCSTR lpszPathLink, LPCSTR lpszDesc)
{
    CoInitialize(NULL);
    HRESULT hres;
    IShellLink* psl;
    // Get a pointer to the IShellLink interface. It is assumed that CoInitialize
    // has already been called.
    hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl);
    if (SUCCEEDED(hres))
    {
        IPersistFile* ppf;

        // Set the path to the shortcut target and add the description.
        psl->SetPath(lpszPathObj);
        psl->SetDescription(lpszDesc);

        // Query IShellLink for the IPersistFile interface, used for saving the
        // shortcut in persistent storage.
        hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf);

        if (SUCCEEDED(hres))
        {
            WCHAR wsz[MAX_PATH];

            // Ensure that the string is Unicode.
            MultiByteToWideChar(CP_ACP, 0, lpszPathLink, -1, wsz, MAX_PATH);

            // Save the link by calling IPersistFile::Save.
            hres = ppf->Save(wsz, TRUE);
            ppf->Release();
        }
        psl->Release();
    }
    return hres;
}

void Hardlink(LPCSTR Filename,LPCSTR ExistingFileName){
  BOOL fCreatedLink = CreateHardLink( Filename,
                                      ExistingFileName,
                                      NULL ); // reserved, must be NULL

  if ( fCreatedLink == FALSE )
   {
    cout<<GetLastError()<<endl;// handle error condition
   }
   cout<<"HardLink created"<<endl;
}

void Softlink(LPCSTR Filename,LPCSTR ExistingFileName){
  BOOL fCreatedLink;
  //fCreatedLink = CreateSymbolicLink ( Filename,ExistingFileName,0x0 );

  if ( fCreatedLink == FALSE )
   {
    cout<<GetLastError()<<endl;// handle error condition
   }
   cout<<"SoftLink created"<<endl;
}

int main() {
    string command,file_path,link_path;
    HRESULT   hr;
    while(true){
    cout << "Enter a Command: "<<endl;
    cin >>command>>file_path>>link_path;
    if(command=="shortcut"){
        hr=CreateLink(file_path.c_str(),link_path.c_str(),"Shortcut");
        if (S_OK != hr)
        {
            cout<<"Failed to create shortcut"<<hr<<endl;

        }
        else
        {
            cout<<"created shortcut"<<endl;
        }
    }
    else if(command=="slink"){
        Softlink(link_path.c_str(),file_path.c_str());
    }
     else if(command=="hlink"){
        Hardlink(link_path.c_str(),file_path.c_str());
    }
    else{
        cout << "not a command "<<endl;;
    }
    }
    return 0;
}
