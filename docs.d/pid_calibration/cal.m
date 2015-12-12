clear all;
close all;

M = csvread('samples2.txt');
v_lr = M(:, 1) ./ 200;
v_fb = M(:, 2) ./ 200;

figure(1)
subplot(211)
plot(v_lr)
subplot(212)
plot(v_fb)

%%

x = [0];
for i = 2:length(v_lr)
    x(i) = x(i-1) + v_lr(i);
end

y = [0];
for i = 2:length(v_fb)
    y(i) = y(i-1) + v_fb(i);
end

figure(2)
subplot(211)
plot(x)
subplot(212)
plot(y)

figure(3)
plot(x, y)
