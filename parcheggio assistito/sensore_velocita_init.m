function sensore_velocita_init
disp('kernel truetime Sensore velocita inizializzato');
ttInitKernel('prioFP'); % inizializzazione kernel

ttSetKernelParameter('energyconsumption', 0.010);

nome_s = 'Task_sensore_velocita';
starttime = 0.0;
periodo = 1;  % Periodo di aggiornamento dei dati
funzione_s = 'sensore_velocita_fnctn';

ttCreatePeriodicTask(nome_s, starttime, periodo, funzione_s);

prio = 1;
ttSetPriority(prio, nome_s);

%creazione network Handler
nome_nh = 'nw_handler';
funzione_nh = 'nethandsens_fnctn';

ttCreateHandler(nome_nh, 1, funzione_nh);
ttAttachNetworkHandler(nome_nh)