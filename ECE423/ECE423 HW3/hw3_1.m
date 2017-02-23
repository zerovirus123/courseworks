%%%%%%%%%%%   hw3_1    %%%%%%%%%%%%%%%%%%%%%%%
%component parameters
gm1 = 0.01;
gm2 = 0.02;
rz = 0;
r1 = 100000;
r2 = 5000;
c1 = 0.5*10^-12;
c2 = 2*10^-12;
cc = 1*10^-15;

%miller cap/resistor sweep values
cc_array = [];

for index = 10^-15:9.999*10^-13:10^-10
    cc_array = [cc_array index];
end

rz_array = [];
for index = 1:1:100
    rz_array = [rz_array index];
end

%transfer function coefficients
A = (r1*r2*(c1*c2+cc*c1+cc*c2));
B = (r1*c1+r2*c2+r1*cc*(1+gm2*r2)+r2*cc);
Av = gm1*r1*gm2*r2;
s = tf('s');

% %%uncomment to draw the Bode plot%%%%%%%%%%
% gain = Av * (1-s*cc*((1/gm2)-rz)) / (1 + B*s + A*s^2);
% H = tf(gain);
% bode(H);
% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%uncomment this block to sweep Cc%%%%%%%%%%%%%%%%
hold on; %latest data points will not override
pole1 = [];
pole2 = [];
zero_cell = [];
for index = 1:numel(cc_array)
    cc = cc_array(index);
    A = (r1*r2*(c1*c2+cc*c1+cc*c2));
    B = (r1*c1+r2*c2+r1*cc*(1+gm2*r2)+r2*cc);
    gain = Av * (1-s*cc*((1/gm2)-rz)) / (1 + B*s + A*s^2);
    H = tf(gain);
    [z,p,k] = zpkdata(H);
    pole = p{1,1};
    zero = z{1,1};
    pole1 = [pole1 pole(1)];
    pole2 = [pole2 pole(2)];
    zero_cell = [zero_cell zero];
end

subplot(2,2,1);
    plot(cc_array, pole1);
    set(gca,'xscale','log');
    title('Pole 1 vs Cc');
    xlabel('Cc/F');
    ylabel('Frequency/Hz');

subplot(2,2,2);
    plot(cc_array, pole2);
    set(gca,'xscale','log');
    title('Pole 2 vs Cc');
    xlabel('Cc/F');
    ylabel('Frequency/Hz');
    
subplot(2,2,3);
    plot(cc_array, zero_cell);
    set(gca,'xscale','log');
    title('Zero vs Cc');
    xlabel('Cc/F');
    ylabel('Frequency/Hz');

hold off;
cc = 1*10^-15;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%uncomment this block to sweep Rz%%%%%%%
cc = 2*10^-12;
hold on;
pole1 = [];
pole2 = [];
zero_cell = [];
for index = 1:1:100
    gm2 = 0.02;
    rz = rz_array(index);
    A = (r1*r2*(c1*c2+cc*c1+cc*c2));
    B = (r1*c1+r2*c2+r1*cc*(1+gm2*r2)+r2*cc);
    gain = Av * (1-s*cc*((1/gm2)-rz)) / (1 + B*s + A*s^2);
    H = tf(gain);
    [z,p,k] = zpkdata(H);
    pole = p{1,1};
    zero = z{1,1}
    
    if 1/gm2 == rz
       zero_cell = [zero_cell 0]; 
    else
       zero_cell = [zero_cell zero]; 
    end
    
    pole1 = [pole1 pole(1)];
    pole2 = [pole2 pole(2)];
end

subplot(2,2,1);
    plot(rz_array, pole1);
    set(gca,'xscale','log');
    title('Pole 1 vs Rc');
    xlabel('Resistance/\Omega');
    ylabel('Frequency/Hz');

subplot(2,2,2);
    plot(rz_array, pole2);
    set(gca,'xscale','log');
    title('Pole 2 vs Rc');
    xlabel('Resistance/\Omega');
    ylabel('Frequency/Hz');
    
subplot(2,2,3);
    plot(rz_array, zero_cell);
    %set(gca,'xscale','log');
    title('Zero vs Rc');
    xlabel('Resistance/\Omega');
    ylabel('Frequency/Hz');

hold off;
rz = 0;
cc = 1*10^-15;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

grid on;