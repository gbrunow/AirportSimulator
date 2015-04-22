%Close all opened figures
delete(findall(0,'Type','figure'));
bdclose('all');

figure;
x = mst_runway;
y = totalDelay;
plot(x, y);
xlabel('Runway Time');
ylabel('Total Delay');

figure;
x = mst_runway;
y = delayMaintenance;
plot(x, y);
xlabel('Runway Time');
ylabel('Maintenance Delay');

figure;
x = mst_runway;
y = delayGates;
plot(x, y);
xlabel('Runway Time');
ylabel('Gates Delay');

figure;
x = mst_runway;
y = delayRunway;
plot(x, y);
xlabel('Runway Time');
ylabel('Runway Delay');

figure;
x = mst_runway;
y = avgDelayPerPlane;
plot(x, y);
xlabel('Runway Time');
ylabel('Average Delay per Plane');