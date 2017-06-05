org 000h
mov p0,#offh;a
mov p1,#offh;b
mov p2,#offh;c
mov a,p1
mov b,a
mul ab
mov r7,a;b^2 in r7
mov a,p0
mov b,p2
mul ab
mov b,#4
mul ab; 4ac in a
mov b,a
mov a,r7
subb a,b
mov r7,a
acall srt
mov a,p0
mov b,#2
mul ab
mov r3,a; 2a in r3
mov a,p1
mov b,a
mov a,#0
subb a,b
mov r4,a; -b in r4
add a,r2
mov b,r3
div ab
mov r5,a
mov a,r4
subb a,r2
mov b,r3
div ab
mov r6,a;--------------------------roots stored in r5 and r6
srt: mov a,#r7
     mov r2,#0
     mov b,#1
loop:subb a,b
     jc stop
     inc r2
     inc b
     inc b
     sjmp loop
stop:ret
end
