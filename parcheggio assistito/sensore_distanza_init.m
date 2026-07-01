function sensore_distanza_init
    disp('kernel truetime Sensore distanza inizializzato');
    ttInitKernel('prioFP');

    % Parametro di consumo energetico per il sensore distanza
    ttSetKernelParameter('energyconsumption', 0.010);

    % Definizione task periodico sensore distanza
    nome_s = 'Task_sensore_distanza';
    starttime = 0.0;
    periodo = 1;  % periodo di aggiornamento dati
    funzione_s = 'sensore_distanza_fnctn';

    ttCreatePeriodicTask(nome_s, starttime, periodo, funzione_s);

    % Imposta priorità del task
    prio = 1;
    ttSetPriority(prio, nome_s);

    % Definizione e associazione network handler
    nome_nh = 'nw_handler';
    funzione_nh = 'nethandsens_fnctn';

    ttCreateHandler(nome_nh, 1, funzione_nh);
    ttAttachNetworkHandler(nome_nh);
end
