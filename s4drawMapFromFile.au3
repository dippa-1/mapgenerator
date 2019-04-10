; This tool tries to build a map from a file

; Infos zum Projekt
; Der Text in der Werkzeugleiste stimmt tatsächlich: bei größtem Maßstab sind es 6 Pixel pro Feld
; Probleme
;
; ToDo
; - setObject Function // besserer Name bitte // soll an eine bestimmte position ein bestimmtes objekt anbringen, malen
; - advanced: paintArea Function // sie soll z.b. ein rechteck mit gras bemalen koennen


; Return Codes:
; 0 - no ERROR
; 1 - selectTool ERROR
; 2 - changeToolSetting ERROR
; 3 - Init ERROR
; 4 - newMap ERROR

#Include <File.au3>
#Include <MsgBoxConstants.au3>

#RequireAdmin

#region GLOBAL CONSTANTS
Global Const $TITLE = "Settlers IV Level Editor - [No Designation]"
Global Const $TITLETOOLSELECTIONWINDOW = "Tool Selection"
Global Const $TITLETOOLSETTINGSWINDOW = "Tool Settings"
Global Const $SQRT2 = sqrt(2)
Global Const $MAPSIZE = 1024
Global $DICTIONARY = ObjCreate("Scripting.Dictionary") ;~  MsgBox(0x0, 'Item One', $DICTIONARY.Item('Ground'), 2)

#region Set Type of Terrain
; Ground
$DICTIONARY.Add("Ground", "#0|#0")
$DICTIONARY.Add("Grass", "#0|#0|#0")
$DICTIONARY.Add("Dark Grass", "#0|#0|#1")
$DICTIONARY.Add("Grassy Spot", "#0|#0|#2")
$DICTIONARY.Add("Dusty Road", "#0|#0|#3")
$DICTIONARY.Add("Stony Road", "#0|#0|#4")
$DICTIONARY.Add("Desert", "#0|#0|#5")
$DICTIONARY.Add("Rock", "#0|#0|#6")
$DICTIONARY.Add("Swamp", "#0|#0|#7")
$DICTIONARY.Add("Mud", "#0|#0|#8")
$DICTIONARY.Add("Snow", "#0|#0|#9")
$DICTIONARY.Add("Beach", "#0|#0|#10")

; Rivers
$DICTIONARY.Add("Rivers", "#0|#1")
$DICTIONARY.Add("River 1", "#0|#1|#0")
$DICTIONARY.Add("River 2", "#0|#1|#1")
$DICTIONARY.Add("River 3", "#0|#1|#2")
$DICTIONARY.Add("River 4", "#0|#1|#3")
$DICTIONARY.Add("River Mouth", "#0|#1|#4")


; Water
$DICTIONARY.Add("Water", "#0|#2")
$DICTIONARY.Add("Water 1", "#0|#2|#0")
$DICTIONARY.Add("Water 2", "#0|#2|#1")
$DICTIONARY.Add("Water 3", "#0|#2|#2")
$DICTIONARY.Add("Water 4", "#0|#2|#3")
$DICTIONARY.Add("Water 5", "#0|#2|#4")
$DICTIONARY.Add("Water 6", "#0|#2|#5")
$DICTIONARY.Add("Water 7", "#0|#2|#6")
$DICTIONARY.Add("Water 8", "#0|#2|#7")

; Special Brushes
$DICTIONARY.Add("Special Brushes", "#0|#3")
$DICTIONARY.Add("Dark Brush", "#0|#3|#0")
$DICTIONARY.Add("Pond Brush", "#0|#3|#1")
#EndRegion Set Type of Terrain

; Place Start Position
$DICTIONARY.Add("Place Start Position", "#1")

#region Chance Elevations (not for water)
; Change Elevations (not for water)
$DICTIONARY.Add("Raise Terrain Steeply", "#2|#0")
$DICTIONARY.Add("Lower Terrain Steeply", "#2|#1")
$DICTIONARY.Add("Raise Terrain Medium", "#2|#2")
$DICTIONARY.Add("Lower Terrain Medium", "#2|#3")
$DICTIONARY.Add("Raise Terrain Gently", "#2|#4")
$DICTIONARY.Add("Lower Terrain Gently", "#2|#5")
#EndRegion Change Eleveations (not for water)

#region Place Buildings
Global Const $DELETE_BUILDINGS = 0

; Foundation Building
$DICTIONARY.Add("Small Guard Tower", "#3|#1|#0")
$DICTIONARY.Add("Large Guard Tower", "#3|#1|#1")
$DICTIONARY.Add("Castle", "#3|#1|#2")
$DICTIONARY.Add("Add Occupants to Guard Tower or Castle", "#3|#1|#3")

; Heavy Industry
$DICTIONARY.Add("Stone Mine", "#3|#2|#0")
$DICTIONARY.Add("Iron Mine", "#3|#2|#1")
$DICTIONARY.Add("Gold Mine", "#3|#2|#2")
$DICTIONARY.Add("Coal Mine", "#3|#2|#3")
$DICTIONARY.Add("Sulfur Mine", "#3|#2|#4")
$DICTIONARY.Add("Gold Smelting Works", "#3|#2|#5")
$DICTIONARY.Add("Iron Smelting Works", "#3|#2|#6")
$DICTIONARY.Add("Toolsmiths Works", "#3|#2|#7")

; Food

; Alcohol

; Workshops (excluding heavy industry

; Dark Tribe

; Military and Residential Buildings

; Special Buildings

; Ornamental Objects (Roman)

; Ornamental Objects (Mayan)

; Ornamental Objects (Viking)

; Ornamental Objects (Trojans)
#EndRegion Place Buildings

#region Tool Settings

;~ Global Const $POINT = ["Point", "[CLASS:Button; INSTANCE:2]"]
;~ Global Const $NARROW = ["Narrow", "[CLASS:Button; INSTANCE:3]"]
;~ Global Const $MEDIUM = ["Medium", "[CLASS:Button; INSTANCE:4]"]
;~ Global Const $WIDE = ["Wide", "[CLASS:Button; INSTANCE:5]"]
;~ Global Const $CUSTOM = ["&Custom", "[CLASS:Button; INSTANCE:6]"]
;~ Global Const $TOOLSIZE = ["", "[CLASS:Edit; INSTANCE:1]"]
;~ Global Const $SPECKLE = ["Speckle", "[CLASS:Button; INSTANCE:7]"]
;~ Global Const $SPRAY = ["Activate Spray", "[CLASS:Button; INSTANCE:9]"]
;~ Global Const $SPRAY_DENSITY = ["", "[CLASS:Edit; INSTANCE:2]"]
;~ Global Const $SELECT_PLAYER = ["", "[CLASS:Edit; INSTANCE:3]"]
$DICTIONARY.Add("Point", "[CLASS:Button; INSTANCE:2]")
$DICTIONARY.Add("Narrow", "[CLASS:Button; INSTANCE:3]")
$DICTIONARY.Add("Medium", "[CLASS:Button; INSTANCE:4]")
$DICTIONARY.Add("Wide", "[CLASS:Button; INSTANCE:5]")
$DICTIONARY.Add("&Custom", "[CLASS:Button; INSTANCE:6]")
$DICTIONARY.Add("_Toolsize", "[CLASS:Edit; INSTANCE:1]") ; _x means that the control handle has no text
$DICTIONARY.Add("Speckle", "[CLASS:Button; INSTANCE:7]")
$DICTIONARY.Add("Activate Spray", "[CLASS:Button; INSTANCE:9]")
$DICTIONARY.Add("_Spray Density", "[CLASS:Edit; INSTANCE:2]")
$DICTIONARY.Add("_Select Player", "[CLASS:Edit; INSTANCE:3]")

#EndRegion Tool Settings

#EndRegion GLOBAL CONSTANTS

HotKeySet("n", newMap)
HotKeySet("t", test)
HotKeySet("{ESC}", _Exit)

Init()

While 1
	Sleep(1000)
WEnd

Func Init()

	Opt("WinTitleMatchMode", 3)

	; Get Window Information
	Global $windowHandle = WinGetHandle($TITLE)
	If Not WinSetState($windowHandle, "", @SW_MAXIMIZE) Then Exit 3
	Local $windowSize = WinGetClientSize($windowHandle)
	Global $editorHandle = WinGetHandle($TITLE, "Settlers IV Level EditorChildFrame")
	Local $editorSize = WinGetClientSize($editorHandle)
	Global $editorBounds = [[10, 75], [$windowSize[0] - 20, $editorSize[1] - 40]] ; [[x, y],[w,h]]

	Global $maxFieldsOnScreen[2] = [Int($editorBounds[1][0]/6), Int($editorBounds[1][1]/6*$SQRT2)]

;~ 	newMap($MAPSIZE)

	; Zoom factor 1
	If Not ControlClick($windowHandle, "", 1, "left", 1, 142, 14) Then Exit 3

	WinActivate($windowHandle)
	MouseMove($windowSize[0]/2, $windowSize[1]/2, 0)
	WinWaitActive($windowHandle)

EndFunc



Func selectTool($tool)

	ControlTreeView($TITLETOOLSELECTIONWINDOW, $TITLETOOLSELECTIONWINDOW & ":", "[CLASS:SysTreeView32; INSTANCE:1]", "Select", $tool)

EndFunc

Func changeToolSetting($toolSetting, $value = 0)

	ConsoleWrite("Toolsetting: " & $toolSetting & ", value " & $value & @CRLF)

	Switch $toolSetting
		Case "Speckle", "Activate Spray"
			ControlCommand($TITLETOOLSETTINGSWINDOW, $toolSetting, $DICTIONARY.Item($toolSetting), $value)
		Case "Custom" ; like _Toolsize
			ControlClick($TITLETOOLSETTINGSWINDOW, "&Custom", $DICTIONARY.Item("&Custom"))
			ControlClick($TITLETOOLSETTINGSWINDOW, "", $DICTIONARY.Item("_Toolsize"), "left", 2)
			Send($value)
			WinActivate($TITLE)
			WinWaitActive($TITLE)
		Case Else
			If $value == 0 or $value == "" Then
				ControlClick($TITLETOOLSETTINGSWINDOW, $toolSetting, $DICTIONARY.Item($toolSetting))
			Else
				ConsoleWrite("here" & @CRLF)
				ControlSend($TITLETOOLSETTINGSWINDOW, "", $DICTIONARY.Item($toolSetting), $value)
			EndIf
	EndSwitch

EndFunc

Func newMap($mapsize)

	Opt("WinTitleMatchMode", 3)
	ControlClick($TITLE, "", "[CLASS:ToolbarWindow32; INSTANCE:1]", "left", 1, 12, 12)
	Sleep(10)
	If StringInStr(WinGetText("Settlers IV Level Editor"), "Do you want to save the changes?") Then
		If Not ControlClick("Settlers IV Level Editor", "&No", "[CLASS:Button; INSTANCE:2]") Then Exit 4
		Sleep(10)
	EndIf

	; Mapgroesse ändern
	ControlCommand("Create New Map", "", "ListBox1", "SetCurrentSelection", ($mapsize-256)/64)
	If Not ControlClick("Create New Map", "&OK", "[CLASS:Button; INSTANCE:1]") Then Exit 4

EndFunc



Func test() ; Testing Playground, Hotkey "t"

	executeFileInstructions("instructions.txt")

EndFunc



Func getCursorCoords()

	Local $xy = RunWait("CursorCoordinates.exe")
	Local $pos[2]
	$pos[1] = Mod($xy,10000)
	$pos[0] = ($xy - $pos[1]) / 10000

	Return $pos

EndFunc

Func moveCursor($pos, $mouseDown = 0) ; mouseDown: 0 False, 1 left, 2 right

	Local $currPos = getCursorCoords()
	If $currPos[0] < 0 Then
		Return False
	EndIf
	Local $diff[2] = [$pos[0] - $currPos[0], $pos[1] - $currPos[1]]
	Local $mousePos = MouseGetPos()
	Local $newMousePos = [Round($mousePos[0] + ($diff[0] - $diff[1]/2)*6), Round($mousePos[1] + $diff[1]*6/2)]
;~ 	ConsoleWrite('@@ Debug(' & @ScriptLineNumber & ') : $newMousePos[] = ' & $newMousePos[0] & @CRLF & '>Error code: ' & @error & @CRLF) ;### Debug Console
;~ 	ConsoleWrite('@@ Debug(' & @ScriptLineNumber & ') : $newMousePos[] = ' & $newMousePos[1] & @CRLF & '>Error code: ' & @error & @CRLF) ;### Debug Console
	If Not posInBounds($newMousePos, $editorBounds) Then
		Return $newMousePos
	EndIf
	Local $distance = sqrt(($newMousePos[0]-$mousePos[0])*($newMousePos[0]-$mousePos[0]) + ($newMousePos[1]-$mousePos[1])*($newMousePos[1]-$mousePos[1]))
	If $mouseDown == 1 Then
		MouseClickDrag("left", $mousePos[0], $mousePos[1], $newMousePos[0], $newMousePos[1], $distance/250 + 5)
		MouseMove($newMousePos[0]+1, $newMousePos[1], 0)
		MouseMove($newMousePos[0], $newMousePos[1], 0)
		Sleep(25)
	Else
		MouseMove($newMousePos[0], $newMousePos[1], Round($distance/250 + 5))
	EndIf

	Return True

EndFunc
; This function will always get to the destination
Func moveCursorSecure($pos, $center = 0)

	Local $result = moveCursor($pos)
	Local $bounds_[2][2]
	$bounds_[0][0] = $editorBounds[0][0]+$editorBounds[1][0]/2 - 10
	$bounds_[0][1] = $editorBounds[0][1]+$editorBounds[1][1]/2 - 10
	$bounds_[1][0] = 20
	$bounds_[1][1] = 20
	Local $dir[2]
	While Not $result == True
		If Not posInBounds(MouseGetPos(), $bounds_) Then
			; Center map
			Local $dir_0 = [Round(-($editorBounds[0][0]+$editorBounds[1][0]/2 - MouseGetPos(0))), Round(-($editorBounds[0][1]+$editorBounds[1][1]/2 - MouseGetPos(1)))]
			scrollMap($dir_0)
			MouseMove($editorBounds[0][0]+$editorBounds[1][0]/2, $editorBounds[0][1]+$editorBounds[1][1]/2, 0)
			$result = moveCursor($pos)
		EndIf
		If $result == False Then
			; position is -1,-1
			; get Pixel Colors of corners
			For $y = 0 To 2
				For $x = 0 To 2
					If $x == 1 and $y == 1 Then ContinueLoop
					If Not (Hex(PixelGetColor($editorBounds[0][0] + $x * $editorBounds[1][0]/2, $editorBounds[0][1] + $y * $editorBounds[1][1]/2)) == Hex(0)) Then
						Local $d = [$editorBounds[0][0] + $x * $editorBounds[1][0]/2 - MouseGetPos(0), $editorBounds[0][1] + $y * $editorBounds[1][1]/2 - MouseGetPos(1)]
;~ 						MouseMove($editorBounds[0][0] + $x * $editorBounds[1][0]/2, $editorBounds[0][1] + $y * $editorBounds[1][1]/2, 0)
						ConsoleWrite($d[0] & @CRLF & $d[1] & @CRLF)
						scrollMap($d)
						$x = 2
						$y = 2
						$result = moveCursor($pos)
					EndIf
				Next
			Next
			ContinueLoop
		EndIf
		Local $dir[2]
		$dir[0] = $result[0] - MouseGetPos(0)
		$dir[1] = $result[1] - MouseGetPos(1)
		Local $dist = sqrt($dir[0]*$dir[0] + $dir[1]*$dir[1])
		If $dir[0] > $editorBounds[1][0]-MouseGetPos(0) Then
			$dir[0] = $dir[0] * $editorBounds[1][0] / ($dist*2)
			$dir[1] = $dir[1] * $editorBounds[1][0] / ($dist*2)
		EndIf
		If $dir[1] > $editorBounds[1][1]-MouseGetPos(1) Then
			$dir[0] = $dir[0] * $editorBounds[1][1] / ($dist*2)
			$dir[1] = $dir[1] * $editorBounds[1][1] / ($dist*2)
		EndIf
		$dir[0] = Round($dir[0])
		$dir[1] = Round($dir[1])
		ConsoleWrite('@@ Debug(' & @ScriptLineNumber & ') : $dir[0] = ' & $dir[0] & @CRLF & '>Error code: ' & @error & @CRLF) ;### Debug Console
		ConsoleWrite('@@ Debug(' & @ScriptLineNumber & ') : $dir[1] = ' & $dir[1] & @CRLF & '>Error code: ' & @error & @CRLF) ;### Debug Console
		scrollMap($dir)
;~ 		Sleep(1000)
		$result = moveCursor($pos)
	WEnd

EndFunc

Func drawLine($start, $end, $quality = 5) ; 5 is good quality, 30 is OK, 50 is kinda bad

	; Check if start and endpoint can be drawn with one line
	Local $dir[2] = [$end[0] - $start[0], $end[1] - $start[1]]
	If $dir[0] > $maxFieldsOnScreen[0] or $dir[1] > $maxFieldsOnScreen[1] Then
		ConsoleWrite("Error: X Direction " & $dir[0] & ", Y Direction " & $dir[1] & @CRLF)
		Return False
	EndIf

	Local $middle[2]
	$middle[0] = $start[0] + Round($dir[0]/2)
	$middle[1] = $start[1] + Round($dir[1]/2)
	moveCursorSecure($middle)

	; Center map
	Local $dir_0 = [Round(-($editorBounds[0][0]+$editorBounds[1][0]/2 - MouseGetPos(0))), Round(-($editorBounds[0][1]+$editorBounds[1][1]/2 - MouseGetPos(1)))]
	scrollMap($dir_0)
	MouseMove($editorBounds[0][0]+$editorBounds[1][0]/2, $editorBounds[0][1]+$editorBounds[1][1]/2, 0)

	moveCursor($start)

	Local $dist = vectorLength($dir)
	ConsoleWrite($dist & @CRLF)
	For $i = 1 To Round($dist) Step $quality
		Local $v[2] = [$start[0] + ($i*$dir[0]/($dist)), $start[1] + ($i*$dir[1]/($dist))]
		moveCursor($v, 1)
	Next

	moveCursor($end, 1)

	Return True

EndFunc

Func scrollMap($direction)

	MouseDown("right")
	Sleep(50)
	MouseMove(MouseGetPos(0)+$direction[0], MouseGetPos(1), 0)
	MouseMove(MouseGetPos(0), MouseGetPos(1)+$direction[1], 0)
	Sleep(50)
	MouseUp("right")
	Sleep(100)

EndFunc



Func posInBounds($pos, $bounds)

;~ 	ConsoleWrite('@@ Debug(' & @ScriptLineNumber & ') : $bounds[0][0] = ' & $bounds[0][0] & @CRLF & '>Error code: ' & @error & @CRLF) ;### Debug Console
;~ 	ConsoleWrite('@@ Debug(' & @ScriptLineNumber & ') : $bounds[0][1] = ' & $bounds[0][1] & @CRLF & '>Error code: ' & @error & @CRLF) ;### Debug Console
;~ 	ConsoleWrite('@@ Debug(' & @ScriptLineNumber & ') : $bounds[0][0] + $bounds[1][0] = ' & $bounds[0][0] + $bounds[1][0] & @CRLF & '>Error code: ' & @error & @CRLF) ;### Debug Console
;~ 	ConsoleWrite('@@ Debug(' & @ScriptLineNumber & ') : $bounds[0][1] + $bounds[1][1] = ' & $bounds[0][1] + $bounds[1][1] & @CRLF & '>Error code: ' & @error & @CRLF) ;### Debug Console

	If $pos[0] < $bounds[0][0] Then Return False
	If $pos[1] < $bounds[0][1] Then Return False
	If $pos[0] > ($bounds[0][0] + $bounds[1][0]) Then Return False
	If $pos[1] > ($bounds[0][1] + $bounds[1][1]) Then Return False

	Return True

EndFunc

Func vectorLength($v)
	Return sqrt($v[0]*$v[0] + $v[1]*$v[1])
EndFunc

Func pointDistance($p, $q)
	Return sqrt( ($p[0]-$q[0])*($p[0]-$q[0]) + ($p[1]-$q[1])*($p[1]-$q[1]) )
EndFunc


Func executeFileInstructions($filename)

	Local $file = FileOpen($filename)

	Local $insLine = FileReadLine($file) ; Instruction
	Local $firstLine = StringSplit($insLine, ":")
	newMap($firstLine[2])

	While 1
		$insLine = FileReadLine($file)
		If @error = -1 Then ExitLoop

		Local $instructions = StringSplit($insLine, ";") ; Instructions
		selectTool($DICTIONARY.Item($instructions[1]))

		changeToolSetting($instructions[2], $instructions[3])

		For $i = 4 To UBound($instructions)-2
			Local $p1 = StringSplit($instructions[$i], "|")
			Local $p2 = StringSplit($instructions[$i+1], "|")
			Local $a[2] = [$p1[1], $p1[2]]
			Local $b[2] = [$p2[1], $p2[2]]
			ConsoleWrite("Drawing a line from " & $a[0] & "," & $a[1] & " to " & $b[0] & "," & $b[1] & @CRLF)
			drawLine($a, $b, Int($instructions[3])*2)
		Next

	WEnd

	FileClose($filename)

EndFunc



Func _Exit()
	Exit 0
EndFunc






















