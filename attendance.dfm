object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Form1'
  ClientHeight = 726
  ClientWidth = 1078
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 1078
    Height = 726
    ActivePage = StudentTab
    Align = alClient
    TabOrder = 0
    object AttendanceTab: TTabSheet
      Caption = #35760#24405#20986#21220
      ImageIndex = 1
      ExplicitLeft = 8
      ExplicitTop = 28
      object Panel1: TPanel
        Left = 0
        Top = 0
        Width = 1070
        Height = 457
        Align = alTop
        TabOrder = 0
        object ListBox1: TListBox
          Left = 1
          Top = 1
          Width = 121
          Height = 455
          Align = alLeft
          ItemHeight = 13
          TabOrder = 0
          ExplicitLeft = 792
          ExplicitTop = 320
          ExplicitHeight = 97
        end
        object StringGrid1: TStringGrid
          Left = 122
          Top = 1
          Width = 947
          Height = 455
          Align = alClient
          TabOrder = 1
          ExplicitLeft = 0
          ExplicitTop = 0
          ExplicitWidth = 1078
          ExplicitHeight = 726
        end
      end
    end
    object StudentTab: TTabSheet
      Caption = #23398#29983#20986#21220
      ImageIndex = 1
    end
  end
end
