function [exectime, data] = regolatore_fnctn(segment, data)

switch segment

    case 1

        temp_msg = ttTryFetch('control_signal');

        if ~isempty(temp_msg)
            data.velocita=temp_msg.velocita;
            data.distanza= temp_msg.distanza;
            data.angolo=temp_msg.angolo;
            data.fit=temp_msg.fit;
            time = ttCurrentTime;
            disp(['t= ' num2str(time) ' REGOLATORE: ho ricevuto Vel: ' num2str(data.velocita) ' Dist: ' num2str(data.distanza) ' Ang: ' num2str(data.angolo) ' FIT: ' num2str(data.fit)]);
        else
            time=ttCurrentTime;
            disp(['t= ' num2str(time) 'REGOLATORE: nessun messaggio']);
        end

        exectime = 0.07;

    case 2
        ttAnalogOut(1,data.velocita);
        ttAnalogOut(2,data.distanza);
        ttAnalogOut(3,data.angolo);
        ttAnalogOut(4,data.fit);

        exectime= -1;
end
           