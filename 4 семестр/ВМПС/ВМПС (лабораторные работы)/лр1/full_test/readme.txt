Прошивка файла в учебный стенд НТЦ-31.100 осуществляется из среды AVR Studio.
Запустите AVR Studio.
Tools -> Programm AVR -> Connect
Platform: JTAG ICE
Port: COM3 (Или другой порт, соответствующий Usb Serial Port on USB Serial Converter A)
Connect...
На закладке Main
Device and Signature Bytes
  ATMega128
Для проверки Read Signature - должно быть Signature matches selected device
Закладка Program
Область Flash -> Input HEX File - указать файл test.hex
Нажать Program.
Закрыть программу.
Стенд прошит.