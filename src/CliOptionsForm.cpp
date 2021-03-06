//-----------------------------------------------------------------------------
//
//	GraphStudioNext
//
//	Author : CPlusSharp
//
//-----------------------------------------------------------------------------
#include "stdafx.h"


//-----------------------------------------------------------------------------
//
//	CCliOptionsForm class
//
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(CCliOptionsForm, CDialog)
BEGIN_MESSAGE_MAP(CCliOptionsForm, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ASSOCIATE, &CCliOptionsForm::OnAssociateFileType)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
//
//	CCliOptionsForm class
//
//-----------------------------------------------------------------------------

CCliOptionsForm::CCliOptionsForm(CWnd* pParent)	: 
	CDialog(CCliOptionsForm::IDD, pParent)
{

}

CCliOptionsForm::~CCliOptionsForm()
{
}

void CCliOptionsForm::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TITLEBAR, title);
    DDX_Control(pDX, IDC_EDIT_OPTIONS, edit_options);
}

BOOL CCliOptionsForm::OnInitDialog()
{
	BOOL ret = __super::OnInitDialog();
	if (!ret) return FALSE;

    if(GraphStudio::HasFont(_T("Consolas")))
        GraphStudio::MakeFont(font_options, _T("Consolas"), 10, false, false);
    else
        GraphStudio::MakeFont(font_options, _T("Courier New"), 10, false, false);
	edit_options.SetFont(&font_options);

    CString strTxt;
    strTxt =  _T("Command Line Options for the Program:\r\n");
    strTxt += _T("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n\r\n");
    strTxt += _T("graphstudionext.exe [options] [file/moniker] [options]\r\n");
    strTxt += _T("-> file (optional)      -> a graph file or a video-file to render\r\n");
    strTxt += _T("-> options (optional)   -> one or more of the following switches:\r\n");
    strTxt += _T("    -a                      -> connect to a remote graph\r\n");
    strTxt += _T("    -run                    -> runs the graph if a file was rendered\r\n");
    strTxt += _T("    -exitafterrun           -> closes the application after the graph was stopped\r\n");
    strTxt += _T("    -noclock                -> disables the clock if a file was rendered\r\n");
    strTxt += _T("    -progressview           -> opens the application in the progress view\r\n");
    strTxt += _T("    -exitonerror            -> closes the application on an error (no error dialog)\r\n");
    strTxt += _T("                               and sets the error code as the exit code\r\n");
    strTxt += _T("    -filters                -> opens the filters dialog on startup\r\n");
    strTxt += _T("    -?                      -> opens this window on startup\r\n");
    strTxt += _T("\r\n");
    strTxt += _T("Examples:\r\n");
    strTxt += _T("graphstudionext.exe convert.grf -run -exitonerror -exitafterrun -noclock -progressview\r\n");
    strTxt += _T("graphstudionext.exe -a \"!FilterGraph x y\"\r\n\r\n");

	strTxt += _T("Mouse Buttons:\r\n");
    strTxt += _T("~~~~~~~~~~~~~~~\r\n\r\n");
    strTxt += _T("Click                       -> Select filter or connection\r\n");
    strTxt += _T("Ctrl + Click                -> Toggle selection of clicked filter\r\n");
    strTxt += _T("Double click                -> filter or pin properties\r\n");
    strTxt += _T("Right click                 -> filter, pin or graph context menu\r\n");
    strTxt += _T("Middle button               -> Delete filter or connection\r\n");
    strTxt += _T("Shift + click + drag        -> Connect from first unconnected output pin\r\n");
    strTxt += _T("Shift + right click         -> Context menu for first unconnected output pin\r\n");
    strTxt += _T("Shift + middle click        -> Disconnect first connected output pin on filter\r\n\r\n");

    strTxt += _T("Drag and Drop of File(s):\r\n");
    strTxt += _T("~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n\r\n");
    strTxt += _T("Drop files                  -> Open .GRF/.GRFX/.XML graph file or render other media file(s)\r\n");
    strTxt += _T("Shift + drop files          -> Open file(s) with Async File Reader filter\r\n");
    strTxt += _T("Alt + drop files            -> Create source filter for file(s) with IGraphBuilder::AddSourceFilter\r\n");
    strTxt += _T("Control + any above         -> Do not clear existing graph before adding file(s)\r\n");
    strTxt += _T("Drop files on filter        -> Set filename for IFileSourceFilter or IFileSink if supported\r\n\r\n");

    strTxt += _T("Mouse Wheel:\r\n");
    strTxt += _T("~~~~~~~~~~~~~\r\n\r\n");
    strTxt += _T("Wheel                       -> Scroll graph\r\n");
    strTxt += _T("Horizontal wheel            -> Scroll graph horizontally\r\n");
    strTxt += _T("Alt + wheel                 -> Scroll graph horizontally\r\n");
    strTxt += _T("Ctrl + wheel                -> Change zoom level\r\n");
    strTxt += _T("Ctrl + Shift + wheel        -> Change vertical filter spacing\r\n");
    strTxt += _T("Shift + horizontal wheel    -> Change horizontal filter spacing\r\n");
    strTxt += _T("Ctrl + horizontal wheel     -> Change filter wrap width\r\n");
    strTxt += _T("Alt + Shift + wheel         -> Change horizontal filter spacing\r\n");
    strTxt += _T("Alt + Ctrl + wheel		  -> Change filter wrap width\r\n");
    strTxt += _T("Shift + wheel               -> Change playback speed\r\n\r\n");

    edit_options.SetWindowText(strTxt);

    UpdateData(FALSE);

	return TRUE;
}

void CCliOptionsForm::OnOK()
{
    UpdateData(TRUE);
	EndDialog(IDOK);
}

void CCliOptionsForm::OnAssociateFileType()
{
	AssociateFileType();
}

void CCliOptionsForm::AssociateFileType()
{
	TCHAR strExeLocation[MAX_PATH];
	if (GetModuleFileName(NULL, strExeLocation, MAX_PATH))
	{
		CPath pathExe(strExeLocation);
		pathExe.Canonicalize();

		HKEY hKeyBase = HKEY_CURRENT_USER;
		if (DSUtil::IsUserAdmin())
			hKeyBase = HKEY_LOCAL_MACHINE;

		// extension
		ATL::CRegKey regKey;
		CString strReg = _T("Software\\Classes\\.grfx");
		if (ERROR_SUCCESS != regKey.Create(hKeyBase, strReg))
		{
			DSUtil::ShowError(_T("Can't register file extension"));
			return;
		}
		regKey.SetStringValue(NULL, _T("GraphStudioNext.GraphFile.v1"));
		regKey.Close();

		// FileType description
		strReg = _T("Software\\Classes\\GraphStudioNext.GraphFile.v1");
		if (ERROR_SUCCESS != regKey.Create(hKeyBase, strReg))
		{
			DSUtil::ShowError(_T("Can't register filetype"));
			return;
		}
		regKey.SetStringValue(NULL, _T("GraphStudioNext Filter Graph File"));
		regKey.Close();

		// Open command
		strReg = _T("Software\\Classes\\GraphStudioNext.GraphFile.v1\\shell\\open\\command");
		if (ERROR_SUCCESS != regKey.Create(hKeyBase, strReg))
		{
			DSUtil::ShowError(_T("Can't register filetype open command"));
			return;
		}
		CString strOpen = pathExe;
		strOpen.Append(_T(" \"%1\""));
		regKey.SetStringValue(NULL, strOpen);
		regKey.Close();


		// Register Icon for the filetype
		strReg = _T("Software\\Classes\\GraphStudioNext.GraphFile.v1\\DefaultIcon");
		if (ERROR_SUCCESS != regKey.Create(hKeyBase, strReg))
		{
			DSUtil::ShowError(_T("Can't set the icon for the filetype"));
			return;
		}
		CString strIcon = pathExe;
		strIcon.Append(_T(",-129"));
		regKey.SetStringValue(NULL, strIcon);

		// Reload Shell with the new Icon
		SHChangeNotify(SHCNE_ASSOCCHANGED,NULL,NULL,NULL);

		DSUtil::ShowInfo(_T("FileType .grfx successfully registered."));
	}
}
