function [exectime, data] = nethandreg_fnctn (segment, data)

temp= ttGetMsg;

ttTryPost(temp.type, temp.msg);

ttCreateJob('Task_Regolatore');

exectime = -1;