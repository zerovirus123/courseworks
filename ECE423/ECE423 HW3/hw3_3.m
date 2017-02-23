%%%%%%%%%%%%    hw3_3   %%%%%%%%%%%%
a0 = 1e6;
p1 = 1;
b = 1;
s = tf('s');
p2_test = 2e5:1e3:9e5;
idx_pm = [];
pm_array = [];

for idx = 40:5:80
   pm_array = [pm_array idx];
end

for index = 1:1:701
    gain = a0/((1 + s/p1)*(1 + s/p2_test(index)) + a0);
    [Gm, Pm(index)] = margin(gain);
end

for idx = 0:5:40
    pm1 = find(rem(Pm(),(40+idx)) < 0.1);
    idx_pm = [idx_pm pm1(1)];
    pole2 = p2_test(idx_pm);
end

%things to tabulate
overshoot = [];
p2_a0p1 = [];
db_peak = [];

%use stepinfo(gain) to determine overshoot for transfer function with said
%poles
hold on;
for idx = 1:1:9
    gain = a0 / ((1+s/p1)*(1+s/pole2(idx))+a0);
    data = stepinfo(gain);
    overshoot = [overshoot data.Overshoot];
    p2_a0p1 = [p2_a0p1  p2/(a0*p1)];
    db_peak = [db_peak data.Peak];
    step(gain);
end
hold off

% %Overshoot plot
% plot(pm_array, overshoot);
% title('Overshoot versus Phase Margin');
% xlabel('Phase Margin/°');
% ylabel('Overshoot/%');
% 
% %Peak db plot
% plot(pm_array, db_peak);
% title('Peak versus Phase Margin');
% xlabel('Phase Margin/°');
% ylabel('Peak/dB');


