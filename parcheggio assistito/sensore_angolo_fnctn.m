function [exectime, data] = sensore_angolo_fnctn(segment, data)

    switch segment
        case 1
            disp(['Segmento: ', num2str(segment)]);
            data.msg.msg = ttAnalogIn(1);  
            data.msg.time = ttCurrentTime;
            exectime = 0.07;
            
            disp(['Angolo acquisito: ', num2str(data.msg.msg)]);

        case 2
            % Prepara e invia il dato acquisito sulla rete
            disp(['Segmento: ', num2str(segment)]);
            data.msg.type = 'angolo_signal';
            
            ttSendMsg(4,data.msg,80);
            exectime=0.07;
            disp(['Invio dato: ', num2str(data.msg.msg)]);

            
        case 3
            % Termina task
            exectime = -1;
    end