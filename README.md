# APOLLON

Command to execute the gem5:
`$GEM5/build/ARM/gem5.opt $GEM5/configs/example/fs.py --kernel=vmlinux.aarch32.Apollon --disk-image=linux-aarch32-ael.img --mem-size=512MB --machine-type=VExpress_EMM --dtb=vexpress.aarch32.ll_20131205.0-gem5.1cpu.dtb --script=$GEM5/configs/boot/COSSIM/script0.rcS --PtolemySynchTime=1ms`