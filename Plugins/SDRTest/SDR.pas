unit SDR;

interface

uses windows, SysUtils, LOGForm;

// --service code start---------------------------------------------------------

type
  TStateChgReason = (crPtt = 0, crDash = 1, crDot = 2, crAdc = 3);

  TCallbackFunc_SdrStateChanged = procedure(handle: UINT_PTR;
    reason: TStateChgReason; arg1: bool; arg2, arg3: integer); cdecl;

var
  handle: UINT_PTR = 0;
  SdrStateChanged: TCallbackFunc_SdrStateChanged = nil;

procedure PttChanged(KeyPtt: bool);
procedure DashChanged(KeyDash: bool);
procedure DotChanged(KeyDot: bool);
procedure AdcChanged(Ufwd, Uref: integer);

procedure init(_handle: UINT_PTR; func: TCallbackFunc_SdrStateChanged); cdecl;
procedure getInfo(name: PAnsiChar); cdecl;
procedure deinit(); cdecl;
procedure open(SdrNum, DdsMul: integer); cdecl;
procedure close(); cdecl;
function isOpen(): bool; cdecl;
procedure setPreamp(Preamp: integer); cdecl;
procedure setExtCtrl(ExtData: DWORD); cdecl;
procedure setDdsFreq(Freq: integer); cdecl;
procedure setTrxMode(Mode: bool); cdecl;
procedure setMute(Status: bool); cdecl;
procedure setVhfOsc(Freq: Cardinal); cdecl;
procedure setUhfOsc(Freq: Cardinal); cdecl;
procedure setCalGen(Mode: bool); cdecl;
procedure setXvAnt(Mode: integer); cdecl;
procedure showPluginGui(); cdecl;

implementation

procedure PttChanged(KeyPtt: bool);
begin
  if (@SdrStateChanged <> nil) and (handle <> 0) then
    SdrStateChanged(handle, crPtt, KeyPtt, 0, 0);
end;

procedure DashChanged(KeyDash: bool);
begin
  if (@SdrStateChanged <> nil) and (handle <> 0) then
    SdrStateChanged(handle, crDash, KeyDash, 0, 0);
end;

procedure DotChanged(KeyDot: bool);
begin
  if (@SdrStateChanged <> nil) and (handle <> 0) then
    SdrStateChanged(handle, crDot, KeyDot, 0, 0);
end;

procedure AdcChanged(Ufwd, Uref: integer);
begin
  if (@SdrStateChanged <> nil) and (handle <> 0) then
    SdrStateChanged(handle, crAdc, false, Ufwd, Uref);
end;

procedure init(_handle: UINT_PTR; func: TCallbackFunc_SdrStateChanged); cdecl;
begin
  handle := _handle;
  SdrStateChanged := func;
  // --service code end---------------------------------------------------------
  // --put user init code here--------------------------------------------------
  LOGFrm := TLOGFrm.Create(nil);
  LOGFrm.Show;
  console_log('init');
end;

procedure getInfo(name: PAnsiChar); cdecl;
begin
  if name <> nil then
    StrCopy(name, 'SDRTest');
end;

procedure deinit(); cdecl;
begin
  LOGFrm.Free;
end;

procedure open(SdrNum, DdsMul: integer); cdecl;
begin
  console_log('open(' + inttostr(SdrNum) + ',' + inttostr(DdsMul) + ')');
end;

procedure close(); cdecl;
begin
  console_log('close');
end;

function isOpen(): bool; cdecl;
begin
  Result := true;
  console_log('isOpen');
end;

procedure setPreamp(Preamp: integer); cdecl;
begin
  console_log('setPreamp(' + inttostr(Preamp) + ')');
end;

procedure setExtCtrl(ExtData: DWORD); cdecl;
begin
  console_log('setExtCtrl(0x' + IntToHex(ExtData, 8) + ')');
end;

procedure setDdsFreq(Freq: integer); cdecl;
begin
  console_log('setDdsFreq(' + inttostr(Freq) + ')');
end;

procedure setTrxMode(Mode: bool); cdecl;
var
  str: string;
begin
  if Mode then
    str := 'true'
  else
    str := 'false';

  console_log('setTrxMode(' + str + ')');
end;

procedure setMute(Status: bool); cdecl;
var
  str: string;
begin
  if Status then
    str := 'true'
  else
    str := 'false';
  console_log('setMute(' + str + ')');
end;

procedure setVhfOsc(Freq: Cardinal); cdecl;
begin
  console_log('setVhfOsc(' + inttostr(Freq) + ')');
end;

procedure setUhfOsc(Freq: Cardinal); cdecl;
begin
  console_log('setUhfOsc(' + inttostr(Freq) + ')');
end;

procedure setCalGen(Mode: bool); cdecl;
var
  str: string;
begin
  if Mode then
    str := 'true'
  else
    str := 'false';
  console_log('setCalGen(' + str + ')');
end;

procedure setXvAnt(Mode: integer); cdecl;
begin
  console_log('setXvAnt(' + inttostr(Mode) + ')');
end;

procedure showPluginGui(); cdecl;
begin
  if (LOGFrm <> nil) then
    LOGFrm.Show;
  console_log('showPluginGui()');
end;

end.
