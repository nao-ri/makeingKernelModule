# GIPO LED light control
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- -C ../itarch/linux-stable/  M=`pwd` modules


# シフト演算
http://wisdom.sakura.ne.jp/programming/cs/cs7.html

# ラズパイへのファイル送信
sudo scp deviceFile.ko nao-ri@192.168.1.1:/home/nao-ri

# network設定
sudo ifconfig en7 192.168.1.2/2

# デバイスファイルを作成
mknod NAME TYPE MAJOR MINOR

# rapi
# write
echo '0:0f'|xxd -r > hello
# read 1byte
od -x -N 1 ./hello
