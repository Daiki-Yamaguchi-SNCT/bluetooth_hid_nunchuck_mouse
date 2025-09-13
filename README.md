# bluetooth_hid_nunchuck_mouse
ヌンチャクをマウスに改造
Nunchuck.hは「88IO」さんの https://github.com/88IO/nunchuck-mouse/blob/main/Nunchuck.h を使わせていただいた。
マイコンはアリエクで買ったESP32の互換ボードを使用した。

# 手順
1.まず、ヌンチャクとマイコンをジャンパー線で接続。
![](https://kimagureupuser.web.fc2.com/cloud/esp32.png)

2.パソコンでNunchuck.hとbluetooth_hid_nunchuck_mouse.inoを同じディレクトリ内に配置する。

3.Arduino IDEで書き込み。

ここまですればもうBluetoothのペアリングが可能。

# 使用方法
スティックでカーソル移動。
Cボタンで左クリック。
Cボタン＋Zボタンで右クリック。
Zボタンを押しながらスティック上下で画面スクロール。

