//---------------------------------------------------------------------------

#ifndef attendanceH
#define attendanceH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>

#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>/
#include <Vcl.Grids.hpp>/---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TPageControl *PageControl1;
	TTabSheet *AttendanceTab;
	TPanel *Panel1;
	TListBox *ListBox1;
	TStringGrid *StringGrid1;
	TTabSheet *StudentTab;
private:	// User declarations
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
