function [exectime, data] = nethandsens_fnctn(segment, data)

temp = ttGetMsg;

ttTryPost(temp.type, temp.msg);
if strcmp('power_ping', temp.type)
    ttCreateJob('Task_Power_Response');  % Crea un lavoro per gestire la risposta dei sensori
end
exectime = -1;