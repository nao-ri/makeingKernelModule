cmd_/home/kernel-build/makingKernelMod/modules.order := {   echo /home/kernel-build/makingKernelMod/hello-world.ko; :; } | awk '!x[$$0]++' - > /home/kernel-build/makingKernelMod/modules.order
