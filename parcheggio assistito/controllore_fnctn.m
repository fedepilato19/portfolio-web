function [exectime, data] = controllore_fnctn(segment, data)
    switch segment
        case 1
            % Riceve il messaggio di temperatura dal mailbox 'temper_signal'
            temp_msg = ttTryFetch('velocita_signal');
            
            if ~isempty(temp_msg)
                data.velocita = temp_msg;  % Salva il valore della temperatura
                time = ttCurrentTime;
                disp(['t=' num2str(time) ' CONTROLLORE: valore di velocita ricevuto: ' num2str(data.velocita)]);
            else
                time = ttCurrentTime;
                disp(['t=' num2str(time) ' CONTROLLORE: non era messaggio di velocita']);
            end
            exectime = 0.1;  % Tempo di esecuzione per questo segmento
            
        case 2
            % Riceve il messaggio di umidità dal mailbox 'umid_signal'
            temp_msg = ttTryFetch('distanza_signal');
            
            if ~isempty(temp_msg)
                data.distanza = temp_msg;  % Salva il valore dell'umidità
                time = ttCurrentTime;
                disp(['t=' num2str(time) ' CONTROLLORE: valore di distanza ricevuto: ' num2str(data.distanza)]);
            else
                time = ttCurrentTime;
                disp(['t=' num2str(time) ' CONTROLLORE: non era messaggio di umidità']);
            end
            exectime = 0.1;  % Tempo di esecuzione per questo segmento

        case 3
             % Riceve il messaggio di umidità dal mailbox 'umid_signal'
            temp_msg = ttTryFetch('angolo_signal');
            
            if ~isempty(temp_msg)
                data.angolo = temp_msg;  % Salva il valore dell'umidità
                time = ttCurrentTime;
                disp(['t=' num2str(time) ' CONTROLLORE: valore di angolo ricevuto: ' num2str(data.angolo)]);
            else
                time = ttCurrentTime;
                disp(['t=' num2str(time) ' CONTROLLORE: non era messaggio di angolo']);
            end
            exectime = 0.1;  % Tempo di esecuzione per questo segmento


        case 4

             % Calcola la Forza d’Intervento Totale (FIT) con il sistema fuzzy
            data.fit = fuzzy_controller(data.velocita, data.distanza, data.angolo);
            disp(['t=' num2str(ttCurrentTime) ' CONTROLLORE: FIT calcolato = ' num2str(data.fit)]);

            % Uscita analogica per la temperatura e umidità (potrebbero essere usate per il controllo)
            ttAnalogOut(1, data.velocita);  % Imposta il valore della temperatura su un canale di uscita
            ttAnalogOut(2, data.distanza);    % Imposta il valore dell'umidità su un altro canale di uscita
            ttAnalogOut(3, data.angolo); 

            % Crea il messaggio da inviare con i valori di temperatura, umidità e potenza
            msg.msg.velocita = data.velocita;
            msg.msg.distanza = data.distanza;
            msg.msg.angolo = data.angolo;
            msg.msg.fit = data.fit;  % Puoi definire la potenza in base a qualche logica di controllo
            msg.type = 'control_signal';  % Tipo di messaggio (segnale di controllo)

            % Invia il messaggio sulla rete
            ttSendMsg(3, msg, 80);
            
            exectime = -1;  % Termina l'esecuzione del task in questo segmento

    end
end