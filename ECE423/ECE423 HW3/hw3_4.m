%%%%%%%%%%%%%    hw3_4    %%%%%%%%%%%%%%%%
a0 = 1e2;
p1 = 1e4;
b = 1;
p2 = [234000,293000,358000,427000,500000,577000,658000,742000,827000];
s = tf('s');

overshoot = [];
db_peak = [];
p2_a0p1 = [];

hold on;
for index = 1:1:9
    gain = a0/((1 + s/p1)*(1 + s/(p2(index)))+a0);
    H = tf(gain);
    step(H);   
    %bode(H);
    [Gm, Pm(index)] = margin(gain);
    data = stepinfo(gain);
    overshoot = [overshoot data.Overshoot];
    p2_a0p1 = [p2_a0p1  p2/(a0*p1)];
    db_peak = [db_peak data.Peak];
end
hold off;
