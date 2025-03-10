///////////////////////////////////////////////////////////////////////////////
// Name:        src/common/stdpbase.cpp
// Purpose:     wxStandardPathsBase methods common to all ports
// Author:      Vadim Zeitlin
// Created:     2004-10-19
// Copyright:   (c) 2004 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// for compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"


#ifndef WX_PRECOMP
    #include "wx/app.h"
#endif //WX_PRECOMP
#include "wx/apptrait.h"

#include "wx/filename.h"
#include "wx/stdpaths.h"

// ----------------------------------------------------------------------------
// module globals
// ----------------------------------------------------------------------------

namespace
{

// Derive a class just to be able to create it: wxStandardPaths ctor is
// protected to prevent its misuse, but it also means we can't create an object
// of this class directly.
class wxStandardPathsDefault : public wxStandardPaths
{
public:
    wxStandardPathsDefault() { }
};

static wxStandardPathsDefault gs_stdPaths;

} // anonymous namespace

// ============================================================================
// implementation
// ============================================================================

/* static */
wxStandardPaths& wxStandardPathsBase::Get()
{
    return wxApp::GetValidTraits().GetStandardPaths();
}

wxString wxStandardPathsBase::GetExecutablePath() const
{
    if ( !wxTheApp || !wxTheApp->argv )
        return wxEmptyString;

    wxString argv0 = wxTheApp->argv[0];
    if (wxIsAbsolutePath(argv0))
        return argv0;

    // Search PATH.environment variable...
    wxPathList pathlist;
    pathlist.AddEnvList(wxT("PATH"));
    wxString path = pathlist.FindAbsoluteValidPath(argv0);
    if ( path.empty() )
        return argv0;       // better than nothing

    return wxFileName(path).GetAbsolutePath();
}

wxStandardPaths& wxAppTraitsBase::GetStandardPaths()
{
    return gs_stdPaths;
}

wxStandardPathsBase::wxStandardPathsBase()
{
    // Set the default information that is used when
    // forming some paths (by AppendAppInfo).
    // Derived classes can call this in their constructors
    // to set the platform-specific settings
    UseAppInfo(AppInfo_AppName);

    // Default for compatibility with the existing config files.
    SetFileLayout(FileLayout_Classic);
}

wxStandardPathsBase::~wxStandardPathsBase()
{
    // nothing to do here
}

wxString wxStandardPathsBase::GetLocalDataDir() const
{
    return GetDataDir();
}

wxString wxStandardPathsBase::GetUserLocalDataDir() const
{
    return GetUserDataDir();
}

wxString wxStandardPathsBase::GetAppDocumentsDir() const
{
    const wxString docsDir = GetDocumentsDir();
    wxString appDocsDir = AppendAppInfo(docsDir);

    return wxDirExists(appDocsDir) ? appDocsDir : docsDir;
}

// return the temporary directory for the current user
wxString wxStandardPathsBase::GetTempDir() const
{
    return wxFileName::GetTempDir();
}

wxString wxStandardPathsBase::GetUserDir(Dir WXUNUSED(userDir)) const
{
    return wxFileName::GetHomeDir();
}

/* static */
wxString
wxStandardPathsBase::AppendPathComponent(const wxString& dir,
                                         const wxString& component)
{
    wxString subdir(dir);

    // empty string indicates that an error has occurred, don't touch it then
    if ( !subdir.empty() )
    {
        if ( !component.empty() )
        {
            const wxUniChar ch = *(subdir.end() - 1);
            if ( !wxFileName::IsPathSeparator(ch) && ch != wxT('.') )
                subdir += wxFileName::GetPathSeparator();

            subdir += component;
        }
    }

    return subdir;
}


wxString wxStandardPathsBase::AppendAppInfo(const wxString& dir) const
{
    wxString subdir(dir);

    if ( UsesAppInfo(AppInfo_VendorName) )
    {
        subdir = AppendPathComponent(subdir, wxTheApp->GetVendorName());
    }

    if ( UsesAppInfo(AppInfo_AppName) )
    {
        subdir = AppendPathComponent(subdir, wxTheApp->GetAppName());
    }

    return subdir;
}

wxString wxStandardPathsBase::GetSharedLibrariesDir() const
{
    return {};
}
