a3gim R3.0 リリースノート
                                                            2014.12.10

3GIM用のArduinoライブラリa3gim R3.0です。
機能的には、a3gs R3.0と同じです。


【a3gsとの差異について】

a3gimでは、電源制御とSMS受信時の割込処理のみが変更となっています。
以下、変更点を説明します：

- int start(int pin)
　　指定したpinを電源制御に使用します。
　　デフォルト(pin指定なし)では、電源制御は行いませんので
　　常にONとなります。
　　引数pinは1以上の番号を指定してください。
　　たとえば、ArduinoのD6を3GIMの1番ピンに接続した場合は、
　　　　a3gs.start(6)
　　のように呼び出してください。

- void shutdown()
　　電源制御を行っている場合、3GIMの電源をOFFにします。

- onSMS()
　　3GIMではSMSを受信しても何もしません。

【a3gimライブラリの使い方】

a3gimライブラリは、Arduino UNO/Pro等で3GIMを利用するためのライブラリです。
下記の結線を行ってからご使用ください。

- Arduino D2-3,D6-13,A0-5のいずれか又は接続なし  -->  3GIM #1(PWR_SW)
- Arduino D4  -->  3GIM #3(TX)
- Arduino D5  -->  3GIM #2(RX)
- Arduino 5V  -->  3GIM #4(IOREF)
- Arduino GND -->  3GIM #6(GND)
- 3.7V電源またはリチウム電池など  --> 3GIM #5(VCC)

--
