function controllore_init
    disp('kernel controllore inizializzato');
    % Inizializza il kernel TrueTime con la priorità 'prioFP'
    ttInitKernel('prioFP');
    
    % Creazione dei mailbox per i tre segnali
    ttCreateMailbox('velocita_signal', 10);
    ttCreateMailbox('distanza_signal', 10);
    ttCreateMailbox('angolo_signal', 10);
    
    % Creazione del task del controllore (con deadline di 10 ms)
    nome_c = 'Task_Controllore';
    deadline = 0.010;  % Scadenza del task controllore (10 ms)
    funzione_c = 'controllore_fnctn';  % Funzione da eseguire per il controllore
    ttCreateTask(nome_c, deadline, funzione_c);
    
    % Impostazione della priorità del task
    prio = 1;
    ttSetPriority(prio, nome_c);
    
    % Inizializzazione della rete per controllore 
    nome_nh = 'nw_handlercont';  % Nome del network handler
    funzione_nh = 'nethandcont_fnctn';  % Funzione per gestire la rete
    ttCreateHandler(nome_nh, 1, funzione_nh);
    ttAttachNetworkHandler(nome_nh);  % Collega il network handler al kernel TrueTime
