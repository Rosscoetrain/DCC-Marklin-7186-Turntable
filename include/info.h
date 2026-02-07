/*
 * 
 * Infomation only
 * 
 */

/*
For use on a nano


For arduino nano


D2    DCC Input

D12    Solenoid on
D13    Motor direction 0 = cw 1 = acw

D3    Hall 1
D4    Hall 2
D5    Hall 3
D6    Hall 4
D7    Hall 5
D8    Hall 6
D9    Hall 7
D10   Hall 8
D11   Hall 9

A0    Hall 10
A1    Hall 11
A2    Hall 12
A3    Hall 13
A4    Hall 14
A5    Hall 15
A6    Hall 16
A7    Button





Sensors are organised like so:

 1 opposite 8
 2 opposite 10
 3 opposite 11
 4 opposite 12
 5 opposite 13
 6 opposite 14
 7 opposite 15
 9 opposite 16

Magnet on bridge is at opposite end to bridge cab

Tracks are defined opposite where bridge cab is located.

Track 1 = sensor 1
Track 2 = sensor 2
Track 3 = sensor 3
Track 4 = sensor 4
Track 5 = sensor 5
Track 6 = sensor 6
Track 7 = sensor 7
Track 8 = sensor 8
Track 9 = sensor 9
Track 11 = sensor 11

Reverse track is defined where the bridge cab is located
Track 1 Reverse = sensor 8
Track 2 Reverse = sensor 10
Track 3 Reverse = sensor 11
Track 4 Reverse = sensor 12
Track 5 Reverse = sensor 13
Track 6 Reverse = sensor 14
Track 7 Reverse = sensor 15
Track 8 Reverse = sensor 1
Track 9 Reverse = sensor 16
Track 11 Reverse = sensor 3




Addresses are from BaseTurnoutAddress ( default = 1 ) to BaseTurnoutAddress + NUM_COMMANDS ( NUM_TRACKS + 7 )

<  =  anti-clockwise
>  =  clockwise

Address     Function
            Close           Throw

1           goto 1 <        goto 1 >
2           goto 2 <        goto 2 >
3           goto 3 <        goto 3 >
4           goto 4 <        goto 4 >
5           goto 5 <        goto 5 >
6           goto 6 <        goto 6 >
7           goto 7 <        goto 7 >
8           goto 8 <        goto 8 >
9           goto 9 <        goto 9 >
10          goto 10 <       goto 10 >
11          goto 11 <       goto 11 >
12          goto 12 <       goto 12 >
13          goto 13 <       goto 13 >
14          goto 14 <       goto 14 >
15          goto 15 <       goto 15 >
16          goto 16 <       goto 16 >
17          motor <         motor >
18          solenoid off    solenoid on
19          step <          step >
20          180deg <        180deg >
21          rotate <        rotate >
22          spare           stop at next

 













*/



  
