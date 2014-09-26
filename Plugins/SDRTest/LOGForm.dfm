object LOGFrm: TLOGFrm
  Left = 0
  Top = 0
  Caption = 'Log'
  ClientHeight = 308
  ClientWidth = 222
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 280
    Width = 222
    Height = 28
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 0
    ExplicitWidth = 289
    object Ptt: TCheckBox
      Left = 8
      Top = 6
      Width = 41
      Height = 17
      Caption = 'Ptt'
      TabOrder = 0
      OnClick = PttClick
    end
    object Dash: TCheckBox
      Left = 55
      Top = 6
      Width = 57
      Height = 17
      Caption = 'Dash'
      TabOrder = 1
      OnClick = DashClick
    end
    object Dot: TCheckBox
      Left = 103
      Top = 6
      Width = 50
      Height = 17
      Caption = 'Dot'
      TabOrder = 2
      OnClick = DotClick
    end
    object Button1: TButton
      Left = 159
      Top = 5
      Width = 57
      Height = 19
      Caption = 'Clear'
      TabOrder = 3
      OnClick = Button1Click
    end
  end
  object Memo1: TMemo
    Left = 0
    Top = 0
    Width = 222
    Height = 280
    Align = alClient
    ScrollBars = ssVertical
    TabOrder = 1
    ExplicitWidth = 179
  end
end
