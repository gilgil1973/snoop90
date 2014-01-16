TEMPLATE = subdirs
CONFIG  += ordered
SUBDIRS += VDream Snoop snoop_test

VDream.subdir       = ../../../../VDream/VDream80/Lib/VDream.pro
Snoop.subdir        = ../../Lib/Snoop.pro
snoop_test.subdir   = snoop_test.pro

#Snoop.depends       = VDream
#snoop_tests.depends = VDream Snoop
