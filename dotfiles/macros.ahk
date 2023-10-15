
CapsLock::Control

^Capslock::
{
	Sleep(50)
	WinMinimize("A")    ; need A to specify Active window
	return
}

^space::
{
	WinSetAlwaysOnTop -1, "A"
}

!v::
{
	WinSetStyle "^0xC40000", "A"
	;WinHide "A"
	;WinShow "A"
}

/*
!f::
{
	if WinExist("ahk_class Shell_TrayWnd") {
		WinHide "ahk_class Shell_TrayWnd"
		;WinHide "ahk_class Shell_SecondaryTrayWnd"
	} Else {
		WinShow "ahk_class Shell_TrayWnd"
		;WinShow "ahk_class Shell_SecondaryTrayWnd"
	}
}
*/

!y::
{
	SetTitleMatchMode 2
	DetectHiddenWindows(true)
	Sleep 100
	Loop
	{
		oid := WinGetList("Remote Desktop Connection",,,)
		aid := Array()
		id := oid.Length
		For v in oid
		{
			aid.Push(v)
		}
		Loop aid.Length
		{
			this_id := aid[A_Index]
			this_title := WinGetTitle("ahk_id " this_id)
			; Try to activate and restore the window if it's minimized
			try {
				FocusedHwnd := ControlGetHwnd("IHWindowClass1", "ahk_id " this_id)
			} catch Error as err {
				;ToolTip(this_title)
				continue
			}
			state := WinGetMinMax("ahk_id" this_id)
			if (state = -1)
				WinActivate("ahk_id " this_id)
			FocusedClassNN := ControlGetClassNN(FocusedHwnd)
			;ToolTip(FocusedClassNN)
			ControlShow(FocusedClassNN, "ahk_id " this_id)
			ControlFocus(FocusedClassNN, "ahk_id " this_id)
			ControlSend("{Shift}", FocusedClassNN, "ahk_id " this_id)
			if (state = -1)
				WinMinimize("ahk_id " this_id)
		}
		;Sleep(10000)
		Sleep(280000)
	}
	return
}

/*
!t::
{
	Loop {
		Send "^{s}"
		Sleep 1000
		Send "{Enter}"
		Sleep 1000
		Send "{n}"
		Sleep 2500
	}
}
*/

^!t::
{
	Run "C:\Users\User\Desktop\eng36.rdp"
	Sleep(2500)
	ControlSendText(EnvGet("36pass"), , "ahk_exe CredentialUIBroker.exe")
	Sleep(1500)
	ControlSend("{Enter}", , "ahk_exe CredentialUIBroker.exe")
}

#SuspendExempt
!o::Suspend  ; Ctrl+Alt+S
#SuspendExempt False
