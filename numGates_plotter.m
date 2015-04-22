%Close all opened figures
delete(findall(0,'Type','figure'));
bdclose('all');

figure;
x = numGates;
y = totalDelay;
plot(x, y);
xlabel('Number of Gates');
ylabel('Total Delay');

figure;
x = numGates;
y = delayMaintenance;
plot(x, y);
xlabel('Number of Gates');
ylabel('Maintenance Delay');

figure;
x = numGates;
y = delayGates;
plot(x, y);
xlabel('Number of Gates');
ylabel('Gates Delay');

figure;
x = numGates;
y = delayRunway;
plot(x, y);
xlabel('Number of Gates');
ylabel('Runway Delay');

figure;
x = numGates;
y = avgDelayPerPlane;
plot(x, y);
xlabel('Number of Gates');
ylabel('Average Delay per Plane');