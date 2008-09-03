#include "moNewProject.h"

#include <wx/dirdlg.h>

//(*InternalHeaders(moNewProject)




//(*IdInit(moNewProject)









BEGIN_EVENT_TABLE(moNewProject,wxDialog)
	//(*EventTable(moNewProject)
	//*)
END_EVENT_TABLE()

moNewProject::moNewProject(wxWindow* parent,wxWindowID id)
{
	//(*Initialize(moNewProject)

















	OkButton->Disable();

}

moNewProject::~moNewProject()
{
	//(*Destroy(moNewProject)
	//*)
}


void moNewProject::OnOkClick(wxCommandEvent& event)
{
    wxString path = DirTextCtrl->GetValue();
    wxString name = ProjectNameTextCtrl->GetValue();

    moText tbasepath = (char*)path.c_str();
    moText tprojectname = (char*)name.c_str();

    moText datapath = (moText)tbasepath + moText("\\") + (moText)tprojectname + moText("\\");

    if ( !wxDirExists( DirTextCtrl->GetValue() + wxString(wxT("\\")) + ProjectNameTextCtrl->GetValue() ) ) {
        wxFileName::Mkdir( DirTextCtrl->GetValue() + wxString(wxT("\\")) + ProjectNameTextCtrl->GetValue() );
    }

    moText configname = (moText)tprojectname + moText(".mol");

    m_ProjectDescriptor = moProjectDescriptor( configname, datapath );


    EndModal( wxID_OK );
}

void moNewProject::OnCancelClick(wxCommandEvent& event)
{
    EndModal( wxID_CANCEL );
}

void moNewProject::OnBrowseClick(wxCommandEvent& event)
{
    wxDirDialog* pDirDialog = NULL;

    pDirDialog = new wxDirDialog(this,wxT("Choose a directory or create one"),wxT(""),wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);

    if (pDirDialog->ShowModal() == wxID_OK) {
        DirTextCtrl->SetValue( pDirDialog->GetPath() );
    }

}

void moNewProject::OnDirTextCtrlText(wxCommandEvent& event)
{
    if (DirTextCtrl->GetValue().IsEmpty()) {
        OkButton->Enable(false);
    } else if (!ProjectNameTextCtrl->GetValue().IsEmpty()) {
        OkButton->Enable(true);
    }
}

void moNewProject::OnProjectNameTextCtrlText(wxCommandEvent& event)
{
    if (ProjectNameTextCtrl->GetValue().IsEmpty()) {
        OkButton->Enable(false);
    } else if (!DirTextCtrl->GetValue().IsEmpty()) {
        OkButton->Enable(true);
    }
}