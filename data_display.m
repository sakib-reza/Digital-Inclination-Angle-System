%COMPENG 2DP4 Final Project: Serial Communication
%Sakib Reza
%rezas2
%400131994

 if ~isempty(instrfind) %making sure MATLAB can find the right port
     fclose(instrfind);
      delete(instrfind);
 end
 
plotTitle = 'Angle of ADXL337 vs Time';  %creating the graph to display the data
xLabel = 'Time (s)';    
yLabel = 'Angle (deg)';                
plotGrid = 'on';                
min = 0;                     
max = 90;                      
timeAxis = 10;               
delay = 0.01;

time = 0; %dynamic graphing variables, c1 & c2 used as time intervals
angle = 0;
c1 = 0;
c2 = 0;

plotGraph = plot(time, angle); %time to run it
title(plotTitle);
xlabel(xLabel);
ylabel(yLabel);
axis([0 10 min max]);
grid(plotGrid);
 
 
s = serial('COM3'); %setting up the serial communication, with the right port/baud rate
set(s,'BaudRate',19200);
fopen(s);

tic

while ishandle(plotGraph)   %actual function taking the serial communcation and outputting data to graph
    c1 = c1 + 1;
    dat = fscanf(s);
    c2 = c2 + 1;    
    time(c2) = toc; 
    angle(c2) = (str2double(dat(1:2)));
         
        if(timeAxis > 0)
        set(plotGraph,'XData',time(time > time(c2)-timeAxis),'YData',angle(time > time(c2)-timeAxis));
        axis([time(c2)-timeAxis time(c2) min max]);
        else
        set(plotGraph,'XData',time,'YData',angle);
        axis([0 time(c2) min max]);
        end
     
        pause(delay);
        
    if(c1 == 1000)
        clear angle;
        c1 = 0;
    end
end

fclose(s); %Turning off serial communcation and clearing variables
clear c2 dat delay max min plotGraph plotGrid plotTitle s timeAxis serialPort xLabel yLabel;
delete(s);
clear s
