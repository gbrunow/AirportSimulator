%Close all opened figures
delete(findall(0,'Type','figure'));
bdclose('all');

figure;
x = mst_maintenance;
y = totalDelay;
plot(x, y);
xlabel('Maintenance Time');
ylabel('Total Delay');

figure;
x = mst_maintenance;
y = delayMaintenance;
plot(x, y);
xlabel('Maintenance Time');
ylabel('Maintenance Delay');

figure;
x = mst_maintenance;
y = delayGates;
plot(x, y);
xlabel('Maintenance Time');
ylabel('Gates Delay');

figure;
x = mst_maintenance;
y = delayRunway;
plot(x, y);
xlabel('Maintenance Time');
ylabel('Runway Delay');

figure;
x = mst_maintenance;
y = avgDelayPerPlane;
plot(x, y);
xlabel('Maintenance Time');
ylabel('Average Delay per Plane');