unit LOGForm;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants,
  System.Classes, Vcl.Graphics,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, Vcl.StdCtrls, Vcl.ExtCtrls;

type
  TLOGFrm = class(TForm)
    Panel1: TPanel;
    Memo1: TMemo;
    Ptt: TCheckBox;
    Dash: TCheckBox;
    Dot: TCheckBox;
    Button1: TButton;
    procedure PttClick(Sender: TObject);
    procedure DashClick(Sender: TObject);
    procedure DotClick(Sender: TObject);
    procedure Button1Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

procedure console_log(text: string);

var
  LOGFrm: TLOGFrm;

implementation

uses SDR;

{$R *.dfm}

procedure console_log(text: string);
begin
  LOGFrm.Memo1.Lines.Add(text);
end;

procedure TLOGFrm.Button1Click(Sender: TObject);
begin
  Memo1.Clear;
end;

procedure TLOGFrm.DashClick(Sender: TObject);
begin
  DashChanged(Dash.Checked);
end;

procedure TLOGFrm.DotClick(Sender: TObject);
begin
  DotChanged(Dot.Checked);
end;

procedure TLOGFrm.PttClick(Sender: TObject);
begin
  PttChanged(Ptt.Checked);
end;

end.
