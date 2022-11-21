# Protocol

The pairing procedure is actually just a configuration procedure. Judging from the BLE logs, there is no real pairing or encryption performed. Instead, the client connects to the device, registers a UID and receives a baby code. This code can then be used for configuring it. The button sends notifications on all relevant events.

Attention: The last 8 bytes of all UUIDs are the device address!

## Initialization
```
DATA_CHAR_1 = "01021921-9e06-a079-2e3f-ee6673f4b7a4"
DATA_CHAR_2 = "01021922-9e06-a079-2e3f-ee6673f4b7a4"
DATA_CHAR_3 = "01021923-9e06-a079-2e3f-ee6673f4b7a4"
DATA_CHAR_4 = "01021925-9e06-a079-2e3f-ee6673f4b7a4"

  * Enable Notification 01021921-9e06a0792e3fee6673f4b7a4 (DATA_CHAR_1)
  * Write 693132333435363738393031323334353600 to DATA_CHAR_1
RegisterUID - that's some kind of user ID!?
  * Recv Notification on DATA_CHAR_1 6900323334353637383930313233343536000000
  * Write 685a89752d01003203000000000000000000 to DATA_CHAR_1
RegisterType
[1-4] currentTimeMillis()
[5] 0x01
[6] 0x00
[7] 0x32
[8] 0x03
[9] 0x00
  * Recv Notification on DATA_CHAR_1 680061d5c56ba38c030000000000000000000000
[2-7] baby code, needs to be remembered and sent with 0x70 message
  * Disconnect
```
## First time connecting
```
  * Connect
  * Enable Notification on all DATA_CHAR_?
  * Write 7061d5c56ba38c5a89753a00000000000000 on DATA_CHAR_1
[0] == 0x70 => ConnectionType (ReconnectionType)
-> The code received during pairing and the current timestamp (ms)
  * Recv Notification on DATA_CHAR_1 700000000000010001753a000000000000000000
[0] == 0x70 && [1] == 0x00 => Connected Success
[0] == 0x70 && [1] == 0x01 => Disconnected
[0] == 0x68 => ACK required?
[0] == 0x69 && [1] == 0x00 => sendRegisterRequest
  *  Write c001000000000000000000000000000000000000 on DATA_CHAR_4
BLE_MSG_GET_BATCH - is sent exactly like this
  *  Recv Notification on DATA_CHAR_4 c000000000000000000000000000000000000000
[1] == 1 ==> setBatchVersion, nothing else
  *  Write f5f2320300000000000000000000000000000000 on DATA_CHAR_4
LeaningType
[1] BitMask with the various alerts to enable - f2
   [0] = 0
   [1] = Face Down Alert (1)
   [2] = T_Alert (0)
   [3] = H_Alert (0)
   [4] = 1
   [5] = On_Alert (1)
   [6] = Null Breath Alert (1)
   [7] = 1
[2] 0x32
[3] 0x03
[4] 0x00
  *  Recv Notification on DATA_CHAR_4 f500320300000000000000000000000000000000
nothing happens with that
  *  Write b2006801c8000000000000000000000000000000 on DATA_CHAR_4
TemAlarm; 
f1: 0x0168 ==> 360 => 36.0 C
f2: 0x00c8 ==> 200 => 20.0 C
Two floats, multiplied by two, then sent as integers
[1] isHideTemSet (?)
[2] (int)(f1 * 10.0) & 0xff
[3] (int)(f1 * 10.0) >> 8
[4] (int)(f2 * 10.0) & 0xff
[5] (int)(f2 * 10.0) >> 8
  * Recv Notification on DATA_CHAR_4 b2006801c8000000000000000000000000000000
nothing happens with that
  * Write b3000f0300000000000000000000000000000000 on DATA_CHAR_4
KickingAlarm
3 integers as parameters
[1] int1 & 0xff
[2] 0x0f
[3] int2 & 0xff
[4] int3 & 0xff
  * Recv Notification on DATA_CHAR_4 b3000f0300000000000000000000000000000000
nothing happens with that
  * Write b001190000000000000000000000000000000000 on DATA_CHAR_4
BreathAlarm
2 integers as parameters (probably the lower and upper limits? 1 and 25 in this case)
[1] int1 & 0xff
[2] int2 & 0xff
  * Recv Notification on DATA_CHAR_4 b000190000000000000000000000000000000000
nothing happens with that

  * Recv Notification on DATA_CHAR_2 405445d4330001000000000005000000f43d0001
[1-5] Timestamp
[0] RecordType; [0] >> 3 & 0x1f
    0x06 => Battery
    0x07 => Nothing
    0x08 => depends on statusType; statusType = (int([0-1]) >> 6) & 0x1f
        0x01 => Temperature Package
           int(hex([5-6])) / 10.0 = Temperature
           int(hex([7])) / 100.0 = Humidity
        0x02 => Alert
        0x03 => Nothing happens
        0x04 => Gesture
           int(hex[5]) = LayStatus
        0x05 => Breath
           int(hex[5]) = Breath Rate
        0x07 => Activity
        0x08 => BlSignal
  * Recv Notification on DATA_CHAR_2 415445d437ff0100b82000200200000005000002
  * Recv Notification on DATA_CHAR_2 411445d438020021b82000200000000002000003
  * Recv Notification on DATA_CHAR_2 411445d43a000020b82000200000000002000004
  * Recv Notification on DATA_CHAR_2 411445d500030021b82000200000000002000005
  * Recv Notification on DATA_CHAR_2 411445d503000020b82000200000000002000006
```
## Disconnect
```
  * Write f5d2320300000000000000000000000000000000 on DATA_CHAR_4
Disables On_Alert -> this seems to switch it off
  * Recv Notification on DATA_CHAR_4 f500320300000000000000000000000000000000
  * Recv Notification on DATA_CHAR_2 415445d504ff0100b82000200200000005000007
```
## Connect
```
  * Write f5f2320300000000000000000000000000000000 on DATA_CHAR_4
Enables On_Alert -> this seems to switch it on
  * Recv Notification auf DATA_CHAR_4 f500320300000000000000000000000000000000
  * Recv Notification auf DATA_CHAR_2 405445d52bfd00000000000005000000f43d0008
...
  * Write f5d2320300000000000000000000000000000000 auf DATA_CHAR_4
  * Recv Notification auf DATA_CHAR_4 f500320300000000000000000000000000000000
  * Recv Notification auf DATA_CHAR_2 415445d604ff0100b8200020020000000500000c
```
