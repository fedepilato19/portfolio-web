function gateway_init
    disp('kernel truetime gateway inizializzato');
    ttInitKernel('prioFP');
    ttSetKernelParameter('energyconsumption', 0.010); % 10 mJ

    % Creazione mailbox per i segnali dai sensori
    ttCreateMailbox('velocita_signal', 10);
    ttCreateMailbox('distanza_signal', 10);
    ttCreateMailbox('angolo_signal', 10);

    % Creazione del task Gateway
    nome_c = 'Task_Gateway';
    deadline = 0.010;
    funzione_c = 'gateway_fnctn';

    ttCreateTask(nome_c, deadline, funzione_c);
    prio = 1;
    ttSetPriority(prio, nome_c);

 
    % Inizializzazione del network handler per la ricezione dati
    nome_nh = 'nw_handlergat';
    funzione_nh = 'nethandgat_fnctn';

    ttCreateHandler(nome_nh, 1, funzione_nh);
    ttAttachNetworkHandler(1, nome_nh);  % Metti il nodo in ascolto sulla rete 1 (wireless)