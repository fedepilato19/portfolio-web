function [exectime, data] = nethandgat_fnctn(segment, data)

temp = ttGetMsg(1);


ttTryPost(temp.type, temp.msg);

if strcmp('power_ping', temp.type)
    ttCreateJob('Task_Power_Response');
else
    ttCreateJob('Task_Gateway');
end

exectime= -1;