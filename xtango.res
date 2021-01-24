# 1 "<stdin>"
# 1 "<built-in>"
# 1 "<command-line>"
# 31 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 32 "<command-line>" 2
# 1 "<stdin>"
# 58 "<stdin>"
XTango*geometry: =512x512

XTango*background: White
XTango*foreground: Black

XTango*borderWidth: 1

XTango*Command.font: -*-helvetica-bold-o-normal--12-*-*-*-*-*-*-*
XTango*MenuButton.font: -*-helvetica-bold-o-normal--12-*-*-*-*-*-*-*

xtango*form.defaultDistance: 0

xtango*title.label: XTANGO
xtango*title.justify: JustifyCenter
xtango*title.font: -*-helvetica-bold-r-normal--14-*-*-*-*-*-*-*
xtango*title.background: Yellow
xtango*title.resizable: TRUE
xtango*title.right: ChainRight
xtango*title.bottom: ChainTop

xtango*leftbox.orientation: vertical
xtango*leftbox.fromVert: title
xtango*leftbox.top: ChainTop
xtango*leftbox.right: ChainLeft
xtango*leftbox.bottom: ChainBottom
xtango*leftbox*width: 42
xtango*leftbox*height: 35

xtango*left.bitmap: /home/my/src/xtango/bitmaps/left.bm
xtango*right.bitmap: /home/my/src/xtango/bitmaps/right.bm
xtango*up.bitmap: /home/my/src/xtango/bitmaps/up.bm
xtango*down.bitmap: /home/my/src/xtango/bitmaps/down.bm
xtango*in.bitmap: /home/my/src/xtango/bitmaps/in.bm
xtango*out.bitmap: /home/my/src/xtango/bitmaps/out.bm

xtango*sbar.fromVert: title
xtango*sbar.horizDistance: 49
xtango*sbar.length: 250
xtango*sbar.thickness: 20
xtango*sbar.top: ChainTop
xtango*sbar.left: ChainRight
xtango*sbar.right: ChainRight
xtango*sbar.shown: 0.1

xtango*quit.fromHoriz: sbar
xtango*quit.top: ChainBottom
xtango*quit.bottom: ChainBottom
xtango*quit.horizDistance: -12
xtango*quit.vertDistance: 235
xtango*quit.width: 5
xtango*quit.height: 30

xtango*refresh.fromHoriz: quit
xtango*refresh.top: ChainBottom
xtango*refresh.bottom: ChainBottom
xtango*refresh.horizDistance: -15
xtango*refresh.vertDistance: 235
xtango*refresh.width: 6
xtango*refresh.height: 30

xtango*debug.fromHoriz: refresh
xtango*debug.top: ChainBottom
xtango*debug.bottom: ChainBottom
xtango*debug.horizDistance: -16
xtango*debug.vertDistance: 235
xtango*debug.width: 6
xtango*debug.height: 30
xtango*debug.menuName: debugMenu

xtango*mode.fromHoriz: debug
xtango*mode.top: ChainBottom
xtango*mode.bottom: ChainBottom
xtango*mode.horizDistance: -16
xtango*mode.vertDistance: 235
xtango*mode.width: 6
xtango*mode.height: 30
xtango*mode.menuName: modeMenu

xtango*pause.label: run animation
xtango*pause.fromHoriz: mode
xtango*pause.top: ChainBottom
xtango*pause.bottom: ChainBottom
xtango*pause.horizDistance: -34
xtango*pause.vertDistance: 235
xtango*pause.width: 11
xtango*pause.height: 30

xtango*patterns.fromHoriz: mode
xtango*patterns.top: ChainBottom
xtango*patterns.bottom: ChainBottom
xtango*patterns.horizDistance: -16
xtango*patterns.vertDistance: 235
xtango*patterns.width: 6
xtango*patterns.height: 30
xtango*patterns.menuName: patternsMenu

xtango*easel.fromVert: title
xtango*easel.fromHoriz: leftbox
xtango*easel.top: ChainTop
xtango*easel.left: ChainLeft
xtango*easel.right: ChainRight
xtango*easel.bottom: ChainBottom
xtango*easel.width: 7
xtango*easel.height: 200

xtango*modeMenu.geometry: 64x48
xtango*debugMenu.geometry: 64x30
xtango*patternsMenu.geometry: 83x30
