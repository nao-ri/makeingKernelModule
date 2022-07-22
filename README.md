# GIPO LED light control
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- -C ../itarch/linux-stable/  M=`pwd` modules


# シフト演算
http://wisdom.sakura.ne.jp/programming/cs/cs7.html

# ラズパイへのファイル送信
sudo scp deviceFile.ko nao-ri@192.168.1.1:/home/nao-ri
