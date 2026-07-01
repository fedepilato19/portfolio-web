function [exectime, data] = sensore_velocita_fnctn(segment, data)

switch segment
    case 1
        disp(['Segmento: ', num2str(segment)]);
        data.msg.msg = ttAnalogIn(1);  % poichè il sensore di velocità è sul canale 1
        data.msg.time = ttCurrentTime;
        exectime = 0.05;
        disp(['velocità acquisita: ', num2str(data.msg.msg)]);
        
    case 2  % Invia i dati di velocità
        disp(['Segmento: ', num2str(segment)]);
        data.msg.type = 'velocita_signal';
        
        ttSendMsg(4, data.msg, 80);  % Invia i dati alla rete
        exectime = 0.05;
        disp(['Invio dato: ', num2str(data.msg.msg)]);

    case 3
        exectime = -1;  % Termina l'esecuzione
end