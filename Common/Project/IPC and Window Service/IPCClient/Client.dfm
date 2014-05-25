object Form2: TForm2
  Left = 0
  Top = 0
  Caption = 'Client'
  ClientHeight = 202
  ClientWidth = 447
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object lbl1: TLabel
    Left = 24
    Top = 128
    Width = 16
    Height = 13
    Caption = 'lbl1'
  end
  object lbl2: TLabel
    Left = 312
    Top = 144
    Width = 16
    Height = 13
    Caption = 'lbl2'
  end
  object btn1: TButton
    Left = 312
    Top = 40
    Width = 75
    Height = 25
    Caption = 'btn1'
    TabOrder = 0
    OnClick = btn1Click
  end
  object edt1: TEdit
    Left = 40
    Top = 24
    Width = 169
    Height = 21
    TabOrder = 1
    Text = 'edt1'
  end
end
