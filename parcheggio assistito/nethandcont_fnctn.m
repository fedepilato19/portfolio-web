function [exectime, data] = nethandcont_fnctn(segment, data)
   temp = ttGetMsg;
ttTryPost(temp.type, temp.msg);
ttCreateJob('Task_Controllore');
exectime = -1;