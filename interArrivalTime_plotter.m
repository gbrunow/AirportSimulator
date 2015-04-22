%Close all opened figures
delete(findall(0,'Type','figure'));
bdclose('all');

figure;
x = interArrivalTime;
y = totalDelay;
plot(x, y);
xlabel('Inter Arrival Time');
ylabel('Total Delay');

figure;
x = interArrivalTime;
y = delayMaintenance;
plot(x, y);
xlabel('Inter Arrival Time');
ylabel('Maintenance Delay');

figure;
x = interArrivalTime;
y = delayGates;
plot(x, y);
xlabel('Inter Arrival Time');
ylabel('Gates Delay');

figure;
x = interArrivalTime;
y = delayRunway;
plot(x, y);
xlabel('Inter Arrival Time');
ylabel('Runway Delay');

figure;
x = interArrivalTime;
y = avgDelayPerPlane;
plot(x, y);
xlabel('Inter Arrival Time');
ylabel('Average Delay per Plane');