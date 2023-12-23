% MAT model: Kobayashi et al., 2009 (Reproduce Figure 5B)
clear all;  %  Clear all parameters

dt= 0.1;       %  Time Step[ms] 
T=  700;       %  Duration [ms], 100 + 500 + 100 ms
N=  T/dt;      %  Num. of Steps 

% MAT parameters: Fixed for all the neurons
E_L= -65;       R= 50;
taum= 5;        tau1= 10;     tau2= 200;

% MAT parameters: Tuned for the prediction
%     Typical parameters for RS, IB, FS, and CH neurons
omega= -45;     a_1= 30;       a_2= 2.0;       %  RS neuron
%  omega= -46;     a_1= 7.5;      a_2= 1.5;       %  IB neuron
%  omega= -55;     a_1= 10;       a_2= 0.2;       %  FS neuron
%  omega= -39;     a_1= -0.5;     a_2= 0.4;       %  CH neuron

%  Set Initial Condition 
v= E_L* ones(N+1,1);    vth= omega* ones(N,1);    
u1= 0;      u2= 0;      I= 0;                 
t_f= 0;                 y= E_L* ones(N,1);

for t=1:N                             % simulation of 0.6 s
    v_th(t)= omega+ u1+ u2;           % Threshold
    y(t)= v(t);                       % y: Recorded Voltage (mV)

    if v(t) >= v_th(t) &&  t-t_f> 2.0/dt   % Spike Generation
        u1= u1+a_1;    u2= u2+a_2;         % Adaptation
        t_f=   t;                          % Spike times
        y(t)=  0;                          % y: Recorded Voltage (mV)
    end

    % Input Current
    if  t== 1000      %  Start Current injection from 0.1 s
        I= 0.6;
    end
    if  t== 6000      %  Stop Current injection from 0.6 s
        I= 0;
    end

    % Euler method
    v(t+1) = v(t)+ (R*I- (v(t)-E_L))/taum* dt;
    u1= u1- u1/tau1* dt;
    u2= u2- u2/tau2* dt;
end;

x= 0.1:0.1:700;   %  x: Time (ms)
%  Generate Graph
plot(x, y, x, v_th);
xlabel('Time (ms)')     % x-axis label
ylabel('Voltage (mV)')  % y-axis label
legend('Voltage','Threshold'); 
