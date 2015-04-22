%Close all opened figures
delete(findall(0,'Type','figure'));
bdclose('all');

figure;
x = mst_transition;
y = totalDelay;
plot(x, y);
xlabel('Transition Time');
ylabel('Total Delay');

figure;
x = mst_transition;
y = delayMaintenance;
plot(x, y);
xlabel('Transition Time');
ylabel('Maintenance Delay');

figure;
x = mst_transition;
y = delayGates;
plot(x, y);
xlabel('Transition Time');
ylabel('Gates Delay');

figure;
x = mst_transition;
y = delayRunway;
plot(x, y);
xlabel('Transition Time');
ylabel('Runway Delay');

figure;
x = mst_transition;
y = avgDelayPerPlane;
plot(x, y);
xlabel('Transition Time');
ylabel('Average Delay per Plane');