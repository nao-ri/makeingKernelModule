cmd_/home/kernel-build/makingKernelMod/Module.symvers := sed 's/\.ko$$/\.o/' /home/kernel-build/makingKernelMod/modules.order | scripts/mod/modpost -m -a  -o /home/kernel-build/makingKernelMod/Module.symvers -e -i Module.symvers   -T -
