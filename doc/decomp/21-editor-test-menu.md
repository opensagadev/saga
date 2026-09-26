# Editor test menu source placement

Retail `CreateTestMenu` and `TestMenu` sit immediately after
`eduiSetFontScale`, in the editor UI translation unit. The current source had
their stubs in `gamemenuall.cpp`, while `edtoolsall_plain.cpp` contains
`eduiSetFontScale` and directly includes `edmenucallbacks.cpp`. Move the two
test menu functions into `edtoolsall_plain.cpp`; the static callbacks `cbSel`,
`cbSubMenu`, and `cbGradChange` are then available in the same translation
unit without exporting or wrapping them. Their callback declarations have
fewer formal parameters than `EdUiItemCallback`; casting the function pointer
at each creation call preserves the original callback symbols and code.

Retail stores `submenu1`, `submenu0`, `testmenu0`, and `selection_value`
consecutively in BSS. Keeping the first three pointers just before
`selection_value` in `edmenucallbacks.cpp` reproduces the call and branch
structure. The GOT-aware objdiff fork reports 1,119/1,119 bytes at
99.647890% for `CreateTestMenu` and 66/66 bytes at 99.875000% for `TestMenu`.
The displayed differences are data addresses for those pointers and string
literals; all decoded instructions and calls align.
