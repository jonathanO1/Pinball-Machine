R = 1000;
C = 318*10^-9;
s = tf('s');
h = ((s*R*C)/(s*R*C+1));
bode(h)
