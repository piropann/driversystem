# Raspberry Pi 4　LED_DeviceDriver
2020 RobotSystem Task1

## 概要
Raspberry Pi 4 Model B 4GBを使って、LEDの点滅を行うデバイスドライバの作成を行った。

## 構成

### 使用機材・部品

|使用機材・部品|個数|
|:---|:---|
|Raspberry Pi 4 Model B 4GB|1|
|ブレッドボード|1|
|LED|2|
|抵抗(200Ω)|2|
|ジャンパー線(オスメス)|4|

### 回路図
![image](https://user-images.githubusercontent.com/55969921/101446713-2039b680-3967-11eb-95f4-cbf24a51b713.png)

### 動画
画像をクリックしていただけると動画(YouTube)に飛びます。
[![](http://img.youtube.com/vi/CQbDgr0piRI/0.jpg)](http://www.youtube.com/watch?v=CQbDgr0piRI "")

## 実装方法

```bash
  git clone https://github.com/piropann/driversystem.git
  cd driversystem
  make
  ```
  
## プログラム起動方法

```bash
  sudo insmod myled.ko
  sudo chmod 666 /dev/myled0
  ```
  
### LED点滅
```bash
  echo 0 > /dev/myled0
  ```
  
### LED消灯
```bash
  echo 1 > /dev/myled0
  ```
  
### プログラム終了
```bash
  sudo rmmod myled
  ```

## LICENSE

GNU General Public License v3.0
