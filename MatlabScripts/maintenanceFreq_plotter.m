%Close all opened figures
delete(findall(0,'Type','figure'));
bdclose('all');

figure;
x = maintenanceFreq;
y = totalDelay;
plot(x, y);
xlabel('Maintenance Frequency');
ylabel('Total Delay');

figure;
x = maintenanceFreq;
y = delayMaintenance;
plot(x, y);
xlabel('Maintenance Frequency');
ylabel('Maintenance Delay');

figure;
x = maintenanceFreq;
y = delayGates;
plot(x, y);
xlabel('Maintenance Frequency');
ylabel('Gates Delay');

figure;
x = maintenanceFreq;
y = delayRunway;
plot(x, y);
xlabel('Maintenance Frequency');
ylabel('Runway Delay');

figure;
x = maintenanceFreq;
y = avgDelayPerPlane;
plot(x, y);
xlabel('Maintenance Frequency');
ylabel('Average Delay per Plane');