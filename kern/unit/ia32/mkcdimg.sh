#! /bin/sh

cp zero cdimg/kern
#genisoimage -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o cd.iso cdimg/
genisoimage -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-info-table -o cd.iso cdimg/
