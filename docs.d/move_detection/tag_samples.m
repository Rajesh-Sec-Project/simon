clear all;
close all;

M = csvread('tag_samples2.txt');
vx = M(:, 1);
vy = M(:, 2);

v = vx;

window = 20;
amplitude = 50;

i = 1:window;
eps = 5;
% epsilon = 1e-3;
% pattern = amplitude .* (((i-1) - (window / 2) + epsilon)) .^ (-1);
% pattern = max(-amplitude, min(amplitude, pattern));
% pattern = vx(495:(495+32));
% pattern = triangularPulse(eps, window/4+eps, window/2, i) - triangularPulse(window/2, 3*window/4-eps, window-eps, i);
pattern = triangularPulse(eps, window/2, window-eps, i);

figure(1);
plot(pattern);

% fprintf('{ ');
% for p = pattern
%     fprintf('%f, ', p);
% end
% fprintf('}\n');

plocs = []
pkinds = []
step = window/8;
for j = 1:step:(length(v)-window)
    data = v(j:j+window);
    
    [corr, ~] = xcorr(data, pattern);
    [~, Mpklocs] = findpeaks(corr, 'MinPeakHeight', 100); %, 'MinPeakDistance', window/2);
    [~, mpklocs] = findpeaks(-corr, 'MinPeakHeight', 100); %, 'MinPeakDistance', window/2);
    
    if (length(Mpklocs) == 1 && length(mpklocs) == 1)
        Mpk = Mpklocs(1);
        mpk = mpklocs(1);
        
        plocs = [plocs, j];
        if (Mpk < mpk)
            what = 'left';
            pkinds = [pkinds, -1];
        else
            what = 'right';
            pkinds = [pkinds, 1];
        end
        fprintf('detected peak at %d (%s)\n', j, what);
%         plocs = [plocs, j+Mpk-window/2];
%         pkinds = [pkinds, 1];
%         plocs = [plocs, j+mpk-window/2];
%         pkinds = [pkinds, -1];
    end
end

figure(2);
plot(v);
hold on;
for k = 1:length(plocs)
    id = plocs(k);
    what = pkinds(k);
    
    if what > 0
        plot([id id], [50 -50], 'r');
    else
        plot([id id], [50 -50], 'g');
    end
end

% [acorr, ~] = xcorr(v, pattern);
% [Mpks, Mpklocs] = findpeaks(acorr, 'MinPeakHeight', 100, 'MinPeakDistance', window/2);
% [mpks, mpklocs] = findpeaks(-acorr, 'MinPeakHeight', 100, 'MinPeakDistance', window/2);
% 
% figure(1)
% subplot(311)
% plot(v)
% hold on
% for pk = Mpklocs
%     id = pk - length(vx) + window/2;
%     plot([id id], [50 -50], 'r');
% end
% for pk = mpklocs
%     id = pk - length(vx) + window/2;
%     plot([id id], [50 -50], 'g');
% end
% subplot(312)
% plot(pattern);
% subplot(313);
% plot(acorr);
% hold on
% for pk = [Mpklocs mpklocs]
%     plot([pk pk], [1000 -1000], 'r');
% end
