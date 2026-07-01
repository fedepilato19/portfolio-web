function regolatore_init
disp('kernel truetime Regolatore inizializzato');
ttInitKernel('prioFP');

ttCreateMailbox('control_signal', 10)

nome_c = 'Task_Regolatore';
deadline = 0.010;
funzione_c = 'regolatore_fnctn';

ttCreateTask(nome_c, deadline, funzione_c);

prio = 1;
ttSetPriority(prio, nome_c);

nome_nh = 'nw_handlerreg';
funzione_nh = 'nethandreg_fnctn';

ttCreateHandler(nome_nh, 1, funzione_nh);
ttAttachNetworkHandler(nome_nh);