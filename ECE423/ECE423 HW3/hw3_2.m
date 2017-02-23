%%%%%%%%%    hw3_2    %%%%%%%%%%%
a0 = 1e3;
p1 = 1e3;
p2 = 1e5;
p3 = 1e6;
b_array =0:0.001:1;
s = tf('s');

hold on; 
gain = a0 /((1 + s/p1) * (1 + s/p2) * (1 + s/p3));
rlocus(gain,b_array)
hold off;